#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "types.h"
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
        tcp_socket* new_socket = new tcp_socket(client_endpoint_, server_endpoint_, &this->socket_, protocol);

    }

    void handle_recieving_file(){
        while(true){
            tcp_packet newly_recieved;
            socket_.receive(boost::asio::buffer(&newly_recieved, sizeof(newly_recieved)));
            protocol->handle_received_ack(newly_recieved);
            protocol->send_ack(newly_recieved.ack_no);
            /*
             *TODO : check if last a packet
             * 1 - break
             * 2 - Close the connection and terminate
             **/
        }
    }

private:
    udp::endpoint client_endpoint_,server_endpoint_;
    boost::asio::io_service io_service_;
    udp::socket socket_;
    transmission_protocol* protocol;
};

int main(int argc, char* argv[])
{

    /*
     * 0 The first argument is file Name.
     * 1 The second argument is the client's port number.
     * 2 The third argument is the server's port number.
     * 3 The fourth argument is the protocol type :
     * 0 ---> stop and wait.
     * 1 ---> selective repeat
     *
     * */
    //Getting command line parameters
    int client_port_number = atoi(argv[1]);
    int server_port_number = atoi(argv[2]);
    int protocol = atoi(argv[3]);
    //TODO: intialize the protocol type (stop and wait/selective repeat)
    udp::endpoint client_endpoint(udp::v4(), client_port_number);
    udp::endpoint server_endpoint(udp::v4(), server_port_number);
    client c(client_endpoint,server_endpoint);
    c.handle_recieving_file();
    return 0;
}
