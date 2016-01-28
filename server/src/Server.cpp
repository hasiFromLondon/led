#include "Server.h"

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const int max_command_len = 256;

Server::Server(const std::string& fifo_from_client, const std::string &fifo_to_client_name, std::function<void(const std::string&)> callback)
    :m_fifo_from_client(fifo_from_client),
     m_fifo_to_client(fifo_to_client_name),
     m_data_callback(callback)
{
}

Server::~Server()
{
    Stop();
}

void Server::Stop()
{

}

void Server::Send(const std::string& data)
{
/*
    m_sendThread = std::thread(&Server::SendThread, this, data);

    //std::cout << "send " << data << std::endl;

  //  write(m_write_fd, data.c_str(), data.size());
    //flush(m_write_fd);
    //close(fd);
    */
    int fd;
    if ( (fd = open(m_fifo_to_client.c_str(), O_WRONLY|O_NONBLOCK)) <= 0 )
    {
        return;
    }
    std::string new_data(data);
    new_data.append("\n");
    write(fd, new_data.c_str(), new_data.size());
    close(fd);
}

void Server::SendThread(std::string data)
{
    // не рабочий код

}

void Server::Start()
{
    unlink(m_fifo_from_client.c_str());
    unlink(m_fifo_to_client.c_str());
    if ( mkfifo(m_fifo_from_client.c_str(), 0777) )
    {
        throw std::runtime_error(std::string("Cannot create fifo, ") + strerror(errno));
    }
    if ( mkfifo(m_fifo_to_client.c_str(), 0777) )
    {
        throw std::runtime_error(std::string("Cannot create fifo, ") + strerror(errno));
    }
    if ( (m_read_fd = open(m_fifo_from_client.c_str(), O_RDWR)) <= 0 )
    {
        throw std::runtime_error(std::string("Cannot open fifo, ") + strerror(errno));
    }
    m_thread = std::thread(&Server::Thread, this);
}

void Server::Thread()
{

    while (1)
    {
         struct pollfd fds;
         int ret;
         int timeout = 100000;

         fds.fd = m_read_fd;
         fds.events = POLLIN;

         ret = poll(&fds, 1, timeout);
         if ( ret < 0 )
             perror("poll error");
         if ( ret > 0 )
         {
            char str[max_command_len];
            int res = read(m_read_fd, str, max_command_len);
            if (res < 0 )
                 throw std::runtime_error(std::string("Reading file error, ") + strerror(errno));

            std::string command(str, res);
            m_data_callback(command);
        }
    }
    close(m_read_fd);
}
