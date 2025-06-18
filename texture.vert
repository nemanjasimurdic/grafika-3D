#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTex;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

out vec2 TexCoord;

void main()
{
    gl_Position = uP * uV * uM * vec4(aPos, 0.0, 1.0);
    TexCoord = aTex;
}
