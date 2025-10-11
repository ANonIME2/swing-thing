#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 cameraPos; //where is the center of camera view expressed in the coordinate system used by the vertices
uniform vec2 cameraSize; //how many points wide and tall is camera view. expressed in the coordinate system used by vertices

void main(){
	float aspectRatio = cameraSize.x / cameraSize.y
	gl_Position = vec4((aPos.x - cameraPos.x) / (cameraSize.x * 2) * aspectRatio, (aPos.y - cameraPos.y) / (cameraSize.y * 2), aPos.z, 1.0f);
}
