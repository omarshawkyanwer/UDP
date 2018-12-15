//
// Created by omar shawky on 2018-12-15.
//

#include "stop_and_wait.h"


stop_and_wait::stop_and_wait(udp::socket *socket,const udp::endpoint &endpoint):

        selective_repeat(socket,endpoint) {
    window_base = 1;
    controller = new fixed_window_size_controller();
}