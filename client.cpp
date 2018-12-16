#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "types.h"
#include <fstream>
#include "tcp_socket.h"
#include "protocols/selective_repeat.h"

using boost::asio::ip::udp;

class client {
public:
    explicit client(const udp::endpoint &client_endpoint,const udp::endpoint &server_endpoint) :
            socket_(io_service_, client_endpoint) {
        client::client_endpoint_ = client_endpoint;
        client::server_endpoint_ = server_endpoint;
        protocol = new selective_repeat(&this->socket_, this->server_endpoint_);
        new_socket = new tcp_socket(client_endpoint_, server_endpoint_, &this->socket_, protocol);

        new_socket->open();
    }

    void handle_receiving_data(char *file_path){
        char* buffer = new char[BUFF_SIZE];
        uint32_t offset = 0;
        new_socket->set_buffer(buffer, offset, BUFF_SIZE);

        std::ofstream output_file(file_path);
        while (true) {
            tcp_packet pkt_received{};

            socket_.receive(boost::asio::buffer(&pkt_received, sizeof(pkt_received)));
            print_pkt(pkt_received);
            
            new_socket->handle_received(pkt_received, 5000l);
            size_t bytes_written = new_socket->received();

            if (bytes_written <= 0 || CHECK_BIT(pkt_received.flags, 6)) {
                char write_buffer[bytes_written];
                memcpy(write_buffer, buffer, bytes_written);
                output_file << write_buffer;
            } else {
                offset = bytes_written;
            }
            if(CHECK_BIT(pkt_received.flags,6))
                break;
        }
        output_file.close();
    }

private:
    udp::endpoint client_endpoint_,server_endpoint_;
    boost::asio::io_service io_service_;
    transmission_protocol* protocol;
    udp::socket socket_;
    tcp_socket* new_socket;

    const int BUFF_SIZE = 10 * 1024;
};

int main(int argc, char* argv[])
{

    /*
     * 1 The first argument is file Name.
     * 2 The second argument is the client's port number.
     * 3 The third argument is the server's port number.
     * 4 The fourth argument is the protocol type :
     * 0 ---> stop and wait.
     * 1 ---> selective repeat
     *
     * */
    //Getting command line parameters

    //int client_port_number = atoi(argv[2]);
    int client_port_number = 7999;

    //int server_port_number = atoi(argv[3]);
    int server_port_number = 8000;
    std::cout << "started" << std::endl;

    //int protocol = atoi(argv[3]);
    //TODO: intialize the protocol type (stop and wait/selective repeat)
    udp::endpoint client_endpoint(udp::v4(), client_port_number);
    udp::endpoint server_endpoint(udp::v4(), server_port_number);
    client c(client_endpoint,server_endpoint);
    c.handle_receiving_data("test.txt");
    return 0;
}