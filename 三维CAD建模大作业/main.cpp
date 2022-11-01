#include "solid.h"
#include "face.h"
#include "loop.h"
#include "half_edge.h"
#include "vertex.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <GL/glu.h>
#include "shader.h"
#include "camera.h"
#include <vector>
#define WIDTH 1280
#define HEIGHT 720

using namespace glm;

float verticesA[] = {

	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f

};

float verticesB[] = {

	-0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	 0.0f,  -0.5f, 0.0f

};

float verticesC[] = {

	-0.9f, -1.1f, -0.3f,
	 1.1f, -1.1f, -0.3f,
	 0.1f,  0.9f, -0.3f

};

float verticesD[] = {

	-0.4f, 0.4f, -0.3f,
	 0.6f, 0.4f, -0.3f,
	 0.1f,  -0.6f, -0.3f

};

//frame time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//camera
Camera* cam;
float camSpeed = 15.0f;

//cursor
vec2 lastCursor;
float cursorSensitivity = 0.3f;
bool firstMouse = true;
float blend = 0.2f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double posX, double posY) {

	vec2 cursorPos = vec2(posX, posY);
	if (firstMouse) lastCursor = cursorPos, firstMouse = false;
	float offsetX = cursorPos.x - lastCursor.x;
	float offsetY = lastCursor.y - cursorPos.y;
	lastCursor = cursorPos;

	deltaTime = glfwGetTime() - lastFrame;
	lastFrame += deltaTime;
	offsetX *= cursorSensitivity;
	offsetY *= cursorSensitivity;

	cam->ProcessMouseMovement(offsetX, offsetY, (GLboolean)true);

}

void scroll_callback(GLFWwindow* window, double offsetX, double offsetY) {

	cam->ProcessMouseScroll(offsetY);

}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		if (blend < 0.999f) blend += 0.001f;

	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

		if (blend > 0.001f) blend -= 0.001f;

	}

	deltaTime = glfwGetTime() - lastFrame;
	lastFrame += deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam->ProcessKeyboard(RIGHT, deltaTime);


}

extern Solid* mvsf(point p, Vertex*& v);
extern HalfEdge* mev(Vertex* v1, Loop* lp, point p);
extern Face* mef(Vertex* v1, Vertex* v2, Loop* lp);
extern Loop* kemr(Vertex* v1, Vertex* v2, Loop* innerLp);
extern void kfmrh(Loop* l1, Loop* l2);
extern void Sweep(Loop* lp, Vec3 d);

void printLoop(Loop* lp) {

	HalfEdge* he = lp->Ledge;
	he = he->Enext;
	while (he != lp->Ledge) {

		printf("%.2lf %.2lf %.2lf\n", he->startV->v.x, he->startV->v.y, he->startV->v.z);
		he = he->Enext;

	}
	printf("%.2lf %.2lf %.2lf\n\n", he->startV->v.x, he->startV->v.y, he->startV->v.z);


}

