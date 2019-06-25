#ifndef PROTOHANDLER_H
#define PROTOHANDLER_H

#include <pb_decode.h>
#include <pb_common.h>
#include <utility>
#include <Arduino.h>

#include "../common.h"
#include "ProtoHandlers.h"

enum class ProtoManagerResult {
    OK,
    BAD_OPCODE,
    BAD_MESSAGE,
    NO_HANDLER
};

using ManagerResultPair = std::pair<ProtoManagerResult, uint8_t>;

class ProtoManager {
public:
    static ManagerResultPair HandleMessage(ProtoMessage *msg);

    static bool lastStatus;
private:
    ProtoManager() {}

    static ManagerResultPair GetOpcodeFromMessage(ProtoMessage *msg);
};

#endif /* PROTOHANDLER_H */
