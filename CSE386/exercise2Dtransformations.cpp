/****************************************************
 * 2016-2022 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <ctime>
#include <iostream>
#include "defs.h"
#include "colorandmaterials.h"
#include "framebuffer.h"
#include "utilities.h"
#include "rasterization.h"

const int WINDOW_SZ = 500;
FrameBuffer colorBuffer(WINDOW_SZ, WINDOW_SZ);
const int N = 50;

vector<dvec3> triangleVertices = { dvec3(-2 * N,2 * N,1), dvec3(-N,2 * N,1), dvec3(-1.5 * N,3 * N,1) };
vector<dvec3> square1Vertices = { dvec3(-N,-N,1), dvec3(N,-N,1),
											dvec3(N,N,1), dvec3(-N,N,1) };
vector<dvec3> square2Vertices = { dvec3(3 * N,-2 * N,1), dvec3(3 * N,-3 * N,1),
											dvec3(2 * N,-3 * N,1), dvec3(2 * N,-2 * N,1) };

int displayedProblem = 0;

vector<dvec3> transformVertices(const dmat3& transMatrix, const vector<dvec3>& vertices) {
	vector<dvec3> transformedVertices;

	for (size_t i = 0; i < vertices.size(); i++) {
		dvec3 vt(transMatrix * vertices[i]);
		transformedVertices.push_back(vt);
	}

	return transformedVertices;
}

void drawWirePolygonWithShift(vector<dvec3> verts, const color& C) {
	int W2 = colorBuffer.getWindowWidth() / 2;
	int H2 = colorBuffer.getWindowHeight() / 2;
	for (unsigned int i = 0; i < verts.size(); i++) {
		verts[i].x += W2;
		verts[i].y += H2;
	}
	drawWirePolygon(colorBuffer, verts, C);
}

void drawObjectOnly(const vector<dvec3>& verts, bool drawAxis = true) {
	if (drawAxis) {
		drawAxisOnWindow(colorBuffer);
	}
	drawWirePolygonWithShift(verts, black);
}

void drawObjAndOneTransformation(const dmat3& TM, const vector<dvec3>& verts, bool drawAxis = true) {
	vector<dvec3> vertsTransformed = transformVertices(TM, verts);
	if (drawAxis) {
		drawAxisOnWindow(colorBuffer);
	}
	drawWirePolygonWithShift(verts, black);
	drawWirePolygonWithShift(vertsTransformed, red);
}

void drawObjectAndAllTransformations(const dmat3& TM) {
	drawObjAndOneTransformation(TM, triangleVertices, false);
	drawObjAndOneTransformation(TM, square1Vertices, false);
	drawObjAndOneTransformation(TM, square2Vertices, false);
	drawAxisOnWindow(colorBuffer);
}

// Draw all the shapes, transformed by S(2, 0.5)
void doScaleBy2xOneHalf() {
	dmat3 TM = S(2.0, 0.5);
	drawObjectAndAllTransformations(TM);
}

// Draw all the shapes, transformed by T(50, 50)
void doTranslate50_50() {
	dmat3 TM = T(50, 50);
	drawObjectAndAllTransformations(TM);
}

// Draw all the shapes, transformed by R(45)
void doRotate45() {
	dmat3 TM = R(45);
	drawObjectAndAllTransformations(TM);
}

// Draw all the shapes, transformed by R(-10)
void doRotateNeg10() {
	dmat3 TM = R(-10);
	drawObjectAndAllTransformations(TM);
}

// Draw all shapes, reflected across the Y axis
void doReflectAcrossYaxis() {
	dmat3 TM = S(-1, 1);
	drawObjectAndAllTransformations(TM);
}

// Draw all shapes, reflected across the origin
void doReflectAcrossOrigin() {
	dmat3 TM = S(-1, -1);
	drawObjectAndAllTransformations(TM);
}

// Draw only triangle, scaled 2X about its center (-1.5N, 2.5N)
void doScale2XAboutCenterOfTriangle() {
	dmat3 TM = T(1.5 * N, -2.5 * N) * S(2, 2);
	drawObjAndOneTransformation(TM, triangleVertices);
}

// Draw all shapes, reflected across y=x+50
void doReflectAcrossLineYeqXplus50() {
	dmat3 TM = T(0, 50) * R(45) * S(1, -1) * R(-45) * T(0, -50);
	drawObjectAndAllTransformations(TM);
}

// Animate the rotation of the square1
void doAnimationOfRotationByAngle() {
	static double angle = 0.0;
	angle += 1.0;
	dmat3 TM = R(angle);
	drawObjAndOneTransformation(TM, square1Vertices);
}

// Render square1 so that it rotates about its own axis, and then orbits the origin.
void doSquareRotatingAroundOwnAxisAndAroundSun() {
	static double D = 0;
	D += 2.5;
	dmat3 TM = R(D) * T(2*N, 2*N) * R(D);
	vector<dvec3> square1VerticesTransformed = transformVertices(TM, square1Vertices);
	drawWirePolygonWithShift(square1VerticesTransformed, red);
	drawAxisOnWindow(colorBuffer);
}

typedef void(*TRANS)();

struct DisplayFunc {
	TRANS func;
	string name;
	DisplayFunc(TRANS t, string n) : func(t), name(n) {}
};
vector<DisplayFunc> funcs = { DisplayFunc(doScaleBy2xOneHalf, "Scale by 2 and 1/2"),
								DisplayFunc(doTranslate50_50, "Trans 50 50"),
								DisplayFunc(doRotate45, "Rotate 45"),
								DisplayFunc(doRotateNeg10, "Rotate -10"),
								DisplayFunc(doReflectAcrossYaxis, "Reflect across Y"),
								DisplayFunc(doReflectAcrossOrigin, "Reflect across origin"),
								DisplayFunc(doScale2XAboutCenterOfTriangle, "Scale 2X about tri center point"),
								DisplayFunc(doReflectAcrossLineYeqXplus50, "Reflect across y=x+50"),
								DisplayFunc(doAnimationOfRotationByAngle, "Rotate by angle"),
								DisplayFunc(doSquareRotatingAroundOwnAxisAndAroundSun, "Orbiting planet"),
};

void render() {
	glutSetWindowTitle(funcs[displayedProblem].name.c_str());
	colorBuffer.clearColorAndDepthBuffers();
	(*funcs[displayedProblem].func)();				// call the correct function
	colorBuffer.showColorBuffer();
}

void resize(int width, int height) {
	colorBuffer.setFrameBufferSize(width, height);
	glutPostRedisplay();
}

void timer(int id) {
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	if (key >= '0' && key <= '9') {
		displayedProblem = key - '0';
	} else if (key == ESCAPE) {
		glutLeaveMainLoop();
	}
}

int main(int argc, char* argv[]) {
	graphicsInit(argc, argv, __FILE__, WINDOW_SZ, WINDOW_SZ);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboardUtility);
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	glutKeyboardFunc(keyboard);

	for (int i = 0; i <= 9; i++) {
		cout << i << ' ' << funcs[i].name << endl;
	}

	colorBuffer.setClearColor(white);

	glutMainLoop();
	return 0;
}
