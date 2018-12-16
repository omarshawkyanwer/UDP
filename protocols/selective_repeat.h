#ifndef UDP_STOP_WAIT_PROTOCOL_H
#define UDP_STOP_WAIT_PROTOCOL_H

#include "../transmission.h"
#include "congestion_control.h"
#include <ctime>
#include <vector>
#include <utility>

class selective_repeat : public transmission_protocol {
public:
    selective_repeat(udp::socket *, const udp::endpoint &);
    void send_data(std::map<uint32_t, tcp_packet>&) override;
    void handle_received_ack(tcp_packet &pkt) override;
    void send_callback(const boost::system::error_code &, uint32_t);

protected:
    int window_base = 0;
    congestion_control *controller;
    std::clock_t sending_start_time_,sending_end_time_;
    std::vector< std::pair< double ,int> > window_size_at_time_t;
    double total_sending_duration = 0;

private:
    uint32_t next_seq_no = 0;
    std::map<uint32_t, boost::asio::deadline_timer *> packet_timer_map;
    std::map<uint32_t, tcp_packet> pkts_to_send;
    std::map<uint32_t, tcp_packet> sender_window;
    void handle_timeout(uint32_t);
    void send_single(uint32_t);
};


#endif //UDP_STOP_WAIT_PROTOCOL_H
