#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _Model;

void main(){
	Normal = vNormal;
	gl_Position = _Model * vec4(vPos, 1.0);
	//Convert from RHS (right hand side) to LHS (left hand side)
	gl_Position.z *= -1.0;
}

//in .vert
// layout(location = 0) in vec3 vPos;
// layout(location = 1) in vec3 vNormal;
// //  out Surface {
// vec3 Normal;
// }vs_out;
// 
// uniform mat4 _Model;
// 
//vs_out.Normal = vNormal;
//glPosition = _Model * vec4(vPos, 1.0);
//gl_Position.z *= -1.0;