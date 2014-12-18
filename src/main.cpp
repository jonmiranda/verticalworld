//
// sueda
// October, 2014
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include <iostream>
#include "GLSL.h"
#include "Camera.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "Character.h"
#include "Image.h"

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)

using namespace std;

int time0;
bool keyDown[256] = {false};
bool facingForward = true;
glm::vec2 mouse(0.0f, 6.28f);

Camera camera;
Character* character;
Character* character2;
Shape* flag;
vector<Shape*> shapes;
vector<Shape*> tiles;

int numFloors = 9;
int numFloorTiles = 11;

GLuint pid;
GLint h_vertPos;
GLint h_vertNor;
GLint h_P;
GLint h_MV;
GLint h_lightPos;
GLint h_ka;
GLint h_kd;
GLint h_ks;
GLint h_s;
GLint h_useTexture;

// Particle stuff
float t = 0.0f;
float h = 0.001f;
vector<Particle*> particles;

// Texture stuff
vector<float> texCoordsBuf;
GLuint texCoordsBufID;
GLint h_vertTexCoords;
GLint h_texture;
vector<GLuint*> textures;
vector<const char*> textureNames;
GLint h_T1;
glm::mat3 T1(1.0);

GLuint texture0ID;
GLuint texture1ID;
GLuint texture2ID;
GLuint texture3ID;
GLuint texture4ID;
GLuint texture5ID;
GLuint texture6ID;
GLuint texture7ID;
GLuint texture8ID;
GLuint texture9ID;

vector<float> floorThresholds;
vector<float> ceilThresholds;

int floorHasTile[11][12] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
};

float getRandom(int max) {
	return (rand() % max);
}

int getRandomInt(int max) {
	return abs((int) getRandom(max));
}

glm::vec3 getRandomColor() {
	return glm::vec3(getRandomInt(255) / 255.0f, getRandomInt(255) / 255.0f, getRandomInt(255) / 255.0f);
}

void loadScene()
{
	time0 = 0;
	int numShapes = shapes.size();
	for (int i = 0; i < numShapes; ++i) {
		shapes[i]->load();
	}

	int numparticles = particles.size();
	for (int i = 0; i < numparticles; ++i) {
		particles[i]->load();
	}

	texCoordsBuf.push_back(0.0f);
	texCoordsBuf.push_back(0.0f);
	texCoordsBuf.push_back(1.0f);
	texCoordsBuf.push_back(0.0f);
	texCoordsBuf.push_back(0.0f);
	texCoordsBuf.push_back(1.0f);
	texCoordsBuf.push_back(1.0f);
	texCoordsBuf.push_back(1.0f);
}

