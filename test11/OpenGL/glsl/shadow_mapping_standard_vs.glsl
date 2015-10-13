#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 Fragment_color;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 FragmentPosition_lightspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;
uniform mat4 Matrix_Worldspace_to_Lightspace;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1.0);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(vertexPosition_modelspace, 1.0)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace, 1.0)).xyz;
	EyeDirection_cameraspace = vec3(0.0, 0.0, 0.0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace, 1.0)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace, 0.0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// Color of the vertex
	Fragment_color = vertexColor;

	//Compute fragment position light space
	FragmentPosition_lightspace = Matrix_Worldspace_to_Lightspace * M * vec4(vertexPosition_modelspace, 1.0);

}





//// Output data ; will be interpolated for each fragment.
////out vec3 FragmentColor;

//out VS_OUT{
//	vec3 FragPos;
//    vec3 Normal;
//    vec3 FragmentColor;
//    vec4 FragPosLightSpace;
//} vs_out;

//// Values that stay constant for the whole mesh.
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;

//uniform mat4 Matrix_Worldspace_to_Lightspace;

//void main(){
//    gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0f);
//    vs_out.FragPos = vec3(model * vec4(vertexPosition_modelspace, 1.0));
//    //#TODO Ç»Ç∫ãtçsóÒÇì]ínÇ∑ÇÈÇÃÇ©í≤Ç◊ÇÈ
//	vs_out.Normal = transpose(inverse(mat3(model))) * vertexNormal_modelspace;
//	//vs_out.Normal = (inverse(mat3(model))) * vertexNormal_modelspace;
//    vs_out.FragmentColor = vertexColor;
//    vs_out.FragPosLightSpace = Matrix_Worldspace_to_Lightspace * vec4(vs_out.FragPos, 1.0);	
//}
