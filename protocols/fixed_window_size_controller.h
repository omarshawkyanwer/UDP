//
// Created by omar shawky on 2018-12-15.
//

#ifndef UDP_FIXED_WINDOW_SIZE_CONTROLLER_H
#define UDP_FIXED_WINDOW_SIZE_CONTROLLER_H

#include "congestion_control.h"

class fixed_window_size_controller: public congestion_control {
public:
    fixed_window_size_controller();
    uint32_t new_ack(uint32_t curr_size);
    uint32_t time_out(uint32_t curr_size);
    uint32_t duplicate_ack(uint32_t curr_size);
};


#endif //UDP_FIXED_WINDOW_SIZE_CONTROLLER_H
