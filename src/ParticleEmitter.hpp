#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

// this file was created by porting Spooner's ashton's particle emitter code to c++

#include <map>
#include <string>
#include <Gosu/Fwd.hpp>
#include <Gosu/Color.hpp>
#include <array>
#include <cstdint>
#include <Gosu/Image.hpp>
#include <Gosu/Math.hpp>


#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define VERTICES_IN_PARTICLE 4

class ParticleInstance
{
    // State.
    float _x, _y;
    float _center_x, _center_y;
    float _velocity_x, _velocity_y;
    float _angular_velocity;

    float _gravity;
    Gosu::Color _color;

    // Rate of change
    float _fade;
    float _scale;
    float _zoom;
    float _friction;
    float _angle;

    // Time to die.
    float _time_to_live;
public:
    Gosu::Color getColor() const { return _color; }
    double getX() const { return _x; }
    double getY() const { return _y; }
    double getCenterX() const { return _center_x; }
    double getCenterY() const { return _center_y; }
    double getAngle() const { return _angle; }
    double getScale() const { return _scale; }
    void update(double delta);
    ParticleInstance()
    :_x(0),_y(0)
    ,_center_x(0), _center_y(0)
    ,_velocity_x(0), _velocity_y(0)
    ,_angular_velocity(0)
    ,_gravity(0)
    ,_color(Gosu::Color::WHITE)
    ,_fade(0)
    ,_scale(1.0)
    ,_zoom(0)
    ,_friction(0)
    ,_angle(0)
    ,_time_to_live(0)
    {}
    bool alive() const { return _time_to_live > 0; }
    ParticleInstance& gravity(double g) { _gravity = g; return *this; }
    ParticleInstance& pos(double x, double y) { _x = x; _y = y; return *this; }
    // set time to live
    ParticleInstance& ttl(double ttl) { _time_to_live = ttl; return *this; }
    // center position
    ParticleInstance& center(double x, double y) { _center_x = x; _center_y = y; return *this; }
    // move by x,y per update
    ParticleInstance& vel(double x, double y) { _velocity_x = x; _velocity_y = y; return *this; }
    // rotate by r degrees per update
    ParticleInstance& avel(double r) { _angular_velocity = r; return *this; }
    // color
    ParticleInstance& color(Gosu::Color col) { _color = col; return *this; }
    // zoom by factor zoom per update
    ParticleInstance& zoom(double zoom) { _zoom = zoom; return *this; }
    // fade out by factor fade per update
    ParticleInstance& fade(double fade) { _fade = fade; return *this; }
    ParticleInstance& scale(double scale) { _scale = scale; return *this; }
    ParticleInstance& friction(double frict) { _friction = frict; return *this; }
    ParticleInstance& angle(double angle) { _angle = angle; return *this; }
};

template<size_t max_particles>
class ParticleEmitter
{
    Gosu::Graphics& graphics;
    const Gosu::Image image;
    Gosu::ZPos z;
    std::array<ParticleInstance, max_particles> particles;
    typename decltype(particles)::iterator current;
    size_t num_particles;
    // do not copy
    ParticleEmitter(const ParticleEmitter&) = delete;
    ParticleEmitter& operator=(const ParticleEmitter&) = delete;
    std::mt19937 engine; // Mersenne twister MT19937
    class VBO
    {
        struct Vertex2d
        {
            float x, y;
        };
        union ParticleVertices
        {
            float floats[8];
            Vertex2d vertices[4];
        };
        struct ParticleColors
        {
            Gosu::Color colors[4];
        };
        std::array<ParticleVertices, max_particles> vertex_buffer;
        size_t vertex_buffer_offset;
        std::array<ParticleColors, max_particles> color_buffer;
        size_t color_buffer_offset;
        std::array<ParticleVertices, max_particles> tex_coord_buffer;
        size_t tex_coord_buffer_offset;
        unsigned int id;
        const ParticleEmitter<max_particles>& emitter;
    public:
        VBO(const ParticleEmitter<max_particles>& e);
        void update();
        void draw();
    };
    friend class VBO;
    VBO vbo;
public:
    size_t count() const { return num_particles; }
    ParticleEmitter(Gosu::Graphics& graphics, std::wstring filename, Gosu::ZPos z);
    ParticleEmitter(Gosu::Graphics& graphics, const Gosu::Bitmap& source, Gosu::ZPos z);
    void emit(ParticleInstance p);
    ParticleInstance& emit();
    void update(double delta);
    void draw();
};

template<size_t max_particles>
inline ParticleEmitter<max_particles>::ParticleEmitter(Gosu::Graphics& graphics, const Gosu::Bitmap& source, Gosu::ZPos z)
:graphics(graphics)
,image(graphics, source)
,z(z)
,current(particles.begin())
,num_particles(0)
,vbo(*this)
{
}

template<size_t max_particles>
inline ParticleEmitter<max_particles>::ParticleEmitter(Gosu::Graphics& graphics, std::wstring filename, Gosu::ZPos z)
:graphics(graphics)
,image(graphics, filename)
,z(z)
,current(particles.begin())
,num_particles(0)
,vbo(*this)
{
}

