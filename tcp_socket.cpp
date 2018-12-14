#include <utility>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "tcp_socket.h"

using boost::asio::deadline_timer;

tcp_socket::tcp_socket(const udp::endpoint &listening_endpoint, const udp::endpoint &endpoint, udp::socket *socket,
                       transmission_protocol *protocol) :
        timer_(socket->get_io_service()),
        strand_(socket->get_io_service()) {
    tcp_socket::listening_endpoint_ = listening_endpoint;
    tcp_socket::endpoint_ = endpoint;
    tcp_socket::socket_ = socket;
    tcp_socket::protocol_ = protocol;

    tcp_socket::init();
}

void tcp_socket::init() {
    tcp_socket::cur_state = ESTABLISHED; //TODO: change to INITIALIZED

    tcp_socket::timer_.expires_from_now(boost::posix_time::pos_infin);
    tcp_socket::check_timeout();

    boost::thread th(boost::bind(&boost::asio::io_service::run, &socket_->get_io_service()));
    th.detach();
}

void tcp_socket::listen() {
    if (tcp_socket::cur_state != INITIALIZED)
        return;
    tcp_socket::cur_state = LISTENING;
}

void tcp_socket::open() {
    if (tcp_socket::cur_state != INITIALIZED)
        return;
    tcp_packet pkt_send = tcp_socket::make_pkt();
    /* Set SYN bit at pos 1 */
    SET_BIT(pkt_send.flags, 1);

//    tcp_socket::last_pkt = pkt_send;
    tcp_socket::socket_->async_send_to(boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
                                       tcp_socket::endpoint_, tcp_socket::strand_.wrap(boost::bind(
                    &tcp_socket::state_transition_callback, this,
                    boost::asio::placeholders::error(),
                    boost::asio::placeholders::bytes_transferred(),
                    SYN_RECVD, -1)));
}

void tcp_socket::send(char bytes[], int len) {
    
}


 tcp_packet tcp_socket::make_pkt() {
    tcp_packet pkt{};
    pkt.src_port = tcp_socket::listening_endpoint_.port();
    pkt.dest_port = tcp_socket::endpoint_.port();
    return pkt;
}

void tcp_socket::send_callback(const boost::system::error_code &ec, std::size_t,
                               long timeout_msec) {
    std::cout << "Setting timeout_msec!\n";
    tcp_socket::timer_.expires_from_now(boost::posix_time::milliseconds(timeout_msec));
}

void tcp_socket::handle_received(tcp_packet &pkt, long timeout_msec) {
    switch (tcp_socket::cur_state) {
        case INITIALIZED:
            tcp_socket::cur_state = LISTENING;
            tcp_socket::handle_received(pkt, timeout_msec);
            break;
        case LISTENING:
            tcp_socket::handle_on_listen(pkt, timeout_msec);
            break;
        case SYN_RECVD:
            tcp_socket::handle_on_syn_recvd(pkt, timeout_msec);
            break;
        case ESTABLISHED:
            tcp_socket::handle_on_established(pkt, timeout_msec);
            break;
        default: break;
    }
}

void tcp_socket::state_transition_callback(const boost::system::error_code &ec,
        std::size_t, enum connection_state next_state, long timeout_msec) {
    if (timeout_msec != -1)
        tcp_socket::timer_.expires_from_now(boost::posix_time::milliseconds(timeout_msec));
    tcp_socket::cur_state = next_state;
}

void tcp_socket::handle_on_listen(tcp_packet &pkt, long timeout_msec) {
    /* Check for SYN flag at position 1 */
    bool syn = CHECK_BIT(pkt.flags, 1) != 0;
    if (!syn)
        return;

    /* Create SYN_ACK packet */
    tcp_packet pkt_send = tcp_socket::make_pkt();
    SET_BIT(pkt_send.flags, 1); /* Set SYN flag */
    SET_BIT(pkt_send.flags, 4); /* Set ACK flag */

    tcp_socket::socket_->async_send_to(boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
            tcp_socket::endpoint_, tcp_socket::strand_.wrap(boost::bind(
                    &tcp_socket::state_transition_callback, this,
                    boost::asio::placeholders::error(),
                    boost::asio::placeholders::bytes_transferred(),
                    SYN_RECVD, timeout_msec)));
}

void tcp_socket::handle_on_syn_recvd(tcp_packet &pkt, long) {
    /* Check for ack flag at position 4 */
    bool ack = CHECK_BIT(pkt.flags, 4) != 0;
    if (!ack)
        return;

    tcp_socket::cur_state = ESTABLISHED;
}

void tcp_socket::handle_on_established(tcp_packet &pkt, long timeout_msec) {
    /* Check for ack flag at position 4 */
    bool ack = CHECK_BIT(pkt.flags, 4) != 0;
    if (ack)
        tcp_socket::protocol_->handle_received_ack(pkt);
    else
        tcp_socket::protocol_->handle_received_data(pkt);
}

void tcp_socket::check_timeout() {
    if (timer_.expires_at() <= deadline_timer::traits_type::now()) {
        // socket_.cancel();
        std::cout << "Timeout reached!" << std::endl;
        timer_.expires_at(boost::posix_time::pos_infin);
    }
    timer_.async_wait(boost::bind(&tcp_socket::check_timeout, this));
}
