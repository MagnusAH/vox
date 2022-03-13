#version 450 core

in vec2 i_uv;
out vec4 frag;

uniform sampler2D texture0;

void main()
{
	vec3 rgb = texture(texture0, i_uv).rgb;
	frag = vec4(rgb, 1);
}