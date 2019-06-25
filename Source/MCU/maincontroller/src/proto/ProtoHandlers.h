#ifndef PROTOHANDLERS_H
#define PROTOHANDLERS_H

#include <map>
#include <utility>
#include <functional>
#include <pb_decode.h>
#include <pb_encode.h>
#include <pb_common.h>

#include "../config.h"
#include "../common.h"
#include "FCubed.pb.h"
#include "ProtoMessage.h"

#include "messages\InitReqMsg.h"
#include "messages\InitAckMsg.h"
#include "messages\HeartbeatReqMsg.h"
#include "messages\HeartbeatAckMsg.h"

using HandlerFunction = std::function<int(ProtoMessage*)>;
using HandlerMapCall = std::map<F3Opcode, HandlerFunction>;

class ProtoHandlers {
public:
    static int HandleInitReq(ProtoMessage *msg);
    static int HandleHeartbeatReq(ProtoMessage *msg); // maybe run heartbeats automatically after InitReq?
    static int HandleSysInfoReq(ProtoMessage *msg);
    static int HandleSdInfoReq(ProtoMessage *msg);

    static const HandlerMapCall HandlerMap;
private:
    static ProtoMessage txBuf;
};
#endif /* PROTOHANDLERS_H */
