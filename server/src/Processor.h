#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Server.h"
#include "Led.h"
#include <string>
#include <map>

class Processor
{
    public:
        Processor(const std::string &fifo_to_server_name, const std::string &fifo_to_client_name);
        void Start();

    private:
        void HandleServerData(const std::string& data);
        void InitCommands();
        void ProcessCommand(const std::string& command, const std::string& arg);
        void SendAnswer(bool error, const std::string& result = std::string());

        void HandleSetLedState(const std::string& arg);
        void HandleGetLedState(const std::string& arg);
        void HandleSetLedColor(const std::string& arg);
        void HandleGetLedColor(const std::string& arg);
        void HandleSetLedRate(const std::string& arg);
        void HandleGetLedRate(const std::string& arg);
    private:
        struct Command
        {
            Command(std::function<void(const std::string&)> handler_, bool has_arg_ = false)
                :handler(handler_),has_arg(has_arg_){};
            std::function<void(const std::string& arg)> handler;
            bool has_arg;
        };
        typedef std::map<std::string, Command> CommandList;

        Server m_server;
        Led    m_led;
        std::string m_client_command;

        CommandList m_commands;
};

#endif // PROCESSOR_H
