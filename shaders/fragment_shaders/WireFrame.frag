#version 330 core

out vec4 FragColor;
const vec3 color = vec3(0.98,0.28,0.125);

void main(){
    FragColor = vec4(color,1);
}