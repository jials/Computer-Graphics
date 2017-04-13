// CS3241Lab4.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "GL\freeglut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8
#define PI 3.14159

using namespace std;

// Global variables that you can use
struct Point {
	int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point c1PtList[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;
bool animation = false;

int carPos = 0;
	
void drawRightArrow()
{
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0,0);
		glVertex2f(100,0);
		glVertex2f(95,5);
		glVertex2f(100,0);
		glVertex2f(95,-5);
	glEnd();
}

void calculateC1Continuity() {
	for (int i = 3; i < nPt - 3; i += 3) {
		Point q2;
		Point p3 = ptList[i - 1];
		q2.x = 2 * ptList[i].x - p3.x;
		q2.y = 2 * ptList[i].y - p3.y;
		c1PtList[i + 1] = q2;
	}
}

void drawBezierCurve() {
	calculateC1Continuity();

	for (int i = 0; i < nPt - 3; i += 3) {
		Point p1 = ptList[i];
		Point p2;
		if (C1Continuity) {
			p2 = c1PtList[i + 1];
		} else {
			p2 = ptList[i + 1];
		}
		Point p3 = ptList[i + 2];
		Point p4 = ptList[i + 3];

		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 1);
		for (int j = 0; j < NLINESEGMENT + 1; j++) {
			float t = j / 32.0;
			float x = pow((1 - t), 3) * p1.x + 3 * pow((1 - t), 2) * t * p2.x + 3 * (1 - t) * pow(t, 2) * p3.x + pow(t, 3) * p4.x;
			float y = pow((1 - t), 3) * p1.y + 3 * pow((1 - t), 2) * t * p2.y + 3 * (1 - t) * pow(t, 2) * p3.y + pow(t, 3) * p4.y;
			glVertex2d(x, y);
		}
		glEnd();
	}
}

void drawTrolley() {
	glColor3f(1, 0.8, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(-8, 30);
	glVertex2f(-8, 15);
	glVertex2f(-3, 15);
	glVertex2f(-3, 30);
	glEnd();
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_POLYGON);
	glVertex2f(-13, 15);
	glVertex2f(-13, 5);
	glVertex2f(13, 5);
	glVertex2f(13, 15);
	glEnd();

	// draw 2 wheels
	glBegin(GL_TRIANGLE_FAN); 
	for (int i = 0; i <= 360; i++) {
		glColor3f(0, 0, 0);
		glVertex2f((6 + (3 * cos(i * 2 * PI / 360))), (2.5 + (3 * sin(i * 2 * PI / 360))));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); 
	for (int i = 0; i <= 360; i++) {
		glColor3f(0, 0, 0);
		glVertex2f((-6 + (3 * cos(i * 2 * PI / 360))), (2.5 + (3 * sin(i * 2 * PI / 360))));
	}
	glEnd();
}

void drawTangent() {
	for (int i = 3; i < nPt; i += 3) {
		Point p1 = ptList[i - 3];
		Point p2 = ptList[i - 2];
		Point p3 = ptList[i - 1];
		Point p4 = ptList[i];
		if (C1Continuity) {
			p1 = c1PtList[i - 3];
			p2 = c1PtList[i - 2];
			p3 = c1PtList[i - 1];
			p4 = c1PtList[i];
		}

		for (float j = 0; j < NOBJECTONCURVE + 1; j++) {
			float t = j / NOBJECTONCURVE;
			float x = pow((1 - t), 3) * p1.x + 3 * pow((1 - t), 2) * t * p2.x + 3 * (1 - t) * pow(t, 2) * p3.x + pow(t, 3) * p4.x;
			float y = pow((1 - t), 3) * p1.y + 3 * pow((1 - t), 2) * t * p2.y + 3 * (1 - t) * pow(t, 2) * p3.y + pow(t, 3) * p4.y;

			float tx = 3 * (pow(t, 2) * (p4.x - p1.x + 3 * p2.x - 3 * p3.x) + 2 * t * (p1.x - 2 * p2.x + p3.x) - p1.x + p2.x);
			float ty = 3 * (pow(t, 2) * (p4.y - p1.y + 3 * p2.y - 3 * p3.y) + 2 * t * (p1.y - 2 * p2.y + p3.y) - p1.y + p2.y);

			float angle = 180 * atan2(ty, tx) / PI;

			glPushMatrix();
			glTranslatef(x, y, 0);
			glRotatef(angle, 0, 0, 1);
			drawRightArrow();
			glPopMatrix();
		}
	}
}

