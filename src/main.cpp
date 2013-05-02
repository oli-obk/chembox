#include "game_window.hpp"
#include <Gosu/Timing.hpp>
#include <iostream>

int main(int, char**)
{
    auto start_time = Gosu::milliseconds();
	GameWindow win;
    auto end_time = Gosu::milliseconds();
    std::cout << "startup took " << end_time-start_time << "ms" << std::endl;
	win.show();
    return 0;
}
