/*
 * $Id: p5glove.h 63 2004-10-17 06:18:02Z jmcmullan $
 *
 *  Copyright (c) 2003 Jason McMullan <ezrec@hotmail.com>
 *
 *  USB P5 Data Glove support
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef P5GLOVE_H
#define P5GLOVE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct p5glove *P5Glove;

/* Button bitmasks */
#define P5GLOVE_BUTTON_A	1
#define P5GLOVE_BUTTON_B	2
#define P5GLOVE_BUTTON_C	4

/* IR Sensor Index */
#define P5GLOVE_IR_WRIST_TOP	0
#define P5GLOVE_IR_PINKY_R	1
#define P5GLOVE_IR_PINKY_L	2
#define P5GLOVE_IR_PALM		3
#define P5GLOVE_IR_INDEX	4
#define P5GLOVE_IR_THUMB_TOP	5
#define P5GLOVE_IR_WRIST_BOT	6
#define P5GLOVE_IR_THUMB_BOT	7

/* Finger Sensor Index */
#define P5GLOVE_FINGER_INDEX	0
#define P5GLOVE_FINGER_MIDDLE	1
#define P5GLOVE_FINGER_RING	2
#define P5GLOVE_FINGER_PINKY	3
#define P5GLOVE_FINGER_THUMB	4

/* p5glove_open:
 * Open a handle to the Nth P5 Glove. Returns NULL on error,
 * and sets errno appropriately.
 */
P5Glove p5glove_open(int glove_number);


/* p5glove_close:
 * Close an open handle to a P5 Glove.
 */
void p5glove_close(P5Glove glove);

#define P5GLOVE_DELTA_BUTTONS		0x01
#define P5GLOVE_DELTA_FINGERS		0x02
#define P5GLOVE_DELTA_POSITION		0x04
#define P5GLOVE_DELTA_ROTATION		0x08

/* p5glove_sample
 * Retrieve a sample from the P5, before 'timeout' milliseconds
 * (timeout of < 0 means forever)
 * Returns the 'delta mask' on success, -1 on error, and sets 
 * errno to EAGAIN if it is called faster than the refresh frequency.
 */
int p5glove_sample(P5Glove glove, int timeout);


/* Get information about the current glove state
 */

/* Reference LED positions (in meters)
 */
int p5glove_get_led(P5Glove glove,int led,double position[3]);

/* Button states 
 */
int p5glove_get_buttons(P5Glove glove,uint32_t *button_mask);

/* Finger flex
 * (0.0 - unclenched, 1.0 - clenched)
 */
int p5glove_get_finger(P5Glove glove,int finger,double *clench);

/* Current glove position (in meters)
 */
int p5glove_get_position(P5Glove glove,double position[3]);

/* Get Axis/Angle rotation information (angle is in degrees)
 */
int p5glove_get_rotation(P5Glove glove,double *angle,double axis[3]);


void p5glove_begin_calibration(P5Glove glove);
void p5glove_end_calibration(P5Glove glove);

int p5glove_get_mouse_mode(P5Glove glove);
void p5glove_mouse_mode_on(P5Glove glove);
void p5glove_mouse_mode_off(P5Glove glove);

/* Inline helpers */

/* Distance between two points */
static inline double p5glove_dist(const double a[3],const double b[3])
{
	double w = 0.0;
	int i;

	for (i = 0; i < 3; i++)
		w += (a[i]-b[i])*(a[i]-b[i]);

	return sqrt(w);
}

/* Cross product of two vectors */
static inline void p5glove_cross(const double v1[3],const double v2[3],double res[3])
{
	res[0]=v1[1]*v2[2]-v1[2]*v2[1];
	res[1]=v1[2]*v2[0]-v1[0]*v2[2];
	res[2]=v1[0]*v2[1]-v1[1]*v2[0];
}

/* Optimized for our use. We know the last row and last column are all zeros,
 * except for [3][3]=1.
 */
static inline void p5glove_vec_mat(const double vec[3],double mat[4][4],double res[3])
{
	int r,c;

	for (r=0; r < 3; r++) {
		res[r]=0.0;
		for (c=0; c < 3; c++)
			res[r] += vec[c]*mat[c][r];
	}
}

static inline double p5glove_dot(const double a[3],const double b[3])
{
	double res = 0.0;
	int i;

	for (i=0;i < 3; i++)
		res += a[i]*b[i];

	return res;
}

/* This is safe to call with v==res
 */
static inline void p5glove_normal(const double v[3],double res[3])
{
	double tmp=0.0;
	int i;

	for (i=0; i < 3; i++) {
		tmp += v[i]*v[i];
	}

	tmp=sqrt(tmp);

	for (i=0; i < 3; i++)
		res[i] = v[i] / tmp;
}

/* Get the plane normal of three points */
static inline void p5glove_plane(const double p1[3],const double p2[3],const double p3[3],double xyz[3])
{
	double v1[3],v2[3];

	v1[0]=p1[0]-p2[0];
	v1[1]=p1[1]-p2[1];
	v1[2]=p1[2]-p2[2];

	v2[0]=p3[0]-p2[0];
	v2[1]=p3[1]-p2[1];
	v2[2]=p3[2]-p2[2];

	p5glove_cross(v1,v2,xyz);

	if (xyz[0]==0.0 && xyz[1]==0.0 && xyz[2]==0.0)
		xyz[1]=1.0;

	p5glove_normal(xyz,xyz);
}

/* Angle of three points */
static inline double p5glove_angle(const double p1[3],const double p2[3],const double p3[3])
{
	double v1[3],v2[3];

	v1[0]=p1[0]-p2[0];
	v1[1]=p1[1]-p2[1];
	v1[2]=p1[2]-p2[2];

	v2[0]=p3[0]-p2[0];
	v2[1]=p3[1]-p2[1];
	v2[2]=p3[2]-p2[2];

	p5glove_normal(v1,v1);
	p5glove_normal(v2,v2);

	return acos(p5glove_dot(v1,v2));
}


#ifdef __cplusplus
}
#endif

#endif /* P5GLOVE_H */
