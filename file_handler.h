//
//  file_handler.h
//  chuncking
//
//  Created by mac on 12/8/18.
//  Copyright © 2018 mac. All rights reserved.
//

#ifndef file_handler_h
#define file_handler_h
#include <string>
#include <fstream>


class file_handler{
private:
    void read_file(std::string file_name);
    bool is_last_send(int len);
public:
    file_handler(std::string file_name);
    bool get_next_bytes(char* stream,int len);
    
private :
    int offset;
    int total_len;
    char* data;
};

#endif /* file_handler_h */
