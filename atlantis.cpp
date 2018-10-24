/* Copyright (c) Mark J. Kilgard, 1994. */

/**
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
#include "atlantis.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <vector>
#include <cfloat>

fishRec sharks[MAX_SHARKS];
fishRec momWhale;
fishRec babyWhale;
fishRec dolphs[NUM_DOLPHS];

GLboolean moving;

int NUM_SHARKS = MIN_SHARKS;

const float targetFPS = 60.0f;

float camX = 0, camY = 0, camZ = 0, camPhi = 0;

void InitSharkAt(int i){
	sharks[i].x = 70000.0 + rand() % 6000;
	sharks[i].y = rand() % 6000;
	sharks[i].z = rand() % 6000;
	sharks[i].psi = rand() % 360 - 180.0;
	sharks[i].v = (10.0 + rand() % 5) / 10.0;
	sharks[i].speed = SHARKSPEED;
	sharks[i].size = SHARKSIZE;
	sharks[i].cx = -1000;
	sharks[i].cy = -1000;
	sharks[i].cz = -2500;
	sharks[i].cw = 2000;
	sharks[i].ch = 2000;
	sharks[i].cd = 5000;
	/*sharks[i].cr = 2000;
	sharks[i].cz = 0;*/
}

void InitDolphAt(int i){
	dolphs[i].x = 30000.0 + rand() % 20000;
	dolphs[i].y = rand() % 10000;
	dolphs[i].z = 6000.0 + rand() % 20000;
	dolphs[i].psi = 90.0;
	dolphs[i].theta = 90.0;
	dolphs[i].v = (30.0 + rand() % 10) / 10.0;
	dolphs[i].speed = WHALESPEED;
	dolphs[i].size = DOLPHSIZE;
	dolphs[i].cx = -1500;
	dolphs[i].cy = -1500;
	dolphs[i].cz = -4000;
	dolphs[i].cw = 3000;
	dolphs[i].ch = 3000;
	dolphs[i].cd = 9000;
	/*dolphs[i].cr = 8000;
	dolphs[i].cz = 0;*/
}

void InitFishs(void) {
	for (int i = 0; i < NUM_SHARKS; i++) {
		InitSharkAt(i);
	}
	
	//Original dolphin
	dolphs[0].x = 30000.0;
	dolphs[0].y = 0;
	dolphs[0].z = 6000.0;
	dolphs[0].psi = 90.0;
	dolphs[0].theta = 90.0;
	dolphs[0].v = 3.0;
	dolphs[0].speed = WHALESPEED;
	dolphs[0].size = DOLPHSIZE;
	dolphs[0].cx = -1500;
	dolphs[0].cy = -1500;
	dolphs[0].cz = -4000;
	dolphs[0].cw = 3000;
	dolphs[0].ch = 3000;
	dolphs[0].cd = 9000;
	/*dolphs[0].cr = 8000;
	dolphs[0].cz = 0;*/
	
	//Other dolphins
	for(int i = 1;i < NUM_DOLPHS;i++){
		InitDolphAt(i);
	}
	
	momWhale.x = 70000.0;
	momWhale.y = 0.0;
	momWhale.z = 0.0;
	momWhale.psi = 90.0;
	momWhale.theta = 0.0;
	momWhale.v = 3.0;
	momWhale.speed = WHALESPEED;
	momWhale.size = MOMSIZE;
	momWhale.cx = -3500;
	momWhale.cy = -3500;
	momWhale.cz = -25000;
	momWhale.cw = 7000;
	momWhale.ch = 7000;
	momWhale.cd = 30000;
	/*momWhale.cr = 16000;
	momWhale.cz = -8000;*/
	
	babyWhale.x = 60000.0;
	babyWhale.y = -2000.0;
	babyWhale.z = -2000.0;
	babyWhale.psi = 90.0;
	babyWhale.theta = 0.0;
	babyWhale.v = 3.0;
	babyWhale.speed = WHALESPEED;
	babyWhale.size = BABYSIZE;
	babyWhale.cx = -1575;
	babyWhale.cy = -1575;
	babyWhale.cz = -7500;
	babyWhale.cw = 3150;
	babyWhale.ch = 3150;
	babyWhale.cd = 9000;
	/*babyWhale.cr = 5000;
	babyWhale.cz = -2500;*/
}

void Init(void) {
	static float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	static float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static float position[] = { 0.0, 1.0, 0.0, 0.0 };
	static float mat_shininess[] = { 90.0 };
	static float mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
	static float mat_diffuse[] = { 0.46, 0.66, 0.795, 1.0 };
	static float mat_ambient[] = { 0.0, 0.1, 0.2, 1.0 };
	static float lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	static float lmodel_localviewer[] = { 0.0 };
	
	glFrontFace(GL_CW);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_localviewer);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	
	InitFishs();
	
	glClearColor(0.0, 0.5, 0.9, 0.0);
}

void Reshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(400.0, (float) width / height, 1.0, 400000.0);
	glMatrixMode(GL_MODELVIEW);
}

void Animate(int value) {
	if(moving){
		for (int i = 0; i < NUM_SHARKS; i++) {
			SharkPilot(&sharks[i]);
		}
		for(int i = 0;i < NUM_DOLPHS;i++){
			WhalePilot(&dolphs[i]);
		}
		WhalePilot(&momWhale);
		WhalePilot(&babyWhale);
	}
	glutPostRedisplay();
	glutTimerFunc(1000.0f / targetFPS, &Animate, 0);
}

