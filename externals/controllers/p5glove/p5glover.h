/*
 * $Id: p5glove.h 36 2004-03-29 04:52:50Z rossb $
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
#define P5GLOVE_INDEX	0
#define P5GLOVE_MIDDLE	1
#define P5GLOVE_RING	2
#define P5GLOVE_PINKY	3
#define P5GLOVE_THUMB	4

struct p5glove_data {
	int buttons;	/* Button bitmask */
	int finger[5];	/* Finger clench values (0-63) */
	struct p5glove_ir {
		int visible;	/* Was the sensor visible? */
		int x,y,z;
	} ir[8];	/* IR Sensors values.  (-511 - 511) */
};


/* p5glove_open:
 * Open a handle to a P5 Glove. Returns NULL on error,
 * and sets errno appropriately.
 */
P5Glove p5glove_open(void);


/* p5glove_close:
 * Close an open handle to a P5 Glove.
 */
void p5glove_close(P5Glove glove);


/* p5glove_sample
 * Retrieve a sample from the P5
 * Returns 0 on success, -1 on error, and sets 
 * errno to EAGAIN is called faster then the refresh frequency.
 */
 
 void p5_matrix( int *x_, int *y_, int *z_ );
 
int p5glove_sample(P5Glove glove, struct p5glove_data *data);


/* p5glove_process_sample
 * Clean up raw glove data returned by p5glove_sample
 * Eliminates erroneous values, linearizes coordinates, etc.
 */
void p5glove_process_sample(P5Glove glove, struct p5glove_data *data);


void p5glove_begin_calibration(P5Glove glove);
void p5glove_end_calibration(P5Glove glove);

int p5glove_get_mouse_mode(P5Glove glove);
void p5glove_mouse_mode_on(P5Glove glove);
void p5glove_mouse_mode_off(P5Glove glove);



#ifdef __cplusplus
}
#endif

#endif /* P5GLOVE_H */
