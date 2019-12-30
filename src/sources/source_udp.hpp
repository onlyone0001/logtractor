//
// Created by assasin on 29.12.2019.
//

#ifndef LOGTRACTOR_SOURCE_UDP_HPP
#define LOGTRACTOR_SOURCE_UDP_HPP

#include <string>
#include <functional>
#include <boost/asio.hpp>

#include "../source.hpp"

class SourceUdp : public Source {
private:
	using udp = boost::asio::ip::udp;

	udp::socket socket;
	udp::endpoint remote_endpoint;
	std::array<char, 65536> recv_buffer;
public:
	SourceUdp(boost::asio::io_context &io_context, const std::string &listenAddress, int port, decltype(callback) cbk)
			: Source(std::move(cbk)),
			socket(io_context, udp::endpoint(boost::asio::ip::address::from_string(listenAddress), port))
	{

	}

	void startReceive() override {
		socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
			[this, ptr = shared_from_this()](const boost::system::error_code &error, std::size_t bytesTransferred) {
				auto message = std::make_shared<std::string>(recv_buffer.data(), bytesTransferred);
				callback(std::move(message));
				startReceive();
			}
		);
	}
};


#endif //LOGTRACTOR_SOURCE_UDP_HPP
