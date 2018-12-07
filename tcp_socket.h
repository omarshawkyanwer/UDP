//
// Created by anas_harby on 12/6/18.
//

#ifndef UDP_WORKER_THREAD_H
#define UDP_WORKER_THREAD_H


#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "types.h"
using namespace boost::asio::ip;

class tcp_socket {
    enum state {
        ESTABLISHING,
        LISTENING
    };

public:
    tcp_socket(const udp::endpoint &, udp::socket *);
    void handle(tcp_packet &, long);

private:
    void init();
    void check_timeout();
    void send_callback(const boost::system::error_code &, std::size_t, long);

private:
    udp::endpoint endpoint_;
    udp::socket *socket_;
    boost::asio::io_service io_service_;
    boost::asio::deadline_timer timer_;

    tcp_packet last_pkt;
    uint32_t expected_ack_no;
    uint32_t cur_seq_no;
};
#endif //UDP_WORKER_THREAD_H
