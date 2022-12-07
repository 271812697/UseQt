
#version 330 core

//precision mediump float;

uniform vec4 object_color;
uniform bool texture_mode;

uniform sampler2D texture_id;

//in vec3 frag_normal;
in vec4 frag_color;
in vec2 frag_texcoord;

out vec4 FragColor;

void main()
{
    //是否启用纹理
    if(texture_mode){
        FragColor = frag_color * texture2D(texture_id, frag_texcoord);
    }
    else{
        FragColor = frag_color;
    }
}
