#include "Arduino.h"
#include "TeensyThreads.h"
#include "ardprintf.h"
#include "build_defs.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define STATUS_BLINK_MS 60
#define STATUS_DELAY_BLINK_MS 1000
#define RX_BLINK_MS 15
#define RX_LOOP_MS 40

#define STATUS_LED LED_BUILTIN
#define RX_LED 0

const unsigned char completeVersion[] =
{
	'V',
	VERSION_MAJOR_INIT,
	'.',
	VERSION_MINOR_INIT,
	'-',
	BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
	'-',
	BUILD_MONTH_CH0, BUILD_MONTH_CH1,
	'-',
	BUILD_DAY_CH0, BUILD_DAY_CH1,
	'-',
	BUILD_HOUR_CH0, BUILD_HOUR_CH1,
	':',
	BUILD_MIN_CH0, BUILD_MIN_CH1,
	':',
	BUILD_SEC_CH0, BUILD_SEC_CH1,
	'\0'
};

volatile bool isRx;

void ledOn(int led)
{
	digitalWriteFast(led, HIGH);
}

void ledOff(int led)
{
	digitalWriteFast(led, LOW);
}


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
		threads.delay(STATUS_DELAY_BLINK_MS);
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

void setup()
{
  pinMode(STATUS_LED, OUTPUT);
  pinMode(RX_LED, OUTPUT);

	ledOn(STATUS_LED);
  ledOn(RX_LED);

  Serial.begin(115200);
	delay(500);
	ardprintf("FCubed Main CPU %s", completeVersion);

	threads.addThread(statusThread);
	threads.addThread(rxLedThread);
}

void loop()
{
}
