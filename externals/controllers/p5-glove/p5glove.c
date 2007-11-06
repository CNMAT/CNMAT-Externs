/*
 * $Id: p5glove.c 67 2005-02-14 16:30:02Z jmcmullan $
 *
 *  Copyright (c) 2003 Jason McMullan <ezrec@hotmail.com>
 *  Windows patch (c) 2004 Ross Bencina <rossb@audiomulch.com>
 *  MacOSX patch (c) 2004 Tim Kreger <tkreger@bigpond.net.au>
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
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h> /* for calloc */
#include <stdint.h>
#include <string.h> /* for memcpy */
#include <math.h>

#include "usb_hid.h"
#include <errno.h>
#include "p5glove.h"

#define DEBUG
#ifdef DEBUG
#define DPRINTF(fmt,args...) fprintf(stderr,"p5: " fmt , ##args )
#else
#define DPRINTF(fmt,args...) do { } while (0)
#endif

struct p5glove {
	USBHID usb;

	char name[128];

	/* Calibration data from Report 12 & 6 */
	struct p5glove_cal {
		int	id;	/* Device id */
		struct {
			double	v;	/* in Radians */
			double	h;	/* in Radians */
		} head[2];
		double	head_dist;	/* in Meters */
		double  led[10][3];
	} cal;

	struct p5glove_data {
		struct timeval sampled;	/* Time sampled at */
		uint32_t valid;		/* Which data items are valid (mask) */

		int buttons;	/* Button bitmask */
		int finger[5];	/* Finger clench values (0-63) */

		/* Raw glove data */
		struct p5glove_ir {
			int visible;	/* Was the sensor visible? */
			int v1,v2,h;
		} ir[8];	/* IR Sensors values.  (-511 - 511) */

		/* Computed from p5g_process_sample 
		 */
		double position[3];	/* Position */

		/* Rotation information */
		struct {
			double axis[3];	/* Rotation axis (normalized) */
			double angle;	/* In degrees */
		} rotation;
	} data,prev;
};

static uint32_t get_bits(uint8_t *data,int pos,int len)
{
	int index = (pos>>3);
	uint32_t value = 0;

	pos &= 0x7;
	if (pos != 0) {
		value |= (data[index] & (0xff >> pos)) << (len-(8-pos));
		len -= (8-pos);
		index++;
	}

	for (; len >= 8; len-=8) {
		value |= data[index] << (len-8);
		index++;
	}

	if (len > 0)
		value |= (data[index] & (0xff << (8-len))) >> (8-len);

	return value;
}

static int32_t get_bits_signed(uint8_t *data,int pos,int len)
{
	uint32_t value,mask=0;

	value = get_bits(data,pos,len);
	if (value & (1 << (len-1)))
		mask = 0xffffffff << (len - 32);
	return value | mask;
}

/* Check a signed 10-bit number between -512 and 511 to see if it
 * might have overflown.
 * If it has overflowed, convert it to the range -615 to 615
 *
 * Code contributed by Carl Kenner <carl.kenner@gmail.com>
 */
static void fix_tan_overflow(int *value, int oldvalue)
{
	if (*value <= -409 && (oldvalue-*value) >= 670)
		*value += 1024;
	else if (*value >= 409 && (oldvalue-*value) <= -670)
		*value -= 1024;
}

