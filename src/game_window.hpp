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
#include "Effects.hpp"

class GameWindow : public Gosu::Window
{
private:
    GameWindow(const GameWindow& rhs);
    GameWindow& operator=(const GameWindow& rhs);
	static constexpr double gridx = 100;
	static constexpr double gridy = 20;
	static constexpr double gridwdt = 600;
	static constexpr double gridhgt = 600;
	static constexpr double toolboxx = 800;
	static constexpr double toolboxy = 20;
	static constexpr double toolboxwdt = 200;
	static constexpr double toolboxhgt = 200;
public:
protected:
    Gosu::Font font;
	Grid grid;
	Grid Toolbox;
	std::unique_ptr<Machine> dragdrop;
    unsigned long update_time;
    std::shared_ptr<Effects> effects;
	// 0 is paused, positive is how many particle/draw updates happen per logic update, negative is how many logic updates happen per render update
	int render_speed;
	int render_speed_steps;
public:
	int getMouseXInGrid() const;
	int getMouseYInGrid() const;
	int getMouseXInToolbox() const;
	int getMouseYInToolbox() const;
	void step();
    void load(std::string filename);
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
