//
// Created by ahmed on 14/12/18.
//

#ifndef UDP_SLOW_STARTER_H
#define UDP_SLOW_STARTER_H

#include "congestion_control.h"

class slow_starter : public congestion_control{
public:
    slow_starter(uint32_t threashold) {
        ssthreshold = threashold;
    }
    uint32_t new_ack(uint32_t curr_size);
    uint32_t time_out(uint32_t curr_size);
    uint32_t duplicate_ack(uint32_t curr_size);

private:
    uint32_t ssthreshold;
};
#endif //UDP_SLOW_STARTER_H
