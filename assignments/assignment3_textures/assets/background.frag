#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _GalaxyTexture;
uniform sampler2D _PlanetTexture;
uniform sampler2D _Distortion;

uniform float iTime;
uniform float _DistStrength;
uniform float _Tiling;

void main(){
	float normalizedTime = sin(iTime) * 0.1;

	float distort = texture(_Distortion, UV).r * normalizedTime;

	vec2 uv = UV + distort * 0.1f;
	uv += distort * _DistStrength;

	vec4 galaxy = texture(_GalaxyTexture, (uv) * _Tiling);
	vec4 planet = texture(_PlanetTexture, (uv) * _Tiling);

	vec3 color = mix(galaxy.rgb, planet.rgb, planet.a * 0.5);

	FragColor = vec4(color, 1.0);
}