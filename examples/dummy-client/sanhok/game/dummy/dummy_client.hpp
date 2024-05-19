#pragma once

#include <boost/asio.hpp>
#include <sanhok/game/dummy/dummy_player.hpp>
#include <sanhok/game/protocol/protocol.hpp>
#include <sanhok/net/peer_tcp.hpp>
#include <sanhok/net/peer_udp.hpp>

#include <chrono>

namespace sanhok::game::dummy {
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using namespace std::chrono;
using ClientID = uint32_t;

class DummyClient final {
    constexpr static milliseconds TICK_INTERVAL = 17ms; // 60 Hz
    constexpr static unsigned short UDP_PORT = 50010;

public:
    DummyClient();
    ~DummyClient();

    void start(tcp::endpoint&& remote_endpoint);
    void stop();

private:
    void update(milliseconds dt);

    std::function<void(std::vector<uint8_t>&&)> get_protocol_handler(bool buffer_size_prefixed);
    void handle_client_join(const protocol::ClientJoin* client_join);

    boost::asio::io_context ctx_ {};
    net::PeerTCP peer_tcp_;
    net::PeerUDP peer_udp_;
    std::atomic<bool> is_running_ {false};
    std::thread worker_thread_;

    ClientID id_ {0};
    std::atomic<bool> is_joined_ {false};
    DummyPlayer player_ {};
};
}
