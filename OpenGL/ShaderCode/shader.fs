#version 330 core
layout (location = 0) out vec4 FragColor;//输出的颜色值

in vec2 TexCoord;//传入的纹理坐标
in vec3 Normal;//传入的法相向量
in vec3 FragPos; //传入的顶点的世界位置

uniform vec3 cameraPos; //摄像机位置
uniform samplerCube skybox; //立方体贴图

//模型贴图
struct Material {
    float alpha; //透明度
    sampler2D diffuse;//漫反射贴图
    sampler2D specular;//镜面光贴图

    float shininess;//反光度
}; 
uniform Material material; //材质

#define LIGHT_NUM 6

struct DirLight {
    vec3 direction;

    vec3 ambient;//环境光强度与颜色
    vec3 diffuse;//漫反射光强度与颜色
    vec3 specular;//镜面光颜色与强度
};
uniform DirLight dirLights[LIGHT_NUM]; //定向光源属性
uniform int dirLightNum; //定向光源数量

struct PointLight {
    vec3 position;

    //衰减公式系数
    float constant;
    float linear;
    float quadratic;

    //强度与颜色
    vec3 ambient;//环境光强度与颜色
    vec3 diffuse;//漫反射光强度与颜色
    vec3 specular;//镜面光颜色与强度
};
uniform PointLight pointLights[LIGHT_NUM]; //点光源属性
uniform int pointLightNum; //点光源数量

struct SpotLight {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    //衰减公式系数
    float constant;
    float linear;
    float quadratic;

    //强度与颜色
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLights[LIGHT_NUM]; //聚光灯属性
uniform int spotLightNum; //聚光灯数量

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 result = vec3(0, 0, 0);
    // 第一阶段：定向光照
    for(int i = 0; i < dirLightNum; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < pointLightNum; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // 第三阶段：聚光
    for(int i = 0; i < spotLightNum; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    
    
    //接受的box反射光
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, norm);
    vec3 boxColor = texture(skybox, R).rgb;
    boxColor = boxColor * 0;
    result += boxColor;

    float aa = material.alpha;//texture(material.diffuse, TexCoord).a;
    FragColor = vec4(result, aa);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}