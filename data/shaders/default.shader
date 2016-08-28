<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 scale;
	uniform bool scaleuv;
	uniform vec3 lightposition;

	layout(location=0) in vec3 position;
	layout(location=1) in vec3 normal;
	layout(location=2) in vec2 uv;

	out vec3 normal_out;
	out vec2 uv_out;
	out vec3 to_light_out;

	void main()
	{
		normal_out = normalize((model * vec4(normal, 0)).xyz);
		if(scaleuv)
			uv_out = vec2(uv.x * max(scale.x, scale.z), uv.y * scale.y);
		else
			uv_out = uv;

		vec4 worldPosition = model * vec4(position, 1);
		to_light_out = lightposition - worldPosition.xyz;
		gl_Position = projview * worldPosition;
	}
VS>>>

<<<FS
	#version 330

	uniform vec3 lightcolor;

	out vec4 fragment;

	in vec2 uv_out;
	in vec3 normal_out;
	in vec3 to_light_out;

	uniform sampler2D tex;

	void main()
	{
		vec3 normal = normal_out;
		vec3 to_light = normalize(to_light_out);

		if (!gl_FrontFacing)
			normal *= -1.0;

		float brightness = max(dot(normal, to_light), 0);
		vec3 diffuse = brightness * lightcolor;

		fragment = vec4(diffuse, 1) * texture(tex, uv_out);
	}
FS>>>