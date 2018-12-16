#ifndef UDP_TRANSMISSION_H
#define UDP_TRANSMISSION_H

#include <boost/asio/placeholders.hpp>
#include <boost/asio/ip/udp.hpp>
#include <map>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio/placeholders.hpp>
#include <set>
#include "types.h"

using namespace boost::asio::ip;

class transmission_protocol {
public:
    explicit transmission_protocol(udp::socket *socket,
            const udp::endpoint &endpoint) {
        this->socket_ = socket;
        this->endpoint_ = endpoint;
    }
    virtual void send_data(std::map<uint32_t, tcp_packet>&) = 0;
    virtual void handle_received_ack(tcp_packet &) = 0;

    void send_ack(uint32_t ack_no) {
        tcp_packet pkt_send = make_ack_pkt(ack_no);
        this->socket_->async_send_to(
                boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
                this->endpoint_, boost::bind(
                        &transmission_protocol::send_ack_callback, this,
                        boost::asio::placeholders::error(), ack_no));
    }

    size_t handle_received_data(tcp_packet &pkt,char *buf,uint32_t offset,uint32_t max_len) {
        if (pkt.seq_no < recv_window_base) {
            send_ack(pkt.seq_no);
            return 0;
        }
        else if (pkt.seq_no > recv_window_base + RWND) /* Out of window scope */
            return 0;

        recv_window[pkt.seq_no] = pkt;
        send_ack(pkt.seq_no);
        size_t written = 0;

        while (!recv_window.empty() && recv_window_base == recv_window.begin()->first) {
            tcp_packet recv_pkt = recv_window.begin()->second;
            /* Append to buffer */
            uint32_t  data_len = std::strlen(recv_window.begin()->second.data);
            if(offset + data_len > max_len)
                break;
            std::memcpy(buf + offset, recv_pkt.data, data_len);
            offset+=data_len;
            written += data_len;
            recv_window_base += data_len;
            recv_window.erase(recv_window.begin());
        }
        return written;
    }
protected:
    tcp_packet make_ack_pkt(uint32_t ack_no) {
        tcp_packet pkt{};
        pkt.src_port = this->socket_->local_endpoint().port();
        pkt.dest_port = this->endpoint_.port();

        pkt.ack_no = ack_no;
        SET_BIT(pkt.flags, 4); /* Set ACK */

        return pkt;
    }

    void send_ack_callback(const boost::system::error_code &ec,
                           uint32_t ack_no) {
        if (ec)
            this->send_ack(ack_no);
    }

protected:
    boost::asio::ip::udp::socket *socket_;
    boost::asio::ip::udp::endpoint endpoint_;

    long timeout_msec = 5000;
    int window_size = 30;
    uint32_t next_seq_no = 0;
    std::map<uint32_t , tcp_packet>::iterator sender_window_base;
    std::map<uint32_t , tcp_packet>::iterator sender_window_next;

    std::map<uint32_t, boost::asio::deadline_timer *> packet_timer_map;
    std::map<uint32_t, tcp_packet> pkts_to_send;
    std::map<uint32_t, tcp_packet> sender_window;

    uint32_t recv_window_base = 0;
    std::map<uint32_t, tcp_packet> recv_window;
    const uint32_t RWND = 2500;
    const uint32_t  max_buff = 10000;
    bool done = false;
};
#endif //UDP_TRANSMISSION_H
