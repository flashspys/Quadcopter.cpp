//
// Created by Felix Wehnert on 26.01.2017.
//

#ifndef QUADROCOPTER_UDPSERVER_HPP
#define QUADROCOPTER_UDPSERVER_HPP


#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

class UDPServer {



    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    boost::array<char, 100> recv_buffer_;


    void handle_send(const boost::system::error_code &, size_t);
    void handle_receive(const boost::system::error_code &error, size_t);
    void start_receive();

public:
    UDPServer(boost::asio::io_service &io_service, unsigned short port);
    void sendEuler(std::vector<double>);
    void ackConnect();
};


#endif //QUADROCOPTER_UDPSERVER_HPP
