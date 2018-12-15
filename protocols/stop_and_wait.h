//
// Created by omar shawky on 2018-12-15.
//

#ifndef UDP_STOP_AND_WAIT_H
#define UDP_STOP_AND_WAIT_H
#include "selective_repeat.h"
#include "fixed_window_size_controller.h"

class stop_and_wait: public selective_repeat {
public:
    stop_and_wait(udp::socket *socket,const udp::endpoint &);
};


#endif //UDP_STOP_AND_WAIT_H
