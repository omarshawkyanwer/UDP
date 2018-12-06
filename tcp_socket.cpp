#include "tcp_socket.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <thread>

using boost::asio::deadline_timer;

tcp_socket::tcp_socket(udp::endpoint endpoint) :
        socket_(io_service_, endpoint),
        timer_(io_service_) {
    tcp_socket::endpoint_ = endpoint;
    tcp_socket::timer_.expires_from_now(boost::posix_time::pos_infin);

    tcp_socket::check_timeout();

    std::thread th{[this]() { while(true) this->io_service_.run_one(); }};
    th.detach();
}

void send_callback(tcp_socket *s, boost::posix_time::time_duration *timeout) {
    s->set_timer_expires_from_now(*timeout);
}

void tcp_socket::handle_request(tcp_packet &pkt,
            boost::posix_time::time_duration timeout) {
    tcp_packet pkt_cpy{};
    memcpy(&pkt_cpy, &pkt, sizeof(pkt));

    tcp_packet pkt_to_send{};

    if (pkt_cpy.ack_no != tcp_socket::expected_ack_no)
        pkt_to_send = tcp_socket::last_pkt;
    else
        ; // TODO: Fill in packet members.

    tcp_socket::last_pkt = pkt_to_send;

    boost::system::error_code ec;

    //TODO: Remove: testing
    pkt_to_send = pkt_cpy;

    tcp_socket::socket_.async_send_to(boost::asio::buffer(&pkt_to_send,
            sizeof(pkt_to_send)), tcp_socket::endpoint_,
                boost::bind(&send_callback, this, &timeout));

    if (ec)
        std::cerr << "Error sending!\n"; // Handle sending error
}

void tcp_socket::set_timer_expires_from_now(boost::posix_time::time_duration timeout) {
    tcp_socket::timer_.expires_from_now(timeout);
}

void tcp_socket::check_timeout() {
    if (timer_.expires_at() <= deadline_timer::traits_type::now()) {
        // socket_.cancel();
        std::cout << "The timeout has been occurred" << std::endl;
        timer_.expires_at(boost::posix_time::pos_infin);
    }
    timer_.async_wait(boost::bind(&tcp_socket::check_timeout, this));
}