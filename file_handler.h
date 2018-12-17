#ifndef UDP_FILE_HANDLER_H
#define UDP_FILE_HANDLER_H

#include <string>
#include <fstream>


class file_handler {
public:
    file_handler(std::string file_name);
    int get_next_bytes(char* stream,int len);

private :

    std::ifstream file_reader_stream;
};


#endif //UDP_FILE_HANDLER_H