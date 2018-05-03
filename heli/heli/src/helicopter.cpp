/*
 * helicopter.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: 44857713
 */
#include <GL/glut.h>
// Math library for sqrt function
#include <math.h>
#include "helicopter.h"
#include "sesbase.h"
#include <iostream>
using namespace std;
# define PI 3.1415926535897932

//	sets all the values of variables
helicopter::helicopter() :
		fuelY(7), spin(0), centre_x(400), centre_y(375), rotate(0.0), mouse_x(
				400), mouse_y(375), rotate2(0.0), speed(175.0), length(20), scaleV(
				2), screenWidth(1000.0), screenHeight(750.0), delta_t(0), moveAllowed(
				false), fuelAllowed(false), startFuelling(false) {
	// TODO Auto-generated constructor stub
}

helicopter::~helicopter() {
	// TODO Auto-generated destructor stub
}

//	creates helicopter
void helicop() {
	glBegin(GL_POLYGON);
	glVertex2f(-0.8, 0.05);
	glVertex2f(-0.3, 0.3);
	glVertex2f(0.3, 0.3);
	glVertex2f(0.6, 0.15);

	glVertex2f(0.6, -0.15);
	glVertex2f(0.3, -0.3);
	glVertex2f(-0.3, -0.3);
	glVertex2f(-0.8, -0.05);

	glVertex2f(-1.8, -0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-1.5, -0.0);
	glVertex2f(-1.6, -0.3);
	glVertex2f(-1.6, 0.3);
	glVertex2f(-1.5, -0.0);
	glEnd();
}

//	creates the glass front of helicopter, for added detail!
void helicopGlass() {
	glBegin(GL_POLYGON);
	glColor3f(0, 0.8, 1);
	glVertex2f(0.35, 0.25);
	glVertex2f(0.55, 0.1);
	glVertex2f(0.55, -0.1);
	glVertex2f(0.35, -0.25);
	glEnd();
}

//	creates the blades of the helicopter
void heliblades() {
	glBegin(GL_POLYGON);
	glVertex2f(-0.05, 1.5);
	glVertex2f(0.05, 1.5);
	glVertex2f(0.05, -1.5);
	glVertex2f(-0.05, -1.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(1.5, 0.05);
	glVertex2f(1.5, -0.05);
	glVertex2f(-1.5, -0.05);
	glVertex2f(-1.5, 0.05);
	glEnd();
}

//	creates the fuel bars, and takes in a Y, which changes the fuel level, to allow it to lessen with heli movement.
void fuel(float Y) {
	glLineWidth(3);
	glColor3f(1, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-2, 7);
	glVertex2f(2, 7);
	glVertex2f(2, -7);
	glVertex2f(-2, -7);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.3, 0.2, 0.2);
	glVertex2f(-2, Y);
	glVertex2f(2, Y);
	glVertex2f(2, -7);
	glVertex2f(-2, -7);
	glEnd();
}

//	draws the fuel of the helicopter, and fuel() takes in an X to set the X of the fuel bar, and fuelY for the level.
//	these are global variables that change.
void helicopter::drawFuel(float X) {
	glPushMatrix();
	glTranslatef(X, 75 + 100, 0.0);
	glScalef(5, 5, 1.0);
	fuel(fuelY);
	glPopMatrix();
}

//	draws the helicopter
void helicopter::drawHeli() {
	// adds a shadow to helicopter by drawing a smaller helicopter with a darker shadow and differing x and y,
	// thus giving the illusion of a shadow. If the plane is crashing, shadow is not drawn.
	glPushMatrix();
	glTranslatef(centre_x - 5, centre_y - 10, 0.0);
	glRotatef(rotate, 0.0, 0.0, 1.0);
	if (scaleV > 0.8)
		glScalef((length - 8) * scaleV, (length - 8) * scaleV, 1.0);
	glColor3f(0, 0, 0);
	helicop();
	glPopMatrix();

	// this draws the helicopter and the decorative glass. The centre is a variable so it changes with mouse location.
	// inside a push pop so it doesn't effect other draws. scaleV is a variable that effects the scale of the heli
	// while it is crashing, to mimick the plane falling. I am not changing value of length incase a bug appears.
	glPushMatrix();
	glTranslatef(centre_x, centre_y, 0.0);
	glRotatef(rotate, 0.0, 0.0, 1.0);
	glScalef(length * scaleV, length * scaleV, 1.0);
	glColor3f(0, 255, 0);
	helicop();
	helicopGlass();
	glPopMatrix();

	// this draws the blades of the helicopter, and a spin variable is added to it so the rotation of the blades changes
	// with each frame.
	glPushMatrix();
	glTranslatef(centre_x, centre_y, 0.0);
	glRotatef(rotate + spin, 0.0, 0.0, 1.0);
	glScalef(length * scaleV, length * scaleV, 1.0);
	glColor3f(0, 0, 255);
	heliblades();
	glPopMatrix();
}

