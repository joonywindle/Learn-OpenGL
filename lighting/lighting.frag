#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
  float ambientStrength = 0.1f;
  float specularStrength = 0.5f;



  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos-FragPos);
  float diff = max(dot(norm,lightDir),0.0);
  vec3 diffuse = diff * lightDir;
  vec3 ambient = ambientStrength * lightColor;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
  }