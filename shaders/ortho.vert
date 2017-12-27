#version 330 core
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;

out vec3 gColor;

void main()
{
	vec2 inScreen = (vPosition - vec2(640, 360)) / vec2(640, -360);
    gl_Position = vec4(inScreen, 0.0, 1.0);
	gColor = vColor;
}
