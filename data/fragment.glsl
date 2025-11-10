#version 330 core

// --- Constantes ---
const int MAX_LIGHTS = 8;

// --- Estructura de las luces ---
struct Light {
    int type;                 // 0 = DIRECTIONAL, 1 = POINT
    vec3 color;
    vec3 position;            // solo para POINT
    vec3 direction;           // solo para DIRECTIONAL
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

// --- Uniformes ---
uniform sampler2D texSampler;
uniform int numLights;
uniform vec3 ambient;         // luz ambiente global
uniform vec4 materialColor;   // color difuso del material
uniform float shininess;      // brillo especular del material
uniform Light lights[MAX_LIGHTS];

// --- Variables del vertex shader ---
in vec3 fcolor;
in vec2 ftex;
in vec3 fpos;
in vec3 fnormal;

out vec4 fragColor;

void main() {
    // --- Normalizamos la normal ---
    vec3 N = normalize(fnormal);

    // --- Componentes de iluminación ---
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Si hay luces, empezamos con luz ambiente
    if (numLights > 0) {
        diffuse = ambient;
    }

    // --- Cálculo de la iluminación por luz ---
    for (int i = 0; i < numLights; i++) {
        vec3 L;
        float attenuation = 1.0;

        if (lights[i].type == 0) {
            // Luz direccional
            L = normalize(-lights[i].direction); // hacia la superficie
        }
        else {
            // Luz puntual
            L = lights[i].position - fpos;
            float dist = length(L);
            L = normalize(L);
            attenuation = 1.0 / (lights[i].constantAttenuation +
                                 lights[i].linearAttenuation * dist +
                                 lights[i].quadraticAttenuation * dist * dist);
        }

        // --- Difuso ---
        float NdotL = max(dot(N, L), 0.0);
        diffuse += lights[i].color * NdotL * attenuation;

        // --- Especular ---
        if (shininess > 0.0 && NdotL > 0.0) {
            vec3 V = normalize(-fpos); // vector hacia el observador
            vec3 H = normalize(L + V); // semivector
            float NdotH = max(dot(N, H), 0.0);
            specular += lights[i].color * pow(NdotH, shininess) * attenuation;
        }
    }

    // --- Color base (difuso + textura) ---
    vec4 baseColor = materialColor * vec4(fcolor, 1.0);
    vec4 texColor = texture(texSampler, ftex);
    vec3 finalColor = baseColor.rgb * texColor.rgb * diffuse + specular;

    fragColor = vec4(finalColor, baseColor.a);
}