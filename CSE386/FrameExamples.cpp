/****************************************************
 * 2016-2020 Eric Bachmann and Mike Zmuda
 * All Rights Reserved.
 * NOTICE:
 * Dissemination of this information or reproduction
 * of this material is prohibited unless prior written
 * permission is granted..
 ****************************************************/

#include <iostream>
#include <vector>
#include "utilities.h"
#include "defs.h"
#include "io.h"
void testPosition(const Frame& eyeFrame, const dvec3& pos) {
	dvec3 frameCoords = eyeFrame.toFrameCoords(pos);
	dvec3 backToWorldCoords = eyeFrame.toWorldCoords(frameCoords);
	cout << "POSITION" << endl;
	cout << "World: " << pos <<
		" To frame: " << frameCoords <<
		" Back to world : " << backToWorldCoords << endl;

	dvec3 worldCoords = eyeFrame.toWorldCoords(pos);
	dvec3 backToFrameCoords = eyeFrame.toFrameCoords(worldCoords);
	cout << "Frame: " << pos <<
		" To world: " << worldCoords <<
		" Back to frame: " << backToFrameCoords << endl << endl;
}
void testVector(const Frame & eyeFrame, const dvec3 & dir) {
	cout << "VECTOR" << endl;
	dvec3 frameVector = eyeFrame.toFrameVector(dir);
	dvec3 backToWorldVector = eyeFrame.toWorldVector(frameVector);
	cout << "World: " << dir <<
		" To frame: " << frameVector <<
		" Back to world: " << backToWorldVector << endl;

	dvec3 worldVector = eyeFrame.toWorldVector(dir);
	dvec3 backToFrameVector = eyeFrame.toFrameVector(worldVector);
	cout << "Frame: " << dir <<
		" To world: " << worldVector <<
		" Back to frame: " << backToFrameVector << endl << endl;
}
int main(int argc, char *argv[]) {
	dvec3 cameraPos(1, 1, 1);
	dvec3 lookAt(0, 0, 0);
	dvec3 viewDir = lookAt - cameraPos;
	dvec3 w = -glm::normalize(viewDir);
	dvec3 up = Y_AXIS;
	dvec3 u = glm::normalize(glm::cross(up, w));
	dvec3 v = glm::normalize(glm::cross(w, u));

	Frame cameraFrame(cameraPos, u, v, w);

	cout << cameraFrame << endl;

	testPosition(cameraFrame, dvec3(0, 0, 0));
	testPosition(cameraFrame, dvec3(1, 1, 1));
	testVector(cameraFrame, dvec3(0, 0, 1));
	testVector(cameraFrame, dvec3(1, 0, 0));

	return 0;
}
/*
ExerciseFrame
Pos: [ 1 1 1 ]
U: [ 0.707107 0 -0.707107 ]
V: [ -0.408248 0.816497 -0.408248 ]
W: [ 0.57735 0.57735 0.57735 ]

POSITION
World: [ 0 0 0 ] To frame: [ 0 0 -1.73205 ] Back to world : [ 0 0 0 ]
Frame: [ 0 0 0 ] To world: [ 1 1 1 ] Back to frame: [ 0 0 -2.22045e-16 ]

POSITION
World: [ 1 1 1 ] To frame: [ 0 0 -2.22045e-16 ] Back to world : [ 1 1 1 ]
Frame: [ 1 1 1 ] To world: [ 1.87621 2.39385 0.461995 ] Back to frame: [ 1 1 1 ]

VECTOR
World: [ 0 0 1 ] To frame: [ -0.707107 -0.408248 0.57735 ] Back to world: [ -2.22045e-16 0 1 ]
Frame: [ 0 0 1 ] To world: [ 0.57735 0.57735 0.57735 ] Back to frame: [ 0 0 1 ]

VECTOR
World: [ 1 0 0 ] To frame: [ 0.707107 -0.408248 0.57735 ] Back to world: [ 1 0 -2.22045e-16 ]
Frame: [ 1 0 0 ] To world: [ 0.707107 0 -0.707107 ] Back to frame: [ 1 0 0 ]
*/