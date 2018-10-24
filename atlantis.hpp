#ifndef ATLANTIS_HPP
#define ATLANTIS_HPP

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
const float RAD = 57.295;
const float RRAD = 0.01745;
const int NUM_DOLPHS = 6;
const int MIN_SHARKS = 4;
const int MAX_SHARKS = 32;
extern int NUM_SHARKS;
const float SHARKSIZE = 6000;
const float DOLPHSIZE = 10400;
const float MOMSIZE = 44000;
const float BABYSIZE = 13200;
const float SHARKSPEED = 100.0;
const float WHALESPEED = 250.0;
const float SHARKATTACKRANGE = 50000;
const float SHARKDISENGAGERANGE = 5000;
const float DOLPHTARGETRADIUS = 30000;

extern float camX, camY, camZ, camPhi;

typedef struct _fishRec {
    float x, y, z, phi, theta, psi, v, speed, size;
    float xt, yt, zt;
    float htail, vtail;
    float dtheta;
    int spurt, attack, pet = 0, petTransition = 0, attackUser = 0, onAttackCooldown = 0;
    
    //Collision box
    float cx, cy, cz, cw, ch, cd;
    //Collision sphere
    //float cr, cz;
} fishRec;

extern fishRec sharks[MAX_SHARKS];
extern fishRec momWhale;
extern fishRec babyWhale;
extern fishRec dolphs[NUM_DOLPHS];

extern void FishTransform(fishRec *);
extern void FishDetransform(fishRec *);
extern void WhalePilot(fishRec *);
extern void SharkPilot(fishRec *);
extern void fishMiss(fishRec *);
extern void SharkMiss(int, int);
extern void DrawWhale(fishRec *);
extern void DrawShark(fishRec *);
extern void DrawDolphin(fishRec *);

#endif