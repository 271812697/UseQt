#version 330 core

//precision mediump float;

uniform mat4 mvp;
uniform mat4 model;

layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec4 vertex_color;
layout(location = 3) in vec2 vertex_texcoord;

//out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_texcoord;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0);
    //frag_normal = mat3(model) * vertex_normal;
    frag_color = vertex_color;
    frag_texcoord = vertex_texcoord;
}
