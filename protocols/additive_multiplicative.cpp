//
// Created by ahmed on 14/12/18.
//
#include "additive_multiplicative.h"
#include <iostream>
int additive_multiplicative::new_ack(int curr_size) {
    return curr_size+CHUNK_SIZE;
}
int additive_multiplicative::time_out(int curr_size){

    return (curr_size > CHUNK_SIZE*2)?curr_size >>1 : CHUNK_SIZE;
}
int additive_multiplicative::duplicate_ack(int curr_size) {

    return (curr_size > CHUNK_SIZE*2)?curr_size >>1 : CHUNK_SIZE;
}