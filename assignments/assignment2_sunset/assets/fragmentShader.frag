#version 450
out vec4 FragColor;
in vec2 UV;
uniform float iTime;
uniform vec2 iResolution;
uniform vec3 bgColorDay1[1];
uniform vec3 bgColorDay2[1];
uniform vec3 bgColorNight1[1];
uniform vec3 bgColorNight2[1];
uniform vec3 sunColor[1];
uniform vec3 hillColor[1];
uniform float sunSpeed;
uniform float sunRadius;
float circleSDF(vec2 p, float r){
    return length(p)-r;
}

void main()
{
	// Normalized pixel coordinates (from 0 to 1)
	vec2 uv = UV;
    
    //Adjusting range to be from -1 to 1
    uv = uv * 2.0 - 1.0;
    
    //Set the speed at which the sun moves
    float n = mod((iTime * sunSpeed) / 2.0, 4.0) - 2.0;
    vec2 sunPos = vec2(0.0, 0.5 + (-n * n));
    
	//Background color
    vec3 bgColorA = mix(vec3(bgColorDay1[0]), vec3(bgColorDay2[0]), uv.y);
	vec3 bgColorB = mix(vec3(bgColorNight1[0]), vec3(bgColorNight2[0]), uv.y);
	vec3 bgColor = mix(bgColorA, bgColorB, uv.y - 0.5 - (-n * n));
    
    //Saving the original values for later use
    vec2 firstUV = uv;
    
    //Make the sun
    uv.x *= iResolution.x/iResolution.y;

	vec2 center = vec2(0.0, 0.0);
	float radius = sunRadius;
    
    //Building the sun
    float d = circleSDF(uv - sunPos,radius);
    d = smoothstep(-0.05,0.05,d);
	vec3 sunColor = vec3(sunColor[0]);
    
    bgColor = mix(sunColor, bgColor, d);
    
    //Making the hills
	float hills = -0.6 + sin(firstUV.x*12.56 + (iTime * 1.5))*0.1;
	hills = step(hills,firstUV.y);

	bgColor = mix(vec3(hillColor[0]), bgColor, hills);

	// Output to screen
	FragColor = vec4(bgColor, 1.0);
}

