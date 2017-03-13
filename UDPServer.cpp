//
// Created by Felix Wehnert on 26.01.2017.
//

#include "Quadcopter.hpp"
#include "UDPServer.hpp"


UDPServer::UDPServer(boost::asio::io_service& io_service, unsigned short port) : socket_(io_service, udp::endpoint(udp::v4(), port)){
    start_receive();
}

void UDPServer::start_receive()
    {
        socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_),
                remote_endpoint_,
                boost::bind(
                        &UDPServer::handle_receive,
                        this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                )
        );
    }

void UDPServer::handle_receive(const boost::system::error_code& error,
                        std::size_t /*bytes_transferred*/)
    {
        std::string message(recv_buffer_.elems);

        if (!error || error == boost::asio::error::message_size)
        {
            Quadcopter::sharedInstance().onMessage(this, message);
/*
            boost::shared_ptr<std::string> message(new std::string("TEST ALLIANCE BEST ALLIANCE"));

            socket_.async_send_to(
                    boost::asio::buffer(*message),
                    remote_endpoint_,
                    boost::bind(&UDPServer::handle_send,
                                this,
                                message,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred
                    )
            );
*/
            start_receive();
        }
    }

void UDPServer::handle_send( const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) {

}

void UDPServer::ackConnect() {

    boost::shared_ptr<std::string> acknowledgement(new std::string("CONNECT;\n"));

    socket_.async_send_to(
            boost::asio::buffer(*acknowledgement),
            remote_endpoint_,
            boost::bind(&UDPServer::handle_send,
                        this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
    );

}

void UDPServer::sendEuler(std::vector<double> euler) {
    std::string s = "G;";
    s += std::to_string(euler[0]) + ";";
    s += std::to_string(euler[1]) + ";";
    s += std::to_string(euler[2]) + "\n";

    //boost::shared_ptr<std::string> eulerString(s);

    socket_.async_send_to(
            boost::asio::buffer(s),
            remote_endpoint_,
            boost::bind(&UDPServer::handle_send,
                        this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
    );
}

