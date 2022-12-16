#version 460 core
#pragma optimize(off)
#ifdef vertex_shader
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 uv2;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 binormal;
layout(location = 6) in ivec4 bone_id;
layout(location = 7) in vec4 bone_wt;

layout(location = 0) out _vtx {
    out vec3 _localpos;
    out vec3 _position;
    out vec3 _normal;
    out vec2 _uv;
};
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 projection;
layout(location = 100) uniform mat4 bone_transform[150];  // up to 150 bones

mat4 CalcBoneTransform() {
    mat4 T = mat4(0.0);
    for (uint i = 0; i < 4; ++i) {
        if (bone_id[i] >= 0) {
            T += (bone_transform[bone_id[i]] * bone_wt[i]);
        }
    }
    // if (length(T[0]) == 0) { return mat4(1.0); }
    return T;
}

void main()
{

   _uv=uv;
   _localpos=position;
   _normal=normal;
    //gl_Position = projection*view*vec4(position, 1.0);
    gl_Position = projection*view*CalcBoneTransform()*vec4(position, 1.0);
}
#endif
#ifdef fragment_shader
layout(location = 0) in _vtx {
    in vec3 _localpos;
    in vec3 _position;
    in vec3 _normal;
    in vec2 _uv;
};
layout(binding = 0) uniform samplerCube environment_map;
layout(binding = 1) uniform sampler2D albedo_map;
out vec4 FragColor;
void main()
{
   FragColor = texture(environment_map, _localpos);//vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
#endif