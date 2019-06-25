/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 at Mon Jun 24 00:32:06 2019. */

#include "FCubed.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t InitReqMessage_fields[2] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, InitReqMessage, epoch, epoch, 0),
    PB_LAST_FIELD
};

const pb_field_t InitAckMessage_fields[2] = {
    PB_FIELD(  1, BOOL    , REQUIRED, STATIC  , FIRST, InitAckMessage, rtcStatus, rtcStatus, 0),
    PB_LAST_FIELD
};

const pb_field_t HeartbeatReqMessage_fields[2] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, HeartbeatReqMessage, loopNo, loopNo, 0),
    PB_LAST_FIELD
};

const pb_field_t HeartbeatAckMessage_fields[2] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, HeartbeatAckMessage, newLoopNo, newLoopNo, 0),
    PB_LAST_FIELD
};

const pb_field_t SysInfoReqMessage_fields[2] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, SysInfoReqMessage, null, null, 0),
    PB_LAST_FIELD
};

const pb_field_t SysInfoAckMessage_fields[3] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, SysInfoAckMessage, programVersion, programVersion, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, SysInfoAckMessage, cpuVersion, programVersion, 0),
    PB_LAST_FIELD
};

const pb_field_t SdInfoReqMessage_fields[2] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, SdInfoReqMessage, null, null, 0),
    PB_LAST_FIELD
};

const pb_field_t SdInfoAckMessage_fields[5] = {
    PB_FIELD(  1, BOOL    , REQUIRED, STATIC  , FIRST, SdInfoAckMessage, hasCard, hasCard, 0),
    PB_FIELD(  2, BOOL    , OPTIONAL, STATIC  , OTHER, SdInfoAckMessage, cardUsable, hasCard, 0),
    PB_FIELD(  3, UINT32  , OPTIONAL, STATIC  , OTHER, SdInfoAckMessage, capacity, cardUsable, 0),
    PB_FIELD(  4, UINT32  , OPTIONAL, STATIC  , OTHER, SdInfoAckMessage, freeSpace, capacity, 0),
    PB_LAST_FIELD
};



/* @@protoc_insertion_point(eof) */
