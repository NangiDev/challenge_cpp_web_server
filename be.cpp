#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string read_(tcp::socket & socket) {
	boost::asio::streambuf buf;
	boost::asio::read_until( socket, buf, "\n" );
	std::string data = boost::asio::buffer_cast<const char*>(buf.data());
	return data;
}

void send_(tcp::socket & socket, const std::string& message) {
	const std::string msg = message + "\n";
	boost::asio::write( socket, boost::asio::buffer(message) );
}

int main(int args, char* argv[])
{
	std::cout << "Server is running!" << std::endl;

	boost::asio::io_service io_service;
	tcp::endpoint endpoint(tcp::v4(), 8080);
	tcp::acceptor acceptor_(io_service, endpoint);

	while(true) {
		tcp::socket socket_(io_service);
		acceptor_.accept(socket_);

		std::string message = read_(socket_);
		std::cout << "Received message: " << message << std::endl;

		if (message.find("/hello") != std::string::npos) {
			send_(socket_, "Hello from Server!");
		} else if (message.find("/bye") != std::string::npos) {
			send_(socket_, "Goodbye from Server!");
			break;
		} else {
			send_(socket_, "Invalid path!");
		}

		std::cout << "Server sent Hello message to Client!" << std::endl;
	}
		
	return 0;
}
