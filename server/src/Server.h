#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <thread>
#include <functional>

class Server
{
    public:
        Server(const std::string& fifo_from_client, const std::string& fifo_to_client_name, std::function<void(const std::string&)>);
        ~Server();
        void Start();
        void Stop();
        void Send(const std::string& data);

    private:
        void Thread();
        void SendThread(std::string data);

    private:
        std::string m_fifo_from_client;
        std::string m_fifo_to_client;
        std::thread m_thread;
        std::thread m_sendThread;
        std::function<void(const std::string&)> m_data_callback;
        int         m_read_fd;
        int         m_write_fd;
};

#endif // SERVER_H
