#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TLightPos;
in vec3 TViewPos;
in vec3 TFragPos;
in vec3 realNormal;


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float heightScale;
uniform int type;
uniform int numLayers;

void main()
{
    vec3 viewDir = viewPos - FragPos;
    if(type != 0){
        viewDir = normalize(TViewPos - TFragPos);
    }

    vec2 texCoords = TexCoords;

    if(type == 2){
        float height = texture(depthMap, texCoords).r;
        texCoords  -= viewDir.xy/viewDir.z *height * heightScale;
    }
    if(type == 3 || type == 4){
        float layerWidth = 1.0f/numLayers;
        float currentDepth = 0.0f;

        vec2 deltaTx = viewDir.xy/viewDir.z * heightScale /numLayers;

        float currentMapDepth = texture(depthMap, texCoords).r;
        while(currentDepth < currentMapDepth){
            texCoords -= deltaTx;
            currentDepth += layerWidth;
            currentMapDepth = texture(depthMap, texCoords).r;
        }

        if(type == 4){
            vec2 prevText = texCoords + deltaTx;
            float nextDepth = currentMapDepth - currentDepth;
            float prevDepth = texture(depthMap, prevText).r - currentDepth + layerWidth;

            float t = nextDepth/(nextDepth-prevDepth);
            texCoords = mix(prevText, texCoords, 1.0-t);

        }

    }
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;
    vec3 normal = realNormal;
    if(type != 0){
        normal = texture(normalMap, texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }

    vec3 color = texture(diffuseMap, texCoords).rgb;
    vec3 ambient = 0.1 * color;

    vec3 lightDir = lightPos - FragPos;
    if(type != 0){
        lightDir = normalize(TLightPos - TFragPos);
    }
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);

    if(type == 0){
        FragColor = vec4(color, 1.0);
    }
}
