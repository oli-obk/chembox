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
#include <ImageData.hpp>

#include <cstdint>

// Colour based on integer values (0..255)
typedef struct _color_i
{
    uint8_t red, green, blue, alpha;
} Color_i;


// Colour based on float values (0.0..1.0)
typedef struct _color_f
{
    float red, green, blue, alpha;
} Color_f;


#define VERTICES_IN_PARTICLE 4

// A single particle.
typedef struct _particle
{
    // State.
    float x, y;
    float center_x, center_y;
    float velocity_x, velocity_y;
    float angular_velocity;

    Color_f color;

    // Rate of change
    float fade;
    float scale;
    float zoom;
    float friction;
    float angle;

    // Time to die.
    float time_to_live;
} Particle;
typedef struct _vertex2d
{
    float x, y;
} Vertex2d;
typedef struct _tex_info
{
   uint id;
   float left;
   float right;
   float top;
   float bottom;
} TextureInfo;

class ParticleEmitter
{
    Gosu::Graphics& graphics;
    const Gosu::Image image;
    Gosu::ZPos z;
    size_t width; // Width of image.
    size_t height; // Height of image.
    TextureInfo texture_info; // Texture coords and id.

    Particle* particles;

    Color_i* color_array; // Color array.
    size_t color_array_offset; // Offset to colours within VBO.

    Vertex2d* texture_coords_array; // Tex coord array.
    size_t texture_coords_array_offset; // Offset to texture coords within VBO.

    Vertex2d* vertex_array; // Vertex array.
    size_t vertex_array_offset; // Offset to vertices within VBO.

    // VBO and client-side data arrays.
    unsigned int vbo_id;

    size_t count; // Current number of active particles.
    size_t max_particles; // No more will be created if max hit.
    size_t next_particle_index; // Next place to create a new particle (either dead or oldest living).

    // do not copy
    ParticleEmitter(const ParticleEmitter&) = delete;
    ParticleEmitter& operator=(const ParticleEmitter&) = delete;
    void init_vbo();
    void draw_vbo();
    void update_vbo();
    bool color_changes() const;
    bool texture_changes() const;
    static bool initialized_fast_math;
    void update_particle(Particle* p, const float delta);
public:
    size_t getCount() const { return count; }
    ParticleEmitter(Gosu::Graphics& graphics, std::wstring filename, Gosu::ZPos z);
    ~ParticleEmitter();
    void emit(Particle p);
    void update(const float delta);
    void draw();
};

#endif // PARTICLE_EMITTER_HPP
