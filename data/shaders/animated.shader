<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform mat4 bone[4];

	layout(location=0) in vec3 position;
	layout(location=1) in vec3 normal;
	layout(location=2) in vec2 uv;
	layout(location=3) in vec4 weight;
	layout(location=4) in vec4 boneIndex;

	out vec3 normal_out;
	out vec2 uv_out;

	void main()
	{
		mat4 boneTransform = bone[int(boneIndex[0])] * weight[0];
		boneTransform += bone[int(boneIndex[1])] * weight[1];
		boneTransform += bone[int(boneIndex[2])] * weight[2];
		boneTransform += bone[int(boneIndex[3])] * weight[3];

		normal_out = normalize((model * boneTransform * vec4(normal, 1.0)).xyz);
		uv_out = uv;
		gl_Position = projview * model * boneTransform * vec4(position, 1.0);
	}
VS>>>

<<<FS
	#version 330

	out vec4 fragment;

	in vec2 uv_out;
	in vec3 normal_out;

	uniform sampler2D tex;

	void main()
	{
		vec3 normal = normal_out;
		vec4 texColor = texture(tex, uv_out);
		fragment = texColor;
	}
FS>>>