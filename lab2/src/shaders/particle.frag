#version 330 core
out vec4 FragColor;

in vec4 outPos2;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main(){
    vec4 texCol = texture(ourTexture, texCoord);
    
    if(outPos2.w < 0 || texCol.a < 0.1) discard;
    FragColor = texCol;
}
