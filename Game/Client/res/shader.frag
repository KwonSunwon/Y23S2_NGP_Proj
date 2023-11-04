#version 330 core

out vec4 fragColor;

// in vec3 fColor;
in vec2 fTexture;

in vec3 fNormal;
in vec3 fPos;

uniform sampler2D tex;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientLight;

void main()
{
    vec3 ambient = ambientLight * lightColor;

    vec3 normalVector = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPos);
    float diffuseLight = max(dot(normalVector, lightDir), 0.0);
    vec3 diffuse = diffuseLight * lightColor;

    //int specularStrength = 128;
    //vec3 viewDir = normalize(viewPos - fPos);
    //vec3 reflectDir = reflect(-lightDir, normalVector);
    //float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), specularStrength);
    //vec3 specular = specularLight * lightColor;

    // vec3 result = (ambient + diffuse + specular) * fColor;
    // vec3 result = (ambient + diffuse + specular) * texture(tex, fTexture).rgb;
    // fragColor = vec4(result, 1.0);
    // fragColor = texture(tex, fTexture);

    fragColor = vec4(ambient + diffuse, 1.0) * texture(tex, fTexture);
}