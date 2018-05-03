/*
 * environment.cpp
 *
 *  Created on: Apr 19, 2018
 *      Author: 44857713
 */
#include <GL/glut.h>
// Math library for sqrt function
#include <math.h>
#include "environment.h"

environment::environment() {
	// TODO Auto-generated constructor stub

}

environment::~environment() {
	// TODO Auto-generated destructor stub
}

//	creates the houses.
void houses(void) {
	glBegin(GL_POLYGON);
	glColor3f(0.3, 0.8, 0);
	glVertex2f(7.0, 5.0);
	glVertex2f(7, 3);
	glVertex2f(3, 3);
	glVertex2f(3, 5);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.6, 0.4);
	glVertex2f(-6, -4);
	glVertex2f(-2, -4);
	glVertex2f(-2, -6);
	glVertex2f(-6, -6);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.6, 0.8);
	glVertex2f(-6.5, 3);
	glVertex2f(-2, 3);
	glVertex2f(-2, 0);
	glVertex2f(-6.5, 0);
	glEnd();
}

// 	creates the fire. credit to online sources
//	http://programanddesign.com/cpp/draw-an-unfilled-circle/
void environment::fire(GLfloat x, GLfloat y, GLfloat r) {
	static const double inc = M_PI / 12;
	static const double max = 2 * M_PI;
	glBegin(GL_LINE_LOOP);
	for (double d = 0; d < max; d += inc) {
		glVertex2f(cos(d) * r + x, sin(d) * r + y);
	}
	glEnd();
}

//	draws the environment (uses push and pop to not effect other draws)
void environment::drawEnvi(void) {
	glPushMatrix();
	glColor3f(1, 0.1, 0);
	glLineWidth(10);
	glTranslatef(200, 500, 0.0);
	glScalef(35, 35, 1.0);
	fire(1, 1, 5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(250, 500, 0.0);
	glScalef(16, 16, 1.0);
	houses();
	glPopMatrix();
}
