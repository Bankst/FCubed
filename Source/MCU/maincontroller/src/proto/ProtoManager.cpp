#include "ProtoManager.h"
#include "../common.h"

ManagerResultPair ProtoManager::HandleMessage(ProtoMessage *msg) {
    ManagerResultPair result;
    result.second = -1;

    // if (msg.len < F3Message_size) {
    //     result.first = ProtoManagerResult::BAD_MESSAGE; // message too small
    // }

    ManagerResultPair opcodeResult = GetOpcodeFromMessage(msg);
    Serial5.print("Opcode result: ");
    Serial5.print(opcodeResult.first == ProtoManagerResult::OK ? "OK" : "BAD_OPCODE");
    Serial5.print(", opcode:");
    Serial5.println(opcodeResult.second);
    delay(100);
    switch(opcodeResult.first) {
        case ProtoManagerResult::OK:
            result.second = opcodeResult.second;
            break;
        case ProtoManagerResult::BAD_OPCODE:
            result.first = ProtoManagerResult::BAD_OPCODE;
            result.second = opcodeResult.second;
            return result;
        case ProtoManagerResult::BAD_MESSAGE:
            result.first = ProtoManagerResult::BAD_MESSAGE;
            return result;
        default:
            return result;
    }
    F3Opcode code = static_cast<F3Opcode>(opcodeResult.second);
    Serial5.println("maybe here?");
    delay(100);
    HandlerMapCall::const_iterator it = ProtoHandlers::HandlerMap.find(code);    
    Serial5.println("maybe here 2");
    delay(100);
    HandlerFunction handlerFunc;

    if (it != ProtoHandlers::HandlerMap.end()) {        
        Serial5.println("maybe here 3");
        delay(100);
        handlerFunc = it->second;
    } else {
        result.first = ProtoManagerResult::NO_HANDLER;
        return result;
    }
    
    Serial5.println("maybe here 4");
    delay(100);

    handlerFunc(msg);
    
    return result;
}

ManagerResultPair ProtoManager::GetOpcodeFromMessage(ProtoMessage *msg) {
    ManagerResultPair result;
    result.second = -1;

    if (msg->len >= 1) {
        uint8_t opcode = msg->buffer[0];
        uint8_t vecOpcode = msg->vecbuf[0];
        Serial5.print("Opcode from msg: ");
        Serial5.println(opcode);
        Serial5.print("Opcode from vecbuf: ");
        Serial5.println(vecOpcode);
        delay(100);
        result.first = (opcode <= _F3Opcode_MAX) ?  ProtoManagerResult::OK : ProtoManagerResult::BAD_OPCODE;
        result.second = opcode;
    } else {
        result.first = ProtoManagerResult::BAD_MESSAGE;
    }
    return result;
}