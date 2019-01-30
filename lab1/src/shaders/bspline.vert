#version 330 core

uniform mat4 vp;
layout (location = 0) in vec4 pos;

out vec3 ourColor;

void main(){
    ourColor = vec3(0.0, 1.0, 0.0);
    vec4 tpos = pos;
    if(pos.w == 0.0) {
        ourColor = vec3(1.0, 0.0, 0.0);
        tpos.w = 1;
    }
    gl_Position = vp*tpos;
}
