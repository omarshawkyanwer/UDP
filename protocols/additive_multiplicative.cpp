//
// Created by ahmed on 14/12/18.
//
#include "additive_multiplicative.h"
#include <iostream>
uint32_t additive_multiplicative::new_ack(uint32_t curr_size) {
    return curr_size+CHUNK_SIZE;
}
uint32_t additive_multiplicative::time_out(uint32_t curr_size){

    return (curr_size > CHUNK_SIZE*2)?curr_size >>1 : CHUNK_SIZE;
}
uint32_t additive_multiplicative::duplicate_ack(uint32_t curr_size) {

    return (curr_size > CHUNK_SIZE*2)?curr_size >>1 : CHUNK_SIZE;
}