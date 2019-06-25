#ifndef PROTOMESSAGE_H
#define PROTOMESSAGE_H

#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Arduino.h>
#include <vector>

#include "../common.h"

class ProtoMessage {
public:
    uint8_t *buffer;
    std::vector<uint8_t> vecbuf;
    size_t len;

    ProtoMessage() {}

    ~ProtoMessage() {
        free(buffer);
    }

    ProtoMessage(uint8_t *inBuf, size_t _len) {        
        len = _len;
        buffer = (uint8_t *)malloc(_len * sizeof(uint8_t));
        memcpy(buffer, inBuf, _len);
    }

    ProtoMessage(uint8_t *inBuf, size_t _len, bool) {
        len = _len;
        vecbuf = std::vector<uint8_t>(inBuf, inBuf + _len);
    }

    ProtoMessage(size_t _len) {
        len = _len;
    }

    template <class MessageType>
    static bool GenericDecode(MessageType *msg, ProtoMessage *inBuf) {    
        pb_istream_t instream = pb_istream_from_buffer(inBuf->buffer, inBuf->len);
        return pb_decode(&instream, MessageType::fields, &msg);
    }

    template <class MessageType>
    static bool GenericEncode(MessageType *msg, ProtoMessage *outBuf) {
        pb_ostream_t outstream = pb_ostream_from_buffer(outBuf->buffer, outBuf->len);
        return pb_encode(&outstream, MessageType::fields, &msg);
    }
};

#endif /* PROTOMESSAGE_H */
