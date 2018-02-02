<<<GS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 campos;

	layout(lines) in;
	layout(triangle_strip, max_vertices = 4) out;

	void main()
	{
		vec4 particleToCam = vec4(campos, 1) - model * gl_in[0].gl_Position;
		vec4 up = gl_in[1].gl_Position - gl_in[0].gl_Position;
		vec4 right = vec4(normalize(cross(particleToCam.xyz, up.xyz)), 0);

		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up - right);
		EmitVertex();

		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up - right);
		EmitVertex();

		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up + right);
		EmitVertex();

		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up + right);
		EmitVertex();

		EndPrimitive();
	}
GS>>>
<<<VS
	#version 330

	layout(location = 0) in vec2 pos;

    void main()
    {
        gl_Position = vec4(pos, 0.0, 1.0);
    }
VS>>>

<<<FS
	#version 330

	out vec4 fragment;

	void main()
	{
		fragment = vec4(1.0, 0.0, 0.0, 1.0);
	}
FS>>>