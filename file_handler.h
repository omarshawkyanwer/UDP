//
// Created by omar shawky on 2018-12-14.
//

#ifndef UDP_FILE_HANDLER_H
#define UDP_FILE_HANDLER_H

#include <string>
#include <fstream>


class file_handler {
private:
    void read_file(std::string file_name);
    bool is_last_send(int len);
public:
    file_handler(std::string file_name);
    int get_next_bytes(char* stream,int len);

private :
    int offset;
    int total_len;
    char* data;
};


#endif //UDP_FILE_HANDLER_H
