#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 outPos[];
out vec4 outPos2;
out vec2 texCoord;

uniform mat4 projection;
uniform float size;
uniform float ratio;
void main() {    
    float xs = size;
    float ys = size * ratio;
    gl_Position = gl_in[0].gl_Position + vec4(-xs, -ys, 0.0, 0.0); 
    texCoord = vec2(0.0, 0.0);
    outPos2 = outPos[0];
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( xs, -ys, 0.0, 0.0);
    texCoord = vec2(1.0, 0.0);
    outPos2 = outPos[0];
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4(-xs, ys, 0.0, 0.0); 
    texCoord = vec2(0.0, 1.0);
    outPos2 = outPos[0];
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(xs, ys, 0.0, 0.0);
    texCoord = vec2(1.0, 1.0);
    outPos2 = outPos[0];
    EmitVertex();

    EndPrimitive();
}  
