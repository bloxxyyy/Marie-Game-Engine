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
uniform float specularStrength;  // new uniform for specular intensity
uniform float shininess;         // new uniform for specular shininess

void main()
{
    // --- Base texture color ---
    vec3 texColor = texture(texture1, TexCoord).rgb;
    texColor *= ourColor; // optional tint

    // --- Ambient ---
    vec3 ambient = ambientStrength * lightColor;

    // --- Diffuse ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    // --- Specular ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // --- Combine ---
    vec3 result = (ambient + diffuse + specular) * texColor;

    FragColor = vec4(result, 1.0);
}
