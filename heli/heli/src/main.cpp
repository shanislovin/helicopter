/*
 * main.cpp
 * Helicopter Animation Program
 *
 * Shanis Lovin 44857713
 */
#include <GL/glut.h>
// Math library for sqrt function
#include <math.h>
#include <iostream>
#include "helicopter.h"
#include "environment.h"
#include "sesbase.h"
using namespace std;

// Animation time step in msec. 60 frames per second.
# define ANIMATION_STEP (1000/60)

struct Globals {
	float bgc;
	int animating, startx, starty, endx, endy, menu;
	bool crash, clicked;
	//Calling other classes and setting them as objects to be usable.
	// There are two helicopters to satisfy a small feature.
	helicopter helicop;
	helicopter helicop2;
	environment enviro;
	sesbase ses;

} globals;

void init(void) {
	glClear( GL_COLOR_BUFFER_BIT);
	cout << "started\n";
	// Starting position of the second helicopter (outside of window, initially), setting the background colour
	globals.bgc = 0.3;
	globals.helicop2.centre_x = 600;
	globals.helicop2.mouse_x = 600;

	// Initial position of the rubber end lines
	globals.endx = 0;
	globals.endy = 0;

	// Initialising boolean variables
	globals.crash = false;
	globals.helicop.fuelAllowed = false;
	globals.helicop2.fuelAllowed = false;

	globals.animating = 1;

	glMatrixMode(GL_PROJECTION); // Set the projection matrix
	glLoadIdentity();            // Initialise to identity matrix
	gluOrtho2D(0.0, 1000.0, 0.0, 750.0); // Set to orthographic projection of window
}

/*
 * rubLine function
 Takes in an X and Y, and uses this as the start point of the rubber line.
 Line width is set to 5, and colour is set to pink!
 endx and endy are global variables that will change on mouse click later on.
 */
void rubLine(float centreX, float centreY) {
	glLineWidth(5);
	glColor3f(0.9, 0, 0.5);
	glBegin(GL_LINES);
	glVertex2f(centreX, centreY);
	glVertex2f(globals.endx, globals.endy);
	glEnd();
}

// Credit to Len's transformed chaser program; calculates if two points are nearby
int nearby(float x1, float y1, float x2, float y2, float distance) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < distance * distance;
}

/*
 drawRubber function
 If helicopter (1 or 2) is about to move, and it is not currently fueling up, a
 dynamic rubber line is drawn from its centre X and Y, to the current end points.
 */
void drawRubber(void) {
	if (globals.helicop.moveAllowed && !globals.helicop.fuelAllowed)
		rubLine(globals.helicop.centre_x, globals.helicop.centre_y);
	else if (globals.helicop2.moveAllowed && !globals.helicop2.fuelAllowed)
		rubLine(globals.helicop2.centre_x, globals.helicop2.centre_y);
}

/*
 fuelChecks function
 If helicopter (1 or 2) has a fuel level less than -6.5 (the y bottom of fuel image),
 the length (which effects the scale of the helicopter) lessens by 0.02, and rotates.
 Once it has reached less than 20, a small fire underneath is drawn, and the blades
 stop rotating.
 Also checks if helicopter is near the base, if so, it is allowed to fuel up.
 */
void fuelChecks(void) {
	if (globals.helicop.fuelY <= -6.5) {
		globals.helicop.moveAllowed = false;
		globals.helicop.mouse_x = globals.helicop.centre_x;
		globals.helicop.mouse_y = globals.helicop.centre_y;

		if (globals.helicop.scaleV > 0.7) {
			globals.helicop.scaleV -= 0.02;
			globals.helicop.rotate += 4;
		}
		if (globals.helicop.scaleV <= 0.7) {
			globals.helicop.spin = 0;
			globals.helicop.drawSmallFire();
		}
	}

	if (globals.helicop2.fuelY <= -6.5) {
		globals.helicop2.moveAllowed = false;
		globals.helicop2.mouse_x = globals.helicop2.centre_x;
		globals.helicop2.mouse_y = globals.helicop2.centre_y;

		if (globals.helicop2.scaleV > 0.7) {
			globals.helicop2.scaleV -= 0.02;
			globals.helicop2.rotate += 4;
		}
		if (globals.helicop2.scaleV <= 0.7) {
			globals.helicop2.spin = 0;
			globals.helicop2.drawSmallFire();
		}
	}
}

/*
 * 	crashCheck function
 * 		If both helicopters are nearby, and the length values are not yet 15 (crashed), then both
 * 		helicopters are set to not allow movement. The mouse x and y is set to the heli's x and y,
 * 		so that the heli can no longer continue their destination.
 * 		While the length is greater than 15, minus from it by 0.02, and rotate by 5.
 * 		This begins the crash animation of both heli's shrinking (falling) and spinning out of control.
 * 		Once it is small enough, the plane no longer rotates its blades, and a fire is drawn underneath.
 *
 */
