#version 460 core
#pragma optimize(off)

// caution: if the grid is rendered midway, cells interior may receive the clear color of the
// attached framebuffer, therefore subsequently rendered meshes could be blocked due to depth
// testing. Bear in mind that the grid is just a transparent quad on the X-Z plane except for
// pixels on the lines, it relies on alpha blending to work so needs to be drawn last.

// reference:
// https://ourmachinery.com/post/borderland-between-rendering-and-editor-part-1/
// https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

const float scale = 100.0;
const float lod_floor = 4.0;  // minimum number of pixels between lines before LOD could switch
const vec4 x_axis_color = vec4(220, 20, 60, 255) / 255.0;
const vec4 z_axis_color = vec4(0, 46, 255, 255) / 255.0;

layout(location = 0) uniform float cell_size = 2.0;
layout(location = 1) uniform vec4 thin_line_color = vec4(vec3(0.1), 1.0);
layout(location = 2) uniform vec4 wide_line_color = vec4(vec3(0.2), 1.0);  // every 10th line is thick

////////////////////////////////////////////////////////////////////////////////

#ifdef vertex_shader

layout(std140, binding = 0) uniform Camera {
    vec4 position;
    vec4 direction;
    mat4 view;
    mat4 projection;
} camera;

layout(location = 0) out vec2 _uv;

// vertices of the plane quad, in CCW winding order
const vec3 positions[6] = vec3[] (
    vec3(-1, 0, -1), vec3(-1, 0, 1), vec3(1, 0, 1),
    vec3(1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1)
);

void main() {
    uint index = camera.position.y >= 0 ? gl_VertexID : (5 - gl_VertexID);  // reverse winding order when y < 0
    vec3 position = positions[index] * scale;
    gl_Position = camera.projection * camera.view * vec4(position, 1.0);
    _uv = position.xz;  // limit the grid to the X-Z plane (y == 0)
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef fragment_shader

#ifndef _EXT_H
#define _EXT_H

////////////////////////////////////////////////////////////////////////////////

#define EPS      1e-5
#define PI       3.141592653589793
#define PI2      6.283185307179586
#define INV_PI   0.318309886183791  // 1 over PI
#define HLF_PI   1.570796326794897  // half PI
#define SQRT2    1.414213562373095
#define SQRT3    1.732050807568877
#define SQRT5    2.236067977499789
#define CBRT2    1.259921049894873  // cube root 2
#define CBRT3    1.442249570307408  // cube root 3
#define G_PHI    1.618033988749894  // golden ratio
#define EULER_E  2.718281828459045  // natural exponent e
#define LN2      0.693147180559945
#define LN10     2.302585092994046
#define INV_LN2  1.442695040888963  // 1 over ln2
#define INV_LN10 0.434294481903252  // 1 over ln10

#define clamp01(x) clamp(x, 0.0, 1.0)

////////////////////////////////////////////////////////////////////////////////

// computes the min/max component of a vec2/vec3/vec4
float min2(const vec2 v) { return min(v.x, v.y); }
float max2(const vec2 v) { return max(v.x, v.y); }
float min3(const vec3 v) { return min(min(v.x, v.y), v.z); }
float max3(const vec3 v) { return max(max(v.x, v.y), v.z); }
float min4(const vec4 v) { return min(min3(v.xyz), v.w); }
float max4(const vec4 v) { return max(max3(v.xyz), v.w); }

// logarithm base 10 and logarithm base 2
float log10(float x) { return log(x) * INV_LN10; }
float log2(float x) { return log(x) * INV_LN2; }

// checks if value x is in range (a, b), returns value type to avoid branching
float step3(float a, float x, float b) { return step(a, x) * step(x, b); }
vec2 step3(const vec2 a, const vec2 x, const vec2 b) { return step(a, x) - step(b, x); }
vec3 step3(const vec3 a, const vec3 x, const vec3 b) { return step(a, x) - step(b, x); }
vec4 step3(const vec4 a, const vec4 x, const vec4 b) { return step(a, x) - step(b, x); }

// optimizes lower power functions, removes the implicit `exp/log` call
float pow2(float x) { return x * x; }
float pow3(float x) { return x * x * x; }
float pow4(float x) { return x * x * x * x; }
float pow5(float x) { return x * x * x * x * x; }

// computes the luminance of a linear RGB color, sRGB must be converted to linear first
float luminance(const vec3 linear_rgb) {
    return dot(linear_rgb, vec3(0.2126, 0.7152, 0.0722));
}

vec3 hsl2rgb(float h, float s, float l) {
    vec3 u = mod(h * 6.0 + vec3(0.0, 4.0, 2.0), 6.0);
    vec3 v = abs(u - 3.0) - 1.0;
    return l + s * (clamp01(v) - 0.5) * (1.0 - abs(2.0 * l - 1.0));
}

vec3 hsv2rgb(float h, float s, float v) {
    if (s <= 1e-4) return vec3(v);  // zero saturation = grayscale color

    float x = fract(h) * 6.0;
    float f = fract(x);
    uint  i = uint(x);

    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i) {
        case 0u: return vec3(v, t, p);
        case 1u: return vec3(q, v, p);
        case 2u: return vec3(p, v, t);
        case 3u: return vec3(p, q, v);
        case 4u: return vec3(t, p, v);
        default: return vec3(v, p, q);
    }
}

