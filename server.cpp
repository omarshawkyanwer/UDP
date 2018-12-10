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
void copy_headers(struct header_packet *pak, tcp_packet *tpak) {
    memcpy(tpak,pak,sizeof(*pak));
}
class server {
public:
    explicit server(const udp::endpoint &listen_endpoint) :
             socket_(io_service_, listen_endpoint) {
        server::endpoint_ = listen_endpoint;
    }


    void start() {
        while (true) {
            tcp_packet pkt_recv{};
            struct  header_packet header_packet1;
            server::socket_.receive(boost::asio::buffer(&header_packet1, sizeof(header_packet1)));
            copy_headers(&header_packet1,&pkt_recv);
            if(pkt_recv.data_size > 0) {
                server::socket_.receive(boost::asio::buffer(pkt_recv.data, pkt_recv.data_size));
            }
            print_pkt(pkt_recv);
            handle_packet(pkt_recv);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(pkt_recv.flags == 12)
                break;
        }
    }

private:
    void handle_packet(tcp_packet &pkt) {
        printf("helo");
//        std::string key = std::to_string(pkt.src_port);
//        socket_map_mtx.lock();
//        if (server::open_sockets.find(key) == server::open_sockets.end()) {
//            /* Create a new tcp_socket and put it in map */
//            udp::endpoint client_endpoint(udp::v4(), pkt.src_port);
//            auto *protocol = new selective_repeat(&this->socket_, this->endpoint_);
//            auto *new_socket = new tcp_socket(endpoint_, client_endpoint, &this->socket_, protocol);
//            server::open_sockets.insert(std::pair<std::string, tcp_socket*>(key, new_socket));
//        }
//        /* Forward packet to the open socket after */
//        server::open_sockets[key]->handle_received(pkt, server::timeout);
//       socket_map_mtx.unlock();
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
    tcp_packet pack;
    pack.flags =pack.ack_no = pack.seq_no = pack.dest_port = pack.data_size = pack.src_port = pack.recvw
            = pack.urg_data_ptr = 1;
    pack.checksum = 1267;
     strcpy(pack.data,"a");
    char habal[1000];
    from_pkt(habal,(pack));
    tcp_packet conv = *from_str(habal);
    conv;
    return 0;
}
