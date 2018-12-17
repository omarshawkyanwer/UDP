//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_CONGESTION_CONTROL_H
#define UDP_CONGESTION_CONTROL_H

#include "../types.h"
class congestion_control {
public:
    virtual  uint32_t new_ack(uint32_t curr_size)=0;
    virtual  uint32_t time_out(uint32_t curr_size)=0;
    virtual uint32_t duplicate_ack(uint32_t curr_size)=0;

};
#endif //UDP_CONGESTION_CONTROL_H
