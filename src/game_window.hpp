#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <Gosu/Font.hpp>
#include <Gosu/Window.hpp> // Base class: Gosu::Window
#include <Gosu/Bitmap.hpp>
#include <Gosu/Image.hpp>
#include <map>
#include "optional.hpp"
#include "helpers.hpp"
#include "grid.hpp"
#include "machine.hpp"

class GameWindow : public Gosu::Window
{
private:
    GameWindow(const GameWindow& rhs);
    GameWindow& operator=(const GameWindow& rhs);
	static constexpr double gridx = 100;
	static constexpr double gridy = 20;
	static constexpr double gridwdt = 600;
	static constexpr double gridhgt = 600;
public:
protected:
    Gosu::Font font;
	Grid<Machine, 3> grid;
public:
	int getMouseXInGrid() const;
	int getMouseYInGrid() const;
	void step();
    GameWindow();
    virtual ~GameWindow();
    virtual void buttonDown(Gosu::Button);
    virtual void buttonUp(Gosu::Button);
    virtual void draw();
    virtual void loseFocus();
    virtual bool needsCursor() const;
    virtual bool needsRedraw() const;
    virtual void panic(const std::exception& e);
    virtual void releaseMemory();
    virtual void update();
};

#endif // GAMEWINDOW_HPP
