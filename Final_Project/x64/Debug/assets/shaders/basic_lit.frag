#version 330 core

struct DirLight {
    vec3 Direction;
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
};

struct PointLight {
    vec3 Position;

    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;

    float Constant;
    float Linear;
    float Quadratic;
};

out vec4 FragColor;

in vec4 vertexColor;
in vec3 fragNormal;
in vec3 fragPosition;

in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec3 eyePos;

#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform DirLight dirLight;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    //ambient color
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * light.AmbientColor;

    // diffuse color
    vec3 lightDir = normalize(light.Position - fragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.DiffuseColor;

    //specular color
    float specularStrength = 0.5;
    float shininess = 32;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * light.SpecularColor;

    float distance = length(light.Position - fragPosition);
    float attenuation = 1.0 / (light.Constant + (light.Linear * distance) + light.Quadratic * (distance * distance));

    return (diffuse + ambient + specular) * attenuation;
}

vec3 calcDirectionalLight(vec3 normal, vec3 viewDir) {
    //ambient color
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * dirLight.AmbientColor;

    // diffuse color
    vec3 lightDir = dirLight.Direction;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * dirLight.DiffuseColor;

    //specular color
    float specularStrength = 0.5;
    float shininess = 32;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * dirLight.SpecularColor;

    vec3 dirLightColor = (diffuse + ambient + specular);

    return dirLightColor;
}

void main() {
    vec3 objectColor = vertexColor.xyz * vec3(mix(texture(tex0, texCoord), texture(tex1, texCoord), 0.5)); //last arg: 0 - tex0; 0.5 - 50% mix; 1.0 - tex1

     vec3 norm = normalize(fragNormal);
     vec3 viewDir = normalize(eyePos - fragPosition);

    vec3 result = calcDirectionalLight(norm, viewDir);

    
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, viewDir);
    }
    

    //final color
    vec3 finalColor = result * objectColor;
    FragColor = vec4(finalColor, 1.0);
}