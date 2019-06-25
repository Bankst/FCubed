#ifndef INITREQMSG_H
#define INITREQMSG_H

#include "../FCubed.pb.h"
#include "../../common.h"

class InitReqMsg : public InitReqMessage {
public:

    InitReqMsg() {
        *this = init_zero;
    }

    InitReqMsg(InitReqMessage msg) {
        *this = msg;
    }

    static constexpr const pb_field_t * fields = InitReqMessage_fields;
    static constexpr const size_t size = InitReqMessage_size;
    static constexpr const InitReqMessage init_default = InitReqMessage_init_default;
    static constexpr const InitReqMessage init_zero = InitReqMessage_init_zero;
};

#endif /* INITREQMSG_H */
