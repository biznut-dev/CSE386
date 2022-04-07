#include <ctime> 
#include <iostream>
#include <vector>
#include "eshape.h"
#include "io.h"
#include "light.h"
#include "vertexops.h"

int main(int argc, char* argv[])
{
	dvec3 position = dvec3(2.0, 3.0, 4.0);
	dvec3 focus = dvec3(-2.0, -3.0, 0);
	dvec3 upVector = dvec3(0, 1, 0);

	dmat4 viewingMatrix = glm::lookAt(position, focus, upVector);

	cout << T(1.0, 2.0, 3.0) << endl;
}