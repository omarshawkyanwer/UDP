//
// Created by ahmed on 05/12/18.
//
#include "types.h"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <boost/asio/buffer.hpp>

bool is_packet(char *str) {
    return !(str[3] & 1);
}

struct tcp_packet *from_str(char *str) { //TODO bug fix
    return reinterpret_cast<tcp_packet *>(str);
}
struct tcp_packet encaps(char *,int) { //TODO implement

}
std::size_t from_pkt(char *str, const tcp_packet &pkt) {//TODO bug fix
    std::ostringstream oss;
    oss << pkt.src_port << pkt.dest_port;
    oss << pkt.seq_no << pkt.ack_no;
    oss << pkt.flags << pkt.recvw;
    oss << pkt.checksum << pkt.urg_data_ptr;
    oss << pkt.data;

    strcpy(str, oss.str().c_str());
    return oss.str().size();
}