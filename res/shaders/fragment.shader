#version 330 core

in vec2 vTexCoord;
in float vLight;
out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoord);
    FragColor = vec4(texColor.rgb * vLight, texColor.a);
}
