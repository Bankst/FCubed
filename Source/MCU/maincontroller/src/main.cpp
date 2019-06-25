#include <Arduino.h>
#include <SdFat.h>
#include <TeensyThreads.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <vector>

#include "common.h"

#include "proto/FCubed.pb.h"
#include "proto/ProtoManager.h"
#include "proto/ProtoMessage.h"

#include "util/ardprintf.h"
#include "util/ardprintf5.h"
#include "util/model.h"

#define STATUS_BLINK_MS 60
#define STATUS_BLINK_DELAY_MS 1000
#define RX_BLINK_MS 15
#define RX_LOOP_MS 40

#define STATUS_LED LED_BUILTIN
#define RX_LED 0

#define CONTROL_BAUD 500000
#define DEBUG_BAUD 115200

ThreadWrap(Serial5, Serial5X);
#define Serial5 ThreadClone(Serial5X)
#define DebugSerial Serial5

#define digitalLow(x) digitalWriteFast(x, LOW)
#define digitalHigh(x) digitalWriteFast(x, HIGH);
#define ledOn(x) digitalHigh(x)
#define ledOff(x) digitalLow(x)

// Serial comms stuff
volatile bool isRx;
ProtoMessage msgBuf;
uint8_t serialBuf[128];
size_t message_length;
bool pb_status;
Threads::Mutex serialLock;

volatile int counter; // status counter

// SD stuff
SdFs sd;
uint32_t cardSize;
cid_t m_cid;
csd_t m_csd;
uint32_t m_ocr;
bool sdGood;
float sdMBytes;
float sdFreeMBytes;

void statusThread()
{
  ledOff(STATUS_LED);
  while (1)
  {
    ledOn(STATUS_LED);
    threads.delay(STATUS_BLINK_MS);
    ledOff(STATUS_LED);
    threads.delay(STATUS_BLINK_MS);
    ledOn(STATUS_LED);
    threads.delay(STATUS_BLINK_MS);
    ledOff(STATUS_LED);
    if (Serial)
    { // indicate serial connection
      threads.delay(STATUS_BLINK_MS + 75);
      ledOn(STATUS_LED);
      threads.delay(STATUS_BLINK_MS + 150);
      ledOff(STATUS_LED);
    }
    threads.delay(STATUS_BLINK_DELAY_MS);
  }
  threads.yield();
}

void rxLedThread()
{
  ledOff(RX_LED);
  while (1)
  {
    if (isRx)
    {
      for (int i = 0; i < 5; i++)
      { // blink 3 times quickly on RX
        ledOn(RX_LED);
        threads.delay(RX_BLINK_MS + (RX_BLINK_MS));
        ledOff(RX_LED);
        threads.delay(RX_BLINK_MS + (RX_BLINK_MS / 2));
      }
      isRx = false;
    }
    threads.yield();
  }
}

void serialEvent() {
  uint32_t rd = Serial.available();
  if (rd > 0) {
    isRx = true;
    serialLock.lock();
    isRx = true;
    uint32_t n = Serial.readBytes((char *)serialBuf, rd);
    serialLock.unlock();

    if (n != rd) {
      DebugSerial.println("Failed to read correct amount of data.");
      return;
    }

    ardprintf5("RX - AvailLen: %d, BufLen: %d, ReadBytesLen: %d", rd, (int)sizeof(serialBuf), n);
    ardprintf5("raw opcode: %d", serialBuf[0]);

    delay(100);

    msgBuf = ProtoMessage(serialBuf, n, true);
    ardprintf5("msgBuf len: %d", msgBuf.len);
    ProtoMessage* msgPtr = &msgBuf;
    ManagerResultPair protoRes = ProtoManager::HandleMessage(msgPtr);
    switch(protoRes.first) {
      case ProtoManagerResult::OK:
        ardprintf5("Handled, opcode: %d", protoRes.second);
        break;
      case ProtoManagerResult::BAD_OPCODE:
        ardprintf5("Unhandled, bad opcode: %d", protoRes.second);
        break;
      case ProtoManagerResult::BAD_MESSAGE:
        DebugSerial.println("Unhandled, bad message");
        break;
      case ProtoManagerResult::NO_HANDLER:
        ardprintf5("Unhandled, no handler, opcode: %d", protoRes.second);
        break;
    }
  }
}

// void txThread()
// {
//   while (true)
//   {
//     if (!isRx)
//     { // skip TX if RXing
//       serialLock.lock();

      
//       ardprintf5("SENT HEARTBEAT %d", counter++);
//       serialLock.unlock();
//     }
//     threads.delay(500);
//   }
// }

void updateSDFreeSpace()
{
}

void setupIO()
{
  pinMode(STATUS_LED, OUTPUT);
  pinMode(RX_LED, OUTPUT);

  ledOn(STATUS_LED);
  ledOn(RX_LED);
}

void setupSerial()
{
  Serial.begin(CONTROL_BAUD);
  DebugSerial.begin(DEBUG_BAUD);
  delay(500);
  ardprintf("FCubed Main CPU %s", completeVersion);
  ardprintf5("FCubed Main CPU %s", completeVersion);
}

void setupThreads()
{
  threads.setMicroTimer();
  threads.addThread(statusThread);
  threads.addThread(rxLedThread);
}

void setupSD()
{
  sdGood = sd.begin(SdioConfig());
  if (!sdGood)
  {
    sd.initErrorHalt();
    DebugSerial.println("SD Init failed. Poopyhead.");
  }
  else
  {

    if (!sd.card()->readCID(&m_cid) ||
      !sd.card()->readCSD(&m_csd) ||
      !sd.card()->readOCR(&m_ocr)) {
    DebugSerial.println("SD Info read failed.");
    return;
  }

    cardSize = sdCardCapacity(&m_csd);
    sdMBytes = 0.000512 * cardSize;

    const char *cardType;
    switch (sd.card()->type())
    {
    case SD_CARD_TYPE_SD1:
      cardType = "SD1";
      break;
    case SD_CARD_TYPE_SD2:
      cardType = "SD2";
      break;
    case SD_CARD_TYPE_SDHC:
      cardType = cardSize < 70000000 ? "SDHC" : "SDXC";
      break;
    default:
      cardType = "Unknown";
    }

    char cardCapacity[128];
    char cardFormat[32];
    bool hasVol = sd.fatType() != 0;

    if (hasVol) {
      sdFreeMBytes = 0.000512 * sd.vol()->freeClusterCount() * sd.vol()->sectorsPerCluster();

      snprintf(cardFormat, sizeof(cardFormat), "FAT%d", sd.vol()->fatType());
      snprintf(cardCapacity, sizeof(cardCapacity), "%.2f MB of %.2f MB free", sdFreeMBytes, sdMBytes);
    } else {
      snprintf(cardFormat, sizeof(cardFormat), "Unformatted");
      snprintf(cardCapacity, sizeof(cardCapacity), "%.2f MB capacity", sdMBytes);
    }

    ardprintf5("%s %s Card OK, %s", cardFormat, cardType, cardCapacity);
  }
}

void setup()
{
  setupIO();
  setupSerial();
  // setupSD();
  setupThreads();
}

void loop()
{ // loop is for nerds
}
