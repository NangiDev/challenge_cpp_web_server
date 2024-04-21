#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define LOOP while(true)

#define LOG(msg) (std::cout  << msg << std::endl)

bool read_(tcp::socket & socket, std::string &data) {
	boost::asio::streambuf buf;
	try {
		boost::asio::read_until( socket, buf, "\n" );
	} catch (const boost::system::system_error &e) {
		std::cerr << "Error reading from socket: " << e.what() << std::endl;
		return "";
	}

	data = boost::asio::buffer_cast<const char*>(buf.data());
	return data.find("/favicon") == std::string::npos;
}

void send_(tcp::socket & socket, const std::string& message) {
	const std::string msg = message + "\n";
	boost::asio::write( socket, boost::asio::buffer(message) );
}

void parsePort(const int &args, char** const &argv, int &port) {
	if (args == 2) {
		// TODO: Error handle
		std::size_t pos;
		port = std::stoi(argv[1], &pos);
	}
}

int main(int args, char* argv[])
{
	int port = 8080;
	parsePort(args, argv, port);

	boost::asio::io_service io_service;
	tcp::endpoint endpoint(tcp::v4(), port);
	tcp::acceptor acceptor_(io_service, endpoint);

	LOG("Server is runnning! Port: " << port << "\n");

	LOOP {
		tcp::socket socket_(io_service);
		acceptor_.accept(socket_);

		std::string message;
		if (!read_(socket_, message)) continue;

		LOG("Message received:\n" << message);
		if (message.find("/hello") != std::string::npos) {
			send_(socket_, "Hello from Server!");
		} else if (message.find("/bye") != std::string::npos) {
			send_(socket_, "Goodbye from Server!");
			break;
		} else {
			send_(socket_, "Invalid path!");
		}

		LOG("Server responded to Client!");
	}
		
	return 0;
}
