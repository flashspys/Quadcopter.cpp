//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//



#include <iostream>
#include "LIS3MDL.hpp"
#include "pigpio.h"
#include "LSM6DS33.hpp"
#include "UDPServer.hpp"

#include <chrono>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <boost/asio/io_service.hpp>
#include <thread>

int main()
{

    try
    {
        boost::asio::io_service io_service;
        UDPServer server(io_service, 9000);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;

}