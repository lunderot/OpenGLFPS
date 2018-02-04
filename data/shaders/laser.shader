<<<GS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 campos;

	layout(lines) in;
	layout(triangle_strip, max_vertices = 8) out;

	mat4 rotationMatrix(vec3 axis, float angle)
	{
		axis = normalize(axis);
		float s = sin(angle);
		float c = cos(angle);
		float oc = 1.0 - c;

		return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
			oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
			oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	void main()
	{
		vec4 particleToCam = vec4(campos, 1) - model * gl_in[0].gl_Position;
		vec4 up = gl_in[1].gl_Position - gl_in[0].gl_Position;
		vec4 right = vec4(normalize(cross(particleToCam.xyz, up.xyz)), 0);

		mat4 rot = rotationMatrix(vec3(right), 180);
		vec4 ptoc = normalize(vec4(campos, 0) - model * (gl_in[0].gl_Position));
		vec4 p = ptoc * rot;

		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up - right + p);
		EmitVertex();

		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up + right + p);
		EmitVertex();

		
		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up - right);
		EmitVertex();
		
		gl_Position = projview * model * vec4(gl_in[0].gl_Position - up + right);
		EmitVertex();
		
		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up - right);
		EmitVertex();
		
		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up + right);
		EmitVertex();

		rot = rotationMatrix(vec3(right), 90);
		ptoc = normalize(vec4(campos, 0) - model * (gl_in[0].gl_Position + up));
		p = ptoc * rot;


		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up - right + p);
		EmitVertex();
		
		gl_Position = projview * model * vec4(gl_in[0].gl_Position + up + right + p);
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