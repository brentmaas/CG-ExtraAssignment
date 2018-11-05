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
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "atlantis.hpp"
#include <iostream>
#include <vector>
#include <cfloat>

//Collision test between two fishes
int collisionTestSingle(fishRec * fish, fishRec * fish2){
	//Preliminary check; are the fish moving away from eachother
	float d = (fish->x - fish2->x) * (fish->x - fish2->x) + (fish->y - fish2->y) * (fish->y - fish2->y) + (fish->z - fish2->z) * (fish->z - fish2->z);
	float tx = fish->x + fish->speed * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	float ty = fish->y + fish->speed * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	float tz = fish->z + fish->speed * fish->v * sin(fish->theta / RAD);
	float d2 = (tx - fish2->x) * (tx - fish2->x) + (ty - fish2->y) * (ty - fish2->y) + (tz - fish2->z) * (tz - fish2->z);
	if(d2 >= d) return 0; //Don't collide if moving away from eachother
	
	//Defining untransformed hitboxes
	std::vector<std::vector<float>> fishColl = std::vector<std::vector<float>>(8);
	fishColl[0] = {fish->cx, fish->cy, fish->cz};
	fishColl[1] = {fish->cx + fish->cw, fish->cy, fish->cz};
	fishColl[2] = {fish->cx, fish->cy + fish->ch, fish->cz};
	fishColl[3] = {fish->cx + fish->cw, fish->cy + fish->ch, fish->cz};
	fishColl[4] = {fish->cx, fish->cy, fish->cz + fish->cd};
	fishColl[5] = {fish->cx + fish->cw, fish->cy, fish->cz + fish->cd};
	fishColl[6] = {fish->cx, fish->cy + fish->ch, fish->cz + fish->cd};
	fishColl[7] = {fish->cx + fish->cw, fish->cy + fish->ch, fish->cz + fish->cd};
	std::vector<std::vector<float>> fish2Coll = std::vector<std::vector<float>>(8);
	fish2Coll[0] = {fish2->cx, fish2->cy, fish2->cz};
	fish2Coll[1] = {fish2->cx + fish2->cw, fish2->cy, fish2->cz};
	fish2Coll[2] = {fish2->cx, fish2->cy + fish2->ch, fish2->cz};
	fish2Coll[3] = {fish2->cx + fish2->cw, fish2->cy + fish2->ch, fish2->cz};
	fish2Coll[4] = {fish2->cx, fish2->cy, fish2->cz + fish2->cd};
	fish2Coll[5] = {fish2->cx + fish2->cw, fish2->cy, fish2->cz + fish2->cd};
	fish2Coll[6] = {fish2->cx, fish2->cy + fish2->ch, fish2->cz + fish2->cd};
	fish2Coll[7] = {fish2->cx + fish2->cw, fish2->cy + fish2->ch, fish2->cz + fish2->cd};
	std::vector<std::vector<int>> planeIndices = std::vector<std::vector<int>>(6);
	planeIndices[0] = {0, 1, 3, 2};
	planeIndices[1] = {4, 5, 7, 6};
	planeIndices[2] = {0, 1, 5, 4};
	planeIndices[3] = {2, 3, 7, 6};
	planeIndices[4] = {0, 2, 6, 4};
	planeIndices[5] = {1, 3, 7, 5};
	
	//Translation
	for(int i = 0;i < 8;i++){
		fishColl[i][0] += fish->x;
		fishColl[i][1] += fish->y;
		fishColl[i][2] += fish->z;
		fish2Coll[i][0] += fish2->x;
		fish2Coll[i][1] += fish2->y;
		fish2Coll[i][2] += fish2->z;
	}
	
	//Psi rotation
	for(int i = 0;i < 8;i++){
		float xNew = fishColl[i][0] * cos(fish->psi * RRAD) - fishColl[i][2] * sin(fish->psi * RRAD);
		float zNew = fishColl[i][0] * sin(fish->psi * RRAD) + fishColl[i][2] * cos(fish->psi * RRAD);
		fishColl[i][0] = xNew;
		fishColl[i][2] = zNew;
		float xNew2 = fish2Coll[i][0] * cos(fish2->psi * RRAD) - fish2Coll[i][2] * sin(fish2->psi * RRAD);
		float zNew2 = fish2Coll[i][0] * sin(fish2->psi * RRAD) + fish2Coll[i][2] * cos(fish2->psi * RRAD);
		fish2Coll[i][0] = xNew2;
		fish2Coll[i][2] = zNew2;
	}
	
	//Theta rotation
	for(int i = 0;i < 8;i++){
		float xNew = fishColl[i][0] * cos(fish->theta * RRAD) - fishColl[i][1] * sin(fish->theta * RRAD);
		float yNew = fishColl[i][0] * sin(fish->theta * RRAD) + fishColl[i][1] * cos(fish->theta * RRAD);
		fishColl[i][0] = xNew;
		fishColl[i][1] = yNew;
		float xNew2 = fish2Coll[i][0] * cos(fish2->theta * RRAD) - fish2Coll[i][1] * sin(fish2->theta * RRAD);
		float yNew2 = fish2Coll[i][0] * sin(fish2->theta * RRAD) + fish2Coll[i][1] * cos(fish2->theta * RRAD);
		fish2Coll[i][0] = xNew2;
		fish2Coll[i][1] = yNew2;
	}
	
	//Phi rotation
	for(int i = 0;i < 8;i++){
		float yNew = fishColl[i][1] * cos(fish->phi * RRAD) + fishColl[i][2] * sin(fish->phi * RRAD);
		float zNew = -fishColl[i][1] * sin(fish->phi * RRAD) + fishColl[i][2] * cos(fish->phi * RRAD);
		fishColl[i][1] = yNew;
		fishColl[i][2] = zNew;
		float yNew2 = fish2Coll[i][1] * cos(fish2->phi * RRAD) + fish2Coll[i][2] * sin(fish2->phi * RRAD);
		float zNew2 = -fish2Coll[i][1] * sin(fish2->phi * RRAD) + fish2Coll[i][2] * cos(fish2->phi * RRAD);
		fish2Coll[i][1] = yNew2;
		fish2Coll[i][2] = zNew2;
	}
	
	//Determine centre of hitbox
	float Mx = 0, My = 0, Mz = 0;
	for(int i = 0;i < 8;i++){
		Mx += fishColl[i][0];
		My += fishColl[i][1];
		Mz += fishColl[i][2];
	}
	Mx /= 8;
	My /= 8;
	Mz /= 8;
	
	//Determine plane of hitbox between two centres
	float minMdm = FLT_MAX;
	float mx = 0, my = 0, mz = 0; //Vector from centre of hitbox to centre of hitplane
	int bestI = -1;
	for(int i = 0;i < 6;i++){
		float mx2 = 0, my2 = 0, mz2 = 0;
		for(int j = 0;j < 4;j++){
			mx2 += fishColl[planeIndices[i][j]][0];
			my2 += fishColl[planeIndices[i][j]][1];
			mz2 += fishColl[planeIndices[i][j]][2];
		}
		//The hitplane for which the dot product of the centre of the hitplane and the relative position
		//of the first fish relative to the second fish is minimal is the hitplane between the fishes
		float mdm = mx2 * (fish->x - fish2->x) + my2 * (fish->y - fish2->y) + mz2 * (fish->z - fish2->z);
		if(mdm < minMdm){
			minMdm = mdm;
			bestI = i;
			mx = mx2 / 4;
			my = my2 / 4;
			mz = mz2 / 4;
		}
	}
	float m = sqrt(mx * mx + my * my + mz * mz);
	
	//Checks
	//Dot product between hitplane centre vector and hitplane outwards normal vector
	//The plane will be described by nx * x + ny * y + nz * z = npm
	float npm = (mx * mx + my * my + mz * mz) / m;
	//If the position of the first fish is on one side of the plane, and all vertices of the second fish's
	//hitbox are on the other side of the plane, they are not colliding (separating axes theorem)
	if(mx * Mx + my * My + mz * Mz > npm){
		for(int i = 0;i < 8;i++)
			if((mx * fish2Coll[i][0] + my * fish2Coll[i][1] + mz * fish2Coll[i][2]) / m >= npm){
				//If one second hitbox vertex is on the same side of the plane as the first fish's centre
				//transform the vertex on the plane and check if it is within the plane's bounds (0 <= a,b <= 1)
				float px = fishColl[planeIndices[bestI][0]][0];
				float py = fishColl[planeIndices[bestI][0]][1];
				float pz = fishColl[planeIndices[bestI][0]][2];
				float ax = fishColl[planeIndices[bestI][1]][0] - px;
				float ay = fishColl[planeIndices[bestI][1]][1] - py;
				float az = fishColl[planeIndices[bestI][1]][2] - pz;
				float bx = fishColl[planeIndices[bestI][3]][0] - px;
				float by = fishColl[planeIndices[bestI][3]][1] - py;
				float bz = fishColl[planeIndices[bestI][3]][2] - pz;
				float la = sqrt(ax*ax + ay*ay + az*az);
				float lb = sqrt(bx*bx + by*by + bz*bz);
				float dvx = fish2Coll[i][0] - px;
				float dvy = fish2Coll[i][1] - py;
				float dvz = fish2Coll[i][2] - pz;
				float a = (dvx * ax + dvy * ay + dvz * az) / (la * la);
				float b = (dvx * bx + dvy * by + dvz * bz) / (lb * lb);
				if(a >= 0 && a <= 1 && b >= 0 && b <= 1) return 1; //The fishes collided
			}
	}else for(int i = 0;i < 8;i++)
		if((mx * fish2Coll[i][0] + my * fish2Coll[i][1] + mz * fish2Coll[i][2]) / m <= npm){
			//If one second hitbox vertex is on the same side of the plane as the first fish's centre
			//transform the vertex on the plane and check if it is within the plane's bounds (0 <= a,b <= 1)
			float px = fishColl[planeIndices[bestI][0]][0];
			float py = fishColl[planeIndices[bestI][0]][1];
			float pz = fishColl[planeIndices[bestI][0]][2];
			float ax = fishColl[planeIndices[bestI][1]][0] - px;
			float ay = fishColl[planeIndices[bestI][1]][1] - py;
			float az = fishColl[planeIndices[bestI][1]][2] - pz;
			float bx = fishColl[planeIndices[bestI][3]][0] - px;
			float by = fishColl[planeIndices[bestI][3]][1] - py;
			float bz = fishColl[planeIndices[bestI][3]][2] - pz;
			float la = sqrt(ax*ax + ay*ay + az*az);
			float lb = sqrt(bx*bx + by*by + bz*bz);
			float dvx = fish2Coll[i][0] - px;
			float dvy = fish2Coll[i][1] - py;
			float dvz = fish2Coll[i][2] - pz;
			float a = (dvx * ax + dvy * ay + dvz * az) / (la * la);
			float b = (dvx * bx + dvy * by + dvz * bz) / (lb * lb);
			if(a >= 0 && a <= 1 && b >= 0 && b <= 1) return 1; //The fishes collided
		}
	
	return 0; //The fishes did not collide
}

