#pragma once

#include <sanhok/game/player/player.hpp>
#include <sanhok/game/protocol/protocol.hpp>
#include <sanhok/net/connection_tcp.hpp>
#include <sanhok/net/connection_udp.hpp>

#include <chrono>

namespace sanhok::game {
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using namespace std::chrono;
using ClientID = uint32_t;

class Client final : public std::enable_shared_from_this<Client> {
public:
    Client(boost::asio::io_context& ctx, ClientID id, tcp::socket&& socket);
    ~Client();

    void start();
    void stop();
    void send_tcp(std::shared_ptr<flatbuffers::DetachedBuffer> buffer);
    void send_udp(std::shared_ptr<flatbuffers::DetachedBuffer> buffer);
    void update(milliseconds dt);

    bool is_running() const { return is_running_; }
    udp::endpoint local_endpoint_udp() const { return connection_udp_.local_endpoint(); };

private:
    std::function<void()> get_on_connection();
    std::function<void()> get_on_disconnection();
    std::function<void(std::vector<uint8_t>&&)> get_protocol_handler(bool buffer_size_prefixed);

    void handle_protocol_client_join(const protocol::ClientJoin* client_join);
    void handle_protocol_player_movement(const protocol::PlayerMovement* player_movement);

public:
    const ClientID id;
    player::Player player {};

private:
    boost::asio::io_context& ctx_;
    net::ConnectionTCP connection_tcp_;
    net::ConnectionUDP connection_udp_;
    std::atomic<bool> is_running_ {false};
};
}
