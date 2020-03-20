#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    TexCoords = aPos;
    vec4 color = projection * view * vec4(aPos,1.0);
    gl_Position = color.xyww;
}
