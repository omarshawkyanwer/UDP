#ifndef RELIABLE_DATA_TRANSFER_TYPES_H
#define RELIABLE_DATA_TRANSFER_TYPES_H
#define PACKET_LENGTH 500
#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))
#define SET_BIT(var, pos) ((var) |= (1<<(pos)))

#include <cstdint>
#include <iostream>
struct tcp_packet *from_str(char *str);
struct tcp_packet encaps(char *,int);
std::size_t from_pkt(char *str, const tcp_packet &pkt);


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

#endif //RELIABLE_DATA_TRANSFER_TYPES_H
