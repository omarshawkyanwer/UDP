//
// Created by omar shawky on 2018-12-15.
//

#include "fixed_window_size_controller.h"

fixed_window_size_controller::fixed_window_size_controller(){}

int fixed_window_size_controller::new_ack(int curr_size) {
    return 1;
}
int fixed_window_size_controller::time_out(int curr_size) {
    return 1;
}
int fixed_window_size_controller::duplicate_ack(int curr_size) {
    return 1;
}