/****************************************************
 * 2016-2021 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted.
 ****************************************************/

#include <ctime>
#include <vector>
#include "defs.h"
#include "utilities.h"
#include "framebuffer.h"
#include "colorandmaterials.h"
#include "rasterization.h"
#include "io.h"

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

const int SZ = 51;
const int SZ2 = SZ / 2;

void closedSquare(int x, int y, color C) {
}

void closedSquare(const ivec2 &centerPt, color C) {
}

void openSquare(const ivec2 &centerPt, color C) {
}

void render() {
	frameBuffer.clearColorAndDepthBuffers();

	drawLine(frameBuffer, 0, 0, 100, 100, red);
	drawLine(frameBuffer, 100, 100, 200, 100, green);

	closedSquare(100, 150, red);
	closedSquare(ivec2(200, 150), green);
	openSquare(ivec2(300, 150), blue);

	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	glutPostRedisplay();
}
int main(int argc, char *argv[]) {
    /*graphicsInit(argc, argv, __FILE__);
        
	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboardUtility);
	glutMouseFunc(mouseUtility);

	frameBuffer.setClearColor(white);

	glutMainLoop();*/

	double x = 33;
	double y = 99;
	cout << x << ' ' << y << endl;
	swap(x, y);
	cout << x << ' ' << y << endl;

	cout << approximatelyEqual(0.0, 0.000000001) << endl;

	cout << "" << endl;
	cout << normalizeDegrees(0) << endl;
	cout << normalizeDegrees(1) << endl;
	cout << normalizeDegrees(-1) << endl;
	cout << normalizeDegrees(721) << endl;
	cout << normalizeDegrees(-721) << endl;
	cout << normalizeDegrees(-360) << endl;

	cout << "" << endl;
	cout << normalizeRadians(0) << endl;
	cout << normalizeRadians(1) << endl;
	cout << normalizeRadians(-31 * PI) << endl;
	cout << normalizeRadians(3 * PI) << endl;
	cout << normalizeRadians(-6.28318530717958623200) << endl;

	cout << "" << endl;
	cout << approximatelyZero(0) << endl;
	cout << approximatelyZero(1) << endl;
	cout << approximatelyZero(0.001) << endl;
	cout << approximatelyZero(0.01) << endl;
	cout << approximatelyZero(-1) << endl;
	cout << approximatelyZero(-0.001) << endl;
	cout << approximatelyZero(-0.01) << endl;

	cout << "" << endl;
	cout << rad2deg(PI) << endl;
	cout << rad2deg(2 * PI) << endl;
	cout << rad2deg(-3 * PI) << endl;
	cout << rad2deg(1.57) << endl;

	cout << "" << endl;
	cout << deg2rad(180) << endl;
	cout << deg2rad(360) << endl;
	cout << deg2rad(-540) << endl;
	cout << deg2rad(90) << endl;

	cout << "" << endl;
	cout << min(1,2,3) << endl;
	cout << min(3,2,1) << endl;
	cout << min(3, 1, 2) << endl;
	cout << min(2, 1, 3) << endl;
	cout << min(87.75, -2.6, 3) << endl;

	cout << "" << endl;
	cout << max(1, 2, 3) << endl;
	cout << max(3, 2, 1) << endl;
	cout << max(3, 1, 2) << endl;
	cout << max(2, 1, 3) << endl;
	cout << max(87.75, -2.6, 3) << endl;

	cout << "" << endl;
	cout << distanceFromOrigin(0, 1) << endl;
	cout << distanceFromOrigin(1, 0) << endl;
	cout << distanceFromOrigin(1, 1) << endl;
	cout << distanceFromOrigin(-10, 30) << endl;

	cout << "" << endl;
	cout << distanceBetween(0, 0, 1, 1) << endl;
	cout << distanceBetween(1, 1, 0, 0) << endl;
	cout << distanceBetween(10, 10, 11, 11) << endl;
	cout << distanceBetween(100, 100, 99, 99) << endl;
	cout << distanceBetween(54, 1, -34, -99) << endl;

	cout << "" << endl;
	cout << areaOfTriangle(3, 4, 5) << endl;
	cout << areaOfTriangle(-3, 4, 5) << endl;
	cout << areaOfTriangle(3, 4, 50) << endl;

	cout << "" << endl;
	cout << areaOfTriangle(0, 0, 3, 0, 0, 4) << endl;

	cout << "" << endl;
	double x1;
	double y1;
	pointOnUnitCircle(PI_2, x1, y1);
	cout << x1 << ' ' << y1 << endl;

	cout << "" << endl;
	dvec2 center(3.0, 2.0);
	dvec2 pt = pointOnCircle(center, 2.0, PI_2);
	cout << pt << endl;

	cout << "" << endl;
	cout << directionInRadians(dvec2(0, 0), dvec2(2, 2)) << endl;           // -- > 0.7853981634
	cout << directionInRadians(dvec2(2, 10), dvec2(3, 11)) << endl;                  // -- > 0.7853981634
	cout << directionInRadians(dvec2(2, 2), dvec2(2, 0)) << endl;              //-- > 4.7123889804
	cout << directionInRadians(dvec2(2, 2)) << endl;
	cout << directionInRadians(dvec2(0, -2)) << endl;
	cout << directionInRadians(0, 0, 2, 2) << endl;
	cout << directionInRadians(2, 10, 3, 11) << endl;
	cout << directionInRadians(2, 2, 2, 0) << endl;

	cout << "" << endl;
	cout << map(2, 0, 5, 10, 11) << endl;

	cout << "" << endl;
	cout << quadratic(1, 4, 3) << endl;
	cout << quadratic(1, 0, 0) << endl;
	cout << quadratic(-4, -2, -1) << endl;
	cout << quadratic(-3, 4, -1) << endl;

	cout << "" << endl;
	double roots[2];
	cout << quadratic(1, 4, 3, roots) << endl;
	cout << roots[0] << " and " << roots[1] << endl;
	cout << quadratic(1, 0, 0, roots) << endl;
	cout << roots[0] << endl;
	cout << quadratic(-4, -2, -1, roots) << endl;
	cout << roots[0] << endl;
	cout << quadratic(-3, 4, -1, roots) << endl;
	cout << roots[0] << " and " << roots[1] << endl;

	cout << "" << endl;
	cout << glm::length(dvec3(3, 4, 0)) << endl;

	return 0;
}