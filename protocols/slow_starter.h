//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_SLOW_STARTER_H
#define UDP_SLOW_STARTER_H

#include "congestion_control.h"

class slow_starter : congestion_control{
public:
    slow_starter(int threashold) {
        ssthreshold = threashold;
    }
    int new_ack(int curr_size);
    int time_out(int curr_size);
    int duplicate_ack(int curr_size);

private:
    int ssthreshold;
};
#endif //UDP_SLOW_STARTER_H
