#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::asio::ip::udp;
using boost::asio::deadline_timer;


class server
{
public:
    server(short port)
    : socket_(io_service_,
    udp::endpoint(udp::v4(), port)),
    timer(io_service_)
    {
        timeout = boost::posix_time::seconds(15);
        timer.expires_from_now(boost::posix_time::pos_infin);
        check_timeout();
    }
    
    std::size_t receive(const boost::asio::mutable_buffer& buffer,
boost::system::error_code& ec)
    {
       
        timer.expires_from_now(timeout);

        ec = boost::asio::error::would_block;
        
        std::size_t length = 0;
        
        socket_.async_receive(boost::asio::buffer(buffer),
                              boost::bind(&server::handle_receive, _1, _2, &ec, &length));
        
        do io_service_.run_one(); while (ec == boost::asio::error::would_block);
        
        return length;
    }
    static void handle_receive(
                               const boost::system::error_code& ec, std::size_t length,
                               boost::system::error_code* out_ec, std::size_t* out_length)
    {
        *out_ec = ec;
        *out_length = length;
    }
   
private:
    void check_timeout()
    {
        
        if (timer.expires_at() <= deadline_timer::traits_type::now())
        {
           // socket_.cancel();
            std::cout<<"The timeout has been occurred"<<std::endl;
            timer.expires_at(boost::posix_time::pos_infin);
        }
         timer.async_wait(boost::bind(&server::check_timeout, this));
        
    }
    
private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    boost::posix_time::time_duration timeout;
    boost::asio::deadline_timer timer;
    boost::asio::io_service io_service_;
    
    
};

void work_for_io_service(const boost::system::error_code& /*e*/)
{
    std::cout<<"omar shawky"<<std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        char* port = "8000";
        boost::asio::io_service io_service;
        server s(std::atoi(port));
        char data[1024];
        boost::system::error_code ec;
        std::size_t n = s.receive(boost::asio::buffer(data),ec);
        std::cout.write(data, n);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
