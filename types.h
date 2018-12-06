//
// Created by ahmed on 03/12/18.
//

#ifndef RELIABLE_DATA_TRANSFER_TYPES_H
#define RELIABLE_DATA_TRANSFER_TYPES_H
#define PACKET_LENGTH 500

#include <cstdint>
#include <iostream>

struct tcp_packet {
    /* Headers */
    uint16_t src_port, dest_port;
    uint32_t seq_no;
    uint32_t ack_no;

    uint16_t flags;                 // headlen|unused|U|A|P|R|S|F
    uint16_t recvw;                 // receive window (#bytes recv willing to accept)
    uint16_t checksum, urg_data_ptr;

    /* Data */
    char data[PACKET_LENGTH];
};

static void print_pkt(tcp_packet pkt) {
    std::cout << "------ Packet: ------\n";
    std::cout << pkt.src_port << std::endl;
    std::cout << pkt.dest_port << std::endl;
    std::cout << pkt.seq_no << std::endl;
    std::cout << pkt.ack_no << std::endl;
    std::cout << pkt.flags << std::endl;
    std::cout << pkt.urg_data_ptr << std::endl;
    std::cout << pkt.checksum << std::endl;
    std::cout << pkt.recvw << std::endl;
    std::cout << pkt.data << std::endl;
}

struct packet {
    /* Header */
    uint32_t flags;
    uint16_t cksum;
    uint16_t len;
    uint32_t seqno;
    /* Data */
    char data[PACKET_LENGTH]; /* Not always 500 bytes, can be less */
};

struct ack_packet {
    uint32_t flags;
    uint16_t cksum;
    uint16_t len;
    uint32_t ackno;
};

std::size_t from_pkt(char *str, const tcp_packet &pkt);

bool is_packet(char *);

struct packet str_to_packet(char *);

struct ack_packet str_to_ack(char *);

char *packet_to_str(struct packet);

char *ack_to_str(struct ack_packet);

struct packet encaps(char *msg, uint16_t len, int index);

#endif //RELIABLE_DATA_TRANSFER_TYPES_H
