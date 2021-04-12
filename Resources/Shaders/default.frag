#version 330 core
/*
    Input: [vec2] texture coords from vertex shader
    Output: [vec4] color of text render by textColor
*/

in vec2 fTexCoord;
out vec4 color;

uniform sampler2D uTexture;

void main(){
    vec4 C = texture(uTexture, fTexCoord);
    color.r = pow( C.r, 1 / 2.2 );
    color.g = pow( C.g, 1 / 2.2 );
    color.b = pow( C.b, 1 / 2.2 );
    color.a = C.a;
}