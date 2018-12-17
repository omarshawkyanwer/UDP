//
// Created by ahmed on 17/12/18.
//

#ifndef UDP_DUPLICATE_SIMULATOR_H
#define UDP_DUPLICATE_SIMULATOR_H

#include "congestion_control.h"

class duplicate_simulator : public congestion_control {
private:
    void read_file();
    const char *file_name;
    congestion_control *kernel;
    uint32_t packets;
    uint32_t checkpoints[1000];
    int chckpt_cnt;
    int chckpt_cur;
public:
    duplicate_simulator(congestion_control *kernel,const char* file_name);
    uint32_t new_ack(uint32_t curr_size);
    uint32_t time_out(uint32_t curr_size);
    uint32_t duplicate_ack(uint32_t curr_size);

};
#endif //UDP_DUPLICATE_SIMULATOR_H