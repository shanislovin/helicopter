/*
 * sesbase.cpp
 *
 *  Created on: 20Apr.,2018
 *      Author: lovin
 */

#include "sesbase.h"
#include <GL/glut.h>

#include "helicopter.h"

sesbase::sesbase() :
		baseX(930), baseY(75) {
	// TODO Auto-generated constructor stub

}

sesbase::~sesbase() {
	// TODO Auto-generated destructor stub
}

//	creates the SES refuelling base
void base(void) {
	glBegin(GL_POLYGON);
	glColor3f(0.27, 0.27, 0.27);
	glVertex2f(-10, 0);
	glVertex2f(-5, 7.5);
	glVertex2f(5, 7.5);
	glVertex2f(10, 0);
	glVertex2f(5, -7.5);
	glVertex2f(-5, -7.5);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, 4);
	glVertex2f(0.5, 4);
	glVertex2f(0.5, -4);
	glVertex2f(-0.5, -4);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-4, 0.5);
	glVertex2f(4, 0.5);
	glVertex2f(4, -0.5);
	glVertex2f(-4, -0.5);
	glEnd();
}

//	draws the SES base. It is in a push and pop so it does not effect the drawing of other objects.
void sesbase::drawBase(void) {
	glPushMatrix();
	glTranslatef(baseX, baseY, 0.0);
	glScalef(5, 5, 1.0);
	glColor3f(0.1, 0.1, 0.1);
	base();
	glPopMatrix();
}
