#version 450 core
#ifdef vertex_shader

layout(location = 4)uniform mat4 mvp;
layout(location = 5)uniform mat4 model;

layout(location = 0) in vec3 vertex_position;
//layout(location = 2) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_texcoord;

//out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_texcoord;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0);
    //frag_normal = mat3(model) * vertex_normal;
    frag_color = vec4(1.0,1.0,1.0,1.0);
    frag_texcoord = vertex_texcoord;
}
#endif
#ifdef fragment_shader


//precision mediump float;


layout(binding = 0) uniform sampler2D texture_id;
//in vec3 frag_normal;
in vec4 frag_color;
in vec2 frag_texcoord;

out vec4 FragColor;

void main()
{

        FragColor = frag_color * texture2D(texture_id, frag_texcoord);


}
#endif