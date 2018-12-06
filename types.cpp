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

struct tcp_packet *from_str(char *str) {
    return reinterpret_cast<tcp_packet *>(str);
}

std::size_t from_pkt(char *str, const tcp_packet &pkt) {
    std::ostringstream oss;
    oss << pkt.src_port << pkt.dest_port;
    oss << pkt.seq_no << pkt.ack_no;
    oss << pkt.flags << pkt.recvw;
    oss << pkt.checksum << pkt.urg_data_ptr;
    oss << pkt.data;

    strcpy(str, oss.str().c_str());
    return oss.str().size();
}

struct packet str_to_packet(char *str) {
    uint32_t flags = 0;
    for (int i = 0; i < 4; i++) {
        flags <<= 8;
        flags += str[i];
    }
    str = str + 4;
    uint16_t checksum = (uint16_t) str[0];
    checksum <<= 8;
    checksum += str[1];
    uint16_t len = (uint16_t) str[2];
    len <<= 8;
    len += str[3];
    char *seqch = str + 4;
    uint32_t seqno = 0;
    for (int i = 0; i < 4; i++) {
        seqno <<= 8;
        seqno += seqch[i];
    }
    struct packet ret;
    ret.flags = flags;
    ret.cksum = checksum;
    ret.len = len;
    ret.seqno = seqno;
    strncpy(ret.data, str + 8, len);

    return ret;
}

struct ack_packet str_to_ack(char *str) {
    uint32_t flags = 0;
    for (int i = 0; i < 4; i++) {
        flags <<= 8;
        flags += str[i];
    }
    str = str + 4;
    uint16_t checksum = (uint16_t) str[0];
    checksum <<= 8;
    checksum += str[1];
    uint16_t len = (uint16_t) str[2];
    len <<= 8;
    len += str[3];
    char *seqch = str + 4;
    uint32_t seqno = 0;
    for (int i = 0; i < 4; i++) {
        seqno <<= 8;
        seqno += seqch[i];
    }
    struct ack_packet ret;
    ret.cksum = checksum;
    ret.len = len;
    ret.ackno = seqno;
    return ret;
}

char *packet_to_str(struct packet pack) {
    uint16_t chcksum = pack.cksum;
    uint16_t len = pack.len;
    char *str = (char *) malloc(sizeof(pack));
    str[0] = str[1] = str[2] = str[3] = 0;
    str = str + 4;
    str[0] = (char) (chcksum >> 8);
    str[1] = (char) (chcksum & 0xFF);
    str[2] = (char) (len >> 8);
    str[3] = (char) (len & 0xFF);
    uint32_t seqno = pack.seqno;
    for (int i = 7; i > 3; i--) {
        str[i] = (char) (seqno & 0xFF);
        seqno >>= 8;
    }
    strcpy(str + 8, pack.data);
    return str - 4;

}

char *ack_to_str(struct ack_packet pack) {
    uint16_t chcksum = pack.cksum;
    uint16_t len = pack.len;
    char *str = (char *) malloc(sizeof(pack));
    str[0] = str[1] = str[2] = 0;
    str[3] = 1;
    str = str + 4;
    str[0] = (char) (chcksum >> 8);
    str[1] = (char) (chcksum & 0xFF);
    str[2] = (char) (len >> 8);
    str[3] = (char) (len & 0xFF);
    uint32_t seqno = pack.ackno;
    for (int i = 7; i > 3; i--) {
        str[i] = (char) (seqno & 0xFF);
        seqno >>= 8;
    }
    return str - 4;
}

struct packet encaps(char *msg, uint16_t len, int index) {
    struct packet new_pack;
    new_pack.len = len;
    new_pack.flags = 0;
    strncpy(new_pack.data, msg, len);

    uint32_t cksum = 0;
    for (int i = 0; i < len; i++) {
        cksum += msg[i];
        uint16_t t = (uint16_t) (cksum >> 8);
        cksum &= 0xFFFF;
        cksum += t;
    }
    new_pack.cksum = (uint16_t) (cksum & 0xFFFF);
    new_pack.seqno = (uint16_t) index;
    return new_pack;

}