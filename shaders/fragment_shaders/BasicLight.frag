#version 330 core
#define MAX_POINT_LIGHTS 2
//We declare an output, we only need one
out vec4 FragColor;

in VS_OUT{
    vec2 TexCoord;
    vec3 aNormal;
    vec3 FragPos;
} fr_in;


struct Material{
    bool has_TexDiffuse;
    sampler2D texture_diffuse1;
    bool has_TexSpecular;
    sampler2D texture_specular1;
    vec3 ambient;
    vec3 diffuse;
    float shininess;
};

struct DirLight{        //160
                        //Base aligment //aligned offset
    vec3 directionVS;   //16            //0

    vec3 ambient;       //16            //16
    vec3 diffuse;       //16            //32
    vec3 specular;      //16            //48
                                        //64
};

struct PointLight{      //0
                        //Base aligment //aligned offset
    vec3 positionVS;    //16            //0

    vec3 ambient;       //16            //16
    vec3 diffuse;       //16            //32
    vec3 specular;      //16            //48

    float constant;     //4             //64
    float linear;       //4             //68
    float quadratic;    //4             //72
                                        //76
};

struct SpotLight{       //224
                        //Base aligment //aligned offset
    vec3 positionVS;    //16            //0

    vec3 directionVS;   //16            //16
    vec3 diffuse;       //16            //32
    vec3 specular;      //16            //48

    float constant;     //4             //64 (60 ???)
    float linear;       //4             //68 (64 ???)
    float quadratic;    //4             //72 (70 ???)

    float cutOff;       //4             //76 (72 ???)
    float outerCutOff;  //4             //80 (76 ???)
};                                      //84 (80 ???)

uniform Material material;


vec3 CalcDirLight(DirLight light,vec3 normal, vec3 texDiffColor, vec3 texSpecColor){
    vec3 viewDir = normalize(-fr_in.FragPos);
    vec3 lightDir = normalize(-light.directionVS);
    //Ambient
    vec3 ambient = light.ambient *texDiffColor* material.ambient;
    //Difuse
    float diff = max(dot(normal,lightDir),0);
    vec3 diffuse = (diff * material.diffuse) * texDiffColor * light.diffuse;
    //Specular
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0), material.shininess);
    vec3 specular = light.specular * (spec * texSpecColor);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal, vec3 fragPos, vec3 texDiffColor, vec3 texSpecColor){
    vec3 viewDir = normalize(-fr_in.FragPos);
    vec3 lightDir = normalize(light.positionVS - fragPos);
    float distance = length(light.positionVS - fr_in.FragPos);
    float constant = clamp(light.constant,0.0,1.0);

    float attenuation = 1.0/(constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
    //Ambient
    vec3 ambient = light.ambient * texDiffColor * material.ambient;
    ambient *= attenuation;
    //Difuse
    float diff = max(dot(normal,lightDir),0);
    vec3 diffuse = (diff * material.diffuse) * texDiffColor * light.diffuse;
    diffuse *= attenuation;
    //Specular
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0), material.shininess);
    vec3 specular = light.specular * (spec * texSpecColor);
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 texDiffColor, vec3 texSpecColor){
    vec3 viewDir = normalize(-fragPos);
    float distance = length(light.positionVS - fragPos);
    float attenuation = 1.0/(light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
    vec3 lightDir = normalize(light.positionVS - fragPos);
    //Inner cone
    float theta = dot(lightDir,normalize(-light.directionVS));
    //calculates intensity
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);
    //Difuse
    float diff = max(dot(normal,lightDir),0);
    vec3 diffuse = (diff * material.diffuse) * texDiffColor * light.diffuse;
    diffuse *=  intensity * attenuation;
    //Specular
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0), material.shininess);
    vec3 specular = light.specular * (spec * texSpecColor);
    specular *=  intensity * attenuation;

    return (diffuse + specular);
}

layout(std140) uniform Lights{                  //Base aligment //aligned offset
    PointLight pointLights[MAX_POINT_LIGHTS];   //76            //0
                                                //76            //76->80
    DirLight dirLight;                          //64            //156->160
    SpotLight spotLight;                        //84 (80 ???)            //224

    int hasSpotLight;                           //4             //308   (304???)
    int n_point_lights;                         //4             //312   (308???)
                                                                //(316) (312???)
};
void main()
{
    vec4 texDiffColor = vec4(1);
    vec4 texSpecColor = vec4(1);
    if(material.has_TexDiffuse){
        texDiffColor = texture(material.texture_diffuse1,fr_in.TexCoord);
    }
    if(material.has_TexSpecular){
        texSpecColor = texture(material.texture_specular1,fr_in.TexCoord);
    }
    
    vec3 norm = normalize(fr_in.aNormal);
    vec3 result = CalcDirLight(dirLight, norm,vec3(texDiffColor),vec3(texSpecColor));

    if (hasSpotLight > 0 ){
        result += CalcSpotLight(spotLight, norm, fr_in.FragPos,vec3(texDiffColor),vec3(texSpecColor));
    }
    for(int i = 0; i < n_point_lights && i < MAX_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], norm, fr_in.FragPos,vec3(texDiffColor),vec3(texSpecColor));
    }

    FragColor = vec4(result,1);

} 