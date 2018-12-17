//
// Created by ahmed on 14/12/18.
//
#include "slow_starter.h"
uint32_t slow_starter::new_ack(uint32_t curr_size){
    return (curr_size < ssthreshold)?curr_size*2:curr_size+CHUNK_SIZE;
}
uint32_t slow_starter::time_out(uint32_t curr_size){
    ssthreshold = curr_size;
    ssthreshold/=2;
    return CHUNK_SIZE;
}

uint32_t slow_starter::duplicate_ack(uint32_t curr_size){
    ssthreshold = curr_size;

    ssthreshold/=2;
    return ssthreshold;
}