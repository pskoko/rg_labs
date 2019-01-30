#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vVel;
layout (location = 2) in vec2 vAtr;


uniform float time;
uniform mat4 view;
uniform mat4 projection;

out vec4 outPos;

void main()
{
    
    outPos = projection * view * vec4(vPos + (time - vAtr.x)*vVel, 1.0);
    gl_Position = outPos;


    outPos.w = vAtr.y;
}
