
#include "Led.h"

#include <iostream>
#include <chrono>

Led::Led()
    :m_state(State::off),m_color(Color::red), m_freq(0)
{
    m_colors.push_back("red");
    m_colors.push_back("green");
    m_colors.push_back("blue");

}

void Led::Start()
{
    m_thread = std::thread(&Led::Thread, this);
}

void Led::SetState(State state)
{
    m_state = state;
}

void Led::SetColor(Color color)
{
    m_color = color;
}

void Led::SetFreq(int freq)
{
    m_freq = freq;
}

std::string Led::GetState()
{
    return (m_state == State::off)?"off":"on";
}

std::string Led::GetColor()
{
    return m_colors[static_cast<int>(m_color)];
}

std::string Led::GetRate()
{
    return std::to_string(m_freq);
}

void Led::Thread()
{

    bool blink_state = false;

    while (1)
    {
        std::cout << "                                           \r";
        switch (m_state)
        {
            case State::off:
                std::cout << "off";
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                break;
            case State::on:

                if (!m_freq)
                {
                    std::cout << "on, " << m_colors[static_cast<int>(m_color)] << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                else
                {
                    if ( blink_state )
                        std::cout << "on, " << m_colors[static_cast<int>(m_color)] << std::flush;
                    else
                        std::cout << "off, " << m_colors[static_cast<int>(m_color)] << std::flush;
                    blink_state = !blink_state;
                    int time = 1000/m_freq;
                  //  std::cout << "time = " << time << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(time));

                }
                break;
        };
    }
}
