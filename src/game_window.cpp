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
#include "defines.hpp"
#include "machines/end_pipe.hpp"
#include "machines/pipe.hpp"
#include <Gosu/Timing.hpp>
#include <fstream>
#include "machine_factory.hpp"
#include "machines/pump.hpp"

GameWindow::GameWindow()
:Gosu::Window(1200, 800, false)
,font(graphics(), Gosu::defaultFontName(), 20)
,grid(graphics(), 16, 16)
,Toolbox(graphics(), 2, 2)
,particle_emitter(graphics(), L"particle_gas.png", RenderLayer::Particles)
{
	load("autosave.grid");

	Toolbox.reset(0, 0, new Pipe(graphics(), ReceiveFromDir::Up, 4));
	Toolbox.reset(1, 0, new EndPipe(graphics(), ReceiveFromDir::Down));
	Toolbox.reset(0, 1, new Pump(graphics()));
}

GameWindow::~GameWindow()
{
}

void GameWindow::buttonDown(Gosu::Button btn)
{
	if (btn == Gosu::msLeft) {
		optional<size_t> action_id;
		if (input().down(Gosu::kb1)) {
			action_id.construct(0);
		} else if (input().down(Gosu::kb2)) {
			action_id.construct(1);
		} else if (input().down(Gosu::kb3)) {
			action_id.construct(2);
		} else if (input().down(Gosu::kb4)) {
			action_id.construct(3);
		}
		auto fun = [this, action_id](size_t x, size_t y, Grid& grid) -> bool
		{
			if (x >= grid.width()) return false;
			if (y >= grid.height()) return false;
			auto opt = grid.get(x, y);
			if (!opt) return false;
			if (!action_id) {
				dragdrop = std::move(opt->clone());
				return true;
			}
			if (opt->numActions() <= *action_id) return false;
			opt->Action(*action_id);
            grid.modified(x, y);
			return true;
		};
		if (!fun(getMouseXInToolbox(), getMouseYInToolbox(), Toolbox)) {
			fun(getMouseXInGrid(), getMouseYInGrid(), grid);
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
        if (dragdrop) {
            dragdrop.reset();
        }
	} else if (btn == Gosu::kbF6) {
        std::ofstream file("autosave.grid");
        for (size_t y = 0; y < grid.height(); y++) {
            for (size_t x = 0; x < grid.width(); x++) {
                auto e = grid.get(x, y);
                if (!e) {
                    file << ' ';
                    continue;
                }
                file << e->serialize();
            }
            file << std::endl;
        }
    }
}

void GameWindow::draw()
{
	std::wstringstream wss;
	wss << Gosu::fps();
	wss << L"fps - ";
    wss << update_time;
	wss << L"ms / ";
    wss << 1000/60;
    wss << L"ms - ";
    wss << particle_emitter.count();
    wss << L"ms particles - ";
	if (grid.is_initialized()) {
		wss << L" grid ready";
	} else {
		wss << L" a cell is not initialized";
	}
	font.draw(wss.str(), 0, 0, RenderLayer::GUI);
	graphics().drawTriangle(input().mouseX(), input().mouseY(), Gosu::Color::GRAY,
							input().mouseX()+10, input().mouseY(), Gosu::Color::GRAY,
							input().mouseX(), input().mouseY()+10, Gosu::Color::GRAY, RenderLayer::GUI);
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

    particle_emitter.draw();

	if (dragdrop) {
		graphics().pushTransform(Gosu::scale(gridwdt/double(grid.width()), gridhgt/double(grid.height())));
		dragdrop->draw(input().mouseX() - gridwdt/double(grid.width())/2, input().mouseY()- gridhgt/double(grid.height())/2);
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
    auto start_time = Gosu::milliseconds();
	if (input().down(Gosu::kbSpace)) {
		step();
	}
    for(int i = 0; i < 100; i++)
    particle_emitter.emit()
        .pos(input().mouseX(), input().mouseY())
        .ttl(10)
        .center(0.5, 0.5)
        .scale(0.5)
        .color(Gosu::Color::AQUA)
        .vel(Gosu::random(-50, 50), Gosu::random(-50, 50))
        .fade(30)
        ;
    particle_emitter.update(1.0/60.0);
    update_time = Gosu::milliseconds() - start_time;
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

void GameWindow::load(std::string filename)
{
    std::cout << "loading " << filename << std::endl;
    std::ifstream file(filename);
    std::vector<std::string> data;
    size_t wdt = 0;
    while(file) {
        std::string str;
        std::getline(file, str);
        if (wdt == 0) {
            if (str.empty()) {
                // skip beginning empty lines
                continue;
            }
            wdt = str.length();
        }
        if (str.length() == 0) {
            std::cout << "found empty line, stopped parsing" << std::endl;
            break;
        }
        if (wdt != str.length()) {
            std::cout << "line length do not match, expected " << wdt << " and got " << str.length() << " in line \"" << str << "\"" << std::endl;
        }
        std::cout << str << std::endl;
        data.push_back(str);
    }
    std::cout << "found grid of size " << wdt << "x" << data.size() << std::endl;
    MachineFactory factory;
    factory.add<Pipe>(graphics());
    factory.add<EndPipe>(graphics());
    factory.add<Pump>(graphics());
    grid.resize(wdt, data.size());
    for (size_t y = 0; y < data.size(); y++) {
        for (size_t x = 0; x < wdt; x++) {
            char c = data.at(y).at(x);
            if (c == ' ') {
                continue;
            }
            grid.reset(x, y, factory.create(c));
        }
    }
}
