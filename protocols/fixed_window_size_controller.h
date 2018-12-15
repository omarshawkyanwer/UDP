//
// Created by omar shawky on 2018-12-15.
//

#ifndef UDP_FIXED_WINDOW_SIZE_CONTROLLER_H
#define UDP_FIXED_WINDOW_SIZE_CONTROLLER_H

#include "congestion_control.h"

class fixed_window_size_controller: public congestion_control {
public:
    fixed_window_size_controller();
    int new_ack(int curr_size);
    int time_out(int curr_size);
    int duplicate_ack(int curr_size);
};


#endif //UDP_FIXED_WINDOW_SIZE_CONTROLLER_H
