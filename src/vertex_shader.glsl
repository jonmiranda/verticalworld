attribute vec4 vertPos; // in object space
attribute vec3 vertNor; // in object space
uniform mat4 P;
uniform mat4 MV;
uniform mat3 T1;

varying vec3 camVertexPos; // passed to vertex shader
varying vec3 camVertexNor; // passed to vertex shader

attribute vec2 vertTexCoords;
varying vec2 fragTexCoords;

void main()
{
	gl_Position = P * MV * vertPos;

  vec4 vertNor4 = vec4(vertNor.x, vertNor.y, vertNor.z, 0.0);
  vec4 camVertexNor4 = MV * vertNor4;
  camVertexNor = camVertexNor4.xyz;

  vec4 camVertexPos4 = MV * vertPos;
  camVertexPos = camVertexPos4.xyz;

  vec3 finalTextCoords = T1 * vec3(vertTexCoords, 1.0);
  fragTexCoords = finalTextCoords.xy;
}
