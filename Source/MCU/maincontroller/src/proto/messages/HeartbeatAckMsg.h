#ifndef HEARTBEATACKMSG_H
#define HEARTBEATACKMSG_H

#include "../FCubed.pb.h"
#include "../../common.h"

class HeartbeatAckMsg : public HeartbeatAckMessage {
public:

    HeartbeatAckMsg() {
        *this = init_zero;
    }

    HeartbeatAckMsg(HeartbeatAckMessage msg) {
        *this = msg;
    }

    static constexpr const pb_field_s * fields = HeartbeatAckMessage_fields;
    static constexpr const size_t size = HeartbeatAckMessage_size;
    static constexpr const HeartbeatAckMessage init_default = HeartbeatAckMessage_init_default;
    static constexpr const HeartbeatAckMessage init_zero = HeartbeatAckMessage_init_zero;
};

#endif /* HEARTBEATACKMSG_H */
