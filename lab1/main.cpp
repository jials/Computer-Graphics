// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "GL\freeglut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
const float DEG2RAD = 3.14159 / 180.0;

// http://slabode.exofire.net/circle_draw.shtml
void drawCircle(float cx, float cy, float r, int num_segments, int type)
{
	glBegin(type);
	for (int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component

		glVertex2f(x + cx, y + cy);//output vertex

	}
	glEnd();
}

// http://www.java-gaming.org/index.php/topic,4140.
void drawRadialGradientCircle(float innerColor[], float outerColor[], int slices, float radius) {
	float incr = (float)(2 * M_PI / slices);
	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(innerColor);
	glVertex2f(0.0f, 0.0f);

	glColor3fv(outerColor);
	for (int i = 0; i < slices; i++)
	{
		float angle = incr * i;

		float x = (float)cos(angle) * radius;
		float y = (float)sin(angle) * radius;

		glVertex2f(x, y);
	}

	glVertex2f(radius, 0.0f);

	glEnd();
}

// http://techtutorials95.blogspot.sg/2015/05/opengl-making-spirals.html
void drawSpiral(double a, double b)
{
	double x, y;
	double theta;

	glColor3f(255.0/255, 49.0/255, 30.0/255);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 1500; i++)
	{
		theta = 0.01 * i;
		x = (a + b * theta) * cos(theta);
		y = (a + b * theta) * sin(theta);

		glVertex2f((GLfloat)x, (GLfloat)y);
	}
	glEnd();
}

void drawStick() {
	glColor3f(146.0 / 255, 88.0 / 255, 39.0 / 255);
	glBegin(GL_POLYGON);
		glVertex2f(0.5, 0.0);
		glVertex2f(0.5, -8.0);
		glVertex2f(-0.5, -8.0);
		glVertex2f(-0.5, 0.0);
	glEnd();
}

void drawStickOutline() {
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0.5, 0.0);
		glVertex2f(0.5, -8.0);
		glVertex2f(-0.5, -8.0);
		glVertex2f(-0.5, 0.0);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw your stuff here
	float outerColor[] = { 237.0 / 255, 218.0 / 255, 173.0 / 255 };
	float innerColor[] = { 252.0 / 255, 242.0 / 255, 221.0 / 255 };
	glPushMatrix();
	glTranslatef(0, 4, 0);
	drawRadialGradientCircle(innerColor, outerColor, 10000, 5);
	glPopMatrix();

	glColor3f(211.0 / 255, 161.0 / 255, 43.0 / 255);
	glLineWidth(10.0);
	drawCircle(0, 4, 4.6, 10000, GL_LINE_STRIP);

	glPushMatrix();
	glTranslatef(0, 4, 0);
	glLineWidth(2.0);
	drawSpiral(0.5, 0.28);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	drawStick();
	drawStickOutline();
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	drawCircle(-2, 6, 2, 10000, GL_POLYGON);
	glDisable(GL_BLEND);

	glPopMatrix();
	glFlush ();
}

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
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);

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

		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 1\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