//Full collision of fish
bool collisionTest(fishRec * fish){
	int hits = 0;
	for(int i = 0;i < NUM_SHARKS;i++) hits += collisionTestSingle(fish, &sharks[i]);
	for(int i = 0;i < NUM_DOLPHS;i++) hits += collisionTestSingle(fish, &dolphs[i]);
	hits += collisionTestSingle(fish, &momWhale);
	hits += collisionTestSingle(fish, &babyWhale);
	return hits == 0; //If no hits have been detected, it has not collided
}

//Transform to fish's coordinates
void FishTransform(fishRec * fish) {
	glTranslatef(fish->y, fish->z, -fish->x);
	glRotatef(-fish->psi, 0.0, 1.0, 0.0);
	glRotatef(fish->theta, 1.0, 0.0, 0.0);
	glRotatef(-fish->phi, 0.0, 0.0, 1.0);
}

//Revert transform to fish's coordinates
void FishDetransform(fishRec * fish){
	glRotatef(fish->phi, 0.0, 0.0, 1.0);
	glRotatef(-fish->theta, 1.0, 0.0, 0.0);
	glRotatef(fish->psi, 0.0, 1.0, 0.0);
	glTranslatef(-fish->y, -fish->z, fish->x);
}

//Whale animation
void WhalePilot(fishRec * fish) {
	if(fish->pet){ //Dolphin only, if it is a pet
		float d2 = (fish->x - camX) * (fish->x - camX) + (fish->y - camY) * (fish->y - camY) + (fish->z - camZ) * (fish->z - camZ);
		//If within target range, stop transition
		if(fish->petTransition && d2 < DOLPHTARGETRADIUS * DOLPHTARGETRADIUS) fish->petTransition = 0;
		//If far outside target range, start transition again
		if(!fish->petTransition && d2 > 1.5 * DOLPHTARGETRADIUS * DOLPHTARGETRADIUS) fish->petTransition = 1;
		//If nearly at target radius, rotate to get into a circle
		if(fish->petTransition && d2 < 2 * DOLPHTARGETRADIUS * DOLPHTARGETRADIUS){
			fish->psi += 2;
			
			//Try to remain at the same level as the user
			if(abs(fish->z - camZ) < 100) fish->z = camZ;
			else if(fish->z - camZ > 100) fish->z -= 100;
			else fish->z += 100;
		}else if(fish->petTransition){ //If transitioning from far away, move right at the user
			//Target theta, psi
			float tTheta = -atan2(fish->z - camZ, sqrt((fish->x - camX)*(fish->x - camX) + (fish->y - camY)*(fish->y - camY))) * RAD;
			float tPsi = atan2(fish->y - camY, fish->x - camX) * RAD + 180;
			//Move to target theta
			if(abs(fish->theta - tTheta) < 1.0){
				fish->theta = 2 * tTheta;
			}else if(fish->theta - 2 * tTheta > 0.0){
				fish->theta -= 1.0;
			}else{
				fish->theta += 1.0;
			}
			//Move to target psi
			float dpsi = fish->psi - tPsi;
			while(dpsi < 0) dpsi += 360;
			if(abs(dpsi) < 1.0){
				fish->psi = tPsi;
			}else if(dpsi > 0.0){
				fish->psi -= 1.0;
			}else{
				fish->psi += 1.0;
			}
			//Rotate upright
			if(abs(fish->phi) < 1.0){
				fish->phi = 0;
			}else if(fish->phi > 0.0){
				fish->phi -= 1.0;
			}else{
				fish->phi += 1.0;
			}
		}else{ //If about at the target radius, move in circles
			if(abs(fish->phi - 20.0) < 1.0){
				fish->phi = 20.0;
			}else if(fish->phi - 20.0 > 0.0){
				fish->phi -= 1.0;
			}else{
				fish->phi += 1.0;
			}
			fish->psi -= 2;
			
			fish->phi++;
			
			//Try to remain at the same level as the user
			if(abs(fish->z - camZ) < 100) fish->z = camZ;
			else if(fish->z - camZ > 100) fish->z -= 100;
			else fish->z += 100;
			
			//If slightly outside the target radius, move more strongly inwards
			if(d2 > 1.1 * DOLPHTARGETRADIUS * DOLPHTARGETRADIUS) fish->psi -= 1;
			//If slightly inside the target radius, move less strongly inwards
			else if(d2 < 0.9 * DOLPHTARGETRADIUS * DOLPHTARGETRADIUS) fish->psi += 1;
		}
	}else{ //Original behaviour
		if(abs(fish->phi - 20.0) < 1.0){
			fish->phi = 20.0;
		}else if(fish->phi - 20.0 > 0.0){
			fish->phi -= 1.0;
		}else{
			fish->phi += 1.0;
		}
		fish->psi -= 0.5;
		
		fish->phi++;
	}
	
	fish->htail -= fish->v;
	
	//Move if collision allows it
	if(collisionTest(fish)){
		fish->x += WHALESPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->y += WHALESPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->z += WHALESPEED * fish->v * sin(fish->theta / RAD);
	}
	
	//Collision avoidance
	fishMiss(fish);
}

