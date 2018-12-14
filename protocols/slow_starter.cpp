//
// Created by ahmed on 14/12/18.
//
#include "slow_starter.h"
int slow_starter::new_ack(int curr_size){
    return (curr_size < ssthreshold)?curr_size*2:curr_size+1;
}
int slow_starter::time_out(int curr_size){
    ssthreshold/=2;
    return 1;

}
int slow_starter::duplicate_ack(int curr_size){
    ssthreshold/=2;
    return ssthreshold;
}