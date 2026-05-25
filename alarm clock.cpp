#include <chrono>
#include <raylib.h>

int main()
{
   auto now = std::chrono::system_clock::now();
    int time = std::chrono::system_clock::to_time_t(now);
    return 0;
}
