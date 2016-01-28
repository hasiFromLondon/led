#include "Processor.h"

#include <iostream>

Processor::Processor(const std::string& fifo_to_server_name, const std::string &fifo_to_client_name)
    :m_server(fifo_to_server_name, fifo_to_client_name, std::bind(&Processor::HandleServerData, this, std::placeholders::_1))
{
    InitCommands();
}

void Processor::Start()
{
    m_server.Start();
    m_led.Start();
}

void Processor::InitCommands()
{
    m_commands.insert(CommandList::value_type("set-led-state",  Command(std::bind(&Processor::HandleSetLedState, this, std::placeholders::_1), true)));
    m_commands.insert(CommandList::value_type("get-led-state",  Command(std::bind(&Processor::HandleGetLedState, this, std::placeholders::_1))));
    m_commands.insert(CommandList::value_type("set-led-color",  Command(std::bind(&Processor::HandleSetLedColor, this, std::placeholders::_1), true)));
    m_commands.insert(CommandList::value_type("get-led-color",  Command(std::bind(&Processor::HandleGetLedColor, this, std::placeholders::_1))));
    m_commands.insert(CommandList::value_type("set-led-rate",  Command(std::bind(&Processor::HandleSetLedRate, this, std::placeholders::_1), true)));
    m_commands.insert(CommandList::value_type("get-led-rate",  Command(std::bind(&Processor::HandleGetLedRate, this, std::placeholders::_1))));
}

void Processor::HandleSetLedState(const std::string& arg)
{
    bool  error = false;
    if (!arg.compare("off"))
        m_led.SetState(Led::State::off);
    else if (!arg.compare("on"))
        m_led.SetState(Led::State::on);
    else
        error = true;
    SendAnswer(error);
}

void Processor::HandleGetLedState(const std::string& arg)
{
    SendAnswer(false, m_led.GetState());
}

void Processor::HandleSetLedColor(const std::string& arg)
{
    bool  error = false;
    if (!arg.compare("red"))
        m_led.SetColor(Led::Color::red);
    else if (!arg.compare("green"))
        m_led.SetColor(Led::Color::green);
    else if (!arg.compare("blue"))
        m_led.SetColor(Led::Color::blue);
    else
        error = true;
    SendAnswer(error);
}

void Processor::HandleGetLedColor(const std::string& arg)
{
    SendAnswer(false, m_led.GetColor());
}

void Processor::HandleSetLedRate(const std::string& arg)
{
    try
    {
        int rate = std::stoi(arg);
        if ( rate >= 0 && rate < 6)
            m_led.SetFreq(rate);
        else
            SendAnswer(true);
    }
    catch (std::exception& ex)
    {
        SendAnswer(true);
    }
}

void Processor::HandleGetLedRate(const std::string& arg)
{
    SendAnswer(false, m_led.GetRate());
}

void Processor::SendAnswer(bool error, const std::string& result)
{
    std::string answer;
    if (error)
    {
        answer = "FAILED";
    }
    else
    {
        answer = "OK";
        if (result.size())
        {
            answer += " ";
            answer += result;
        }
    }
    m_server.Send(answer);
    //std::cout << answer << std::endl;
}

void Processor::ProcessCommand(const std::string& command, const std::string& arg)
{
    std::cout << "ProcessCommand" << std::endl;

    auto p = m_commands.find(command);
    if ( p == m_commands.end() )
    {
        std::cout << "ProcessCommand 3 "  << std::endl;
        SendAnswer(true);
    }
    else
    {
        Command& comm = p->second;
        if ( comm.has_arg && !arg.size() )
            SendAnswer(true);
        else if ( !comm.has_arg && arg.size() )
            SendAnswer(true);
        else
            comm.handler(arg);
    }
}

void Processor::HandleServerData(const std::string& data)
{
    std::string command;
    std::string arg;

    std::cout << "HandleServerData" << std::endl;
    m_client_command += data;

    auto rn = m_client_command.find('\n');
    if ( rn == m_client_command.npos )
        return;

    m_client_command.erase(rn);
    auto space = m_client_command.find(' ');
    if ( space == m_client_command.npos )
    {
        command = m_client_command;
    }
    else
    {
        command = m_client_command.substr(0, space);
        arg = m_client_command.substr(space + 1);
    }
    ProcessCommand(command, arg);
    m_client_command.clear();
}
