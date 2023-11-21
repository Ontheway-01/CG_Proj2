//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "texture.hpp"

//shade mode
enum eShadeMode { NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE };

//global variable
glm::mat4 projectMat; //4*4 matrix
glm::mat4 viewMat;
glm::mat4 modelMat;
//glm::vec4 -> vector 저장

int shadeMode = NO_LIGHT;

//texture
enum animal { HEAD, BODY, UPPER_LEG, LOWER_LEG, TAIL };
int animalPart = HEAD;
int isTexture = true;

//vertex shader에서 pvm matrix로 넘겨줄 uniform variable의 id
//GLuint pvmMatrixID;

GLuint projectMatrixID;
GLuint viewMatrixID;
GLuint modelMatrixID;

GLuint shadeModeID;
GLuint textureModeID;
GLuint animalPartID;


//-------------------------------anmial walk-------------------------------------------------------------------------------------------------------
//cube를 돌리기 위한 rotation angle
float rotAngle = 0.0f;

//왼쪽 앞다리
float leftFrontLegAngle = 0.0f;
float leftFrontLowerLegAngle = 0.0f;
bool leftFrontLegForward = true;
bool leftFrontBend = false;
bool leftFrontExtend = false;

//오른쪽 앞다리
float rightFrontLegAngle = 0.0f;
float rightFrontLowerLegAngle = 0.0f;
bool rightFrontLegForward = false;
bool rightFrontBend = false;
bool rightFrontExtend = false;

//왼쪽 뒷다리
float leftBackLegAngle = 0.0f;
float leftBackLowerLegAngle = 0.0f;
bool leftBackLegForward = false;
bool leftBackBend = false;
bool leftBackExtend = false;

//오른쪽 뒷다리
float rightBackLegAngle = 0.0f;
float rightBackLowerLegAngle = 0.0f;
bool rightBackLegForward = true;
bool rightBackBend = false;
bool rightBackExtend = false;

float add = 0.0f;
//---------------------------------------------------------------------------------------------------------------------------------------------


//glm의 vector와 matrix로 교체
typedef glm::vec4  color4;
typedef glm::vec4  point4;
typedef glm::vec4 normal4;
typedef glm::vec2 texture2;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];
normal4 normals[NumVertices];
texture2 headTextures[NumVertices];
texture2 bodyTextures[NumVertices];
texture2 upperLegTextures[NumVertices];
texture2 lowerLegTextures[NumVertices];
texture2 tailTextures[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

//----------------------------------------------------------------------------
  
// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	glm::vec4 u = vertices[b] - vertices[a];
	glm::vec4 v = vertices[d] - vertices[a];

	//normal vector
	float n1 = u[1] * v[2] - u[2] * v[1];
	float n2 = u[2] * v[0] - u[0] * v[2];
	float n3 = u[0] * v[1] - u[1] * v[0];

	//동일한 사각형내에 존재하는 2개의 삼각형이므로 모든 vertex의 normal vector 동일

	//colors[Index] = vertex_colors[a]; 
	points[Index] = vertices[a];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
	//colors[Index] = vertex_colors[b]; 
	points[Index] = vertices[b];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
	//colors[Index] = vertex_colors[c]; 
	points[Index] = vertices[c];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
	//colors[Index] = vertex_colors[a]; 
	points[Index] = vertices[a];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
	//colors[Index] = vertex_colors[c]; 
	points[Index] = vertices[c];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
	//colors[Index] = vertex_colors[d]; 
	points[Index] = vertices[d];  normals[Index] = glm::vec4(n1, n2, n3, 0.0); Index++;
}

