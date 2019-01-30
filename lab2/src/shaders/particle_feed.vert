layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vVel;


uniform float deltaTime;

out VertexData {
    vec3 vPos;
    vec4 vVel;
} outData;
void main(){
    outData.vPos = vPos + deltaTime*vVel;
    outData.vVel = vVel;
}
