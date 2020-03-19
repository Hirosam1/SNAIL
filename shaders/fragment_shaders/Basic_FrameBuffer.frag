#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D screenTex;
const float offset = 1.0 / 120.0; 
const float LBP_threshold = 0.17;

void main(){
    
    FragColor = texture(screenTex,TexCoord);
    /*
    FragColor = texture(screenTex,TexCoord);
    float average =  0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average,average,average,1.0);*/
    //FragColor = texture(screenTex,TexCoord);
    //FragColor =  vec4(vec3(1.0 - texture(screenTex,TexCoord)),1.0);
    //FragColor = vec4(0.3,1,1,1);


    /*
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );*/
    /*
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/
    /*
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );*/
    /*
    float kernel[9] = float[](
        1, 1, 1,
        1, -8,1,
        1, 1, 1
    );
    */
    /*
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTex, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);*/

}