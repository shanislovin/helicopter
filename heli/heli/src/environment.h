/*
 * environment.h
 *
 *  Created on: Apr 19, 2018
 *      Author: 44857713
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

// This creates a new environment class that may be used as an object. It allows use within other
// classes if the header is called. The functions are declared here, but they are
// initialised within the cpp file.
class environment {
public:
	environment();
	virtual ~environment();
	void fire(GLfloat x, GLfloat y, GLfloat r);
	void drawEnvi();
};

#endif /* ENVIRONMENT_H_ */
