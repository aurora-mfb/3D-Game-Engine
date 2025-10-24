
uniform mat4 mvp;

attribute vec3 vpos;
attribute vec3 vcolor;
attribute vec2 vtex;

varying vec2 ftex;
varying vec3 fcolor;

void main() {
    gl_Position = mvp * vec4(vpos, 1.0);
    ftex = vtex;
    fcolor = vcolor; // Para usar color si no hay textura
}