template<size_t max_particles>
inline void ParticleEmitter<max_particles>::emit(ParticleInstance p)
{
    emit() = p;
}

template<size_t max_particles>
inline ParticleInstance& ParticleEmitter<max_particles>::emit()
{
    // Find the first dead particle in the heap, or overwrite the oldest one.
    ParticleInstance& p = *current;
    // If we are replacing an old one, remove it from the count and clear it to fresh.
    if (p.alive()) {
        // Kill off and replace one with time to live :(
        p = ParticleInstance();
    } else {
        num_particles++; // Dead or never been used.
    }

    // Lets move the index onto the next one, or loop around.
    current++;
    if (current == particles.end()) {
        current = particles.begin();
    }

    return p;
}

template<size_t max_particles>
inline void ParticleEmitter<max_particles>::draw()
{
    if(count() == 0) return;

    // Enable the shader, if provided.
    //if(!NIL_P(emitter->rb_shader)) rb_funcall(emitter->rb_shader, rb_intern("enable"), 1, z);

    // Run the actual drawing operation at the correct Z-order.
    graphics.beginGL();
    vbo.draw();
    graphics.endGL();

    // Disable the shader, if provided.
    //if(!NIL_P(emitter->rb_shader)) rb_funcall(emitter->rb_shader, rb_intern("disable"), 1, z);
}

template<size_t max_particles>
inline void ParticleEmitter<max_particles>::VBO::draw()
{
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, emitter.image.getData().glTexInfo()->texName);

    glBindBuffer(GL_ARRAY_BUFFER, id);

    // Only use colour array if colours are dynamic. Otherwise a single colour setting is enough.
    if(true) //(color_changes(emitter))
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, (void*)color_buffer_offset);
    }
    else
    {
        //glColor4fv((GLfloat*)&emitter->color);
    }

    // Always use the texture array, even if it is static.
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)tex_coord_buffer_offset);

    // Vertex array will always be dynamic.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, (void*)vertex_buffer_offset);

    glDrawArrays(GL_QUADS, 0, emitter.count() * VERTICES_IN_PARTICLE);

    if(true/*color_changes(emitter)*/) glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

template<size_t max_particles>
inline void ParticleEmitter<max_particles>::update(double delta)
{
    if (delta < 0.0) {
        throw std::runtime_error("delta must be >= 0");
    }

    if (num_particles > 0)
    {
        for (ParticleInstance& p:particles) {
            // Ignore particles that are already dead.
            if (p.alive()) {
                p.update(delta);
                if (!p.alive()) {
                    num_particles--;
                }
            }
        }
        // Copy all the current data onto the graphics card.
        vbo.update();
    }
}

inline void ParticleInstance::update(double delta)
{
    // Apply friction
    _velocity_x *= 1.0 - _friction * delta;
    _velocity_y *= 1.0 - _friction * delta;

    // Gravity.
    _velocity_y += _gravity * delta;

    // Move
    _x += _velocity_x * delta;
    _y += _velocity_y * delta;

    // Rotate.
    _angle += _angular_velocity * delta;

    // Resize.
    _scale += _zoom * delta;

    // Fade out.
    _color.setAlpha( std::max(0.0, double(_color.alpha()) - _fade * delta));

    _time_to_live -= delta;

    // Die if out of time, invisible or shrunk to nothing.
    if((_time_to_live <= 0) ||
            (_color.alpha() <= 0) ||
            (_scale <= 0))
    {
        _time_to_live = 0;
    }
}

