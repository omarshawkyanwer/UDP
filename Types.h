//
// Created by ahmed on 03/12/18.
//

#ifndef RELIABLE_DATA_TRANSFER_TYPES_H
#define RELIABLE_DATA_TRANSFER_TYPES_H
#define PACKET_LENGTH 500
#include <cstdint>

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

bool is_packet(char *);
struct packet str_to_packet(char *);
struct ack_packet str_to_ack(char*);
char * packet_to_str(struct packet);
char * ack_to_str(struct ack_packet);
struct packet encaps(char *msg,uint16_t len,int index);

#endif //RELIABLE_DATA_TRANSFER_TYPES_H