//	this function is called when the plane crashes. credit to online sources for circle code.
//	http://programanddesign.com/cpp/draw-an-unfilled-circle/
void helicopter::smallFire(GLfloat x, GLfloat y, GLfloat r) {
	static const double inc = M_PI / 12;
	static const double max = 2 * M_PI;
	glBegin(GL_LINE_LOOP);
	for (double d = 0; d < max; d += inc) {
		glColor3f(255, 0, 0);
		glVertex2f(cos(d) * r + x, sin(d) * r + y);
	}
	glEnd();

	glColor3f(0, 0, 0);
	helicop();
}

//	draws the small fire at location of the helicopter (mimicking an explosion/fire start after crash)
void helicopter::drawSmallFire(void) {
	glPushMatrix();
	glLineWidth(10);
	glTranslatef(centre_x, centre_y, 0.0);
	glScalef(1, 1, 1.0);
	smallFire(1, 1, 5);
	glPopMatrix();
}

//	credit to Len from transformed chaser program!
float limit(float x, float min, float max) {
	if (x < min) {
		x = min;
	}
	if (x > max) {
		x = max;
	}
	return x;
}

void helicopter::startFuel(void) {
	moveAllowed = false;
	if (scaleV > 1.5 && fuelY < 6.3)
		scaleV -= 0.01;

	if (scaleV <= 1.5 && fuelY < 6.5) {
		spin = 0;
		rotate = 0;
		fuelY += 0.1;
	}

	if (fuelY >= 6.5) {
		if (scaleV < 2)
			scaleV += 0.01;

		if (scaleV >= 2) {
			moveAllowed = true;
			fuelAllowed = false;
		}

	}

	glutPostRedisplay();
}

//	credit to Len from transformed chaser program!
void helicopter::startMove() {
	// this adds 5 to rotate value each time start move is running, which equals to one rotation in roughly 1.5 seconds.
	// Once a full rotation is done (=365), then spin is reset to 0 to do another full rotation within a second.
	spin += 5;
	if (spin >= 365)
		spin = 0;

	float delta_x, delta_y, Vlength, step_size;

	// Compute vector from current location to mouse
	delta_x = mouse_x - centre_x;
	delta_y = mouse_y - centre_y;

	// Compute length of the vector
	Vlength = sqrt(delta_x * delta_x + delta_y * delta_y);

	// If the triangle is close to the mouse, then no motion is required.
	step_size = speed * delta_t / 1000.0;

	if (Vlength > step_size * 0.55) {
		// Normalise the delta vector and compute the step
		delta_x = delta_x / Vlength;
		delta_y = delta_y / Vlength;

		// Rotate the triangle to point to the mouse.
		// We could compute the rotation matrix directly from delta_x and delta_y,
		// but instead we use atan2 which computes the rotation angle in radians.
		float angle = atan2(delta_y, delta_x);

		// Convert radians to degrees.
		float degrees = angle * 180.0 / PI;

		// If the rotation is less than degrees (and not currently fuelling up), then
		// the rotation is not yet equal to the destination, so + by 1.5.
		if (rotate < degrees - 3 && !fuelAllowed)
			rotate += 1.5;
		// Same as before, but minuses from rotation if it is greater than degrees.
		else if (rotate > degrees + 3 && !fuelAllowed)
			rotate -= 1.5;
		// If the rotation is close enough to the degree, then it snaps the rotation
		// to the degree, then allows the heli to move. This allows the heli to first
		// rotate before heading towards the destination.
		// Each step decreases fuel levels by 0.025.
		else if ((rotate <= degrees + 3 or rotate >= degrees - 3)
				&& !fuelAllowed) {
			rotate = degrees;
			centre_x += delta_x * step_size;
			centre_y += delta_y * step_size;
			fuelY -= 0.025;
		}

		// This limits the fuel and scaleV, so that it doesn't overstep the normal values.
		// (Fuel wont go higher than the bar, heli scale wont go negative when shrinking).
		fuelY = limit(fuelY, -6.5, 7);
		scaleV = limit(scaleV, 0.0, 2.0);

		// Keep the triangle inside the world window.
		centre_x = limit(centre_x, 0.0 + length / 2, screenWidth - length / 2);
		centre_y = limit(centre_y, 0.0 + length / 2, screenHeight - length / 2);
	}

	glutPostRedisplay();
}

