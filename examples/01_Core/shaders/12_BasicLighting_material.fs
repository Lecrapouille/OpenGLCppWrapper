// Copyright: https://learnopengl.com/

#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// ----------------------------------------------------------------------------
// Data from vertex program
// ----------------------------------------------------------------------------
in struct v2f_s
{
   vec3 position;
   vec3 normal;
} v2f;

// ----------------------------------------------------------------------------
// Fragment program output
// ----------------------------------------------------------------------------
out vec4 fragColor;

// ----------------------------------------------------------------------------
// Fragment program
// ----------------------------------------------------------------------------
void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(v2f.normal);
    vec3 lightDir = normalize(light.position - v2f.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - v2f.position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // Final color
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