void crashChecks(void) {
	// if both choppers are within too close vicinity, they both crash. As long as both are normal size (scaleV > 0) during this check,
	// the chopper wont crash if it comes into the other crashed chopper.
	if (globals.crash) {
		globals.helicop.moveAllowed = false;
		globals.helicop2.moveAllowed = false;
		globals.helicop.drawSmallFire();
		globals.helicop2.drawSmallFire();
	}

	// if one helicopter is refuelling, do not crash.
	if (nearby(globals.helicop2.centre_x, globals.helicop2.centre_y,
			globals.helicop.centre_x, globals.helicop.centre_y, 40)
			&& globals.helicop.scaleV > 0.0 && globals.helicop.scaleV > 0.0
			&& (!globals.helicop.fuelAllowed or !globals.helicop2.fuelAllowed)) {
		globals.helicop.moveAllowed = false;
		globals.helicop2.moveAllowed = false;
		globals.helicop.mouse_x = globals.helicop.centre_x;
		globals.helicop.mouse_y = globals.helicop.centre_y;
		globals.helicop2.mouse_x = globals.helicop2.centre_x;
		globals.helicop2.mouse_y = globals.helicop2.centre_y;

		if (globals.helicop.scaleV > 0.7) {
			globals.helicop.scaleV -= 0.02;
			globals.helicop2.scaleV -= 0.02;
			globals.helicop.rotate += 4;
			globals.helicop2.rotate += 4;
		}
		if (globals.helicop.scaleV <= 0.7) {
			globals.crash = true;
			globals.helicop.spin = 0;
			globals.helicop2.spin = 0;
		}
	}
}

void display(void) {
	// sets the screen colour to brown
	glClearColor(0.5, globals.bgc, 0.0, 0.0);
	glClear( GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draws the environment first (house, trees, base)
	globals.enviro.drawEnvi();
	globals.ses.drawBase();

	// runs these functions so that the animations appear under the heli
	crashChecks();
	fuelChecks();
	drawRubber();

	// draws both heli's
	globals.helicop.drawHeli();
	globals.helicop2.drawHeli();

	// draws the fuel bar
	globals.helicop.drawFuel(915);
	globals.helicop2.drawFuel(945);

	/* flush buffer */
	glFlush();

	/* glutSwapBuffers must be called for any drawing to
	 * appear on the screen when in double buffered mode
	 */
	glutSwapBuffers();
}

/* 	timer function
 * 		credit to Len;
 * 		Sets the time depending on delta, and then calls the individual heli's startMove function
 */
void timer(int v) {
	// Computing elapsed time for smooth animation.
	// The elapsed time is in msec since the call of glutInit.
	// Since it is an integer, the timer is limited.  A 32-bit integer
	// limits the timer to roughly 2 000 000 seconds which is 555 hours.
	// This code would exhibit a glitch if the code ran for longer than
	// roughly 555 hours.
	int time = glutGet(GLUT_ELAPSED_TIME); // In msec
	// Set up next timer event
	glutTimerFunc(ANIMATION_STEP, timer, time);
	if (globals.animating) {
		// if helicopter is allowed to fuel, start refueling.
		if (globals.helicop.fuelAllowed)
			globals.helicop.startFuel();
		if (globals.helicop2.fuelAllowed)
			globals.helicop2.startFuel();
		globals.helicop.delta_t = time - v;
		globals.helicop.startMove();
		globals.helicop2.delta_t = time - v;
		globals.helicop2.startMove();
	}
}

/*
 * 	mouse function
 * 		First sets the y coordinates to model coordinates. Background colour is reset to 0.3, as at the end
 * 		it changes to red if the user clicks outside of helicopter centre or base, to show the user that they
 * 		have to drag from helicopter.
 * 		If the left mouse is being pressed down, and if it is pressing on one of the helicopters, the heli is
 * 		allowed to move, and the rubber line sets its end points to be dynamic with there the mouse x and y is.
 * 		If the user lets go of the left mouse button (GLUT_UP/state is up), the heli's mouse x and y is changed
 * 		to the mouse's x and y, thus making the helicopter go towards that location (heli always goes towards
 * 		its own mouse x and y value).
 * 		Otherwise if clicking outside, as mentioned before, the screen will go red.
 */
void mouse(int button, int state, int x, int y) {
	int yc = 750 - y;
	globals.bgc = 0.3;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		if (nearby(x, yc, globals.helicop.centre_x, globals.helicop.centre_y,
				globals.helicop.length) && !globals.helicop.fuelAllowed) {
			globals.helicop.moveAllowed = true;
			globals.endx = x;
			globals.endy = yc;
		} else if (nearby(x, yc, globals.helicop2.centre_x,
				globals.helicop2.centre_y, globals.helicop2.length)
				&& !globals.helicop2.fuelAllowed) {
			globals.helicop2.moveAllowed = true;
			globals.endx = x;
			globals.endy = yc;
		}
	}

	if (globals.helicop.moveAllowed == true && (button == GLUT_LEFT_BUTTON)
			&& (state == GLUT_UP)) {
		globals.helicop.mouse_x = x;
		globals.helicop.mouse_y = yc;
		globals.helicop.moveAllowed = false;
	} else if (globals.helicop2.moveAllowed == true
			&& (button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		globals.helicop2.mouse_x = x;
		globals.helicop2.mouse_y = yc;
		globals.helicop2.moveAllowed = false;
	}

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0
			&& !nearby(x, yc, globals.helicop.mouse_x, globals.helicop.mouse_y,
					globals.helicop.length)
			&& !nearby(x, yc, globals.helicop2.mouse_x,
					globals.helicop2.mouse_y, globals.helicop2.length)
			&& !nearby(x, yc, globals.ses.baseX, globals.ses.baseY, 70))
		globals.bgc = 0.0;

	// if left mouse is pressed and helicopter is at the base, allow for refuelling.
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		if (nearby(globals.ses.baseX, globals.ses.baseY,
				globals.helicop.centre_x, globals.helicop.centre_y, 40)
				&& globals.helicop.fuelY < 6.0) {
			globals.helicop.fuelAllowed = true;
		} else
			globals.helicop.fuelAllowed = false;

		if (nearby(globals.ses.baseX, globals.ses.baseY,
				globals.helicop2.centre_x, globals.helicop2.centre_y, 40)
				&& globals.helicop2.fuelY < 6.0) {
			globals.helicop2.fuelAllowed = true;
		} else
			globals.helicop2.fuelAllowed = false;
	}

	glutPostRedisplay();
}

