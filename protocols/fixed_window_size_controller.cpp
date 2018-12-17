//
// Created by omar shawky on 2018-12-15.
//

#include "fixed_window_size_controller.h"

fixed_window_size_controller::fixed_window_size_controller(){}

uint32_t fixed_window_size_controller::new_ack(uint32_t curr_size) {
    return 1;
}
uint32_t fixed_window_size_controller::time_out(uint32_t curr_size) {
    return 1;
}
uint32_t fixed_window_size_controller::duplicate_ack(uint32_t curr_size) {
    return 1;
}