template<size_t max_particles>
inline ParticleEmitter<max_particles>::VBO::VBO(const ParticleEmitter<max_particles>& e)
:emitter(e)
{
    if(!GL_ARB_vertex_buffer_object)
    {
        throw std::runtime_error("ParticleEmitter requires GL_ARB_vertex_buffer_object, which is not supported by your OpenGL");
    }

    int num_vertices = max_particles * VERTICES_IN_PARTICLE;

    color_buffer_offset = 0;
    tex_coord_buffer_offset = sizeof(Gosu::Color) * num_vertices;
    vertex_buffer_offset = (sizeof(Gosu::Color) + sizeof(Vertex2d)) * num_vertices;

    int data_size = (sizeof(Gosu::Color) + sizeof(Vertex2d) + sizeof(Vertex2d)) * num_vertices;

    // Create the VBO, but don't upload any data yet.
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, data_size, NULL, GL_STREAM_DRAW);

    // Check the buffer was actually created.
    int buffer_size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
    if(buffer_size != data_size)
    {
        throw std::runtime_error("Failed to create a VBO to hold emitter data.");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Fill the tex coord array with all the same coords (won't be used if the image changes dynamically).
    auto texture_info = emitter.image.getData().glTexInfo();
    for (auto& coord:tex_coord_buffer) {
        coord.vertices[0].x = texture_info->left;
        coord.vertices[0].y = texture_info->top;

        coord.vertices[1].x = texture_info->right;
        coord.vertices[1].y = texture_info->top;

        coord.vertices[2].x = texture_info->right;
        coord.vertices[2].y = texture_info->bottom;

        coord.vertices[3].x = texture_info->left;
        coord.vertices[3].y = texture_info->bottom;
    }

    // Push whole array to graphics card.
    glBindBuffer(GL_ARRAY_BUFFER, id);

    glBufferSubData(GL_ARRAY_BUFFER, tex_coord_buffer_offset,
                       sizeof(Vertex2d) * VERTICES_IN_PARTICLE * max_particles,
                       tex_coord_buffer.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

template<size_t max_particles>
inline void ParticleEmitter<max_particles>::VBO::update()
{
    typedef typename decltype(emitter.particles)::const_iterator it_type;
    auto col_it = color_buffer.begin();
    auto write_colors_for_particles = [&col_it, this](it_type start, const it_type end){
        for (;start != end; start++) {
            const ParticleInstance& p = *start;
            if (p.alive()) {
                ParticleColors& cols = *col_it++;
                for (auto& col:cols.colors) {
                    col = p.getColor();
                }
            }
        }
        };

    auto vert_it = vertex_buffer.begin();
    auto write_vertices_for_particles = [&vert_it, this](it_type start, const it_type end){
        for (;start != end; start++) {
            const ParticleInstance& particle = *start;
            if (particle.alive()) {
                ParticleVertices& verts = *vert_it++;
                // Totally ripped this code from Gosu :$

                float sizeX = emitter.image.width() * particle.getScale();
                float sizeY = emitter.image.height() * particle.getScale();

                float offsX = Gosu::offsetX(particle.getAngle(), 1.0);
                float offsY = Gosu::offsetY(particle.getAngle(), 1.0);

                float distToLeftX   = +offsY * sizeX * particle.getCenterX();
                float distToLeftY   = -offsX * sizeX * particle.getCenterX();
                float distToRightX  = -offsY * sizeX * (1 - particle.getCenterX());
                float distToRightY  = +offsX * sizeX * (1 - particle.getCenterX());
                float distToTopX    = +offsX * sizeY * particle.getCenterY();
                float distToTopY    = +offsY * sizeY * particle.getCenterY();
                float distToBottomX = -offsX * sizeY * (1 - particle.getCenterY());
                float distToBottomY = -offsY * sizeY * (1 - particle.getCenterY());

                verts.vertices[0].x = particle.getX() + distToLeftX  + distToTopX;
                verts.vertices[0].y = particle.getY() + distToLeftY  + distToTopY;

                verts.vertices[1].x = particle.getX() + distToRightX + distToTopX;
                verts.vertices[1].y = particle.getY() + distToRightY + distToTopY;

                verts.vertices[2].x = particle.getX() + distToRightX + distToBottomX;
                verts.vertices[2].y = particle.getY() + distToRightY + distToBottomY;

                verts.vertices[3].x = particle.getX() + distToLeftX  + distToBottomX;
                verts.vertices[3].y = particle.getY() + distToLeftY  + distToBottomY;
            }
        }
        };

    // Ensure that drawing order is correct by drawing in order of creation...

    // First, we draw all those from after the current, going up to the last one.
    if(true) //(color_changes(emitter))
    {
        write_colors_for_particles(emitter.current, emitter.particles.end());
    }

#ifdef udtoluitaodtuiaodtuiabodtuniaodtruianedtrn
    if(texture_changes(emitter))
    {
        write_texture_coords_for_particles(emitter->texture_coords_array,
                                           emitter.current, emitter.particles.end(),
                                           emitter.image.getData().glTexInfo());
    }
#endif

    write_vertices_for_particles(   emitter.current, emitter.particles.end());

    // When we copy the second half of the particles, we want to start writing further on.
    // this is done by the col_it and vert_it iterators

    // Then go from the first to the current.
    if(true) //color_changes(emitter))
    {
        write_colors_for_particles(emitter.particles.begin(), emitter.current);
    }

#ifdef udtoluitaodtuiaodtuiabodtuniaodtruianedtrn
    if(texture_changes(emitter))
    {
        write_texture_coords_for_particles(&emitter->texture_coords_array[offset],
                                           first, last,
                                           &emitter->texture_info);
    }
#endif

    write_vertices_for_particles(   emitter.particles.begin(), emitter.current);

    // Upload the data, but only as much as we are actually using.
    glBindBuffer(GL_ARRAY_BUFFER, id);
    if(true) //(color_changes(emitter))
    {
        glBufferSubData(GL_ARRAY_BUFFER, color_buffer_offset,
                           sizeof(Gosu::Color) * VERTICES_IN_PARTICLE * emitter.count(),
                           color_buffer.data());
    }

#ifdef udtoluitaodtuiaodtuiabodtuniaodtruianedtrn
    if(texture_changes(emitter))
    {
        glBufferSubData(GL_ARRAY_BUFFER, emitter->texture_coords_array_offset,
                           sizeof(Vertex2d) * VERTICES_IN_PARTICLE * emitter.count(),
                           emitter->texture_coords_array);
    }
#endif

    glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_offset,
                       sizeof(Vertex2d) * VERTICES_IN_PARTICLE * emitter.count(),
                       vertex_buffer.data());

    glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}

#endif // PARTICLE_EMITTER_HPP