static void p5g_unpack_sample(struct p5glove *p5, uint8_t data[24])
{
	unsigned char tmp[24];
	int visible=1;
	int i;

	/* Decode data.
	 * Format (nibbles from LSB to MSB)
	 *		 11111111111111112222222222222222
	 * 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF
	 * 01ddddddddBCCCCVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVxxx
	 *  0 1 2 3 4 5 6 7 8 9 A B C D E F 0 1 2 3 4 5 6 7
	 *				  1 1 1 1 1 1 1 1
	 *	  (bytes from LSB to MSB)
	 *
	 * d - Packed 6-bit bend sensor data (Index, Middle, Ring, Pinky, Thumb)
	 * B - Button data. A=1, B=2, C=4
	 * C - IR sensor report index
	 * V - Packed 30 bit signed IR info (10 bits X, 10 bits Y, 10 bits Z), x4
	 */

	if (data[0] != 1) {
printf("Odd. Sample [0]=%d\n",data[0]);
		return;
	}

	for (i=0; i < 5; i++) {
		int value;

		switch (i) {
			case 0: value = data[1] >> 2; break;
			case 1: value = ((data[1] & 0x3) << 4) | (data[2] >> 4); break;
			case 2: value = ((data[2] & 0xF) << 2) | (data[3] >> 6); break;
			case 3: value = (data[3] & 0x3F); break;
			case 4: value = data[4] >> 2; break;
			default: value = 0; break;
		}

		p5->data.finger[i]=get_bits(data,8+6*i,6);
	}

	p5->data.buttons=get_bits(data,40,4);

	/* Clear visibility */
	for (i=0 ; i <8; i++)
		p5->data.ir[i].visible=0;

	for (i=0; i < 4; i++) {
		int axis;
		int value;

		axis = get_bits(data,44+4*i,4);
		if (axis & 0x8)
			continue;

		p5->data.ir[axis].v2=get_bits_signed(data,60+(i*30),10);
		p5->data.ir[axis].v1=get_bits_signed(data,60+(i*30)+10,10);
		p5->data.ir[axis].h=get_bits_signed(data,60+(i*30)+20,10);
		p5->data.ir[axis].visible=visible++;

		/* The sensors generate values in (at least) the range
		 * -615 to 615, overflowing the -512 to 512 range.
		 */
		fix_tan_overflow(&p5->data.ir[axis].v2,p5->prev.ir[axis].v2);
		fix_tan_overflow(&p5->data.ir[axis].v1,p5->prev.ir[axis].v1);
		fix_tan_overflow(&p5->data.ir[axis].h,p5->prev.ir[axis].h);
	}
}

void p5g_process_led(P5Glove p5,int led,double pos[3])
{
	double v1,v2,h,d,zprime;

	v1 = atan(p5->data.ir[led].v1/512.0)+p5->cal.head[0].v;
	v2 = atan(p5->data.ir[led].v2/512.0)+p5->cal.head[1].v;
	h = atan(p5->data.ir[led].h/512.0);
	d = p5->cal.head_dist;

	zprime = d/(tan(v1)-tan(v2));

	pos[0]=-sin(h)*zprime;
	pos[1]=tan(v1)*zprime-d/2.0;	/* Places 0 in the center */
	pos[2]=cos(h)*zprime-d*2.0; /* A reasonable Z center */
}

int p5glove_reference_led(P5Glove p5, int led, double pos[3])
{
	if (led < 0 || led > 7)
		return -EINVAL;

	memcpy(pos,p5->cal.led[led],sizeof(double)*3);
	return 0;
}

#ifdef CHECK_ACUTE_TRIANGLES
static int p5glove_check_triangle(P5Glove p5,int led1,int led2,int led3)
{
	double c;
	int led[3];
	int i;
	
	led[0]=led1;
	led[1]=led2;
	led[2]=led3;

	for (i=0; i < 3; i++) {
		c=p5glove_angle(p5->cal.led[led[i]],
				p5->cal.led[led[(i+1)%3]],
				p5->cal.led[led[(i+2)%3]])*180.0/M_PI; 

		/* At too acute/obtune an angle? */
		DPRINTF("Angle %d-%d-%d is %.4lf\n",led[i],led[(i+1)%3],led[(i+2)%3],c);
		if (c < 7.0)
			return -EINVAL;
	}

	return 0;
}
#endif

/* Determine the 'best' leds to use form the data.
 * Return the # of good leds
 */
