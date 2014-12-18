uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd; // diffuse color of the material
uniform vec3 ks; // specular color of the material
uniform float s;
uniform float useTexture;

varying vec3 camVertexPos; // passed from vertex shader
varying vec3 camVertexNor; // passed from vertex shader

varying vec2 fragTexCoords;
uniform sampler2D texture;

void main()
{
  vec3 n = normalize(camVertexNor);
  vec3 l = normalize(lightPos - camVertexPos);
  float maxDot = max(dot(l, n), 0.0);
  vec3 diffuse = kd * maxDot;
  vec3 e = normalize(vec3(0.0, 0.0, 0.0) - camVertexPos);
  vec3 h = normalize(l + e);
  vec3 specular = ks * pow(max(dot(h, n), 0.0), s);

  if (useTexture > 0.0) {
    gl_FragColor = texture2D(texture, fragTexCoords);
  } else {
    gl_FragColor = vec4(ka + specular + diffuse, 1.0);    
  }
}
