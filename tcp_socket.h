#ifndef UDP_WORKER_THREAD_H
#define UDP_WORKER_THREAD_H


#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "types.h"
using namespace boost::asio::ip;

class tcp_socket {
    enum connection_state {
        INITIALIZED,
        LISTENING,
        SYN_RECVD,
        SYN_SENT,
        ESTABLISHED
    };

public:
    tcp_socket(const udp::endpoint &, const udp::endpoint &, udp::socket *);
    void listen();
    void open();
    void handle_received(tcp_packet &pkt, long timeout_msec);

private:
    void init();
    tcp_packet make_pkt();

    /* State handlers */
    void state_transition_callback(const boost::system::error_code &,
            std::size_t, enum connection_state, long);
    void handle_on_listen(tcp_packet &, long);
    void handle_on_syn_recvd(tcp_packet &, long);
    void handle_on_established(tcp_packet &, long);

    void check_timeout();
    void send_callback(const boost::system::error_code &, std::size_t, long);

private:
    udp::endpoint listening_endpoint_;
    udp::endpoint endpoint_;
    udp::socket *socket_;
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer_;

    tcp_packet last_pkt;
    uint32_t expected_ack_no;
    uint32_t cur_seq_no;
    connection_state cur_state;
};
#endif //UDP_WORKER_THREAD_H