static int p5g_best_leds(P5Glove p5,int led[4],double pos[4][3])
{
	int leds;
	int i,j;
	int error[4]={0,0,0,0};

	for (i=0; i < 4; i++)
		led[i]=-1;

	/* Get the position from the 'best led' */
	for (i = 0 ; i < 8; i++)
		if (p5->data.ir[i].visible)
			led[p5->data.ir[i].visible-1]=i;

	/* Hmm. No 'best' led. 
	 */
	if (led[0] < 0)
		return -ENOENT;
	p5g_process_led(p5,led[0],pos[0]);

	if (led[1] < 0)
		return 1;
	p5g_process_led(p5,led[1],pos[1]);

	if (led[2] < 0)
		return 2;

	p5g_process_led(p5,led[2],pos[2]);

	/* If we have more than 2, we need to verify the data */
	if (led[3] < 0)
		leds=3;	
	else {
		p5g_process_led(p5,led[3],pos[3]);
		leds=4;
	}

	/* For each led-led pair, determine the error % versus the
	 * reference distances
	 */
	DPRINTF("Distances: %d - %d - %d - %d\n",led[0],led[1],led[2],led[3]);
	for (i=0; i < leds; i++) {
		for (j=i+1 ;j < leds; j++) {
			double ref_dist = p5glove_dist(p5->cal.led[led[i]],p5->cal.led[led[j]]);
			double dist = p5glove_dist(pos[i],pos[j]);
			double err = fabs(dist-ref_dist)/ref_dist;

			DPRINTF("%d - %d: %.4lf ",led[i],led[j],ref_dist);
			//DPRINTF("(%.4lf) [%.4lf]\n",led[i],led[j],dist,err);

			/* Max 9% error margin on distance */
			if (err > 0.20) {
				error[i]++;
				error[j]++;
			}
		}
	}

	/* Mark as dead anything with an error index of leds-1 */
	j=leds-1;
	for (i=0; i < leds; i++) {
		if (error[i] != j)
			continue;

		DPRINTF("Dead: Led %d\n",led[i]);
		leds--;
		if (i != leds) {
			memcpy(&error[i],&error[i+1],sizeof(int)*(leds-i));
			memcpy(&led[i],&led[i+1],sizeof(double)*(leds-i));
			memcpy(pos[i],pos[i+1],sizeof(double)*3*(leds-i));
		}
		i--;
		if (leds == 1)
			return leds;
	}

#ifdef CHECK_ACUTE_TRIANGLES
	/* Not enough left for rotation. Darn. */
	if (leds < 3)
		return leds;

	/* Check all triangles for obtusity */
	if (leds == 3) {
		int err;
		err=p5glove_check_triangle(p5,led[0],led[1],led[2]);
		if (err < 0)
			return 1;
		return 3;
	}

	/* Leds == 4 */
	for (i=0; i < leds; i++)
		error[i]=0;

	for (i=0; i < leds; i++) {
		int err;
		int l[3];

		err = p5glove_check_triangle(p5,led[i],led[(i+1)%4],led[(i+2)%4]);
		if (err < 0) {
			error[i]++;
			error[(i+1)%4]++;
			error[(i+2)%4]++;
		}
	}

	for (i=0; i < leds; i++)
		DPRINTF("%d: error=%d\n",led[i],error[i]);


	/* Mark as dead anything with an error index of leds-1 */
	j=leds-1;
	for (i=0; i < leds; i++) {
		if (error[i] != j)
			continue;

		DPRINTF("Dead: Led %d\n",led[i]);
		leds--;
		if (i != leds) {
			memcpy(&error[i],&error[i+1],sizeof(int)*(leds-i));
			memcpy(&led[i],&led[i+1],sizeof(double)*(leds-i));
			memcpy(pos[i],pos[i+1],sizeof(double)*3*(leds-i));
		}
		i--;
		if (leds == 1)
			return leds;
	}
#endif /* CHECK_ACUTE_TRIANGES */

	return leds;
}

/* Calculate delta, then store in 'prev'
 */