/*
 *	processMouseMotion function
 *		Credit to Len's rubber line program; it follows the mouse's motion
 *		and updates the line to redraw/redisplay.
 */
void processMouseMotion(int x, int yc) {
	// Whenever the mouse moves, update the second point of rectangle
	globals.endx = x;
	globals.endy = 750 - yc;

	glutPostRedisplay();
}

/*
 * reset function
 * 		If reset is called, length (scale of heli), helicop centres, mouse x and y,
 * 		fuel level and rubber line end points are reset to the initial value, thus
 * 		resetting the whole game/animations.
 */
void reset() {
	globals.helicop.scaleV = 2;
	globals.helicop2.scaleV = 2;

	globals.helicop.rotate = 0;
	globals.helicop2.rotate = 0;

	globals.helicop.centre_x = 400;
	globals.helicop.centre_y = 375;
	globals.helicop.mouse_x = 400;
	globals.helicop.mouse_y = 375;

	globals.helicop2.centre_x = 600;
	globals.helicop2.mouse_x = 600;
	globals.helicop2.centre_y = 375;
	globals.helicop2.mouse_y = 375;

	globals.helicop.fuelY = 7;
	globals.helicop2.fuelY = 7;

	globals.endx = 0;
	globals.endy = 0;

	globals.helicop.fuelAllowed = false;
	globals.helicop2.fuelAllowed = false;
	globals.crash = false;

	globals.animating = 1;
}

/*
 * 	keys function
 * 		Credit to Len's viewport2d progam for all menu related functions!
 * 		If the user presses 1, the second heli will disappear. If the user
 * 		presses 2, the second heli will appear in its initial position.
 * 		The program starts with two helis.
 * 		If user presses key q, the program will quit, if r, the program
 * 		will reset all of its values and animations to the inital values.
 */
void keys(unsigned char key, int _ignore_x, int _ignore_y) {
	if (key == '1') {
		globals.helicop2.centre_x = 2000;
		globals.helicop2.centre_y = 1000;
		globals.helicop2.mouse_x = 2000;
		globals.helicop2.mouse_y = 1000;
	}
	if (key == '2') {
		globals.helicop2.centre_x = 600;
		globals.helicop2.centre_y = 375;
		globals.helicop2.mouse_x = 600;
		globals.helicop2.mouse_y = 375;
		globals.helicop2.fuelY = 7;
		globals.helicop2.rotate = 0;
	}
	if (key == 'q')
		exit(0);
	if (key == 'r')
		reset();
	glutPostRedisplay();
}

/*
 * 	menu function
 * 		Creates menu and sets its input parameters to relate to the key functions.
 */
void menu(int item) {
	keys(item, -1, -1);
}

/*
 * 	makeMenu function
 * 		Adds a menu to the program, and adds entries to the menu that guide the
 * 		user on what buttons to press, and what they do.
 */
void makeMenu(void) {
	globals.menu = glutCreateMenu(menu);
	glutAddMenuEntry("1:    One helicopter", '1');
	glutAddMenuEntry("2:    Two helicopters", '2');
	glutAddMenuEntry("q:    Quit", 'q');
	glutAddMenuEntry("r:    Reset all", 'r');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// sets window size
	glutInitWindowSize(1000, 750);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Helicopter Program 330");
	glutDisplayFunc(display);
	glutTimerFunc(ANIMATION_STEP, timer, 0);
	// register mouse functions with my mouse function
	glutMouseFunc(mouse);
	// register key callbacks with my keys function
	glutKeyboardFunc(keys);
	// registers mouse motion with my processMouseMotion function
	glutMotionFunc(processMouseMotion);
	// makes the menu
	makeMenu();
	// initialises values and program
	init();
	glutMainLoop();
	return 0;
}
