#ifndef IMAGESTORE_H
#define IMAGESTORE_H

#include <Gosu/Image.hpp>
#include <memory>
#include <iostream>

template<class T>
class ImageStore
{
private:
    static std::weak_ptr<Gosu::Image> s_pImg;
    std::shared_ptr<Gosu::Image> m_pImg;
protected:
    const Gosu::Image& Image() const { return *m_pImg; };

    void draw(double x, double y, double z, double w, double h)
    {
        Gosu::Color col = Gosu::Color::WHITE;
        m_pImg -> getData() . draw(x   , y   , col,
                                   x+w , y   , col,
                                   x+w , y+h , col,
                                   x   , y+h , col,
                                   z, Gosu::amDefault);
    }

    ImageStore(Gosu::Graphics& g, const std::wstring filename, bool tileable)
    {
        m_pImg = s_pImg.lock();
        if (!m_pImg) {
            try {
                m_pImg.reset(new Gosu::Image(g, filename, tileable));
            } catch (std::runtime_error& e) {
                std::cerr << "could not load image file: " << std::string(std::begin(filename), std::end(filename)) << std::endl;
                throw e;
            }
            s_pImg = m_pImg;
        }
    }
    virtual ~ImageStore() {};
};

template<class T>
std::weak_ptr<Gosu::Image> ImageStore<T>::s_pImg;

#endif // IMAGESTORE_H