Solid* createA(){

	Vertex* v1;
	Solid* s = mvsf(point(0.0, 4.0, 0.0), v1);
	Face* f = s->Sfaces;
	Loop* lp = f->Floop;
	HalfEdge* he = mev(v1, lp, point(3.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(2.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(1.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(-1.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(-2.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(-3.0, -2.0, 0.0));
	f = mef(he->startV, v1, lp);
	Loop* outLp = lp;
	lp = f->Floop;

	he = mev(v1, lp, point(0.0, 2.0, 0.0));
	Vertex* v2 = he->startV;
	he = mev(he->startV, lp, point(1.0, 0.0, 0.0));
	he = mev(he->startV, lp, point(-1.0, 0.0, 0.0));
	Face* innerF = mef(he->startV, v2, lp);

	lp = kemr(v1, v2, lp);
	kfmrh(outLp, innerF->Floop);
	
	Loop* tmpL = lp->Lface->Floop;
	while (tmpL != NULL) {

		Sweep(tmpL, Vec3(0.0, 0.0, 1.0f));
		tmpL = tmpL->nextL;

	}

	return s;

}

Solid* createB() {

	Vertex* v1;
	Solid* s = mvsf(point(-2.0f, -4.0f, 0.0f), v1);
	Face* f = s->Sfaces;
	Loop* lp = f->Floop;
	HalfEdge* he = mev(v1, lp, point(1.0, -4.0, 0.0));
	he = mev(he->startV, lp, point(2.0, -5.0, 0.0));
	he = mev(he->startV, lp, point(2.0, -8.0, 0.0));
	he = mev(he->startV, lp, point(1.0, -9.0, 0.0));
	he = mev(he->startV, lp, point(-2.0, -9.0, 0.0));
	f = mef(he->startV, v1, lp);
	Loop* outLp = lp;
	lp = f->Floop;

	he = mev(v1, lp, point(-1.0, -5.0, 0.0));
	Vertex* v2 = he->startV;
	he = mev(he->startV, lp, point(1.0, -5.0, 0.0));
	he = mev(he->startV, lp, point(1.0, -6.0, 0.0));
	he = mev(he->startV, lp, point(-1.0, -6.0, 0.0));
	Face* innerF = mef(he->startV, v2, lp);
	lp = kemr(v1, v2, lp);
	kfmrh(outLp, innerF->Floop);

	he = mev(v1, lp, point(-1.0, -7.0, 0.0));
	Vertex* v3 = he->startV;
	he = mev(he->startV, lp, point(1.0, -7.0, 0.0));
	he = mev(he->startV, lp, point(1.0, -8.0, 0.0));
	he = mev(he->startV, lp, point(-1.0, -8.0, 0.0));
	innerF = mef(he->startV, v3, lp);
	lp = kemr(v1, v3, lp);
	kfmrh(outLp, innerF->Floop);

	Loop* tmpL = lp->Lface->Floop;
	while (tmpL != NULL) {

		Sweep(tmpL, Vec3(0.0, 0.0, 1.0f));
		tmpL = tmpL->nextL;

	}

	return s;

}

Solid* createC() {

	Vertex* v1;
	Solid* s = mvsf(point(-6.0f, 4.0f, 0.0f), v1);
	Face* f = s->Sfaces;
	Loop* lp = f->Floop;
	HalfEdge* he = mev(v1, lp, point(-6.0, 3.0, 0.0));
	he = mev(he->startV, lp, point(-8.0, 3.0, 0.0));
	he = mev(he->startV, lp, point(-9.0, 2.0, 0.0));
	he = mev(he->startV, lp, point(-9.0, 0.0, 0.0));
	he = mev(he->startV, lp, point(-8.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(-6.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(-6.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(-9.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(-10.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(-10.0, 3.0, 0.0));
	he = mev(he->startV, lp, point(-9.0, 4.0, 0.0));
	f = mef(he->startV, v1, lp);
	lp = f->Floop;
	Loop* tmpL = lp->Lface->Floop;
	while (tmpL != NULL) {

		Sweep(tmpL, Vec3(0.0, 0.0, 1.0f));
		tmpL = tmpL->nextL;

	}

	return s;

}

Solid* createD() {

	Vertex* v1;
	Solid* s = mvsf(point(6.0f, 4.0f, 0.0f), v1);
	Face* f = s->Sfaces;
	Loop* lp = f->Floop;
	HalfEdge* he = mev(v1, lp, point(8.0, 4.0, 0.0));
	he = mev(he->startV, lp, point(10.0, 2.0, 0.0));
	he = mev(he->startV, lp, point(10.0, 0.0, 0.0));
	he = mev(he->startV, lp, point(8.0, -2.0, 0.0));
	he = mev(he->startV, lp, point(6.0, -2.0, 0.0));
	f = mef(he->startV, v1, lp);
	Loop* outLp = lp;
	lp = f->Floop;

	he = mev(v1, lp, point(7.0, 3.0, 0.0));
	Vertex* v2 = he->startV;
	he = mev(he->startV, lp, point(8.0, 3.0, 0.0));
	he = mev(he->startV, lp, point(9.0, 2.0, 0.0));
	he = mev(he->startV, lp, point(9.0, 0.0, 0.0));
	he = mev(he->startV, lp, point(8.0, -1.0, 0.0));
	he = mev(he->startV, lp, point(7.0, -1.0, 0.0));
	Face* innerF = mef(he->startV, v2, lp);

	lp = kemr(v1, v2, lp);
	kfmrh(outLp, innerF->Floop);

	Loop* tmpL = lp->Lface->Floop;
	while (tmpL != NULL) {

		Sweep(tmpL, Vec3(0.0, 0.0, 1.0f));
		tmpL = tmpL->nextL;

	}

	return s;

}

float** vertices;

void positionAndNormal(int i, int x, point a, Vec3 b) {

	vertices[i][x * 6] = a.x;
	vertices[i][x * 6 + 1] = a.y;
	vertices[i][x * 6 + 2] = a.z;
	vertices[i][x * 6 + 3] = b.x;
	vertices[i][x * 6 + 4] = b.y;
	vertices[i][x * 6 + 5] = b.z;

}

int main() {


	////Euler
	//Vertex* v1;
	//Solid* S = mvsf(point(-1.0, 1.0, 0.0), v1);
	//Face* f = S->Sfaces;
	//Loop* lp = f->Floop;
	//HalfEdge* he = mev(v1, lp, point(1.0, 1.0, 0.0));
	//he = mev(he->startV, lp, point(1.0, -1.0, 0.0));
	//he = mev(he->startV, lp, point(-1.0, -1.0, 0.0));
	//f = mef(he->startV, v1, lp);
	//Loop* outLp = lp;
	//lp = f->Floop;
	//
	//he = mev(v1, lp, point(-0.5, 0.5, 0.0));
	//Vertex* v2 = he->startV;
	//he = mev(he->startV, lp, point(0.5, 0.5, 0.0));
	//he = mev(he->startV, lp, point(0.5, -0.5, 0.0));
	//he = mev(he->startV, lp, point(-0.5, -0.5, 0.0));
	//Face* innerF = mef(he->startV, v2, lp);

	//lp = kemr(v1, v2, lp);
	//printLoop(outLp);
	//printLoop(innerF->Floop);
	//kfmrh(outLp, innerF->Floop);
	//
	//Loop* tmpL = lp->Lface->Floop;
	//while (tmpL != NULL) {

	//	Sweep(tmpL, Vec3(0.0, 0.0, 1.0f));
	//	tmpL = tmpL->nextL;

	//}

	Solid* A = createA();
	Solid* B = createB();
	A->nextS = B;
	B->preS = A;
	Solid* C = createC();
	B->nextS = C;
	C->preS = B;
	Solid* D = createD();
	C->nextS = D;
	D->preS;

	//process vertices data
	Solid* S = A;
	int loopNum = 0, faceNum = 0;;
	while (S != NULL) {

		Face* tmpf = S->Sfaces;
		while (tmpf != NULL) {

			Loop* lp = tmpf->Floop;
			//printf("NO.%d\n", faceNum + 1);
			while (lp != NULL) {

				//printLoop(lp);
				loopNum++;
				lp = lp->nextL;

			}
			faceNum++;
			tmpf = tmpf->nextF;

		}

		S = S->nextS;

	}

	int* verticesPerFace = new int[faceNum];
	memset(verticesPerFace, 0, sizeof(int) * faceNum);
	S = A;
	int i = 0;
	while (S != NULL) {

		Face* tmpf = S->Sfaces;
		while (tmpf != NULL) {

			Loop* lp = tmpf->Floop;
			while (lp != NULL) {

				HalfEdge* he = lp->Ledge;
				he = he->Enext;
				while (he != lp->Ledge) {

					verticesPerFace[i]++;
					he = he->Enext;

				}

				verticesPerFace[i]++;
				lp = lp->nextL;

			}
			i++;
			tmpf = tmpf->nextF;

		}
		S = S->nextS;

	}

	vertices = new float*[faceNum];
	S = A;
	i = 0;
	while (S != NULL) {

		Face* tmpf = S->Sfaces;
		while (tmpf != NULL) {

			vertices[i] = new float[18 * (verticesPerFace[i] - 2)];
			memset(vertices[i], 0, sizeof(float) * 18 * (verticesPerFace[i] - 2));
			Loop* lp = tmpf->Floop;
			int j = 0;
			while (lp != NULL) {

				HalfEdge* he = lp->Ledge;
				point v1 = he->startV->v;
				he = he->Enext;
				while (he->Enext != lp->Ledge) {

					point a = v1, b = he->startV->v;
					Vec3 A = Vec3(b.x - a.x, b.y - a.y, b.z - a.z);
					he = he->Enext;
					point c = he->startV->v;
					Vec3 B = Vec3(c.x - b.x, c.y - b.y, c.z - b.z);
					A = A.Cross(B);
					float len = A.length();
					A.x /= len;
					A.y /= len;
					A.z /= len;
					positionAndNormal(i, j, a, A);
					j++;
					positionAndNormal(i, j, b, A);
					j++;
					positionAndNormal(i, j, c, A);
					j++;

				}

				lp = lp->nextL;

			}
			i++;
			tmpf = tmpf->nextF;

		}

		S = S->nextS;

	}

	//camera set up
	cam = new Camera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	cam->MouseSensitivity = cursorSensitivity;
	cam->MovementSpeed = camSpeed;
	lastCursor = vec2(WIDTH * 0.5f, HEIGHT * 0.5f);

	//get view transform matrix
	mat4 view = cam->GetViewMatrix();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CAD Project", NULL, NULL);

	if (window == NULL) {

		printf("Failed to create a window\n");
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		printf("Failed to initialize GLAD\n");
		return -1;

	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader shader("basicVS.vs", "Phong.fs");

	uint* VAO, * VBO;
	VAO = new uint[faceNum];
	VBO = new uint[faceNum];

	glGenVertexArrays(faceNum, &VAO[0]);
	glGenBuffers(faceNum, &VBO[0]);
	for (int i = 0; i < faceNum; i++) {

		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * (verticesPerFace[i] - 2), vertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}

	vec3 lightPos = vec3(2.0f, 1.0f, 4.0f);
	shader.use();
	shader.setVec3("objectColor", 1.0f, 0.3f, 0.31f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		shader.use();
		shader.setMat4("view", cam->GetViewMatrix());
		shader.setMat4("projection", perspective(radians(cam->Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 90.0f));
		mat4 model = mat4(1.0f);
		shader.setMat4("model", model);
		shader.setVec3("viewPos", cam->Position.x, cam->Position.y, cam->Position.z);

		for (int i = 0; i < faceNum; i++) {

			glBindVertexArray(VAO[i]);
			glClear(GL_STENCIL_BUFFER_BIT);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDisable(GL_DEPTH_TEST);
			glStencilFunc(GL_ALWAYS, 0x1, 0x1);
			glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
			shader.use();
			glDrawArrays(GL_TRIANGLES, 0, verticesPerFace[i] * 6);

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			glStencilFunc(GL_EQUAL, 0x1, 0x1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			shader.use();
			glDrawArrays(GL_TRIANGLES, 0, verticesPerFace[i] * 6);

		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	for (int i = 0; i < faceNum; i++){

		glDeleteVertexArrays(1, &VAO[i]);
		glDeleteBuffers(1, &VBO[i]);

	}

	glfwTerminate();
	return 0;

}