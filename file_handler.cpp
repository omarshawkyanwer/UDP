//
// Created by omar shawky on 2018-12-14.
//

#include "file_handler.h"
#include <stdio.h>
#include "file_handler.h"
#include <iostream>
#include <cstring>
file_handler::file_handler(std::string file_name){
    file_reader_stream.open(file_name.c_str());

}

int file_handler::get_next_bytes(char* stream,int len){
    int i;
    char c;
    std::string part_to_send = "";
    for( i = 0 ; i < len and not file_reader_stream.eof() ;++i){
        file_reader_stream.get(c);
        part_to_send.push_back(c);
    }

    if(file_reader_stream.eof()){
        file_reader_stream.close();
        //part_to_send.push_back('\0');
    }
    memcpy(stream,part_to_send.c_str(),i);
    return i;

}