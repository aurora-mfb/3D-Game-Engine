#version 330 core

// --- Atributos del vértice ---
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 vTex;
layout(location = 3) in vec3 vNormal;

// --- Uniformes ---
uniform mat4 mvp;          // Model-View-Projection
uniform mat4 modelView;    // Model * View
uniform mat4 normalMatrix; // Matriz para transformar normales correctamente

// --- Variables hacia el fragment shader ---
out vec3 fcolor;
out vec2 ftex;
out vec3 fpos;     // posición del vértice en espacio del observador
out vec3 fnormal;  // normal transformada en espacio del observador

void main() {
    // Calculamos posición del vértice en espacio del observador
    vec4 posView = modelView * vec4(vPos, 1.0);
    fpos = posView.xyz;

    // Normal transformada con matriz normal
    fnormal = (normalMatrix * vec4(vNormal, 0.0)).xyz;

    // Pasamos color y coordenadas de textura
    fcolor = vColor;
    ftex = vTex;

    // Calculamos posición final del vértice en clip space
    gl_Position = mvp * vec4(vPos, 1.0);
}