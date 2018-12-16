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
    selective_repeat::packet_timer_map.clear();
    selective_repeat::pkts_to_send.clear();
    selective_repeat::pkts_to_send.insert(pkts.begin(), pkts.end());
    selective_repeat::sender_window_base = selective_repeat::sender_window_next = selective_repeat::pkts_to_send.begin();
    //selective_repeat::window_size = 30;
    /*
     * the time at which it starts sending
     * */
    selective_repeat::sent_size = 0;
    selective_repeat::sending_start_time_ = clock();
    int count = selective_repeat::window_size;
    auto it = selective_repeat::pkts_to_send.begin();
    int pak_len = 0;
    while (count > 0 &&
    it != selective_repeat::pkts_to_send.end()) {
        selective_repeat::send_single(it->first);
        selective_repeat::window_size_at_time_t.push_back(std::make_pair((clock() - sending_start_time_) / CLOCKS_PER_SEC
                , window_size));
        it++;
        selective_repeat::sender_window_next++;
        pak_len = static_cast<int>(strlen(it->second.data));
        count-=pak_len;
    }

    while (selective_repeat::sender_window_base != selective_repeat::pkts_to_send.end())
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

    std::cout<<"sent packets" <<selective_repeat::pkts_to_send.size()<<std::endl;
    selective_repeat::sending_end_time_ = clock();
    selective_repeat::total_sending_duration = (selective_repeat::sending_end_time_ -
            selective_repeat::sending_start_time_) / CLOCKS_PER_SEC;

}
void selective_repeat::resend(uint32_t seq_no) {
    std::cout<<"sending "<<seq_no<<std::endl;
    tcp_packet pkt_send = selective_repeat::pkts_to_send[seq_no];
    selective_repeat::sender_window[seq_no] = pkt_send;

    selective_repeat::socket_->async_send_to(
            boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
            selective_repeat::endpoint_, boost::bind(
                    &selective_repeat::send_callback, this,
                    boost::asio::placeholders::error(), seq_no));
}
void selective_repeat::send_single(uint32_t seq_no) {

    std::cout<<"sending "<<seq_no<<std::endl;
    tcp_packet pkt_send = selective_repeat::pkts_to_send[seq_no];
    selective_repeat::sender_window[seq_no] = pkt_send;

    selective_repeat::socket_->async_send_to(
            boost::asio::buffer(&pkt_send, sizeof(pkt_send)),
            selective_repeat::endpoint_, boost::bind(
                    &selective_repeat::send_callback, this,
                    boost::asio::placeholders::error(), seq_no));

    selective_repeat::next_seq_no += std::strlen(pkt_send.data);
    selective_repeat::sent_size += std::strlen(pkt_send.data);
}

void selective_repeat::send_callback(const boost::system::error_code &ec,
                                     uint32_t seq_no) {
    if (ec) {
        selective_repeat::resend(seq_no);
        return;
    }

    if (selective_repeat::packet_timer_map.find(seq_no) ==
        selective_repeat::packet_timer_map.end()) {

        auto *ack_timer = new boost::asio::deadline_timer(
                selective_repeat::socket_->get_io_service());
        selective_repeat::packet_timer_map[seq_no] = ack_timer;
    }
    else{
        std::cout<<"retransmitting "<<seq_no<<std::endl;
    }
    auto *timer = selective_repeat::packet_timer_map[seq_no];
    timer->expires_from_now(boost::posix_time::milliseconds(selective_repeat::timeout_msec));
    timer->async_wait(boost::bind(&selective_repeat::handle_timeout, this, seq_no));
}

void selective_repeat::handle_timeout(uint32_t seq_no) {

    auto timer = selective_repeat::packet_timer_map[seq_no];
    if (timer != NULL && timer->expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
        std::cout<<"timing out "<<seq_no <<std::endl;
          window_size = (size_t) controller->time_out(window_size);;
        selective_repeat::resend(seq_no);
    }
}

void selective_repeat::handle_received_ack(tcp_packet &pkt) {
    if(selective_repeat::sender_window.find(pkt.ack_no) == selective_repeat::sender_window.end())
        return;
    tcp_packet pkt_acked = selective_repeat::sender_window[pkt.ack_no];
    selective_repeat::sender_window.erase(pkt_acked.seq_no);
    std::cout<<"acknowledging "<<pkt_acked.seq_no<<std::endl;
    /* Reset and delete timer */
    auto timer = selective_repeat::packet_timer_map[pkt.ack_no];
    selective_repeat::packet_timer_map.erase(pkt.ack_no);
    timer->cancel();
    delete timer;

        window_size =(size_t) controller->new_ack(window_size);
    while (selective_repeat::sender_window_base != selective_repeat::pkts_to_send.end() &&
           selective_repeat::sender_window_base != selective_repeat::sender_window_next &&
           selective_repeat::sender_window.find(selective_repeat::sender_window_base->second.seq_no)
           == selective_repeat::sender_window.end()) {
        selective_repeat::sent_size -= strlen(selective_repeat::sender_window_base->second.data);
        selective_repeat::sender_window_base++;
        std::cout<<"Moving window to "<<sender_window_base->first<<std::endl;
    }

    std::cout<<"sizes are "<<sent_size<<" "<<window_size<<std::endl;
    while(selective_repeat::sender_window_next != selective_repeat::pkts_to_send.end() &&
    selective_repeat::sent_size <= selective_repeat::window_size ) {
        selective_repeat::send_single(selective_repeat::sender_window_next->first);
        selective_repeat::sender_window_next++;

    }


}
