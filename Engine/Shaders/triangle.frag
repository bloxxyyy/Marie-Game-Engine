#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture1;

// Light properties
uniform vec3 lightColor;
uniform vec3 lightPos;

// Camera position
uniform vec3 viewPos;

// Lighting strengths
uniform float ambientStrength; 
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

void main()
{
    // --- Base texture color ---
    vec3 texColor = texture(texture1, TexCoord).rgb;
    texColor *= ourColor;

    // --- Diffuse + Specular ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // attenuation
    float distance = length(lightPos - FragPos);
    float constant = 0.5;
    float linear = 0.2;
    float quadratic = 2.0;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    // ambient
    vec3 ambient = ambientStrength * lightColor * attenuation;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor * attenuation;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor * attenuation;

    // --- Combine ---
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result * texColor, 1.0);
}
