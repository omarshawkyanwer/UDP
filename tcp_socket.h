#ifndef UDP_WORKER_THREAD_H
#define UDP_WORKER_THREAD_H


#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "types.h"
#include "transmission.h"

using namespace boost::asio::ip;

class tcp_socket {
    enum connection_state {
        INITIALIZED, //waiting for handshake
        LISTENING, //starting handshake
        SYN_RECVD,
        ESTABLISHED
    };

public:
    tcp_socket(const udp::endpoint &, const udp::endpoint &,
            udp::socket *, transmission_protocol *protocol);
    void listen();
    void open();
    int send(char bytes[], int len);
    int recieve(char bytes[],int len);
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

    void segmenize(char bytes[],int len);

private:
    static uint32_t  MSS;

    udp::endpoint listening_endpoint_;
    udp::endpoint endpoint_;
    udp::socket *socket_;
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer_;
    transmission_protocol *protocol_;
    tcp_packet packets_to_send[300];
    int no_of_packets;
    connection_state cur_state;
    uint32_t seq_no = 0;
    std::map<uint32_t, boost::asio::deadline_timer> packet_timer_map;
    std::map<uint32_t, tcp_packet> seq_window;
    std::map<uint32_t, tcp_packet> ack_window;

    const int CHUNK_SIZE = 500;
};
#endif //UDP_WORKER_THREAD_H
