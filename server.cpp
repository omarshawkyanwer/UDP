#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "types.h"
#include "tcp_socket.h"

using boost::asio::ip::udp;
using boost::asio::deadline_timer;

class server {
public:
    explicit server(const udp::endpoint& listen_endpoint) :
                socket_(io_service_, listen_endpoint),
                timer_(io_service_) {
        timeout = boost::posix_time::seconds(5);
        timer_.expires_from_now(boost::posix_time::pos_infin);
        check_timeout();
    }

    std::size_t send(const tcp_packet &pkt, boost::system::error_code &ec) {
        return socket_.send(boost::asio::buffer(&pkt, sizeof(pkt)), 0, ec);
    }

    std::size_t send(const tcp_packet &pkt, udp::endpoint &client_endpoint,
                     boost::system::error_code &ec) {
        return socket_.send_to(boost::asio::buffer(&pkt, sizeof(pkt)),
                client_endpoint, 0, ec);
    }

    std::size_t receive(tcp_packet &pkt, boost::system::error_code &ec) {
        socket_.receive(boost::asio::buffer(&pkt, sizeof(pkt)));
        return 520;
    }
    std::size_t receive(tcp_packet &pkt, udp::endpoint &client_endpoint,
                        boost::system::error_code &ec) {
        timer_.expires_from_now(timeout);

        ec = boost::asio::error::would_block;
        std::size_t length = 0;
        socket_.async_receive_from(boost::asio::buffer(&pkt, sizeof(pkt)), client_endpoint,
                              boost::bind(&server::handle_receive, _1, _2, &ec, &length));

        do io_service_.run_one(); while (ec == boost::asio::error::would_block);

        return length;
    }

    tcp_socket *accept(const udp::endpoint &endpoint) {
        tcp_socket *tcp_s = new tcp_socket(endpoint, &this->socket_);
        return tcp_s;
    }

private:
    void check_timeout() {
        if (timer_.expires_at() <= deadline_timer::traits_type::now()) {
           // socket_.cancel();
            std::cout << "The timeout has been occurred" << std::endl;
            timer_.expires_at(boost::posix_time::pos_infin);
        }
        timer_.async_wait(boost::bind(&server::check_timeout, this));
    }

    static void handle_receive(const boost::system::error_code& ec, std::size_t length,
                               boost::system::error_code* out_ec, std::size_t* out_length) {
        *out_ec = ec;
        *out_length = length;
    }
    
private:
    boost::asio::io_service io_service_;
    udp::socket socket_;
    boost::asio::deadline_timer timer_;
    boost::posix_time::time_duration timeout;

    uint32_t cur_seq_no;
    uint32_t expected_ack_no;
};

static tcp_packet create_pkt() {
    tcp_packet pkt;
    pkt.src_port = 8080;
    pkt.dest_port = 2000;
    pkt.seq_no = 1000;
    pkt.ack_no = 404;
    pkt.flags = 2020;
    pkt.urg_data_ptr = 1;
    pkt.checksum = 2000;
    pkt.recvw = 120;
    strcpy(pkt.data, "TESTING");
    return pkt;
}

int main(int argc, char* argv[]) {
    udp::endpoint listen_endpoint(udp::v4(), 8000);
    udp::endpoint client_endpoint(udp::v4(), 8080);
    server s(listen_endpoint);

    boost::system::error_code ec;
    tcp_packet pkt_recv;
    s.receive(pkt_recv, ec);
    print_pkt(pkt_recv);

    tcp_socket *tcp_s = s.accept(client_endpoint);
    while (true)
        tcp_s->handle(pkt_recv, 5000L);

    return 0;
}
