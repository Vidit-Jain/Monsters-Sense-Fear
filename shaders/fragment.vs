#version 330 core
in vec2 TexCoords;
in vec4 FragPos;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform int LightsOn;
uniform vec2 PlayerPos;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    if (LightsOn == 0) {
        float distance = max(10.0f, length(FragPos - vec4(PlayerPos, 0.0f, 1.0f)));
        color = vec4((20/distance)*vec3(color),color.w);
    }
}