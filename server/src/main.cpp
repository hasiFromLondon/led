
#include <iostream>
#include <thread>

#include "Processor.h"

int main()
{
    try
    {
        Processor processor("/tmp/led_from_client_fifo", "/tmp/led_to_client_fifo");
        processor.Start();

        while(1)
            std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    catch (std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
}
