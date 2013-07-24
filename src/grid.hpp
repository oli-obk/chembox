#ifndef GRID_HPP
#define GRID_HPP

#include <Gosu/Graphics.hpp>
#include "machine.hpp"

class Grid
{
public:
	typedef Machine T;
private:
	std::vector<std::unique_ptr<T>> m_data;
	size_t wdt, hgt;
	size_t index(size_t x, size_t y) const { return x + y*wdt; }
	const std::unique_ptr<T>& element(size_t x, size_t y) const { return m_data[index(x,y)]; }
	std::unique_ptr<T>& element(size_t x, size_t y) { return m_data[index(x,y)]; }
	Gosu::Graphics& graphics;
	bool _initialized;
	void initialize(size_t x, size_t y)
	{
		if (get(x, y)) get(x, y)->Initialize(get(x, y-1), get(x, y+1), get(x-1, y), get(x+1, y));
	}
	Grid(const Grid&) = delete;
public:
	bool is_initialized() const { return _initialized; }
    void modified(size_t x, size_t y)
    {
        _initialized = false;
		initialize(x, y);
		initialize(x, y-1);
		initialize(x, y+1);
		initialize(x-1, y);
		initialize(x+1, y);
    }
	bool check_initialization()
	{
		if (_initialized) return true;
		for (auto& ptr:m_data) {
			if (!ptr) continue;
			if (!ptr->isInitialized()) {
				return false;
			}
		}
		return (_initialized = true);
	}
	Grid(Gosu::Graphics& g, size_t wdt, size_t hgt)
	:graphics(g)
	{
        resize(wdt, hgt);
    }
    void resize(size_t w, size_t h)
    {
        m_data.clear();
        m_data.resize(w*h);
        wdt = w;
        hgt = h;
        _initialized = true;
    }
	size_t width() const { return wdt; }
	size_t height() const { return hgt; }
	//T& operator[](size_t x, size_t y) { return m_data[x+y<<sidepow]; }
	//const T& operator[](size_t x, size_t y) const { return m_data[x+y<<sidepow]; }
	T& at(size_t x, size_t y) { assert(x < width()); assert(y < height()); return *element(x, y); }
	const T& at(size_t x, size_t y) const { assert(x < width()); assert(y < height()); return *element(x, y); }
	optional<T&> get(size_t x, size_t y)
	{
		if (x >= width()) return optional<T&>();
		if (y >= height()) return optional<T&>();
		if (!element(x, y)) return optional<T&>();
		return optional<T&>(*element(x, y));
	}

	void update()
	{
		for (size_t y = 0; y < height(); y++) {
			for (size_t x = 0; x < width(); x++) {
				auto& e = element(x, y);
				if (!e) continue;
				e -> send();
			}
		}
		for (size_t y = 0; y < height(); y++) {
			for (size_t x = 0; x < width(); x++) {
				auto& e = element(x, y);
				if (!e) continue;
				// only let every second one communicate
				if ((y%2 && x%2) || (!(y%2) && !(x%2))) {
					e -> communicate();
				}
			}
		}
		for (size_t y = 0; y < height(); y++) {
			for (size_t x = 0; x < width(); x++) {
				auto& e = element(x, y);
				if (!e) continue;
				e -> receive();
			}
		}
	}

	void draw()
	{
		for (size_t y = 0; y < height(); y++) {
			for (size_t x = 0; x < width(); x++) {
				if (!element(x, y)) continue;
				element(x, y) -> draw(x, y);
			}
		}
	}

	void reset(size_t x, size_t y, T* ptr = nullptr)
	{
		reset(x, y, std::unique_ptr<T>(ptr));
	}

	void reset(size_t x, size_t y, std::unique_ptr<T> ptr)
	{
		_initialized = false;
		assert(x < width()); assert(y < height());
		element(x, y) = std::move(ptr);
        modified(x, y);
	}
};

#endif // GRID_HPP
