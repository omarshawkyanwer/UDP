//
// Created by ahmed on 14/12/18.
//
#include "additive_multiplicative.h"

int additive_multiplicative::new_ack(int curr_size) {
    return curr_size+500;
}
int additive_multiplicative::time_out(int curr_size) {
    return curr_size >>1;
}
int additive_multiplicative::duplicate_ack(int curr_size) {
    return curr_size>>1;
}