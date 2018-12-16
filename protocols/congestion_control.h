//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_CONGESTION_CONTROL_H
#define UDP_CONGESTION_CONTROL_H

#include "../types.h"
class congestion_control {
public:
    virtual  int new_ack(int curr_size)=0;
    virtual  int time_out(int curr_size)=0;
    virtual int duplicate_ack(int curr_size)=0;

};
#endif //UDP_CONGESTION_CONTROL_H
