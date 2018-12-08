#ifndef UDP_TRANSMISSION_H
#define UDP_TRANSMISSION_H

#include <boost/asio/ip/udp.hpp>
#include <map>
#include <boost/asio/deadline_timer.hpp>
#include "types.h"
using namespace boost::asio::ip;

class transmission_protocol {
public:
    explicit transmission_protocol(udp::socket *socket,
            const udp::endpoint &endpoint) {
        this->socket_ = socket;
        this->endpoint_ = endpoint;
    }
    virtual void send_data(std::map<uint32_t, tcp_packet>&) = 0;
    virtual void handle_received_ack(tcp_packet &) = 0;

    void send_ack(uint32_t ack_no) {

    }

    void handle_received_data(tcp_packet &pkt) {

    }

protected:
    boost::asio::ip::udp::socket *socket_;
    boost::asio::ip::udp::endpoint endpoint_;

    long timeout_msec = 5000;
    int window_size = 0;
    int window_base = 0;
    uint32_t next_seq_no = 0;
    std::map<uint32_t, boost::asio::deadline_timer *> packet_timer_map;
    std::map<uint32_t, tcp_packet> pkts_to_send;
    std::map<uint32_t, tcp_packet> sender_window;

};
#endif //UDP_TRANSMISSION_H
