#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float iTime;
uniform float _moveSpeed;

void main(){
	UV = vUV;
	vec3 Pos = vPos;

	float x = mod((iTime * _moveSpeed), 5);
	float moveX = abs(mod((iTime * _moveSpeed), 10) - 5) -2.5;
	float moveY = x * x * -1 + 5 * x - 3;

	Pos += vec3(moveX, moveY, 0.0) * vec3(0.2);

	gl_Position = vec4(Pos, 2.0);
}