void textureCoord() {
	//headTextures

	//bodyTextures

	//upperLegTextures

	//lowerLegTextures

	//tailTextures
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
	textureCoord();
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	int vertSize = sizeof(points);
	int normalSize = sizeof(normals);
	int headTexSize = sizeof(headTextures);
	int bodyTexSize = sizeof(bodyTextures);
	int upperLegTexSize = sizeof(upperLegTextures);
	int lowerLegTexSize = sizeof(lowerLegTextures);
	int tailTexSize = sizeof(tailTextures);

	glBufferData(GL_ARRAY_BUFFER, vertSize + normalSize + headTexSize + bodyTexSize + upperLegTexSize + lowerLegTexSize + tailTexSize,
		NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + normalSize, headTexSize, headTextures);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + normalSize + headTexSize, bodyTexSize, bodyTextures);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + normalSize + headTexSize + bodyTexSize, upperLegTexSize, upperLegTextures);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + normalSize + headTexSize + bodyTexSize + upperLegTexSize, lowerLegTexSize, lowerLegTextures);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + normalSize + headTexSize + bodyTexSize + upperLegTexSize + lowerLegTexSize, tailTexSize, tailTextures);

	//여기까지 vertex buffer object -> cube의 vertex들과 color정보를 vertext buffer array로 만들어서 한번에 GPU로 넘김


	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize));

	//각각의 부위 별로 mapping하는 texture coordinate이 다르므로 각각 넘겨줌
	GLuint vHeadTexCoord = glGetAttribLocation(program, "vHeadTexCoord");
	glEnableVertexAttribArray(vHeadTexCoord);
	glVertexAttribPointer(vHeadTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize + normalSize));

	GLuint vBodyTexCoord = glGetAttribLocation(program, "vBodyTexCoord");
	glEnableVertexAttribArray(vBodyTexCoord);
	glVertexAttribPointer(vBodyTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize + normalSize + headTexSize));

	GLuint vUpperLegTexCoord = glGetAttribLocation(program, "vUpperLegTexCoord");
	glEnableVertexAttribArray(vUpperLegTexCoord);
	glVertexAttribPointer(vUpperLegTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize + normalSize + headTexSize + bodyTexSize));

	GLuint vLowerLegTexCoord = glGetAttribLocation(program, "vLowerLegTexCoord");
	glEnableVertexAttribArray(vLowerLegTexCoord);
	glVertexAttribPointer(vLowerLegTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize + normalSize + headTexSize + bodyTexSize + upperLegTexSize));


	GLuint vTailTexCoord = glGetAttribLocation(program, "vTailTexCoord");
	glEnableVertexAttribArray(vTailTexCoord);
	glVertexAttribPointer(vTailTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize + normalSize + headTexSize + bodyTexSize + upperLegTexSize + lowerLegTexSize));


	//
	projectMatrixID = glGetUniformLocation(program, "mProject");
	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);

	viewMatrixID = glGetUniformLocation(program, "mView");
	viewMat = glm::lookAt(glm::vec3(2, 1, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);

	modelMatrixID = glGetUniformLocation(program, "mModel");
	modelMat = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);

	
	shadeModeID = glGetUniformLocation(program, "shadeMode");
	glUniform1i(shadeModeID, shadeMode);

	textureModeID = glGetUniformLocation(program, "isTexture");
	glUniform1i(textureModeID, isTexture);

	//animal 몸 부위 중 어디인지
	animalPartID = glGetUniformLocation(program, "animalPart");
	glUniform1i(animalPartID, animalPart);


	//head texture
	GLuint headTexture = loadBMP_custom("head.bmp");
	GLuint headTextureID = glGetUniformLocation(program, "headTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, headTexture);
	glUniform1i(headTextureID, 0);

	//body texture
	GLuint bodyTexture = loadBMP_custom("body.bmp");
	GLuint bodyTextureID = glGetUniformLocation(program, "bodyTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bodyTexture);
	glUniform1i(bodyTextureID, 0);

	//upper leg texture
	GLuint upperLegTexture = loadBMP_custom("upper_leg.bmp");
	GLuint upperLegTextureID = glGetUniformLocation(program, "upperLegTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, upperLegTexture);
	glUniform1i(upperLegTextureID, 0);

	//lower leg texture
	GLuint lowerLegTexture = loadBMP_custom("lower_leg.bmp");
	GLuint lowerLegTextureID = glGetUniformLocation(program, "lowerLegTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lowerLegTexture);
	glUniform1i(lowerLegTextureID, 0);

	//tail texture
	GLuint tailTexture = loadBMP_custom("tail.bmp");
	GLuint tailTextureID = glGetUniformLocation(program, "tailTexture");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tailTexture);
	glUniform1i(tailTextureID, 0);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void drawCat()
{
	glm::mat4 standard_cube = glm::mat4(1.0); // x, y, z 길이 1인 기준 cube
	glm::mat4 modelMat, pvmMat;


	// body
	glUniform1i(animalPartID, BODY);
	modelMat = glm::scale(standard_cube, glm::vec3(2, 0.6, 1));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// head
	glUniform1i(animalPartID, HEAD);
	modelMat = glm::translate(standard_cube, glm::vec3(-1, 0.65, 0));  
	modelMat = glm::scale(modelMat, glm::vec3(0.8, 0.5, 0.6));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// tail
	glUniform1i(animalPartID, TAIL);
	modelMat = glm::translate(standard_cube, glm::vec3(0.9, 1.15, 0));  
	modelMat = glm::scale(modelMat, glm::vec3(0.2, 1.5, 0.2));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);



	//오른쪽 앞다리
	//upper
	glUniform1i(animalPartID, UPPER_LEG);
	//회전 기준점으로 이동
	modelMat = glm::translate(standard_cube, glm::vec3(-0.7, -0.4, 0.6));  
	//rotate
	if (rightFrontLegForward) {
		rightFrontLegAngle += add;
	}
	else {
		rightFrontLegAngle -= add;
	}
	modelMat = glm::rotate(modelMat, rightFrontLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0,-0.2,0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.6, 0.2));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	
	//lower
	glUniform1i(animalPartID, LOWER_LEG);
	//오른쪽 앞다리 upper을 기준으로 계산
	//회전 기준점으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.7, 0));
	//rotate
	if (rightFrontExtend) {
		rightFrontLowerLegAngle += add;
	}
	else if (rightFrontBend) {
		rightFrontLowerLegAngle -= 2 * add;
	}
	else {
		rightFrontLowerLegAngle = 0.0f;
	}
	modelMat = glm::rotate(modelMat, rightFrontLowerLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.5, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4f / 0.4f, 0.8f / 0.6f, 0.2f / 0.2f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//오른쪽 뒷다리
	//upper
	glUniform1i(animalPartID, UPPER_LEG);
	//회전 기준점으로 이동
	modelMat = glm::translate(standard_cube, glm::vec3(0.7, -0.4, 0.6));
	//rotate
	if (rightBackLegForward) {
		rightBackLegAngle += add;
	}
	else {
		rightBackLegAngle -= add;
	}
	modelMat = glm::rotate(modelMat, rightBackLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.2, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.6, 0.2));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//lower
	glUniform1i(animalPartID, LOWER_LEG);
	//오른쪽 뒷다리 upper을 기준으로 계산
	//회전 기준점으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.7, 0));
	//rotate
	if (rightBackExtend) {
		rightBackLowerLegAngle -= add;
	}
	else if (rightBackBend) {
		rightBackLowerLegAngle += 2 * add;
	}
	else {
		rightBackLowerLegAngle = 0.0f;
	}
	modelMat = glm::rotate(modelMat, rightBackLowerLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.5, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4f / 0.4f, 0.8f / 0.6f, 0.2f / 0.2f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);


	//왼쪽 앞다리
	//upper
	glUniform1i(animalPartID, UPPER_LEG);
	//회전 기준점으로 이동
	modelMat = glm::translate(standard_cube, glm::vec3(-0.7, -0.4, -0.6));
	//rotate
	if (leftFrontLegForward) {
		leftFrontLegAngle += add;
	}
	else {
		leftFrontLegAngle -= add;
	}
	modelMat = glm::rotate(modelMat, leftFrontLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.2, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.6, 0.2));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//lower
	glUniform1i(animalPartID, LOWER_LEG);
	//왼쪽 앞다리 upper을 기준으로 계산
	//회전 기준점으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.7, 0));
	//rotate
	if (leftFrontExtend) {
		leftFrontLowerLegAngle += add;
	}
	else if (leftFrontBend) {
		leftFrontLowerLegAngle -= 2 * add;
	}
	else {
		leftFrontLowerLegAngle = 0.0f;
	}
	modelMat = glm::rotate(modelMat, leftFrontLowerLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.5, 0));
	//크기도 upper 기준으로 계산
	modelMat = glm::scale(modelMat, glm::vec3(0.4f / 0.4f, 0.8f / 0.6f, 0.2f / 0.2f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//왼쪽 뒷다리
	//upper
	glUniform1i(animalPartID, UPPER_LEG);
	//회전 기준점으로 이동
	modelMat = glm::translate(standard_cube, glm::vec3(0.7, -0.4, -0.6));
	//rotate
	// 
	if (leftBackLegForward) {
		leftBackLegAngle += add;
	}
	else {
		leftBackLegAngle -= add;
	}
	modelMat = glm::rotate(modelMat, leftBackLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.2, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.6, 0.2));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//lower
	glUniform1i(animalPartID, LOWER_LEG);
	//오른쪽 뒷다리 upper을 기준으로 계산
	//회전 기준점으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.7, 0));
	//rotate
	if (leftBackExtend) {
		leftBackLowerLegAngle -= add;
	}
	else if (leftBackBend) {
		leftBackLowerLegAngle += 2 * add;
	}
	else {
		leftBackLowerLegAngle = 0.0f;
	}
	modelMat = glm::rotate(modelMat, leftBackLowerLegAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//실제 중심으로 이동
	modelMat = glm::translate(modelMat, glm::vec3(0, -0.5, 0));
	//크기도 upper 기준으로 계산
	modelMat = glm::scale(modelMat, glm::vec3(0.4f / 0.4f, 0.8f / 0.6f, 0.2f / 0.2f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawCat();

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);


	if (abs(currTime - prevTime) >= 20)
	{
		float t = abs(currTime - prevTime);
		add = -glm::radians(t * 360.0f / 10000.0f);

		//오른쪽 앞다리
		if (rightFrontLegAngle >= glm::radians(20.0f)) {
			rightFrontLegForward = true;
			rightFrontBend = false;
			rightFrontExtend = true;
		}
		else if (rightFrontLegAngle <= -(glm::radians(20.0f))) {
			rightFrontLegForward = false;
		}

		if (rightFrontLegAngle >= 0 && rightFrontLegAngle < glm::radians(20.0f)) {
			if (!rightFrontLegForward) {
				rightFrontBend = true;
				rightFrontExtend = false;
			}
		}else {
			if (!rightFrontLegForward) {
				rightFrontBend = false;
				rightFrontExtend = false;
			}
		}


		//왼쪽 앞다리
		if (leftFrontLegAngle >= glm::radians(20.0f)) {
			leftFrontLegForward = true;
			leftFrontBend = false;
			leftFrontExtend = true;
		}
		else if (leftFrontLegAngle <= -(glm::radians(20.0f))) {
			leftFrontLegForward = false;
		}

		if (leftFrontLegAngle >= 0 && leftFrontLegAngle < glm::radians(20.0f)) {
			if (!leftFrontLegForward) {
				leftFrontBend = true;
				leftFrontExtend = false;
			}
		}
		else {
			if (!leftFrontLegForward) {
				leftFrontBend = false;
				leftFrontExtend = false;
			}
		}


		//오른쪽 뒷다리
		if (rightBackLegAngle >= glm::radians(20.0f)) {
			rightBackLegForward = true;
			rightBackBend = false;
			rightBackExtend = true;
		}
		else if (rightBackLegAngle <= -(glm::radians(20.0f))) {
			rightBackLegForward = false;
		}
		if (rightBackLegAngle >= 0 && rightBackLegAngle < glm::radians(20.0f)) {
			if (!rightBackLegForward) {
				rightBackBend = true;
				rightBackExtend = false;
			}
		}
		else {
			if (!rightBackLegForward) {
				rightBackBend = false;
				rightBackExtend = false;
			}
		}


		//왼쪽 뒷다리;
		if (leftBackLegAngle >= glm::radians(20.0f)) {
			leftBackLegForward = true;
			leftBackBend = false;
			leftBackExtend = true;
		}
		else if (leftBackLegAngle <= -(glm::radians(20.0f))) {
			leftBackLegForward = false;
		}
		if (leftBackLegAngle >= 0 && leftBackLegAngle < glm::radians(20.0f)) {
			if (!leftBackLegForward) {
				leftBackBend = true;
				leftBackExtend = false;
			}
		}
		else {
			if (!leftBackLegForward) {
				leftBackBend = false;
				leftBackExtend = false;
			}
		}
		prevTime = currTime;
		glutPostRedisplay();
	}

	
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 't': case 'T':
		isTexture = !isTexture;
		glUniform1i(textureModeID, isTexture);
		glutPostRedisplay();
		break;
	case 'l': case 'L':
		shadeMode = (++shadeMode % NUM_LIGHT_MODE);
		glUniform1i(shadeModeID, shadeMode);
		glutPostRedisplay();
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Car");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
