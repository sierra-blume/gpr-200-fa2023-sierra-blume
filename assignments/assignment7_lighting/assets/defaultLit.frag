#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WPosition; //Pre-fragment interpolated world position
	vec3 WNormal; //Pre-fragment interpolated world normal
}fs_in;

uniform sampler2D _Texture;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

uniform vec3 cameraPos;
uniform int numLights;
uniform float vAmbient;
uniform float vDiffuse;
uniform float vSpecular;
uniform float vShine;
uniform int phong;

void main(){
	vec3 normal = normalize(fs_in.WNormal);
	vec3 cameraDir = normalize(cameraPos - fs_in.WPosition);
	vec3 lightColor = vec3(0);

	//TODO: Lighting calculations using corrected normal
	for(int i = 0; i < numLights; i++)
	{
		vec3 lightDir = normalize(_Lights[i].position - fs_in.WPosition);

		float diffAngle = max(dot(normal, lightDir),0);
		vec3 diffuse = _Lights[i].color * vDiffuse* diffAngle;

		vec3 specular = {0,0,0};

		if(phong == 0){
			//Blinn-Phong specular calculations
			vec3 halfVec = normalize(lightDir + cameraDir);
			float specAngle = max(dot(halfVec, normal), 0);
			specular = _Lights[i].color * vSpecular * pow(specAngle, vShine);
		}
		else{
			//Phong specular calculations
			vec3 r = reflect(-lightDir, normal);
			specular = _Lights[i].color * vSpecular * pow(max(dot(r, cameraDir), 0), vShine);
		}

		lightColor += (diffuse + specular) * 0.5;
	}

	lightColor += vAmbient;

	vec4 textColor = texture(_Texture, fs_in.UV);
	FragColor = vec4(textColor.rgb * lightColor, textColor.a);
}