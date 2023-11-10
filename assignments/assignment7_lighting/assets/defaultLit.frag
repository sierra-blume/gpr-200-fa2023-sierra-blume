#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WPosition; //Pre-fragment interpolated world position
	vec3 WNormal; //Pre-fragment interpolated world normal
}fs_in;

struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light _Light;

uniform sampler2D _Texture;

float vAmbient;
float vDiffuse;
float vSpecular;
float vShine;

void main(){
	FragColor = texture(_Texture,fs_in.UV);
	vec3 normal = normalize(fs_in.WNormal);
	vec3 lightDir = normalize(_Light.position - fs_in.WPosition);

	//TODO: Lighting calculations using corrected normal
	//max(x,0)
	float diffAngle = max(dot(normal, lightDir),0);
	vec3 diffuse = _Light.color * diffAngle;

	//Do specular calculations

	//add diffuse and specular together
}