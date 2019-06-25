#ifndef HEARTBEATREQMSG_H
#define HEARTBEATREQMSG_H

#include "../FCubed.pb.h"
#include "../../common.h"

class HeartbeatReqMsg : public HeartbeatReqMessage {
public:

    HeartbeatReqMsg() {
        *this = init_zero;
    }

    HeartbeatReqMsg(HeartbeatReqMessage msg) {
        *this = msg;
    }

    static constexpr const pb_field_t * fields = HeartbeatReqMessage_fields;
    static constexpr const size_t size = HeartbeatReqMessage_size;
    static constexpr const HeartbeatReqMessage init_default = HeartbeatReqMessage_init_default;
    static constexpr const HeartbeatReqMessage init_zero = HeartbeatReqMessage_init_zero;
};

#endif /* HEARTBEATREQMSG_H */
