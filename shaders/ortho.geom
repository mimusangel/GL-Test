#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 180) out;
in vec3 gColor[];

out vec3 fColor;

const float PI = 3.1415926;
const vec2	PIXEL = vec2(1.0 / 640.0, 1.0 / 360.0);
void main()
{
	fColor = gColor[0];
	for (int i = 0; i <= 179; i++)
	{
        float ang = PI * 2.0 / 179.0 * i;
		vec2 offsetPos = vec2(cos(ang), -sin(ang)) * 180.0 * PIXEL;
        vec4 offset = vec4(offsetPos, 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + offset;

        EmitVertex();
    }


    EndPrimitive();
}
