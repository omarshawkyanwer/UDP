#ifndef RELIABLE_DATA_TRANSFER_TYPES_H
#define RELIABLE_DATA_TRANSFER_TYPES_H
#define PACKET_LENGTH 500
#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))
#define SET_BIT(var, pos) ((var) |= (1<<(pos)))

#include <cstdint>
#include <iostream>
#include <bitset>

struct tcp_packet {
    /* Headers */
    uint16_t src_port, dest_port;
    uint32_t seq_no;
    uint32_t ack_no;
    uint32_t data_size;

    uint16_t flags;                 // headlen|unused|U|A|P|R|S|F
    uint16_t recvw;                 // receive window (#bytes recv willing to accept)
    uint16_t checksum, urg_data_ptr;

    /* Data */
    char data[PACKET_LENGTH];
};

static void print_pkt(tcp_packet pkt) {
    std::cout << "------ Packet: ------\n";
    std::cout << "src port: " << pkt.src_port << std::endl;
    std::cout << "dest port: " << pkt.dest_port << std::endl;
    std::cout << "seq_no: " << pkt.seq_no << std::endl;
    std::cout << "ack_no: " << pkt.ack_no << std::endl;
    std::cout << "flags: " << std::bitset<8>(pkt.flags).to_string() << std::endl;
//    std::cout << "chksum: " << pkt.checksum << std::endl;
    std::cout << "data: " << pkt.data << std::endl;
}

#endif //RELIABLE_DATA_TRANSFER_TYPES_H
