#include "ProtoHandlers.h"

ProtoMessage ProtoHandlers::txBuf;

const HandlerMapCall ProtoHandlers::HandlerMap = {
    {F3Opcode::F3Opcode_F3_InitReq, ProtoHandlers::HandleInitReq},
    {F3Opcode::F3Opcode_F3_HeartbeatReq, ProtoHandlers::HandleHeartbeatReq},
    {F3Opcode::F3Opcode_F3_SysInfoReq, ProtoHandlers::HandleSysInfoReq},
    {F3Opcode::F3Opcode_F3_SdInfoReq, ProtoHandlers::HandleSdInfoReq},
};

int ProtoHandlers::HandleInitReq(ProtoMessage *msg) {
    // don't even bother decoding if the RTC isn't enabled
    #if USE_RTC
        InitReqMessage rxMsg = InitReqMessage_init_zero;
        pb_istream_t instream = pb_istream_from_buffer(msg.buffer, msg.len);
        bool status = pb_decode(&instream, InitReqMessage_fields, &rxMsg);

        if (status) {
            
        }
    #endif

    InitAckMsg txMsg;
    txMsg.rtcStatus = USE_RTC;
    txBuf.len = InitAckMsg::size;
    bool x = ProtoMessage::GenericEncode<InitAckMsg>(&txMsg, &txBuf);
    return x ? 1 : 0;
}

int ProtoHandlers::HandleHeartbeatReq(ProtoMessage *msg) {
    return 0;
}

int ProtoHandlers::HandleSysInfoReq(ProtoMessage *msg) {
    return 0;
}

int ProtoHandlers::HandleSdInfoReq(ProtoMessage *msg) {
    return 0;
}
