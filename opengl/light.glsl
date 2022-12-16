#version 460 core

layout(std140, binding = 0) uniform Camera {
    vec4 position;
    vec4 direction;
    mat4 view;
    mat4 projection;
} camera;


#ifndef _RENDERER_H
#define _RENDERER_H

// uniform blocks >= 10 are reserved for internal use only
layout(std140, binding = 10) uniform RendererInput {
    ivec2 resolution;     // viewport size in pixels
    ivec2 cursor_pos;     // cursor position relative to viewport's upper-left corner
    float near_clip;      // frustum near clip distance
    float far_clip;       // frustum far clip distance
    float time;           // number of seconds since window created
    float delta_time;     // number of seconds since the last frame
    bool  depth_prepass;  // early z test
    uint  shadow_index;   // index of the shadow map render pass (one index per light source)
} rdr_in;

// default-block (loose) uniform locations >= 1000 are reserved for internal use only
struct self_t {
    mat4 transform;    // 1000, model matrix of the current entity
    uint material_id;  // 1001, current mesh's material id
    uint ext_1002;
    uint ext_1003;
    uint ext_1004;
    uint ext_1005;
    uint ext_1006;
    uint ext_1007;
};

layout(location = 1000) uniform self_t self;

#endif


////////////////////////////////////////////////////////////////////////////////

#ifdef vertex_shader

layout(location = 0) in vec3 position;

void main() {
    gl_Position = camera.projection * camera.view * self.transform * vec4(position, 1.0);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef fragment_shader

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bloom;

layout(location = 3) uniform vec3  light_color;
layout(location = 4) uniform float light_intensity;
layout(location = 5) uniform float bloom_factor;

// light sources are often rendered with bloom effect to simulate light rays bleeding so
// we can always write to the second render target regardless of the luminance threshold
// check, the bloom factor controls the saturation of bloom, > 1 = amplify, < 1 = reduce

void main() {
    float fade_io = 0.3 + abs(cos(rdr_in.time));
    float intensity = light_intensity * fade_io;

    // if the 2nd MRT isn't enabled, bloom will write to GL_NONE and be discarded
    color = vec4(light_color * intensity, 1.0);
    bloom = intensity > 0.2 ? vec4(color.rgb * bloom_factor, 1.0) : vec4(0.0);
    //bloom=vec4(color.rgb * bloom_factor, 1.0);

}

#endif