static int p5g_delta(P5Glove p5,int have_pos, int have_rot)
{
	int mask=0;
	int i;

	if (p5->prev.buttons != p5->data.buttons)
		mask |= P5GLOVE_DELTA_BUTTONS;

	for (i=0; i < 5; i++)
		if (p5->prev.finger[i] != p5->data.finger[i]) {
			mask |= P5GLOVE_DELTA_FINGERS;
			break;
		}

	if (have_pos && (! memcmp(p5->data.position,p5->prev.position,sizeof(double)*3)))
		have_pos=0;

	if (have_rot && (! memcmp(&p5->data.rotation,&p5->prev.rotation,sizeof(p5->data.rotation))))
		have_rot=0;

	memcpy(&p5->prev,&p5->data,sizeof(p5->data));
	return mask | 
		(have_pos ? P5GLOVE_DELTA_POSITION : 0) |
		(have_rot ? P5GLOVE_DELTA_ROTATION : 0);
}


static int p5g_process_sample(P5Glove p5)
{
	int i,j,leds;
	int led[4] = {-1,-1,-1,-1};
	double pos[4][3];
	double pos_plane[3],ref_plane[3];
	double rot_matrix[4][4];
	double c,s,t;
	double x,y,z;
	const double zero[3]={0.0,0.0,0.0};
	const double y_up[3]={0.0,1.0,0.0};

	/* Get the best leds from the data */
	leds=p5g_best_leds(p5,led,pos);
	if (leds < 1)
		return p5g_delta(p5,0,0);

	p5g_process_led(p5,led[0],pos[0]);

	/* Less than 3 leds. Can't calculate rotation info.
	 */
	if (leds < 3)
		return p5g_delta(p5,1,0);

	/* Calculate reference normal of the leds */
	p5glove_plane(p5->cal.led[led[0]],
		       p5->cal.led[led[1]],
		       p5->cal.led[led[2]],ref_plane);

	/* Calculate actual normal of the leds */
	p5glove_plane(pos[0],pos[1],pos[2],pos_plane);

	/* Calculate cos(theta) from the dot product of the ref and actual */
	c=p5glove_dot(ref_plane,pos_plane);
	p5->data.rotation.angle=acos(c)*180.0/M_PI;
	/* Calculate the rotation axis normal */
	p5glove_plane(ref_plane,zero,pos_plane,p5->data.rotation.axis);

DPRINTF("Position: %d - %d - %d\n",led[0],led[1],led[2]);
DPRINTF("Position: [%.4lf, %.4lf,%.4lf]\n",pos[0][0],pos[0][1],pos[0][2]);
DPRINTF("Reference normal: [%.4lf, %.4lf, %.4lf]\n",ref_plane[0],ref_plane[1],ref_plane[2]);
DPRINTF("Position  normal: [%.4lf, %.4lf, %.4lf]\n",pos_plane[0],pos_plane[1],pos_plane[2]);
DPRINTF("Reference angle:   %.4lf\n",p5glove_dot(ref_plane,y_up)*180.0/M_PI);
DPRINTF("Normal up angle:   %.4lf\n",p5glove_dot(pos_plane,y_up)*180.0/M_PI);
DPRINTF("Rotation angle:      %.4lf\n",p5->data.rotation.angle);
DPRINTF("Rotation axis :   [%.4lf, %.4lf, %.4lf]\n",p5->data.rotation.axis[0],p5->data.rotation.axis[1],p5->data.rotation.axis[2]);

	c=cos(p5->data.rotation.angle*M_PI/180.0);
	s=sin(p5->data.rotation.angle*M_PI/180.0);
	t=1.0-c;

	/* Calculate transformation matrix, in column major order */
	x=p5->data.rotation.axis[0];
	y=p5->data.rotation.axis[1];
	z=p5->data.rotation.axis[2];

	/* Column 0 */
	rot_matrix[0][0]=t*x*x+c;
	rot_matrix[0][1]=t*x*y-s*z;
	rot_matrix[0][2]=t*x*z+s*y;
	rot_matrix[0][3]=0;

	/* Column 1 */
	rot_matrix[1][0]=t*x*y+s*z;
	rot_matrix[1][1]=t*y*y+c;
	rot_matrix[1][2]=t*y*z-s*x;
	rot_matrix[1][3]=0;

	/* Column 2 */
	rot_matrix[2][0]=t*x*z-s*y;
	rot_matrix[2][1]=t*y*z+s*x;
	rot_matrix[2][2]=t*z*z+c;
	rot_matrix[2][3]=0;

	/* Column 3 */
	rot_matrix[3][0]=0;
	rot_matrix[3][1]=0;
	rot_matrix[3][2]=0;
	rot_matrix[3][3]=1;

	/* Now, apply the rotation angle to the position */
	p5glove_vec_mat(p5->cal.led[led[0]],rot_matrix,pos[1]);

	/* Then simply subtract the difference from the 
	 * reference position to get the *real* point */
	for (i=0; i<3; i++)
		p5->data.position[i] = pos[0][i] - pos[1][i];

	/* There! The position is corrected, the rotation matrix is complete - we're done! */
	return p5g_delta(p5,1,1);
}

