#include <utility>

#include "tcp_socket.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::deadline_timer;

tcp_socket::tcp_socket(const udp::endpoint &endpoint, udp::socket *socket) :
        timer_(io_service_) {
    tcp_socket::endpoint_ = endpoint;
    tcp_socket::socket_ = socket;

    tcp_socket::init();
}

void tcp_socket::init() {
    tcp_socket::timer_.expires_from_now(boost::posix_time::pos_infin);
    tcp_socket::check_timeout();

    boost::thread th(boost::bind(&boost::asio::io_service::run, &io_service_));
    th.detach();
}

void tcp_socket::send_callback(const boost::system::error_code &ec, std::size_t,
                               long timeout_msec) {
    std::cout << "Setting timeout!\n";
    tcp_socket::timer_.expires_from_now(boost::posix_time::milliseconds(timeout_msec));
}

void tcp_socket::handle(tcp_packet &pkt, long timeout_msec) {
    tcp_packet pkt_cpy{};
    memcpy(&pkt_cpy, &pkt, sizeof(pkt));

    tcp_packet pkt_to_send{};

    if (pkt_cpy.ack_no != tcp_socket::expected_ack_no)
        pkt_to_send = tcp_socket::last_pkt;
    else
        ; // TODO: Fill in packet member variables.

    tcp_socket::last_pkt = pkt_to_send;

    boost::system::error_code ec;

    pkt_to_send = pkt_cpy; //TODO: Remove: testing

    tcp_socket::socket_->async_send_to(boost::asio::buffer(&pkt_to_send,
            sizeof(pkt_to_send)), tcp_socket::endpoint_,
                boost::bind(&tcp_socket::send_callback, this,
                        boost::asio::placeholders::error(),
                        boost::asio::placeholders::bytes_transferred(),
                        timeout_msec));

    if (ec)
        std::cerr << "Error sending!\n"; // Handle sending error
}

void tcp_socket::check_timeout() {
    if (timer_.expires_at() <= deadline_timer::traits_type::now()) {
        // socket_.cancel();
        std::cout << "Timeout reached!" << std::endl;
        timer_.expires_at(boost::posix_time::pos_infin);
    }
    timer_.async_wait(boost::bind(&tcp_socket::check_timeout, this));
}
