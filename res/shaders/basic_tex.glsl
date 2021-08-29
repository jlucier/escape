#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

uniform mat4 u_MVP;
out vec2 v_TexCoord;

void main() {
    gl_Position = u_MVP * position;
    v_TexCoord = uv;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform sampler2D u_Texture;
in vec2 v_TexCoord;

void main() {
    color = texture(u_Texture, v_TexCoord);
};
