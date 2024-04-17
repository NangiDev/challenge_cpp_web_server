#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

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
	tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 8080 ));
	tcp::socket socket_(io_service);
	acceptor_.accept(socket_);
	std::string message = read_(socket_);
	std::cout << message << std::endl;
	send_(socket_, "Hello From Server!");

	std::cout << "Server sent Hello message to Client!" << std::endl;
	return 0;
}