void drawOrientatedTrolley() {
	for (int i = 3; i < nPt; i += 3) {
		Point p1 = ptList[i - 3];
		Point p2 = ptList[i - 2];
		Point p3 = ptList[i - 1];
		Point p4 = ptList[i];
		if (C1Continuity) {
			p1 = c1PtList[i - 3];
			p2 = c1PtList[i - 2];
			p3 = c1PtList[i - 1];
			p4 = c1PtList[i];
		}

		if (animation) {
			float t = (carPos % NOBJECTONCURVE) * 1.0 / NOBJECTONCURVE;
			float x = pow((1 - t), 3) * p1.x + 3 * pow((1 - t), 2) * t * p2.x + 3 * (1 - t) * pow(t, 2) * p3.x + pow(t, 3) * p4.x;
			float y = pow((1 - t), 3) * p1.y + 3 * pow((1 - t), 2) * t * p2.y + 3 * (1 - t) * pow(t, 2) * p3.y + pow(t, 3) * p4.y;
			float tx = 3 * (pow(t, 2) * (p4.x - p1.x + 3 * p2.x - 3 * p3.x) + 2 * t * (p1.x - 2 * p2.x + p3.x) - p1.x + p2.x);
			float ty = 3 * (pow(t, 2) * (p4.y - p1.y + 3 * p2.y - 3 * p3.y) + 2 * t * (p1.y - 2 * p2.y + p3.y) - p1.y + p2.y);

			float angle = 180 * atan2(ty, tx) / PI;

			glPushMatrix();
			glTranslatef(x, y, 0);
			glRotatef(angle, 0, 0, 1);
			drawTrolley();
			glPopMatrix();
		}
		else {
			for (float j = 0; j < NOBJECTONCURVE + 1; j++) {
				float t = j / NOBJECTONCURVE;
				float x = pow((1 - t), 3) * p1.x + 3 * pow((1 - t), 2) * t * p2.x + 3 * (1 - t) * pow(t, 2) * p3.x + pow(t, 3) * p4.x;
				float y = pow((1 - t), 3) * p1.y + 3 * pow((1 - t), 2) * t * p2.y + 3 * (1 - t) * pow(t, 2) * p3.y + pow(t, 3) * p4.y;

				float tx = 3 * (pow(t, 2) * (p4.x - p1.x + 3 * p2.x - 3 * p3.x) + 2 * t * (p1.x - 2 * p2.x + p3.x) - p1.x + p2.x);
				float ty = 3 * (pow(t, 2) * (p4.y - p1.y + 3 * p2.y - 3 * p3.y) + 2 * t * (p1.y - 2 * p2.y + p3.y) - p1.y + p2.y);

				float angle = 180 * atan2(ty, tx) / PI;

				glPushMatrix();
				glTranslatef(x, y, 0);
				glRotatef(angle, 0, 0, 1);
				drawTrolley();
				glPopMatrix();
			}
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	drawBezierCurve();

	if(displayControlPoints)
	{

		glPointSize(5);
		glBegin(GL_POINTS);
		for(int i = 0; i < nPt; i++)
		{
			if (C1Continuity && i > 3 && i % 3 == 1) {
				glColor3f(0.7, 0.7, 0.7);
				glVertex2d(ptList[i].x, ptList[i].y);
				glColor3f(1, 0, 0);
				glVertex2d(c1PtList[i].x, c1PtList[i].y);
			} else {
				glColor3f(0,0,0);
				glVertex2d(ptList[i].x,ptList[i].y);
			}
		}
		glEnd();
		glPointSize(1);

	}

	if(displayControlLines)
	{	
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < nPt - 1; i++) {
			if (C1Continuity) {
				glColor3f(0, 1, 0);
				glVertex2d(c1PtList[i].x, c1PtList[i].y);
				glVertex2d(c1PtList[i + 1].x, c1PtList[i + 1].y);
			}
			else {
				glColor3f(0,1,0);
				glVertex2d(ptList[i].x, ptList[i].y);
				glVertex2d(ptList[i + 1].x, ptList[i + 1].y);
			}
		}
		glEnd();
	}

	if (displayTangentVectors) {
		drawTangent();
	}

	if (displayObjects) {
		drawOrientatedTrolley();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{

	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;

	if(nPt>MAXPTNO)
	{
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}

	for(int i=0;i<nPt;i++)
	{
		file >> ptList[i].x;
		file >> ptList[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;

	for(int i=0;i<nPt;i++)
	{
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'a':
		case 'A':
			animation = !animation;
			break;
		case 'r':
		case 'R':
			readFile();
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
		break;

		case 'o':
		case 'O':
			displayObjects = !displayObjects;
		break;

		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
		break;

		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
		break;

		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
		break;

		case 'e':
		case 'E':
			// Do something to erase all the control points added
			nPt = 0;
		break;

		case 27:
			exit(0);
		break;

		default:
		break;
	}

	glutPostRedisplay();
}

void animate(int value) {
	if (animation) {
		glClearColor(1.0 - rand() % 255 / 255.0, 1.0, 1.0 - rand() % 255 / 255.0, 1.0);
		carPos++;
	}
	else {
		glClearColor(1.0, 1.0, 1.0, 1.0);
	}

	glutPostRedisplay();
	glutTimerFunc(500, animate, 0);
}

void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
	{
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		c1PtList[nPt].x = ptList[nPt].x = x;
		c1PtList[nPt].y = ptList[nPt].y = y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "ESC: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	cout << "A: Toggle animation" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	glutTimerFunc(0, animate, 0);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
