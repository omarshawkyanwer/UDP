#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "selective_repeat.h"
#include "additive_multiplicative.h"
#include <thread>
selective_repeat::selective_repeat(udp::socket *socket,
                                   const udp::endpoint &endpoint) :
        transmission_protocol(socket, endpoint) {
    controller = new additive_multiplicative();

}

void selective_repeat::send_data(std::map<uint32_t, tcp_packet> &pkts) {
    selective_repeat::sender_window.clear();
    selective_repeat::pkts_to_send.clear();
    selective_repeat::pkts_to_send.insert(pkts.begin(), pkts.end());
    selective_repeat::sender_window_base = selective_repeat::sender_window_next = selective_repeat::pkts_to_send.begin();

    int count = selective_repeat::window_size;
    auto it = selective_repeat::pkts_to_send.begin();
    int pak_len = 0;
    while (count > 0) {
        selective_repeat::send_single(it->first);
        it++;
        sender_window_next++;
        pak_len = strlen(it->second.data);
        count-=pak_len;
    }

    while (selective_repeat::sender_window_base != selective_repeat::pkts_to_send.end())
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    return;
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
   // timer->expires_from_now(boost::posix_time::milliseconds(selective_repeat::timeout_msec));
    //timer->async_wait(boost::bind(&selective_repeat::handle_timeout, this, seq_no));
}

void selective_repeat::handle_timeout(uint32_t seq_no) {
    auto timer = selective_repeat::packet_timer_map[seq_no];
    if (timer->expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
        window_size = controller->time_out(window_size);
        selective_repeat::send_single(seq_no);
    }
}

void selective_repeat::handle_received_ack(tcp_packet &pkt) {
    tcp_packet pkt_acked = selective_repeat::sender_window[pkt.ack_no];
    selective_repeat::sender_window.erase(pkt_acked.seq_no);
   // window_size = controller->new_ack(window_size);
    while (selective_repeat::sender_window_base != selective_repeat::pkts_to_send.end() &&
           selective_repeat::sender_window.find(selective_repeat::sender_window_base->second.seq_no)
           == selective_repeat::sender_window.end()) {
      //  next_seq_no = sender_window_base->first + window_size;
        selective_repeat::sender_window_base++;
        selective_repeat::send_single(selective_repeat::sender_window_next->first);
        selective_repeat::sender_window_next++;
    }
}
