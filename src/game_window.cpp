#include <stdexcept>
#include <Gosu/Utility.hpp>
#include <Gosu/Math.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Text.hpp>
#include "game_window.hpp"
#include <iostream>
#include <sstream>
#include <Gosu/Graphics.hpp>
#include <Gosu/ImageData.hpp>
#include <Gosu/Inspection.hpp>

GameWindow::GameWindow()
:Gosu::Window(1200, 800, false)
,font(graphics(), Gosu::defaultFontName(), 20)
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::buttonDown(Gosu::Button)
{
}

void GameWindow::buttonUp(Gosu::Button btn)
{
	if (btn == Gosu::kbReturn) {
		step();
	} else if (btn.id() >= Gosu::msRangeBegin && btn.id() <= Gosu::msRangeEnd) {
		int x = getMouseXInGrid();
		int y = getMouseYInGrid();
		if (x < int(grid.width()) && y < int(grid.height()) && x >= 0 && y >= 0) {
			//Machine& c = grid.at(x, y);
			
		}
	}
}

void GameWindow::draw()
{
	std::wstringstream wss;
	wss << Gosu::fps();
	font.draw(wss.str(), 0, 0, 10);
	graphics().drawTriangle(input().mouseX(), input().mouseY(), Gosu::Colors::gray,
							input().mouseX()+10, input().mouseY(), Gosu::Colors::gray,
							input().mouseX(), input().mouseY()+10, Gosu::Colors::gray, 10);
	grid.draw(gridx, gridy, gridwdt, gridhgt);
}

void GameWindow::loseFocus()
{
}

bool GameWindow::needsCursor() const
{
    return false;
}

bool GameWindow::needsRedraw() const
{
    return true;
}

void GameWindow::panic(const std::exception&)
{
}

void GameWindow::releaseMemory()
{
    std::cout << "more memory is needed" << std::endl;
}

void GameWindow::update()
{
	if (input().down(Gosu::kbSpace)) {
		step();
	}
}

void GameWindow::step()
{
	grid.update();
}

int GameWindow::getMouseXInGrid() const
{
	return (input().mouseX()-gridx)/gridwdt*double(grid.width());
}

int GameWindow::getMouseYInGrid() const
{
	return (input().mouseY()-gridy)/gridhgt*double(grid.height());
}