static int p5g_parse_report6(struct p5glove *p5,int8_t *buff)
{
	/* We are given 32nds of a degree, so: */
#define REPORT6_TO_RAD(x)	((x)/32.0*M_PI/180.0)
	p5->cal.head[0].v += REPORT6_TO_RAD(buff[1]);
	p5->cal.head[0].h += REPORT6_TO_RAD(buff[2]);
	p5->cal.head[1].v += REPORT6_TO_RAD(buff[3]);
	p5->cal.head[1].h += REPORT6_TO_RAD(buff[4]);

	/* The following is in 10ths of an inch. Convert to meters */
#define REPORT6_TO_METERS(x)	((x)/393.70039)
	p5->cal.head_dist = REPORT6_TO_METERS(buff[5]);

	return 0;
}

static int p5g_parse_report12(struct p5glove *p5,uint8_t *buff)
{
	int led,axis;

	p5->cal.id = get_bits(buff,8,8);
	switch (p5->cal.id) {
		case 0:
			p5->cal.head[0].v +=  17.0 * (M_PI/180.0);
			p5->cal.head[1].v += -10.0 * (M_PI/180.0);
			break;
		default: fprintf(stderr,"libp5glove: unknown glove type %d\n",p5->cal.id);
			 return -EINVAL;
	}
	/* The following is in 100ths of an inch. Convert to meters */
#define REPORT12_TO_METERS(x)	((x)/3937.0039)
	for (led = 0; led < 10; led++) {
		for (axis = 0; axis < 3; axis++) {
			int16_t val;
			val = get_bits(buff,16+(48*led)+(16*axis),16);
			p5->cal.led[led][axis]=REPORT12_TO_METERS(val);
			if (axis == 0)
				p5->cal.led[led][axis] *= -1.0;
		}
	}

#ifdef DEBUG_CAL
for (led=0; led < 12; led++) {
	DPRINTF("%d %.4lf %.4lf %.4lf\n",led,
			p5->cal.led[led][0],
			p5->cal.led[led][1],
			p5->cal.led[led][2]);
}
#endif

	return 0;
}

