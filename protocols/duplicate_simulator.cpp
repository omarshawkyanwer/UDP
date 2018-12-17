//
// Created by ahmed on 17/12/18.
//
#include "duplicate_simulator.h"
#include <iostream>
#include <fstream>

duplicate_simulator::duplicate_simulator(congestion_control *kernel, const char *file_name) {
    this->kernel = kernel;
    this->file_name = file_name;
    this->read_file();
    this->packets = 0;
    this->chckpt_cur = 0;
}
void duplicate_simulator::read_file() {
    std::ifstream file_reader_stream;
    file_reader_stream.open(file_name);
    char elem[10];
    this->chckpt_cnt = 0;
    while(!file_reader_stream.eof()) {
        file_reader_stream >> elem;
        checkpoints[chckpt_cnt++]=atoi(elem);
    }
}
uint32_t duplicate_simulator::new_ack(uint32_t curr_size) {

    if(this->packets == this->checkpoints[chckpt_cur])
        return this->duplicate_ack(curr_size);
    this->packets++;
    return kernel->new_ack(curr_size);
}
uint32_t duplicate_simulator::time_out(uint32_t curr_size) {
    this->packets = 0;
    return kernel->time_out(curr_size);
}
uint32_t duplicate_simulator::duplicate_ack(uint32_t curr_size) {
    this->packets = 0;
    this->chckpt_cur = (this->chckpt_cur+1)%chckpt_cnt;
    uint32_t toret = kernel->duplicate_ack(curr_size);
    std::cout<<"reducing "<<curr_size<<"to "<<toret<<std::endl;
    return toret;
}