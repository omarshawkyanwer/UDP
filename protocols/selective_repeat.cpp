#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "selective_repeat.h"

selective_repeat::selective_repeat(udp::socket *socket,
        const udp::endpoint &endpoint) :
            transmission_protocol(socket, endpoint) {

}

void selective_repeat::send_data(std::map<uint32_t, tcp_packet> &pkts) {
    selective_repeat::pkts_to_send.clear();
    selective_repeat::pkts_to_send.insert(pkts.begin(), pkts.end());

    int count = selective_repeat::window_size;
    auto it = selective_repeat::pkts_to_send.begin();
    while (count--) {
        selective_repeat::send_single(it->first);
        it++;
    }
}

void selective_repeat::send_single(uint32_t seq_no) {
    tcp_packet pkt_send = selective_repeat::pkts_to_send[seq_no];
    selective_repeat::sender_window[seq_no] = pkt_send;
    selective_repeat::socket_->async_send_to(
            boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
            selective_repeat::endpoint_, boost::bind(
                    &selective_repeat::send_callback, this,
                    boost::asio::placeholders::error(), seq_no));

    selective_repeat::next_seq_no += std::strlen(pkt_send.data);
}

void selective_repeat::send_callback(const boost::system::error_code &ec,
         uint32_t seq_no) {
    if (ec) {
        selective_repeat::send_single(seq_no);
        return;
    }

    if (selective_repeat::packet_timer_map.find(seq_no) ==
            selective_repeat::packet_timer_map.end()) {
        auto *ack_timer = new boost::asio::deadline_timer(
                selective_repeat::socket_->get_io_service());
        selective_repeat::packet_timer_map[seq_no] = ack_timer;
    }
    auto *timer = selective_repeat::packet_timer_map[seq_no];
    timer->expires_from_now(boost::posix_time::milliseconds(selective_repeat::timeout_msec));
}

void selective_repeat::handle_received_ack(tcp_packet &pkt) {
    /* Check for ack flag at position 4 */
    

}