void initGL()
{
	textures.push_back(&texture0ID);
	textures.push_back(&texture1ID);
	textures.push_back(&texture2ID);
	textures.push_back(&texture3ID);
	textures.push_back(&texture4ID);
	textures.push_back(&texture5ID);
	textures.push_back(&texture6ID);
	textures.push_back(&texture7ID);


	textureNames.push_back("../images/1.bmp");
	textureNames.push_back("../images/2.bmp");
	textureNames.push_back("../images/3.bmp");
	textureNames.push_back("../images/4.bmp");
	textureNames.push_back("../images/5.bmp");
	textureNames.push_back("../images/6.bmp");
	textureNames.push_back("../images/7.bmp");
	textureNames.push_back("../images/8.bmp");

	//////////////////////////////////////////////////////
	// Initialize GL for the whole scene
	//
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	
	//////////////////////////////////////////////////////
	// Initialize the geometry
	//
	int numShapes = shapes.size();
	for (int i = 0; i < numShapes; ++i) {
		shapes[i]->init();
	}

	int numparticles = particles.size();
	for (int i = 0; i < numparticles; ++i) {
		particles[i]->init();
	}

	// Send the texcoord array to the GPU
	glGenBuffers(1, &texCoordsBufID);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufID);
	glBufferData(GL_ARRAY_BUFFER, texCoordsBuf.size()*sizeof(float), &texCoordsBuf[0], GL_STATIC_DRAW);

	//////////////////////////////////////////////////////
	// Initialize shaders
	//
	
	// Create shader handles
	string vShaderName = "vertex_shader.glsl";
	string fShaderName = "fragment_shader.glsl";
	GLint rc;
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
	glCompileShader(VS);
	GLSL::printError();
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	GLSL::printError();
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
	}
	
	// Create the program and link
	pid = glCreateProgram();
	glAttachShader(pid, VS);
	glAttachShader(pid, FS);
	glLinkProgram(pid);
	GLSL::printError();
	glGetProgramiv(pid, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(pid);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
	}
	
	h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
	h_vertNor = GLSL::getAttribLocation(pid, "vertNor");
	h_P = GLSL::getUniformLocation(pid, "P");
	h_MV = GLSL::getUniformLocation(pid, "MV");
	h_lightPos = GLSL::getUniformLocation(pid, "lightPos");
	h_ka = GLSL::getUniformLocation(pid, "ka");
	h_kd = GLSL::getUniformLocation(pid, "kd");
	h_ks = GLSL::getUniformLocation(pid, "ks");
	h_s = GLSL::getUniformLocation(pid, "s");
	h_useTexture = GLSL::getUniformLocation(pid, "useTexture");

	h_T1 = GLSL::getUniformLocation(pid, "T1");
	h_texture = GLSL::getUniformLocation(pid, "texture");
	h_vertTexCoords = GLSL::getAttribLocation(pid, "vertTexCoords");

	//////////////////////////////////////////////////////
	// Intialize textures
	//////////////////////////////////////////////////////
	for (int i = 0; i < 8; ++i) {
		Image *image = (Image *) malloc(sizeof(Image));
		if (image == NULL) {
			printf("Error allocating space for image");
		}
		if (!ImageLoad(textureNames[i], image)) {
			printf("Error loading texture image: %s\n", textureNames[i]);
		}
		// Set active texture to texture unit 0
		glActiveTexture(GL_TEXTURE0 + i);
		// Generate a texture buffer object
		glGenTextures(1, textures[i]);
		// Bind the current texture to be the newly generated texture object
		glBindTexture(GL_TEXTURE_2D, *(textures[i]));

		// Load the actual texture data
		// Base level is 0, number of channels is 3, and border is 0.
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY,
					 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
			// Free image, since the data is now on the GPU
		free(image);

		// Generate image pyramid
		glGenerateMipmap(GL_TEXTURE_2D);
		// Set texture wrap modes for the S and T directions
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		// Set filtering mode for magnification and minimification
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Texture matrix for mustang
	T1[0][0] = 1.0f;
	T1[0][1] = 0.0f;
	T1[0][2] = 0.0f;
	T1[1][0] = 0.0f;
	T1[1][1] = 1.0f;
	T1[1][2] = 0.0f;
	T1[2][0] = 0.0f;
	T1[2][1] = 0.0f;
	T1[2][2] = 1.0f;


	// Check GLSL
	GLSL::checkVersion();
}

void reshapeGL(int w, int h)
{
	// Set view size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// Set camera aspect ratio
	camera.setWindowSize((float)w, (float)h);
}

