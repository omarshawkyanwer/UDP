#ifndef UDP_WORKER_THREAD_H
#define UDP_WORKER_THREAD_H


#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "types.h"
#include "transmission.h"
#include <iostream>

using namespace boost::asio::ip;

class tcp_socket {
    enum connection_state {
        INITIALIZED,     // Waiting for handshake
        LISTENING,       // Starting handshake
        SYN_RECVD,
        SYN_SENT,
        ESTABLISHED,
        CLOSING,        // Either starting handshake or waiting for it
        CLOSED
    };

public:
    tcp_socket(const udp::endpoint &, const udp::endpoint &,
            udp::socket *, transmission_protocol *protocol);
    void listen();
    void open();
    void close();
    void send(char bytes[], int len);
    void handle_received(tcp_packet &pkt, long timeout_msec);
    size_t received();
    void set_buffer(char *, uint32_t, uint32_t);

private:

    void init();
    tcp_packet make_pkt();

    /* State handlers */
    void state_transition_callback(const boost::system::error_code &,
            std::size_t, enum connection_state, long);
    void handle_on_listen(tcp_packet &, long);
    void handle_on_syn_recvd(tcp_packet &, long);
    void handle_on_syn_sent(tcp_packet &, long);
    void handle_on_established(tcp_packet &, long);
    void handle_on_terminate(tcp_packet &, long);
    void handle_data(tcp_packet &, long);
    void check_timeout();

private:
    udp::endpoint listening_endpoint_;
    udp::endpoint endpoint_;
    udp::socket *socket_;
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer_;
    transmission_protocol *protocol_;
    connection_state cur_state;

    char *buff;
    uint32_t offset;
    uint32_t maxlen;

    const int CHUNK_SIZE = 10;//= 500;
};
#endif //UDP_WORKER_THREAD_H
