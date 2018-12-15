#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "types.h"
#include "tcp_socket.h"
#include "protocols/selective_repeat.h"
#include "file_handler.h"
#include <mutex>
#include <thread>
using boost::asio::ip::udp;
using boost::asio::deadline_timer;

class server {
public:
    explicit server(const udp::endpoint &listen_endpoint) :
            socket_(io_service_, listen_endpoint) {
        server::endpoint_ = listen_endpoint;
        data_chunk = new char[1000];
    }
    ~server(){
        delete [] data_chunk;
    }

    void start() {
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
        if (server::open_sockets.find(key) == server::open_sockets.end()) {
            /* Create a new tcp_socket and put it in map */
            udp::endpoint client_endpoint(udp::v4(), pkt.src_port);
            auto *protocol = new selective_repeat(&this->socket_, client_endpoint);
            auto *new_socket = new tcp_socket(endpoint_, client_endpoint, &this->socket_, protocol);
            server::open_sockets.insert(std::pair<std::string, tcp_socket*>(key, new_socket));
            /*
             * TODO: add the correct file name
             */
            file_handler_  = new file_handler("../habalo.txt");
            int read_bytes = file_handler_->get_next_bytes(data_chunk,500);
            boost::thread th (boost::bind(&server::handle_client,this,new_socket));
            // server::open_sockets[key]->send(data_chunk,read_bytes);
            th.detach();

        }
        server::open_sockets[key]->handle_received(pkt, server::timeout);

    }
    void handle_client(tcp_socket *socket) {
        char data_chunk[500];
        file_handler_ = new file_handler("../habalo.txt");
        int read_bytes;
        while (read_bytes = file_handler_->get_next_bytes(data_chunk, 500)) {
            socket->send(data_chunk, read_bytes);
        }
        delete file_handler_;
        socket->close();
    }

private:
    udp::endpoint endpoint_;
    boost::asio::io_service io_service_;
    udp::socket socket_;
    file_handler* file_handler_;
    std::map<std::string, tcp_socket *> open_sockets;
    std::mutex socket_map_mtx;
    char* data_chunk;
    long timeout = 5000l;
};

int main(int argc, char* argv[]) {
    udp::endpoint listen_endpoint(udp::v4(), 8000);
    server s(listen_endpoint);
    s.start();

    return 0;
}