//
// Created by anas_harby on 12/6/18.
//

#ifndef UDP_WORKER_THREAD_H
#define UDP_WORKER_THREAD_H


#include <boost/asio.hpp>
#include "types.h"
using namespace boost::asio::ip;

class tcp_socket {
    enum state {
        ESTABLISHING,
        LISTENING
    };

public:
    tcp_socket(udp::endpoint);
    void handle_request(tcp_packet &, boost::posix_time::time_duration);
    void set_timer_expires_from_now(boost::posix_time::time_duration);

private:
    void check_timeout();

private:
    udp::endpoint endpoint_;
    boost::asio::io_service io_service_;
    udp::socket socket_;
    boost::asio::deadline_timer timer_;
    tcp_packet last_pkt;

    uint32_t expected_ack_no;
    uint32_t cur_seq_no;
};
#endif //UDP_WORKER_THREAD_H
