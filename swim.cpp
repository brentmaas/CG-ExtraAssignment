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

/*int collisionSingle(fishRec * fish, float x, float y, float z, float theta, float psi, float r, float cz){
	float xf2 = fish->x + fish->speed * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	float yf2 = fish->y + fish->speed * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	float zf2 = fish->z + fish->speed * fish->v * sin(fish->theta / RAD);
	float czx = cz * sin(psi) * sin(theta);
	float czy = cz * cos(psi) * sin(theta);
	float czz = cz * cos(theta);
	float fczx = fish->cz * sin(fish->psi) * sin(fish->theta);
	float fczy = fish->cz * cos(fish->psi) * sin(fish->theta);
	float fczz = fish->cz * cos(fish->theta);
	//Collision spheres intersecting && distances between centers decreasing
	if((xf2 + fczx - x - czx) * (xf2 + fczx - x - czx) + (yf2 + fczy - y - czy) * (yf2 + fczy - y - czy)
			+ (zf2 + fczz - z - czz) * (zf2 + fczz - z - czz) <= (fish->cr + r) * (fish->cr + r)
			&& (fish->x + fczx - x - czx) * (fish->x + fczx - x - czx) + (fish->y + fczy - y - czy) * (fish->y + fczy - y - czy)
			+ (fish->z + fczz - z - czz) * (fish->z + fczz - z - czz) <= (xf2 + fczx - x - czx) * (xf2 + fczx - x - czx)
			+ (yf2 + fczy - y - czy) * (yf2 + fczy - y - czy) + (zf2 + fczz - z - czz) * (zf2 + czz - z - czz)) return 1;
	return 0;
}

bool collisionTest(fishRec * fish, float x, float y, float z){
	int hits = 0;
	for(int i = 0;i < NUM_SHARKS;i++){
		hits += collisionSingle(&sharks[i], x, y, z, fish->theta, fish->psi, fish->cr, fish->cz);
	}
	for(int i = 0;i < NUM_DOLPHS;i++){
		hits += collisionSingle(&dolphs[i], x, y, z, fish->theta, fish->psi, fish->cr, fish->cz);
	}
	hits += collisionSingle(&momWhale, x, y, z, fish->theta, fish->psi, fish->cr, fish->cz);
	hits += collisionSingle(&babyWhale, x, y, z, fish->theta, fish->psi, fish->cr, fish->cz);
	return hits > 0;
}*/

