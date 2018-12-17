//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_ADDITIVE_MULTIPLICATIVE_H
#define UDP_ADDITIVE_MULTIPLICATIVE_H

#include "congestion_control.h"

class additive_multiplicative : public congestion_control{
public:
    additive_multiplicative(){

    }
    uint32_t new_ack(uint32_t curr_size);
    uint32_t time_out(uint32_t curr_size);
    uint32_t duplicate_ack(uint32_t curr_size);
};
#endif //UDP_ADDITIVE_MULTIPLICATIVE_H