vec3 hsl2rgb(const vec3 hsl) {
    return hsl2rgb(hsl.x, hsl.y, hsl.z);
}

vec3 hsv2rgb(const vec3 hsv) {
    return hsv2rgb(hsv.x, hsv.y, hsv.z);
}

// returns a rgb color from the continuous rainbow bar, based on the hue param (0 ~ 1)
// to create a smooth color transition, hue must be unidirectional, so once hue hits 1
// it must jump back to 0, such a value can be created from a modulo or fract function
vec3 rainbow(float hue) {
    return hsv2rgb(hue, 1.0, 1.0);
}

// returns a float that bounces between 0.0 and k as the value of x changes monotonically
float bounce(float x, float k) {
    return k - abs(k - mod(x, k * 2));
}

// packs a 32-bit float into a vec4 (RGBA format), each component of RGBA = 8 bits and 1/256 precision
// this is often used on mobiles that do not support a high precision format like GL_DEPTH_COMPONENT32
// for example, implementations of shadow mapping in WebGL have been using this code extensively
vec4 pack(float x) {
    const vec4 bit_shift = vec4(1.0, 255.0, 255.0 * 255.0, 255.0 * 255.0 * 255.0);
    const vec4 bit_mask = vec4(vec3(1.0 / 255.0), 0.0);
    vec4 v = fract(x * bit_shift);
    return v - v.gbaa * bit_mask;  // cut off values that do not fit in 8 bits
}

// unpacks a vec4 RGBA into a 32-bit precision floating point scalar, for the explanation see:
// https://stackoverflow.com/questions/9882716/packing-float-into-vec4-how-does-this-code-work
float unpack(vec4 v) {
    const vec4 bit_shift = vec4(1.0, 1.0 / 255.0, 1.0 / (255.0 * 255.0), 1.0 / (255.0 * 255.0 * 255.0));
    return dot(v, bit_shift);
}

// returns a naive pseudo-random number between 0 and 1, seed x can be any number within (-inf, +inf)
// note that this randomness is more concentrated around 0.5 and sparser at the two ends 0.0 and 1.0
// it's extremely flawed at the sinusoid's peaks, so seed x should not be close to multiples of PI/2
// source: https://thebookofshaders.com/10/
float random1D(float x) {
    return fract(sin(x) * 100000.0);
}

// returns a naive pseudo-random number between 0 and 1, based on a 2D seed vector
// the random pattern can be changed using different values of a, b and c, but the
// seed value (for each component) must be a floating point number between 0 and 1
float random2D(const vec2 uv) {
    const float a = 12.9898, b = 78.233, c = 43758.5453123;
    float dt = dot(uv.xy, vec2(a, b));
    float sn = mod(dt, PI);
    return fract(sin(sn) * c);
}

#endif


layout(location = 0) in vec2 _uv;
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bloom;

void main() {
    // higher derivative = farther cell = smaller LOD = less details = more transparent
    vec2 derivative = fwidth(_uv);
    float lod = max(0.0, log10(length(derivative) * lod_floor / cell_size) + 1.0);
    float fade = fract(lod);

    // cell size at LOD level 0, 1 and 2, each higher level is 10 times larger
    float cell_size_0 = cell_size * pow(10.0, floor(lod));
    float cell_size_1 = cell_size_0 * 10.0;
    float cell_size_2 = cell_size_1 * 10.0;

    derivative *= 4.0;  // each anti-aliased line covers up to 4 pixels

    // compute absolute distance to cell line centers for each LOD and pick max x/y to be the alpha
    // alpha_0 >= alpha_1 >= alpha_2
    float alpha_0 = max2(1.0 - abs(clamp01(mod(_uv, cell_size_0) / derivative) * 2.0 - 1.0));
    float alpha_1 = max2(1.0 - abs(clamp01(mod(_uv, cell_size_1) / derivative) * 2.0 - 1.0));
    float alpha_2 = max2(1.0 - abs(clamp01(mod(_uv, cell_size_2) / derivative) * 2.0 - 1.0));

    // line margins can be used to check where the current line is (e.g. x = 0, or y = 3, etc)
    vec2 margin = min(derivative, 1.0);
    vec2 basis = step3(vec2(0.0), _uv, margin);

    // blend between falloff colors to handle LOD transition and highlight world axis X and Z
    vec4 c = alpha_2 > 0.0
        ? (basis.y > 0.0 ? x_axis_color : (basis.x > 0.0 ? z_axis_color : wide_line_color))
        : (alpha_1 > 0.0 ? mix(wide_line_color, thin_line_color, fade) : thin_line_color);

    // calculate opacity falloff based on distance to grid extents
    float opacity_falloff = 1.0 - clamp01(length(_uv) / scale);

    // blend between LOD level alphas and scale with opacity falloff
    c.a *= (alpha_2 > 0.0 ? alpha_2 : alpha_1 > 0.0 ? alpha_1 : (alpha_0 * (1.0 - fade))) * opacity_falloff;

    color = c;
    bloom = c;  // also bloom the gridlines if MRT is enabled, else write to GL_NONE
};

#endif
