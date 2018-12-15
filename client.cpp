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
        char dummy[500] = "dumm";
        tcp_packet pkt{};
        pkt.src_port = client_endpoint.port();
        pkt.dest_port = server_endpoint.port();

        strcpy(pkt.data,dummy);

       new_socket->open();

//       this->socket_.async_send_to(
//                boost::asio::buffer(&pkt, sizeof(pkt)),
//                server_endpoint, boost::bind(
//                        &selective_repeat::send_callback, protocol,
//                        boost::asio::placeholders::error(), 0));
    }
    //how to get ack number from recieved data packet

    void handle_recieving_file(char* file_path){
        char* buffer = new char[10*1024];
        uint32_t offset = 0;
        new_socket->set_buffer(buffer,offset,10*1024);

        std::ofstream output_file(file_path);
        while(true){
            tcp_packet newly_recieved;

            socket_.receive(boost::asio::buffer(&newly_recieved, sizeof(newly_recieved)));
            new_socket->handle_received(newly_recieved,5000l);
             uint32_t bytes_written = new_socket->recieved();

            if(bytes_written <= 0 || CHECK_BIT(newly_recieved.flags,6)){
                offset += bytes_written;
                char write_buffer[offset];
                memcpy( write_buffer, buffer, offset );
                output_file<<write_buffer;
                offset = 0;
            }else{
                offset = bytes_written;
            }
            if(CHECK_BIT(newly_recieved.flags,6))
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
    std::cout<<"started"<<std::endl;

    //int protocol = atoi(argv[3]);
    //TODO: intialize the protocol type (stop and wait/selective repeat)
    udp::endpoint client_endpoint(udp::v4(), client_port_number);
    udp::endpoint server_endpoint(udp::v4(), server_port_number);
    client c(client_endpoint,server_endpoint);
    c.handle_recieving_file("test.txt");
    return 0;
}