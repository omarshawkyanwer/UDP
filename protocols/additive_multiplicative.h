//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_ADDITIVE_MULTIPLICATIVE_H
#define UDP_ADDITIVE_MULTIPLICATIVE_H

#include "congestion_control.h"

class additive_multiplicative : congestion_control{
public:
    additive_multiplicative(){

    }
    int new_ack(int curr_size);
    int time_out(int curr_size);
    int duplicate_ack(int curr_size);
};
#endif //UDP_ADDITIVE_MULTIPLICATIVE_H
