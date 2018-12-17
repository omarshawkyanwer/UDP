//
// Created by ahmed on 14/12/18.
//
#include "slow_starter.h"
#include <algorithm>
#include <iostream>
#include <string>
uint32_t slow_starter::new_ack(uint32_t curr_size){


    return (curr_size < ssthreshold)?curr_size*2:curr_size+CHUNK_SIZE;
}
uint32_t slow_starter::time_out(uint32_t curr_size){
    if(curr_size > INT32_MAX>>1)
        curr_size = INT32_MAX>>1;
    ssthreshold = curr_size;
    ssthreshold = std::max(ssthreshold/2,(uint32_t)CHUNK_SIZE);
    return CHUNK_SIZE;
}

uint32_t slow_starter::duplicate_ack(uint32_t curr_size){
    if(curr_size > INT32_MAX>>1)
        curr_size = INT32_MAX>>1;
    ssthreshold = curr_size;

    ssthreshold = std::max(ssthreshold/2,(uint32_t)CHUNK_SIZE);

    return ssthreshold;
}