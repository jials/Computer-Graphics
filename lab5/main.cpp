// CS3241Lab5.cpp
#include <cmath>
#include <iostream>
#include "vector3D.h"
#include <chrono>


/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define WINWIDTH 600
#define WINHEIGHT 400
#define NUM_OBJECTS 4
#define MAX_RT_LEVEL 50
#define NUM_SCENE 2

float* pixelBuffer = new float[WINWIDTH * WINHEIGHT * 3];

class Ray { // a ray that start with "start" and going in the direction "dir"
public:
	Vector3 start, dir;
};

class RtObject {

public:
	virtual double intersectWithRay(Ray, Vector3& pos, Vector3& normal) = 0; // return a -ve if there is no intersection. Otherwise, return the smallest postive value of t

																			 // Materials Properties
	double ambiantReflection[3];
	double diffusetReflection[3];
	double specularReflection[3];
	double speN = 300;


};
class Sphere : public RtObject {

	Vector3 center_;
	double r_;
public:
	Sphere(Vector3 c, double r) { center_ = c; r_ = r; };
	Sphere() {};
	void set(Vector3 c, double r) { center_ = c; r_ = r; };
	double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

RtObject **objList; // The list of all objects in the scene


					// Global Variables
					// Camera Settings
Vector3 cameraPos(0, 0, -500);

// assume the the following two vectors are normalised
Vector3 lookAtDir(0, 0, 1);
Vector3 upVector(0, 1, 0);
Vector3 leftVector(1, 0, 0);
float focalLen = 500;

// Light Settings

Vector3 lightPos(900, 1000, -1500);
double ambiantLight[3] = { 0.4,0.4,0.4 };
double diffusetLight[3] = { 0.7,0.7, 0.7 };
double specularLight[3] = { 0.5,0.5, 0.5 };


double bgColor[3] = { 0.1,0.1,0.4 };

int sceneNo = 0;
/*==============================
Modify Section Here
Hint: Add additional methods to help you in solving equations
==============================*/


double Sphere::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{// Step 1
	double a = dot_prod(r.dir, r.dir);
	double b = 2 * dot_prod(r.dir, (r.start - center_));
	double c = dot_prod(r.start - center_, r.start - center_) - r_ * r_;

	// check imaginary root
	// 1 root, considered no intersection
	if (b * b - 4 * a * c <= 0) {
		return -1;
	}

	double x1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	double x2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

	double t = x1 < x2 ? x1 : x2;
	intersection = r.start + r.dir * t;

	normal = intersection - center_;
	normal.normalize();
	return t;
}

void addAnotherScene() {
	//Step 5: Modify objList here
	((Sphere*)objList[0])->set(Vector3(0, -350, 250), 350);
	((Sphere*)objList[1])->set(Vector3(-60, 90, 30), 100);
	((Sphere*)objList[2])->set(Vector3(150, 80, 0), 20);
	((Sphere*)objList[3])->set(Vector3(130, 20, 50), 50);

	objList[0]->ambiantReflection[0] = 0.1;
	objList[0]->ambiantReflection[1] = 0.4;
	objList[0]->ambiantReflection[2] = 0.4;
	objList[0]->diffusetReflection[0] = 0;
	objList[0]->diffusetReflection[1] = 1;
	objList[0]->diffusetReflection[2] = 1;
	objList[0]->specularReflection[0] = 0.2;
	objList[0]->specularReflection[1] = 0.4;
	objList[0]->specularReflection[2] = 0.4;
	objList[0]->speN = 300;

	objList[1]->ambiantReflection[0] = 0.6;
	objList[1]->ambiantReflection[1] = 0.6;
	objList[1]->ambiantReflection[2] = 0.2;
	objList[1]->diffusetReflection[0] = 1;
	objList[1]->diffusetReflection[1] = 1;
	objList[1]->diffusetReflection[2] = 0;
	objList[1]->specularReflection[0] = 0.0;
	objList[1]->specularReflection[1] = 0.0;
	objList[1]->specularReflection[2] = 0.0;
	objList[1]->speN = 50;

	objList[2]->ambiantReflection[0] = 0.1;
	objList[2]->ambiantReflection[1] = 0.6;
	objList[2]->ambiantReflection[2] = 0.1;
	objList[2]->diffusetReflection[0] = 0.1;
	objList[2]->diffusetReflection[1] = 1;
	objList[2]->diffusetReflection[2] = 0.1;
	objList[2]->specularReflection[0] = 0.3;
	objList[2]->specularReflection[1] = 0.7;
	objList[2]->specularReflection[2] = 0.3;
	objList[2]->speN = 650;

	objList[3]->ambiantReflection[0] = 0.3;
	objList[3]->ambiantReflection[1] = 0.3;
	objList[3]->ambiantReflection[2] = 0.3;
	objList[3]->diffusetReflection[0] = 0.7;
	objList[3]->diffusetReflection[1] = 0.7;
	objList[3]->diffusetReflection[2] = 0.7;
	objList[3]->specularReflection[0] = 0.6;
	objList[3]->specularReflection[1] = 0.6;
	objList[3]->specularReflection[2] = 0.6;
	objList[3]->speN = 650;

}

void rayTrace(Ray ray, double& r, double& g, double& b, int fromObj = -1 ,int level = 0)
{
	// Step 4
	// Checks if it reaches MAX_RT_LEVEL
	if (level == MAX_RT_LEVEL) return;

	int goBackGround = 1, i = 0;
	double tempR, tempG, tempB;

	Vector3 intersection, normal;
	Vector3 lightV;
	Vector3 viewV;
	Vector3 lightReflectionV;
	Vector3 rayReflectionV;

	Ray newRay;
	double mint = DBL_MAX, t;
	int obj;

	for (i = 0; i < NUM_OBJECTS; i++)
	{
		if ((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0)
		{
			if (i == fromObj) continue;

			if (t < mint) {
				mint = t;
			}
			else {
				break;
			}

			// Step 2
			tempR = objList[i]->ambiantReflection[0] * ambiantLight[0];
			tempG = objList[i]->ambiantReflection[1] * ambiantLight[1];
			tempB = objList[i]->ambiantReflection[2] * ambiantLight[2];

			// Step 3
			lightV = lightPos - intersection;
			lightV.normalize();
			tempR += objList[i]->diffusetReflection[0] * diffusetLight[0] * dot_prod(normal, lightV);
			tempG += objList[i]->diffusetReflection[1] * diffusetLight[1] * dot_prod(normal, lightV);
			tempB += objList[i]->diffusetReflection[2] * diffusetLight[2] * dot_prod(normal, lightV);

			viewV= -ray.dir;
			lightReflectionV = normal * 2 * (dot_prod(normal, lightV)) - lightV;
			lightReflectionV.normalize();
			double specular = pow(dot_prod(lightReflectionV, viewV), objList[i]->speN);

			tempR += objList[i]->specularReflection[0] * specularLight[0] * specular;
			tempG += objList[i]->specularReflection[1] * specularLight[1] * specular;
			tempB += objList[i]->specularReflection[2] * specularLight[2] * specular;

			rayReflectionV = normal * 2 * (dot_prod(normal, viewV)) - viewV;
			rayReflectionV.normalize();
			newRay.dir = rayReflectionV;
			newRay.start = intersection;
			
			obj = i;

			goBackGround = 0;

			// shadow
			Ray shadowRay;
			shadowRay.dir = lightV;
			shadowRay.start = intersection;
			int s;

			for (int j = 0; j < NUM_OBJECTS; j++)
			{
				if (i == j) continue;
				if ((s = objList[j]->intersectWithRay(shadowRay, intersection, normal)) > 0) {
					tempR = ambiantLight[0] * objList[i]->ambiantReflection[0];
					tempG = ambiantLight[1] * objList[i]->ambiantReflection[1];
					tempB = ambiantLight[2] * objList[i]->ambiantReflection[2];
					break;
				}
			}
		}
	}

	if (goBackGround)
	{
		r = bgColor[0];
		g = bgColor[1];
		b = bgColor[2];
	}
	else {
		rayTrace(newRay, r, g, b, obj, level + 1);
		r = specularLight[0] * objList[obj]->specularReflection[0] * r + tempR;
		g = specularLight[1] * objList[obj]->specularReflection[1] * g + tempG;
		b = specularLight[2] * objList[obj]->specularReflection[2] * b + tempB;
	}

}
/*==============================
End of Section,
IT IS NOT RECOMMENDED TO CHANGE ANYTHING BELOW
==============================*/



void drawInPixelBuffer(int x, int y, double r, double g, double b)
{
	pixelBuffer[(y*WINWIDTH + x) * 3] = (float)r;
	pixelBuffer[(y*WINWIDTH + x) * 3 + 1] = (float)g;
	pixelBuffer[(y*WINWIDTH + x) * 3 + 2] = (float)b;
}

void renderScene()
{
	int x, y;
	Ray ray;
	double r, g, b;

	cout << "Rendering Scene " << sceneNo << " with resolution " << WINWIDTH << "x" << WINHEIGHT << "........... ";
	__int64 time1 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the starting time

	ray.start = cameraPos;

	Vector3 vpCenter = cameraPos + lookAtDir * focalLen;  // viewplane center
	Vector3 startingPt = vpCenter + leftVector * (-WINWIDTH / 2.0) + upVector * (-WINHEIGHT / 2.0);
	Vector3 currPt;

	for (x = 0; x<WINWIDTH; x++)
		for (y = 0; y < WINHEIGHT; y++)
		{
			currPt = startingPt + leftVector*x + upVector*y;
			ray.dir = currPt - cameraPos;
			ray.dir.normalize();
			rayTrace(ray, r, g, b);
			drawInPixelBuffer(x, y, r, g, b);
		}

	__int64 time2 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the ending time

	cout << "Done! \nRendering time = " << time2 - time1 << "ms" << endl << endl;
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DOUBLEBUFFER);
	glDrawPixels(WINWIDTH, WINHEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void setScene(int i = 0)
{
	if (i > NUM_SCENE)
	{
		cout << "Warning: Invalid Scene Number" << endl;
		return;
	}

	if (i == 0)
	{

		((Sphere*)objList[0])->set(Vector3(-130, 80, 120), 100);
		((Sphere*)objList[1])->set(Vector3(130, -80, -80), 100);
		((Sphere*)objList[2])->set(Vector3(-130, -80, -80), 100);
		((Sphere*)objList[3])->set(Vector3(130, 80, 120), 100);

		objList[0]->ambiantReflection[0] = 0.1;
		objList[0]->ambiantReflection[1] = 0.4;
		objList[0]->ambiantReflection[2] = 0.4;
		objList[0]->diffusetReflection[0] = 0;
		objList[0]->diffusetReflection[1] = 1;
		objList[0]->diffusetReflection[2] = 1;
		objList[0]->specularReflection[0] = 0.2;
		objList[0]->specularReflection[1] = 0.4;
		objList[0]->specularReflection[2] = 0.4;
		objList[0]->speN = 300;

		objList[1]->ambiantReflection[0] = 0.6;
		objList[1]->ambiantReflection[1] = 0.6;
		objList[1]->ambiantReflection[2] = 0.2;
		objList[1]->diffusetReflection[0] = 1;
		objList[1]->diffusetReflection[1] = 1;
		objList[1]->diffusetReflection[2] = 0;
		objList[1]->specularReflection[0] = 0.0;
		objList[1]->specularReflection[1] = 0.0;
		objList[1]->specularReflection[2] = 0.0;
		objList[1]->speN = 50;

		objList[2]->ambiantReflection[0] = 0.1;
		objList[2]->ambiantReflection[1] = 0.6;
		objList[2]->ambiantReflection[2] = 0.1;
		objList[2]->diffusetReflection[0] = 0.1;
		objList[2]->diffusetReflection[1] = 1;
		objList[2]->diffusetReflection[2] = 0.1;
		objList[2]->specularReflection[0] = 0.3;
		objList[2]->specularReflection[1] = 0.7;
		objList[2]->specularReflection[2] = 0.3;
		objList[2]->speN = 650;

		objList[3]->ambiantReflection[0] = 0.3;
		objList[3]->ambiantReflection[1] = 0.3;
		objList[3]->ambiantReflection[2] = 0.3;
		objList[3]->diffusetReflection[0] = 0.7;
		objList[3]->diffusetReflection[1] = 0.7;
		objList[3]->diffusetReflection[2] = 0.7;
		objList[3]->specularReflection[0] = 0.6;
		objList[3]->specularReflection[1] = 0.6;
		objList[3]->specularReflection[2] = 0.6;
		objList[3]->speN = 650;

	}

	if (i == 1)
	{

		// Step 5
		addAnotherScene();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {
	case 's':
	case 'S':
		sceneNo = (sceneNo + 1) % NUM_SCENE;
		setScene(sceneNo);
		renderScene();
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		exit(0);

	default:
		break;
	}
}

int main(int argc, char **argv)
{


	cout << "<<CS3241 Lab 5>>\n\n" << endl;
	cout << "S to go to next scene" << endl;
	cout << "Q to quit" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINWIDTH, WINHEIGHT);

	glutCreateWindow("CS3241 Lab 5: Ray Tracing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	objList = new RtObject*[NUM_OBJECTS];

	// create four spheres
	objList[0] = new Sphere(Vector3(-130, 80, 120), 100);
	objList[1] = new Sphere(Vector3(130, -80, -80), 100);
	objList[2] = new Sphere(Vector3(-130, -80, -80), 100);
	objList[3] = new Sphere(Vector3(130, 80, 120), 100);

	setScene(0);

	setScene(sceneNo);
	renderScene();

	glutMainLoop();

	for (int i = 0; i < NUM_OBJECTS; i++)
		delete objList[i];
	delete[] objList;

	delete[] pixelBuffer;

	return 0;
}