void Key(unsigned char key, int x, int y) {
	int newPet;
	float minD;
	switch (key) {
	case 27: /* Esc will quit */
		exit(0);
		break;
	case ' ': /* space will advance frame */
		if (!moving) Animate(0);
		break;
	case 'a':
		camZ += 5000;
		break;
	case 'f':
		newPet = -1;
		minD = FLT_MAX;
		for(int i = 0;i < NUM_DOLPHS;i++) if(dolphs[i].pet == 0){
			float ddolph = (dolphs[i].z - camX)*(dolphs[i].z - camX) + (dolphs[i].y - camY)*(dolphs[i].y - camY) + (dolphs[i].x - camZ)*(dolphs[i].x - camZ);
			if(ddolph < minD){
				newPet = i;
				minD = ddolph;
			}
		}
		if(newPet != -1){
			dolphs[newPet].pet = 1;
			dolphs[newPet].petTransition = 1;
		}
		break;
	case 'h':
		camX = 0;
		camY = 0;
		camZ = 0;
		camPhi = 0;
		break;
	case 'i':
		camX += 5000 * cos(-camPhi);
		camY -= 5000 * sin(-camPhi);
		break;
	case 'j':
		camPhi -= 15 * RRAD;
		break;
	case 'k':
		camX -= 5000 * cos(-camPhi);
		camY += 5000 * sin(-camPhi);
		break;
	case 'l':
		camPhi += 15 * RRAD;
		break;
	case 'r':
		for(int i = 0;i < NUM_DOLPHS;i++) dolphs[i].pet = 0;
		break;
	case 's':
		if(NUM_SHARKS < MAX_SHARKS){
			InitSharkAt(NUM_SHARKS);
			NUM_SHARKS++;
		}
		break;
	case 'x':
		if(NUM_SHARKS > MIN_SHARKS) NUM_SHARKS--;
		break;
	case 'z':
		camZ -= 5000;
		break;
	}
}

void drawBox(float x, float y, float z, float width, float height, float depth){
	glBegin(GL_QUADS);
	//bottom
	glNormal3f(0, -1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y, z);
	//top
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z);
	//left side
	glNormal3f(-1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	//right side
	glNormal3f(1, 0, 0);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	//front
	glNormal3f(0, 0, 1);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	//back
	glNormal3f(0, 0, -1);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y, z);
	glEnd();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(camPhi * RAD, 0.0, 1.0, 0.0);
	//glTranslatef(camX, camY, camZ);
	glTranslatef(-camY, -camZ, camX);
	
	for(int i = 0; i < NUM_SHARKS; i++) {
		FishTransform(&sharks[i]);
		/*glScalef(0.5f, 0.5f, 1.0f);
		glTranslatef(0, 0, sharks[i].cz);
		glutSolidSphere(sharks[i].cr, 10, 10);
		glTranslatef(0, 0, -sharks[i].cz);
		glScalef(2.0f, 2.0f, 1.0f);*/
		DrawShark(&sharks[i]);
		FishDetransform(&sharks[i]);
	}
	
	for(int i = 0;i < NUM_DOLPHS;i++){
		FishTransform(&dolphs[i]);
		/*glScalef(0.5f, 0.5f, 1.0f);
		glTranslatef(0, 0, dolphs[i].cz);
		glutSolidSphere(dolphs[i].cr, 10, 10);
		glTranslatef(0, 0, -dolphs[i].cz);
		glScalef(2.0f, 2.0f, 1.0f);*/
		DrawDolphin(&dolphs[i]);
		FishDetransform(&dolphs[i]);
	}
	
	FishTransform(&momWhale);
	/*glScalef(0.5f, 0.5f, 1.0f);
	glTranslatef(0, 0, momWhale.cz);
	glutSolidSphere(momWhale.cr, 10, 10);
	glTranslatef(0, 0, -momWhale.cz);
	glScalef(2.0f, 2.0f, 1.0f);*/
	DrawWhale(&momWhale);
	FishDetransform(&momWhale);
	
	FishTransform(&babyWhale);
	/*glScalef(0.5f, 0.5f, 1.0f);
	glTranslatef(0, 0, babyWhale.cz);
	glutSolidSphere(babyWhale.cr, 10, 10);
	glTranslatef(0, 0, -babyWhale.cz);
	glScalef(2.0f, 2.0f, 1.0f);*/
	glScalef(0.45, 0.45, 0.3);
	DrawWhale(&babyWhale);
	glScalef(1.0 / 0.45, 1.0 / 0.45, 1.0 / 0.3);
	FishDetransform(&babyWhale);
	
	//glTranslatef(-camX, -camY, -camZ);
	glTranslatef(camY, camZ, -camX);
	glRotatef(-camPhi * RAD, 0.0, 1.0, 0.0);
	
	glutSwapBuffers();
}

void menuSelect(int value) {
	switch (value) {
	case 1:
		moving = GL_TRUE;
		break;
	case 2:
		moving = GL_FALSE;
		break;
	case 3:
		exit(0);
		break;
	}
}

int main(int argc, char **argv) {
	glutInitWindowSize(1000, 500);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUT Atlantis Demo");
	Init();
	glutDisplayFunc(&Display);
	glutReshapeFunc(&Reshape);
	glutKeyboardFunc(&Key);
	moving = GL_TRUE;
	glutTimerFunc(1000.0f / targetFPS, &Animate, 0);
	glutCreateMenu(menuSelect);
	glutAddMenuEntry("Start motion", 1);
	glutAddMenuEntry("Stop motion", 2);
	glutAddMenuEntry("Quit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}
