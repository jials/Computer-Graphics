// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <time.h>
#pragma warning(disable:4996)

/* Include header files depending on platform */
#ifdef _WIN32
    #include "GL\freeglut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 100
#define numPlanets 4
#define hourPlanet 2
#define minPlanet 1
#define secPlanet 3

class Planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;
	int numSatellites;
	float satellitesDist;

	Planet()
	{
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
		numSatellites = 0;
		satellitesDist = 0.0;
	}
};

class Satellite
{
public:
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;

	Satellite()
	{
		angularSpeed = 0.1;
		color[0] = color[1] = color[2] = 1;
		size = 0.1;
		angle = rand() % 180;
	}
};

struct Star
{
public:
	float x;
	float y;
};

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
Planet planetList[numPlanets];
Star starList[numStars];
Satellite satellite;
int timeMode = 0;

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor(18.0 / 255, 36.0 / 255, 62.0 / 255, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat colorAmbient[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, colorAmbient);
	GLfloat lightColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // light placed at the origin
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	Planet sun;
	sun.color[0] = 244.0 / 255;
	sun.color[1] = 232.0 / 255;
	sun.color[2] = 66.0 / 255;
	sun.size = 2;
	sun.numSatellites = 0;

	Planet earth;
	earth.color[0] = 100.0 / 255;
	earth.color[1] = 145.0 / 255;
	earth.color[2] = 218.0 / 255;
	earth.size = 0.7;
	earth.distFromRef = 4.0;
	earth.angularSpeed = 0.05;
	earth.numSatellites = 1;
	earth.satellitesDist = 1.0;

	Planet mars;
	mars.color[0] = 232.0 / 255;
	mars.color[1] = 178.0 / 255;
	mars.color[2] = 142.0 / 255;
	mars.size = 1.0;
	mars.distFromRef = 7.6;
	mars.angularSpeed = 0.01;
	mars.numSatellites = 1;
	mars.satellitesDist = 1.4;

	Planet jupiter;
	jupiter.color[0] = 150.0 / 255;
	jupiter.color[1] = 149.0 / 255;
	jupiter.color[2] = 153.0 / 255;
	jupiter.size = 0.3;
	jupiter.distFromRef = 5.7;
	jupiter.angularSpeed = 0.1;
	jupiter.numSatellites = 0;

	planetList[0] = sun;
	planetList[1] = earth;
	planetList[2] = mars;
	planetList[3] = jupiter;

	// draw it within the canvas size
	for (int i = 0; i < numStars; i++) {
		starList[i].x = rand() % 20 - 10;
		starList[i].y = rand() % 20 - 10;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);

	//draw star
	for (int i = 0; i < numStars; i++) {
		glPushMatrix();
		glTranslatef(starList[i].x, starList[i].y, 0);
		glColor4f(1.0, 1.0, 1.0, (double)rand() / (RAND_MAX));
		glutSolidSphere(0.05, 10, 10);
		glPopMatrix();
	}

	// draw sun
	glPushMatrix();
	Planet sun = planetList[0];
	glColor3fv(sun.color);
	GLfloat mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glutSolidSphere(sun.size, 50, 50);
	glPopMatrix();

	// draw planets
	for (int i = 1; i < numPlanets; i++) {
		glPushMatrix();
		glColor3fv(planetList[i].color);
		glRotatef(planetList[i].angle, 0, 0, 1);
		glTranslatef(planetList[i].distFromRef, 0, 0);
		glutSolidSphere(planetList[i].size, 50, 50);

		// draw satellites
		for (int j = 0; j < planetList[i].numSatellites; j++) {
			glRotatef(satellite.angle, 0, 0, 1);
			glPushMatrix();
			glTranslatef(planetList[i].satellitesDist, 0, 0);
			glColor3fv(satellite.color);
			glutSolidSphere(satellite.size, 13, 26);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glFlush ();
}

void animateSolarSystem() {
	for (int i = 1; i < numPlanets; i++) {
		planetList[i].angle += planetList[i].angularSpeed;
	}
}

void animateTime() {
	time_t current_time = time(NULL);
	struct tm *timeinfo = localtime(&current_time);
	double angleSec = 360 - (float)timeinfo->tm_sec / 60 * 360;
	double angleMin = 360 - (float)timeinfo->tm_min / 60 * 360;
	double angleHour = 360 - (float)timeinfo->tm_hour / 12 * 360;
	
	// + 90 degree because the displacement is to the right of the planet
	double diffSec = angleSec - planetList[secPlanet].angle + 90;
	double diffMin = angleMin - planetList[minPlanet].angle + 90;
	double diffHour = angleHour - planetList[hourPlanet].angle + 90;

	for (int i = 1; i < numPlanets; i++) {
		switch (i) {
			case secPlanet:
				planetList[i].angle += diffSec;
				break;
			case minPlanet:
				planetList[i].angle += diffMin;
				break;
			case hourPlanet:
				planetList[i].angle += diffHour; 
				break;
			default:
				break;
		}
	}
}

void idle()
{
	// update animation here
	if (!timeMode) {
		animateSolarSystem();
	}
	else {
		animateTime();
	}

	satellite.angle += satellite.angularSpeed;

	glutPostRedisplay();	//after updating, draw the screen again
}

void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;
		
		case 't':
			timeMode = 1 - timeMode;
			glutPostRedisplay();
		break;

		case 27:
			exit(0);
		break;


		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n\n";

	cout << "Time mode: T\n";
	cout << "Smallest planet represents second\n";
	cout << "Largest planet represents hour\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
