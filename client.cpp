
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "types.h"

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        udp::endpoint endpoint(udp::v4(), 8000);
        udp::endpoint sender_endpoint(udp::v4(), 8080);
        udp::socket s(io_service, sender_endpoint);

        boost::system::error_code ec;
        tcp_packet pkt;

        /* Receives packet from server */
        while (true) {
            std::cout << "Receiving... \n";
            s.receive_from(boost::asio::buffer(&pkt, sizeof(pkt)), sender_endpoint, 0, ec);

            if (ec) {
                std::cout << "Receiving error: " << ec.message() << std::endl;
                continue;
            }
            break;
        }

        print_pkt(pkt);

        /** Sends back received packet */
        while (true) {
            std::cout << "Sending back packet... \n";
            s.send_to(boost::asio::buffer(&pkt, sizeof(pkt)), endpoint, 0, ec);

            if (ec) {
                std::cout << "Sending error: " << ec.message() << std::endl;
                continue;
            }
            break;
        }

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
