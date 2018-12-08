//
//  file_hander.cpp
//  chuncking
//
//  Created by mac on 12/8/18.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include "file_handler.h"
#include <iostream>

file_handler::file_handler(std::string file_name){
    read_file(file_name);
    offset = 0;
}
void file_handler::read_file(std::string file_name){
    std::ifstream is;
    is.open(file_name.c_str());
    is.seekg(0,std::ios::end);
    total_len = is.tellg();
    is.seekg(0,std::ios::beg);
    data = new char[total_len];
    is.read(data,total_len);
    is.close();
}

bool file_handler::get_next_bytes(char* stream,int len){
    len = std::min(len,total_len - offset);
    memcpy(stream, data + offset, len);
    if(is_last_send(len)){
        stream[len] = '\0';
        delete[] data;
        return false;
    }
    offset += len;
    return true;

}

bool file_handler::is_last_send(int len){
    //the indexing is zero-based
    return ((offset + len) >= total_len);
}
