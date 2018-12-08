#ifndef UDP_STOP_WAIT_PROTOCOL_H
#define UDP_STOP_WAIT_PROTOCOL_H

#include "../transmission.h"

class selective_repeat : public transmission_protocol {
public:
    selective_repeat(udp::socket *, const udp::endpoint &);

    void send_data(std::map<uint32_t, tcp_packet>&) override;

    void handle_received_ack(tcp_packet &pkt) override;

private:
    void send_single(uint32_t);
    void send_callback(const boost::system::error_code &, uint32_t);
};


#endif //UDP_STOP_WAIT_PROTOCOL_H
