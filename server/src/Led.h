#ifndef LED_H
#define LED_H

#include <thread>
#include <vector>
class Led
{

    public:
        enum class State { off, on };
        enum class Color { red=0, green, blue };

    public:
        Led();
        void Start();
        void SetState(State state);
        void SetColor(Color color);
        void SetFreq(int freq);
        std::string GetState();
        std::string GetColor();
        std::string GetRate();

    private:
        void Thread();

    private:
        State       m_state;
        Color       m_color;
        int         m_freq;
        std::thread m_thread;
        std::vector<std::string> m_colors;
};

#endif // LED_H
