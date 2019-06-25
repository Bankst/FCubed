#ifndef INITACKMSG_H
#define INITACKMSG_H

#include "../FCubed.pb.h"
#include "../../common.h"

class InitAckMsg : public InitAckMessage {
public:

    InitAckMsg() {
        *this = init_zero;
    }

    InitAckMsg(InitAckMessage msg) {
        *this = msg;
    }

    static constexpr const pb_field_t * fields = InitAckMessage_fields;
    static constexpr size_t size = InitAckMessage_size;
    static constexpr InitAckMessage init_default = InitAckMessage_init_default;
    static constexpr InitAckMessage init_zero = InitAckMessage_init_zero;
};
#endif /* INITACKMSG_H */