int collisionTestSingle(fishRec * fish, fishRec * fish2){
	//Preliminary check; are the fish moving away from eachother
	float d = (fish->x - fish2->x) * (fish->x - fish2->x) + (fish->y - fish2->y) * (fish->y - fish2->y) + (fish->z - fish2->z) * (fish->z - fish2->z);
	float tx = fish->x + fish->speed * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	float ty = fish->y + fish->speed * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	float tz = fish->z + fish->speed * fish->v * sin(fish->theta / RAD);
	//float tx2 = fish2->x + fish2->speed * fish2->v * cos(fish2->psi / RAD) * cos(fish2->theta / RAD);
	//float ty2 = fish2->y + fish2->speed * fish2->v * sin(fish2->psi / RAD) * cos(fish2->theta / RAD);
	//float tz2 = fish2->z + fish2->speed * fish2->v * sin(fish2->theta / RAD);
	//float d2 = (tx - tx2) * (tx - tx2) + (ty - ty2) * (ty - ty2) + (tz - tz2) * (tz - tz2);
	float d2 = (tx - fish2->x) * (tx - fish2->x) + (ty - fish2->y) * (ty - fish2->y) + (tz - fish2->z) * (tz - fish2->z);
	if(d2 >= d) return 0;
	
	//Defining untransformed boxes
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
	
	//Are fish far from eachother
	float Mx = 0, My = 0, Mz = 0, Mx2 = 0, My2 = 0, Mz2 = 0;
	for(int i = 0;i < 8;i++){
		Mx += fishColl[i][0];
		My += fishColl[i][1];
		Mz += fishColl[i][2];
		Mx2 += fish2Coll[i][0];
		My2 += fish2Coll[i][1];
		Mz2 += fish2Coll[i][2];
	}
	Mx /= 8;
	My /= 8;
	Mz /= 8;
	Mx2 /= 8;
	My2 /= 8;
	Mz2 /= 8;
	d = (Mx - Mx2) * (Mx - Mx2) + (My - My2) * (My - My2) + (Mz - Mz2) * (Mz - Mz2);
	d2 = 0;
	float d3 = 0;
	for(int i = 0;i < 8;i++){
		float a2 = (Mx - fishColl[7][0]) * (Mx - fishColl[7][0]) + (My - fishColl[7][1]) * (My - fishColl[7][1]) + (Mz - fishColl[7][2]) * (Mz - fishColl[7][2]);
		float a3 = (Mx2 - fish2Coll[7][0]) * (Mx2 - fish2Coll[7][0]) + (My2 - fish2Coll[7][1]) * (My2 - fish2Coll[7][1]) + (Mz2 - fish2Coll[7][2]) * (Mz2 - fish2Coll[7][2]);
		if(a2 > d2) d2 = a2;
		if(a3 > d3) d3 = a3;
	}
	if(sqrt(d) > sqrt(d2) + sqrt(d3)) return 0;
	
	//Determine mi
	float minMdm = FLT_MAX;
	float mx = 0, my = 0, mz = 0;
	int bestI = -1;
	for(int i = 0;i < 6;i++){
		float mx2 = 0, my2 = 0, mz2 = 0;
		for(int j = 0;j < 4;j++){
			mx2 += fishColl[planeIndices[i][j]][0];
			my2 += fishColl[planeIndices[i][j]][1];
			mz2 += fishColl[planeIndices[i][j]][2];
		}
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
	float npm = (mx * mx + my * my + mz * mz) / m;
	if(mx * Mx + my * My + mz * Mz > npm){
		for(int i = 0;i < 8;i++)
			if((mx * fish2Coll[i][0] + my * fish2Coll[i][1] + mz * fish2Coll[i][2]) / m >= npm){
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
				if(a >= 0 && a <= 1 && b >= 0 && b <= 1) return 1;
			}
	}else for(int i = 0;i < 8;i++)
		if((mx * fish2Coll[i][0] + my * fish2Coll[i][1] + mz * fish2Coll[i][2]) / m <= npm){
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
			if(a >= 0 && a <= 1 && b >= 0 && b <= 1) return 1;
		}
	
	return 0;
}

bool collisionTest(fishRec * fish){
	int hits = 0;
	for(int i = 0;i < NUM_SHARKS;i++) hits += collisionTestSingle(fish, &sharks[i]) * collisionTestSingle(&sharks[i], fish);
	for(int i = 0;i < NUM_DOLPHS;i++) hits += collisionTestSingle(fish, &dolphs[i]) * collisionTestSingle(&dolphs[i], fish);
	hits += collisionTestSingle(fish, &momWhale) * collisionTestSingle(&momWhale, fish);
	hits += collisionTestSingle(fish, &babyWhale) * collisionTestSingle(&babyWhale, fish);
	return hits == 0;
}

void FishTransform(fishRec * fish) {
	glTranslatef(fish->y, fish->z, -fish->x);
	glRotatef(-fish->psi, 0.0, 1.0, 0.0);
	glRotatef(fish->theta, 1.0, 0.0, 0.0);
	glRotatef(-fish->phi, 0.0, 0.0, 1.0);
}

void FishDetransform(fishRec * fish){
	glRotatef(fish->phi, 0.0, 0.0, 1.0);
	glRotatef(-fish->theta, 1.0, 0.0, 0.0);
	glRotatef(fish->psi, 0.0, 1.0, 0.0);
	glTranslatef(-fish->y, -fish->z, fish->x);
}

void WhalePilot(fishRec * fish) {
	fish->phi = -20.0;
	//fish->theta = 0.0;
	fish->psi -= 0.5;
	
	fish->htail -= fish->v;
	
	if(collisionTest(fish)){
		fish->x += WHALESPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->y += WHALESPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->z += WHALESPEED * fish->v * sin(fish->theta / RAD);
	}
	
	/*float tx = fish->x + WHALESPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	float ty = fish->y + WHALESPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	float tz = fish->z + WHALESPEED * fish->v * sin(fish->theta / RAD);
	
	if(!collisionTest(fish, tx, ty, tz)){
		fish->x = tx;
		fish->y = ty;
		fish->z = tz;
	}*/
}

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
	
	float dshark = (fish->z - camX)*(fish->z - camX) + (fish->y - camY)*(fish->y - camY) + (fish->x - camZ)*(fish->x - camZ);
	//std::cout << sqrt(dshark) << " " << SHARKATTACKRANGE << " " << SHARKDISENGAGERANGE;
	if(fish->attackUser && dshark > SHARKATTACKRANGE * SHARKATTACKRANGE && rand() % 10 == 0) fish->attackUser = 0;
	if(fish->attackUser && dshark < SHARKDISENGAGERANGE * SHARKDISENGAGERANGE) fish->attackUser = 0;
	
	if(dshark < SHARKATTACKRANGE * SHARKATTACKRANGE && dshark > SHARKDISENGAGERANGE * SHARKDISENGAGERANGE && rand() % 10 == 0) fish->attackUser = 1;
	
	if(fish->attackUser){
		//float tTheta = atan2(fish->y - camY, sqrt((fish->x - camX)*(fish->x - camX) + (fish->z - camZ)*(fish->z - camZ))) * RAD;
		//float tPsi = -atan2(fish->x - camZ, fish->z + camX) * RAD - 90;
		float tTheta = atan2(fish->y - camY, sqrt((fish->x - camZ)*(fish->x - camZ) + (fish->z - camX)*(fish->z - camX))) * RAD;
		float tPsi = atan2(fish->z - camX, fish->x - camZ) * RAD + 180;
		//std::cout << fish->theta << " " << tTheta << " " << fish->psi << " " << tPsi << std::endl;
		//if(abs(fish->theta - tTheta) < 1.0){
			fish->theta = tTheta;
		//}else if(fish->theta - tTheta > 0.0){
		//	fish->theta -= 1.0;
		//}else{
		//	fish->theta += 1.0;
		//}
		//if(abs(fish->psi - tPsi) < 1.0){
			fish->psi = tPsi;
		//}else if(fish->psi - tPsi > 0.0){
		//	fish->psi -= 1.0;
		//}else{
		//	fish->psi += 1.0;
		//}
	}else{
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
	}
	
	/*fish->x += SHARKSPEED * fish->v * cos(fish->psi / RAD)
			* cos(fish->theta / RAD);
	fish->y += SHARKSPEED * fish->v * sin(fish->psi / RAD)
			* cos(fish->theta / RAD);
	fish->z += SHARKSPEED * fish->v * sin(fish->theta / RAD);*/
	
	if(collisionTest(fish)){
		fish->x += SHARKSPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->y += SHARKSPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
		fish->z += SHARKSPEED * fish->v * sin(fish->theta / RAD);
	}
	
	/*float tx = fish->x + SHARKSPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	float ty = fish->y + SHARKSPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	float tz = fish->z + SHARKSPEED * fish->v * sin(fish->theta / RAD);
	
	if(!collisionTest(fish, tx, ty, tz)){
		fish->x = tx;
		fish->y = ty;
		fish->z = tz;
	}*//*else{
		fish->theta += 10 - 2 * (rand() % 10);
		fish->psi += 10 - 2 * (rand() % 10);
	}*/
	
	/*fish->x = camZ + 100000;
	fish->y = -camX;
	fish->z = -camY;*/
}

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
		fish->theta *= 0.95;
	}
	fish->dtheta += fish->theta - thetal;
}

void fishMiss(fishRec * fish){
	for(int i = 0;i < NUM_SHARKS;i++) fishMissSingle(fish, &sharks[i]);
	for(int i = 0;i < NUM_DOLPHS;i++) fishMissSingle(fish, &dolphs[i]);
	fishMissSingle(fish, &momWhale);
	fishMissSingle(fish, &babyWhale);
}

void SharkMiss(int i, int NUM_SHARKS) {
	float avoid, thetal;
	float X, Y, Z, R;
	
	for (int j = 0;j < NUM_SHARKS;j++) {
		if (j != i) {
			X = sharks[j].x - sharks[i].x;
			Y = sharks[j].y - sharks[i].y;
			Z = sharks[j].z - sharks[i].z;
			
			R = sqrt(X * X + Y * Y + Z * Z);
			
			avoid = 1.0;
			thetal = sharks[i].theta;
			
			if (R < SHARKSIZE) {
				if (Z > 0.0) {
					sharks[i].theta -= avoid;
				} else {
					sharks[i].theta += avoid;
				}
			}
			sharks[i].dtheta += (sharks[i].theta - thetal);
		}
	}
}
