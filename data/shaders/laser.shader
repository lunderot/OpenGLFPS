<<<GS
	#version 330

	layout(points) in;
	layout(line_strip, max_vertices = 2) out;

	void main()
	{
		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
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