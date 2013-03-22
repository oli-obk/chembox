#ifndef GRID_HPP
#define GRID_HPP

#include <Gosu/Graphics.hpp>
template <class T, size_t sidepow>
class Grid
{
private:
	std::array<std::unique_ptr<T>, 1<<(sidepow*2)> m_data;
	constexpr size_t index(size_t x, size_t y) { return x + (y<<sidepow); }
	std::unique_ptr<T>& element(size_t x, size_t y) { return m_data[index(x,y)]; }
	Gosu::Graphics& graphics;
public:
	Grid(Gosu::Graphics& g):graphics(g) {}
	constexpr size_t width() const { return 1<<sidepow; }
	constexpr size_t height() const { return width(); }
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
				if (!element(x, y)) continue;
				element(x, y) -> update();
			}
		}
	}
	void draw()
	{
		for (size_t y = 0; y < height(); y++) {
			for (size_t x = 0; x < width(); x++) {
				if (!element(x, y)) continue;
				graphics.pushTransform(Gosu::translate(x, y));
				element(x, y) -> draw();
				graphics.popTransform();
			}
		}
	}
	void reset(size_t x, size_t y, T* ptr)
	{
		reset(x, y, std::unique_ptr<T>(ptr));
	}
	void reset(size_t x, size_t y, std::unique_ptr<T> ptr)
	{
		assert(x < width()); assert(y < height());
		element(x, y) = std::move(ptr);
	}
};

#endif // GRID_HPP
