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
#include "machines/fourway_pipe.hpp"
#include "defines.hpp"
#include "machines/end_pipe.hpp"

GameWindow::GameWindow()
:Gosu::Window(1200, 800, false)
,font(graphics(), Gosu::defaultFontName(), 20)
,grid(graphics())
,Toolbox(graphics())
{
	for (size_t y = 1; y < grid.height() - 1; y++) {
		grid.reset(0, y, new end_pipe(graphics(), ReceiveFromDir::Right));
		grid.reset(grid.width()-1, y, new end_pipe(graphics(), ReceiveFromDir::Left));
		for (size_t x = 1; x < grid.width() - 1; x++) {
			grid.reset(x, y, new FourwayPipe(graphics()));
		}
	}
	for (size_t x = 1; x < grid.height() - 1; x++) {
		grid.reset(x, 0, new end_pipe(graphics(), ReceiveFromDir::Down));
		grid.reset(x, grid.height()-1, new end_pipe(graphics(), ReceiveFromDir::Up));
	}
	static_cast<FourwayPipe&>(grid.at(1,1)).particles.add(ParticleState::Gas, ParticleType::Hydrogen, 10);

	Toolbox.reset(0, 0, new FourwayPipe(graphics()));
	Toolbox.reset(1, 0, new end_pipe(graphics(), ReceiveFromDir::Down));
}

GameWindow::~GameWindow()
{
}

void GameWindow::buttonDown(Gosu::Button btn)
{
	if (btn == Gosu::msLeft) {
		size_t x = getMouseXInToolbox();
		size_t y = getMouseYInToolbox();
		if (x < int(Toolbox.width()) && y < int(Toolbox.height())) {
			auto opt = Toolbox.get(x, y);
			if (opt) {
				dragdrop = std::move(opt->clone());
			}
		} else {
			size_t x = getMouseXInGrid();
			size_t y = getMouseYInGrid();
			if (x < int(grid.width()) && y < int(grid.height())) {
				auto opt = grid.get(x, y);
				if (opt) {
					dragdrop = std::move(opt->clone());
				}
			}
		}
	}
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
			if (btn == Gosu::msRight) {
				grid.reset(x, y);
			} else if (btn == Gosu::msLeft) {
				if (dragdrop) {
					grid.reset(x, y, std::move(dragdrop));
				}
			}
		}
	}
}

void GameWindow::draw()
{
	std::wstringstream wss;
	wss << Gosu::fps();
	wss << L" - ";
	if (grid.is_initialized()) {
		wss << L" grid ready";
	} else {
		wss << L" a cell is not initialized";
	}
	font.draw(wss.str(), 0, 0, RenderLayer::GUI);
	graphics().drawTriangle(input().mouseX(), input().mouseY(), Gosu::Colors::gray,
							input().mouseX()+10, input().mouseY(), Gosu::Colors::gray,
							input().mouseX(), input().mouseY()+10, Gosu::Colors::gray, RenderLayer::GUI);
	size_t ix = getMouseXInGrid();
	size_t iy = getMouseYInGrid();
	if (ix < grid.width() && iy < grid.height()) {
		double x = double(ix)/double(grid.width())*gridwdt+gridx;
		double y = double(iy)/double(grid.height())*gridhgt+gridy;
		graphics().drawQuad(x, y, Gosu::Color(128, 100, 100, 100),
							x+gridwdt/double(grid.width()), y, Gosu::Color(128, 100, 100, 100),
							x+gridwdt/double(grid.width()), y+gridhgt/double(grid.height()), Gosu::Color(128, 100, 100, 100),
							x, y+gridhgt/double(grid.height()), Gosu::Color(128, 100, 100, 100),
							RenderLayer::GUI
			);
	}
	ix = getMouseXInToolbox();
	iy = getMouseYInToolbox();
	if (ix < Toolbox.width() && iy < Toolbox.height()) {
		double x = double(ix)/double(Toolbox.width())*toolboxwdt+toolboxx;
		double y = double(iy)/double(Toolbox.height())*toolboxhgt+toolboxy;
		graphics().drawQuad(x, y, Gosu::Color(128, 100, 100, 100),
							x+toolboxwdt/double(Toolbox.width()), y, Gosu::Color(128, 100, 100, 100),
							x+toolboxwdt/double(Toolbox.width()), y+toolboxhgt/double(Toolbox.height()), Gosu::Color(128, 100, 100, 100),
							x, y+toolboxhgt/double(Toolbox.height()), Gosu::Color(128, 100, 100, 100),
							RenderLayer::GUI
			);
	}
	graphics().pushTransform(Gosu::translate(gridx, gridy));
	graphics().pushTransform(Gosu::scale(gridwdt/double(grid.width()), gridhgt/double(grid.height())));
	grid.draw();
	graphics().popTransform();
	graphics().popTransform();
	graphics().pushTransform(Gosu::translate(toolboxx, toolboxy));
	graphics().pushTransform(Gosu::scale(toolboxwdt/double(Toolbox.width()), toolboxhgt/double(Toolbox.height())));
	Toolbox.draw();
	graphics().popTransform();
	graphics().popTransform();
	if (dragdrop) {
		graphics().pushTransform(Gosu::translate(input().mouseX() - gridwdt/double(grid.width())/2, input().mouseY()- gridhgt/double(grid.height())/2));
		graphics().pushTransform(Gosu::scale(gridwdt/double(grid.width()), gridhgt/double(grid.height())));
		dragdrop->draw();
		graphics().popTransform();
		graphics().popTransform();
	}
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
	if (grid.check_initialization()) {
		grid.update();
	}
}

int GameWindow::getMouseXInGrid() const
{
	return std::floor((input().mouseX()-gridx)/gridwdt*double(grid.width()));
}

int GameWindow::getMouseYInGrid() const
{
	return std::floor((input().mouseY()-gridy)/gridhgt*double(grid.height()));
}

int GameWindow::getMouseXInToolbox() const
{
	return std::floor((input().mouseX()-toolboxx)/toolboxwdt*double(Toolbox.width()));
}

int GameWindow::getMouseYInToolbox() const
{
	return std::floor((input().mouseY()-toolboxy)/toolboxhgt*double(Toolbox.height()));
}