static int p5glove_calibrate(struct p5glove *p5)
{
	int8_t report6_buff[6];
	int8_t report12_buff[255];
	int err;

	memset(&p5->cal,0,sizeof(p5->cal));
	report12_buff[0]=12;
	err=GetUSBHIDFeature(p5->usb,report12_buff,sizeof(report12_buff));
	if (err < 0) goto end;

	err=p5g_parse_report12(p5,report12_buff);
	if (err < 0) goto end;

	report6_buff[0]=6;
	err=GetUSBHIDFeature(p5->usb,report6_buff,sizeof(report6_buff));
	if (err < 0) goto end;

	err=p5g_parse_report6(p5,report6_buff);

#ifdef DEBUG_CALIB
DPRINTF("Cal results:\n");
{ int i; for (i=0;i<2;i++) {
DPRINTF("\tHead %d.v = %.2f deg\n",i,p5->cal.head[i].v * 180.0/M_PI);
DPRINTF("\tHead %d.h = %.2f deg\n",i,p5->cal.head[i].h * 180.0/M_PI);} }
DPRINTF("\tHead Distance = %.2f meters\n",p5->cal.head_dist);
#endif

end:
	return err;
}

/*********** Public API **************/

P5Glove p5glove_open(int glove_number)
{
	struct p5glove *p5 = NULL;
	USBHID usb;
	int err;
       
	usb = OpenUSBHID (
	    glove_number,				  /* nth matching device */
	    0x0d7f,					  /* vendor id */
	    0x0100,					  /* product id */
	    0,						  /* version number (not used) */
	    SELECT_VENDOR_ID_FLAG | SELECT_PRODUCT_ID_FLAG );   /* selection flags */
	if ( usb == INVALID_USBHID_VALUE )
		return NULL;

	p5=calloc(1,sizeof(*p5));
	p5->usb=usb;
	err = p5glove_calibrate(p5);
	if (err < 0) {
		if (err == -EBUSY) {
			fprintf(stderr,"libp5glove: Can't get feature reports.\n");
			fprintf(stderr,"libp5glove: If you have the 'usbmouse' module loaded, please unload it.\n");
		}
		free(p5);
		CloseUSBHID(usb);
		return NULL;
	}

	return p5;
}

void p5glove_close(P5Glove p5)
{
	if (p5->usb != NULL) {
		CloseUSBHID(p5->usb);
	}
	p5->usb=NULL;
	free(p5);
}

int p5glove_sample(P5Glove p5,int timeout)
{
	int err;
	uint8_t data[24];
	
	if ( ReadUSBHID( p5->usb, data, 24 ) == 24 && data[0]==1) {
		p5g_unpack_sample(p5, data);
		err=p5g_process_sample(p5);
	} else {
		errno = EACCES;
		err = -1;
	}
    
	return err;
}

int p5glove_get_buttons(P5Glove p5,uint32_t *buttons)
{
	*buttons = p5->data.buttons;
	return 0;
}

int p5glove_get_finger(P5Glove p5,int finger,double *clench)
{
	*clench = p5->data.finger[finger]/63.0;
	return 0;
}

int p5glove_get_position(P5Glove p5,double pos[3])
{
	memcpy(pos,p5->data.position,sizeof(double)*3);
	return 0;
}

int p5glove_get_rotation(P5Glove p5,double *angle,double axis[3])
{
	memcpy(axis,p5->data.rotation.axis,sizeof(double)*3);
	*angle = p5->data.rotation.angle;
	return 0;
}

void p5glove_begin_calibration(P5Glove p5)
{
    char report[2] = { 0x01, 0x01 };
    SetUSBHIDFeature( p5->usb, report, 2 );
}

void p5glove_end_calibration(P5Glove p5)
{
    char report[2] = { 0x01, 0x00 };
    SetUSBHIDFeature( p5->usb, report, 2 );
}

int p5glove_get_mouse_mode(P5Glove p5)
{
    char report[2] = { 0x05, 0x00 };
    GetUSBHIDFeature( p5->usb, report, 2 );

    return (report[1] == 0x01)? 1 : 0;
}

void p5glove_mouse_mode_on(P5Glove p5)
{
    char report[2] = { 0x05, 0x01 };
    SetUSBHIDFeature( p5->usb, report, 2 );
}

void p5glove_mouse_mode_off(P5Glove p5)
{
    char report[2] = { 0x05, 0xFF };
    SetUSBHIDFeature( p5->usb, report, 2 );
}

