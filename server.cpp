#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "types.h"
#include "tcp_socket.h"
#include "protocols/selective_repeat.h"
#include <mutex>
#include <thread>
using boost::asio::ip::udp;
using boost::asio::deadline_timer;
class server {
public:
    explicit server(const udp::endpoint &listen_endpoint) :
             socket_(io_service_, listen_endpoint) {
        server::endpoint_ = listen_endpoint;
    }


    void start() {
        io_service_.run();
        while (true) {
            tcp_packet pkt_recv{};
            server::socket_.receive(boost::asio::buffer(&pkt_recv, sizeof(pkt_recv)));

            print_pkt(pkt_recv);
            handle_packet(pkt_recv);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
private:
    void handle_packet(tcp_packet &pkt) {

        std::string key = std::to_string(pkt.src_port);
        socket_map_mtx.lock();
        if (server::open_sockets.find(key) == server::open_sockets.end()) {
            /* Create a new tcp_socket and put it in map */
            udp::endpoint client_endpoint(udp::v4(), pkt.src_port);
            auto *protocol = new selective_repeat(&this->socket_, this->endpoint_);
            auto *new_socket = new tcp_socket(endpoint_, client_endpoint, &this->socket_, protocol);
            server::open_sockets.insert(std::pair<std::string, tcp_socket*>(key, new_socket));
        }
        /* Forward packet to the open socket after */
        server::open_sockets[key]->handle_received(pkt, server::timeout);
       socket_map_mtx.unlock();
    }

private:
    udp::endpoint endpoint_;
    boost::asio::io_service io_service_;
    udp::socket socket_;

    std::map<std::string, tcp_socket *> open_sockets;
    std::mutex socket_map_mtx;

    long timeout = 5000l;
};

int main(int argc, char* argv[]) {
    udp::endpoint listen_endpoint(udp::v4(), 8000);
    server s(listen_endpoint);

    s.start();

    return 0;
}