//Shark animation
void SharkPilot(fishRec * fish) {
	static int sign = 1;
	float X, Y, Z, tpsi, ttheta, thetal;
	
	fish->xt = 60000.0;
	fish->yt = 0.0;
	fish->zt = 0.0;
	
	X = fish->xt - fish->x;
	Y = fish->yt - fish->y;
	Z = fish->zt - fish->z;
	
	fish->htail -= 2.0 * fish->v;
	
	thetal = fish->theta;
	
	ttheta = RAD * atan(Z / (sqrt(X * X + Y * Y)));
	
	float dshark = (fish->z - camZ)*(fish->z - camZ) + (fish->y - camY)*(fish->y - camY) + (fish->x - camX)*(fish->x - camX);
	//If attacking user, but user is too far away, stop attacking without cooldown
	if(fish->attackUser && dshark > SHARKATTACKRANGE * SHARKATTACKRANGE){
		fish->attackUser = 0;
		fish->onAttackCooldown = 0;
	}
	//If close enough that attack is finished, stop attacking with cooldown
	if(fish->attackUser && dshark < SHARKDISENGAGERANGE * SHARKDISENGAGERANGE){
		fish->attackUser = 0;
		fish->onAttackCooldown = 1;
	}
	
	//If within attack range, but outside disengage range, 1 in 10 chance it will start attacking
	if(dshark < SHARKATTACKRANGE * SHARKATTACKRANGE && dshark > SHARKDISENGAGERANGE * SHARKDISENGAGERANGE && rand() % 10 == 0) fish->attackUser = 1;
	
	if(fish->attackUser){ //If attacking, move straight at the user
		float tTheta = -atan2(fish->z - camZ, sqrt((fish->x - camX)*(fish->x - camX) + (fish->y - camY)*(fish->y - camY))) * RAD;
		float tPsi = atan2(fish->y - camY, fish->x - camX) * RAD + 180;
		if(abs(fish->theta - tTheta) < 1.0){
			fish->theta = tTheta;
		}else if(fish->theta - tTheta > 0.0){
			fish->theta -= 1.0;
		}else{
			fish->theta += 1.0;
		}
		if(abs(fish->psi - tPsi) < 1.0){
			fish->psi = tPsi;
		}else if(fish->psi - tPsi > 0.0){
			fish->psi -= 1.0;
		}else{
			fish->psi += 1.0;
		}
	}else{ //Original behaviour
		if (ttheta > fish->theta + 0.25) {
			fish->theta += 0.5;
		} else if (ttheta < fish->theta - 0.25) {
			fish->theta -= 0.5;
		}
		if (fish->theta > 90.0) {
			fish->theta = 90.0;
		}
		if (fish->theta < -90.0) {
			fish->theta = -90.0;
		}
		fish->dtheta = fish->theta - thetal;
		
		fish->vtail += (fish->dtheta - fish->vtail) * 0.1;
		
		if (fish->vtail > 0.5) {
			fish->vtail = 0.5;
		} else if (fish->vtail < -0.5) {
			fish->vtail = -0.5;
		}
		
		tpsi = RAD * atan2(Y, X);
		
		fish->attack = 0;
		
		if (fabs(tpsi - fish->psi) < 10.0) {
			fish->attack = 1;
		} else if (fabs(tpsi - fish->psi) < 45.0) {
			if (fish->psi > tpsi) {
				fish->psi -= 0.5;
				if (fish->psi < -180.0) {
					fish->psi += 360.0;
				}
			} else if (fish->psi < tpsi) {
				fish->psi += 0.5;
				if (fish->psi > 180.0) {
					fish->psi -= 360.0;
				}
			}
		} else {
			if (rand() % 100 > 98) {
				sign = 1 - sign;
			}
			fish->psi += sign;
			if (fish->psi > 180.0) {
				fish->psi -= 360.0;
			}
			if (fish->psi < -180.0) {
				fish->psi += 360.0;
			}
		}
	
		if (fish->attack) {
			if (fish->v < 1.1) {
				fish->spurt = 1;
			}
			if (fish->spurt) {
				fish->v += 0.2;
			}
			if (fish->v > 5.0) {
				fish->spurt = 0;
			}
			if ((fish->v > 1.0) && (!fish->spurt)) {
				fish->v -= 0.2;
			}
		} else {
			if (!(rand() % 400) && (!fish->spurt)) {
				fish->spurt = 1;
			}
			if (fish->spurt) {
				fish->v += 0.05;
			}
			if (fish->v > 3.0) {
				fish->spurt = 0;
			}
			if ((fish->v > 1.0) && (!fish->spurt)) {
				fish->v -= 0.05;
			}
		}
		
		fishMiss(fish);
	}
	
	//Move if collision allows it
	if(collisionTest(fish)){
		fish->x += SHARKSPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->y += SHARKSPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->z += SHARKSPEED * fish->v * sin(fish->theta / RAD);
	}
}

//Collision avoidance between two fishes (adaptation of original SharkMiss
void fishMissSingle(fishRec * fish, fishRec * fish2){
	float avoid, thetal;
	float X, Y, Z, R;
	X = fish2->x - fish->x;
	Y = fish2->y - fish->y;
	Z = fish2->z - fish->z;
	
	R = sqrt(X*X + Y*Y + Z*Z);
	
	avoid = 1.0;
	thetal = fish->theta;
	
	if(R < fish->size){
		if(Z > 0.0) fish->theta -= avoid;
		else fish->theta += avoid;
	}else{
		fish->theta *= 0.95; //Theta decay to make sure whales don't make backflips at an increasing rate
	}
	fish->dtheta += fish->theta - thetal;
}

//Full collision avoidance of fish
void fishMiss(fishRec * fish){
	for(int i = 0;i < NUM_SHARKS;i++) fishMissSingle(fish, &sharks[i]);
	for(int i = 0;i < NUM_DOLPHS;i++) fishMissSingle(fish, &dolphs[i]);
	fishMissSingle(fish, &momWhale);
	fishMissSingle(fish, &babyWhale);
}