void drawGL()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Create matrix stacks
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	camera.applyProjectionMatrix(&P);
	MV.pushMatrix();
	camera.applyViewMatrix(&MV);

	glUseProgram(pid);
	glUniformMatrix4fv( h_P, 1, GL_FALSE, glm::value_ptr( P.topMatrix()));
	glUniform3fv(h_lightPos, 1, glm::value_ptr(glm::vec3(0.0f, -1.0f, -5.0f)));
	glUniform3fv(h_ka,  1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	glUniform3fv(h_ks, 1, glm::value_ptr(glm::vec3(1.0f, 0.9f, 0.8f)));
	glUniform3fv(h_kd, 1, glm::value_ptr(glm::vec3(1.0f, 0.9f, 0.8f)));
	glUniform1f(h_s, 200.0f);
	glUniform1f(h_useTexture, 0.0);

	// Bind textures
	glUniformMatrix3fv(h_T1, 1, GL_TRUE, glm::value_ptr(T1));

	GLSL::enableVertexAttribArray(h_vertTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufID);
	glVertexAttribPointer(h_vertTexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

	MV.translate(glm::vec3(-5.0f, 0.0f, -10.0f));
	MV.rotate(0.4f, glm::vec3(1.0f, 0.0f, 0.0f));

	int numShapes = shapes.size();
	for (int i = 0; i < numShapes; ++i) {
		if (shapes[i]->hasTexture) {
			glUniform1i(h_texture, shapes[i]->id);
			glActiveTexture(GL_TEXTURE0 + shapes[i]->id);
			glBindTexture(GL_TEXTURE_2D, *(textures[shapes[i]->id]));

			glUniform1f(h_useTexture, 1.0);
			shapes[i]->draw(h_vertPos, h_vertNor, h_MV, h_kd, &MV);
			glUniform1f(h_useTexture, 0.0);
			glBindTexture(GL_TEXTURE_2D, 0);
		} else {
			shapes[i]->draw(h_vertPos, h_vertNor, h_MV, h_kd, &MV);
		}
	}

	int numparticles = particles.size();
	for (int i = 0; i < numparticles; ++i) {
		if (particles[i]->show) {
			particles[i]->draw(h_vertPos, h_vertNor, h_MV, h_kd, &MV);
		}
	}

	GLSL::disableVertexAttribArray(h_vertTexCoords);

	// Unbind the program
	glUseProgram(0);

	// Pop stacks
	MV.popMatrix();
	P.popMatrix();

	// Double buffer
	glutSwapBuffers();
}

int getX(float x) {
	float characterOffset = -0.40; // magic number specific to "duck.obj"
	float scale = 1;
	for (int i = 10; i >= 0; --i) {
		if (x >= i + characterOffset) {
			return i;
		}
	}
	return 0;
}

int getY(float y) {
	float scale = 1;
	for (int i = 9; i >= 0; --i) {
		if (y >= (i * 3) + 1.0f) {
			return i;
		}
	}
	return 0;
}

float getFloor(float x, float y) {
  int X = getX(x);
	int Y = getY(y);

	if (floorHasTile[X][Y]) {
		return floorThresholds[Y];
	} else {
		return floorThresholds[max(0, Y - 1)];
	}
}

float getCeil(float x, float y) {
  int X = getX(x);
	int Y = getY(y);

	if (floorHasTile[X][min(10, Y + 1)]) {
		return ceilThresholds[Y];
	} else {
		return ceilThresholds[min(10, Y + 1)];
	}
}

void keyboardGL(unsigned char key, int x, int y) {
	keyDown[key] = true;
	// cout << key << endl;
	switch(key) {
		case 27:
			// ESCAPE
			exit(0);
			break;
		case 'a':
			character->faceForward(false);
		 	break;
		case 'd':
			character->faceForward(true);
			break;
		case 'j':
			character2->faceForward(false);
		 	break;
		case 'l':
			character2->faceForward(true);
			break;
	}

	// Refresh screen
	glutPostRedisplay();
}


void setFlag(int i, int j) {
	flag->setColor(getRandomColor());
	flag->setTranslationVec(glm::vec3((float) (i), (j * 3.0f) + 1.0f, 0.0f));
}

void onFlagCaptured() {
	setFlag(getRandomInt(numFloorTiles), getRandomInt(numFloorTiles));
	int numTilesPlaced = 0;
	int numTilesPlacedInRow = 0;
	for (int j = 1; j < numFloors; ++j) {
		for (int i = 0; i < numFloorTiles; ++i) {
			tiles[numTilesPlaced]->setColor(getRandomColor());

			// lazy "smart" logic to make sure an entire row isn't filled
			if ((getRandomInt(50) % 4) && (numTilesPlacedInRow + 1 < numFloorTiles)) {
				numTilesPlacedInRow++;
				floorHasTile[i][j] = 1;
				tiles[numTilesPlaced++]->setTranslationVec(glm::vec3((float) (i), (j * 3.0f) , 0.0f));
			} else {
				floorHasTile[i][j] = 0;
				tiles[numTilesPlaced++]->setTranslationVec(glm::vec3(0.0f, 0.0f, 0.0f));
			}
		}
		numTilesPlacedInRow = 0;
	}
}


void handleCharacterMovement() {
	if (keyDown['a']) {
		character->moveLeft(0.05f, 0.0f);
	}
	if (keyDown['d']) {
		character->moveRight(0.05f, 10.0f);
	}

	if (keyDown['j']) {
		character2->moveLeft(0.05f, 0.0f);
	}
	if (keyDown['l']) {
		character2->moveRight(0.05f, 10.0f);
	}
}

void handleCameraMovement() {
	float py = character->getTranslationVec().y;
	float p2y = character2->getTranslationVec().y;
	camera.setPosition(character->getTranslationVec(), character2->getTranslationVec());
}

void idleGL()
{
	int numparticles = particles.size();
	for (int i = 0; i < numparticles; ++i) {
		particles[i]->update(t, h);
	}
	t += h;
	int time1 = glutGet(GLUT_ELAPSED_TIME);
	int dt = time1 - time0;

	// Update every 60Hz
	if (dt > 1000.0 / 60.0) {
		flag->rotationAngle = flag->rotationAngle + 0.05;
		glm::vec3 translationVec = character->getTranslationVec();
		float x = translationVec.x;
		float y = translationVec.y;
		float ceiling = getCeil(x, y);
		float flooring = getFloor(x, y);
		character->fly(keyDown['s'], ceiling, flooring);
		character->handleFloating();

		translationVec = character2->getTranslationVec();
		float x2 = translationVec.x;
		float y2 = translationVec.y;
		ceiling = getCeil(x2, y2);
		flooring = getFloor(x2, y2);
		character2->fly(keyDown['k'], ceiling, flooring);
		character2->handleFloating();

		// Character collision
		if (abs(x - x2) < 0.05 && abs(y - y2) < 0.05) {
			for (int i = 0; i < numparticles; ++i) {
				particles[i]->rebirth(translationVec, t);
			}
		}

		translationVec = flag->getTranslationVec();
		float flagX = translationVec.x;
		float flagY = translationVec.y;

		// Flag collision detection
		if ((abs(flagX - x2) < 0.5 && abs(flagY - y2) < 0.5)
			|| (abs(flagX - x) < 0.5 && abs(flagY - y) < 0.5)) {
			onFlagCaptured();
			for (int i = 0; i < numparticles; ++i) {
				particles[i]->rebirth(translationVec, t);
			}
		}

		handleCharacterMovement();
		handleCameraMovement();

		time0 = time1;
		glutPostRedisplay();
	}
}

void keyboardUpGL(unsigned char key, int x, int y)
{
	keyDown[key] = false;
}

void initGame() 
{
	srand(time(NULL));
	
	for (int i = 0; i < numFloors; ++i) {
		floorThresholds.push_back((i * 3) + 1.0f);
		ceilThresholds.push_back(((i + 1) * 3) - 0.5f);
	}

	Character* duck = new Character();
	duck->setMeshName("duck.obj");
	duck->setTranslationVec(glm::vec3(0.0f, 1.0f, 0.0f));
	duck->setRotationAxis(Y_AXIS);
	duck->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	shapes.push_back(duck);
	character = duck;

	duck = new Character();
	duck->setMeshName("duck.obj");
	duck->setTranslationVec(glm::vec3(10.0f, 1.0f, 0.0f));
	duck->setRotationAxis(Y_AXIS);
	duck->setColor(getRandomColor());
	shapes.push_back(duck);
	character2 = duck;

	for (int i = 0; i < numFloors; ++i) {
		floorHasTile[i][getRandomInt(10) + 1] = 0;
	}

	bool flagSet = false;
	flag = new Shape();
	flag->setMeshName("flag.obj");
	flag->setScaleVec(glm::vec3(2.0f, 1.5f, 2.0f));
	flag->setRotationAxis(Y_AXIS);
	flag->setRotationAngle(45);
	shapes.push_back(flag);

	for (int i = 0; i < numFloorTiles; ++i) {
		for (int j = 0; j < numFloors; ++j) {
			Shape* shape = new Shape();
			shape->setMeshName("cube.obj");
			shape->setTranslationVec(glm::vec3((float) (i), (j * 3.0f) , 0.0f));
			shape->setColor(getRandomColor());(getRandomColor());
			shape->setScaleVec(glm::vec3(1.0f, 0.5f, 1.0f));
			if (j != 0) tiles.push_back(shape); // we never move Level 0's tiles

			if (floorHasTile[i][j]) {
				shape->setTranslationVec(glm::vec3((float) (i), (j * 3.0f) , 0.0f));
			} else {
				shape->setTranslationVec(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			shapes.push_back(shape);
		}
	}

	setFlag((int) abs(getRandom(11)), (int) abs(getRandom(7)));

	int treeFloor = 5;
	for (int i = 0; i < numFloorTiles; ++i) {
		if (((int) getRandom(25)) % 5) {
			Shape* tree = new Shape();
			tree->setMeshName("tree.obj");
			tree->setRotationAngle(i);
			tree->setRotationAxis(Y_AXIS);
			tree->setTranslationVec(glm::vec3((float) i, (treeFloor * 3.0f) + 1.2f, -1.0f));
			tree->setScaleVec(glm::vec3(2.0f, 2.0f, 2.0f));
			tree->setColor(getRandomColor());
			shapes.push_back(tree);
		}
	}

	for (int i = 0; i < numFloors - 1; ++i) { // no bg for last floor
		Shape* floorBackground = new Shape();
		floorBackground->setMeshName("square.obj");
		floorBackground->setTranslationVec(glm::vec3(5.0f, 1.0f + (i * 3.0f) , -1.0f));
		floorBackground->setRotationAngle(0.0f * (90.0f / 180.0f * M_PI));
		floorBackground->setColor(getRandomColor());
		floorBackground->setRotationAxis(X_AXIS);
		floorBackground->setScaleVec(glm::vec3(12.0f, 3.0f, 1.0f));
		floorBackground->hasTexture = true;
		floorBackground->id = i;
		shapes.push_back(floorBackground);
	}

	Shape* background = new Shape();
	background->setMeshName("square.obj");
	background->setTranslationVec(glm::vec3(0.0f, 0.0f, -2.0f));
	background->setRotationAngle(0);
	background->setColor(glm::vec3(0.05f, 0.05f, 0.05f));
	background->setRotationAxis(X_AXIS);
	background->setScaleVec(glm::vec3(1000.0f, 1000.0f, 1.0f));
	shapes.push_back(background);

	for (int i = 0; i < 15; ++i) {
		Particle *particle = new Particle();
		particle->setScaleVec(glm::vec3(0.2f, 0.2f, 0.2f));
		particle->setMeshName("cube.obj");
		particle->setColor(getRandomColor());
		particles.push_back(particle);
	}
}

int main(int argc, char **argv)
{
	initGame();
	glutInit(&argc, argv);
	glutInitWindowSize(600, 700);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Vertical World");
	glutKeyboardFunc(keyboardGL);	
	glutKeyboardUpFunc(keyboardUpGL);
	glutReshapeFunc(reshapeGL);
	glutDisplayFunc(drawGL);
	glutIdleFunc(idleGL);
	loadScene();
	initGL();
	glutMainLoop();
	return 0;
}
