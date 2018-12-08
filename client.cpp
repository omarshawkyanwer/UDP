#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "types.h"

using boost::asio::ip::udp;

enum { max_length = 1024 };

static tcp_packet create_pkt() {
    tcp_packet pkt;
    pkt.src_port = 8080;
    pkt.dest_port = 8000;
    pkt.seq_no = 1000;
    pkt.ack_no = 404;
    pkt.flags = 2020;
    pkt.urg_data_ptr = 1;
    pkt.checksum = 2000;
    pkt.recvw = 120;
    strcpy(pkt.data, "TESTING");
    return pkt;
}

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        udp::endpoint endpoint(udp::v4(), 8000);
        udp::endpoint sender_endpoint(udp::v4(), 8080);
        udp::socket s(io_service, sender_endpoint);

        boost::system::error_code ec;
        tcp_packet pkt = create_pkt();

        /* Sends received packet */
        while (true) {
            std::cout << "Sending back packet... \n";
            s.send_to(boost::asio::buffer(&pkt, sizeof(pkt)), endpoint, 0, ec);

            if (ec) {
                std::cout << "Sending error: " << ec.message() << std::endl;
                continue;
            }
            break;
        }

        /* Receives packet from server */
        while (true) {
            std::cout << "Receiving... \n";
            s.receive_from(boost::asio::buffer(&pkt, sizeof(pkt)), sender_endpoint, 0, ec);

            if (ec) {
                std::cout << "Receiving error: " << ec.message() << std::endl;
                break;
            }
            print_pkt(pkt);
        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
