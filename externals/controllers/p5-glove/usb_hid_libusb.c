/*
    Copyright (c) 2004 Jason McMullan <ezrec@hotmail.com>

     USB/HID I/O routines
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

#include <stdio.h>
#include <errno.h>
#include <IOKit/usb/usb.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define USBHID_struct usb_dev_handle
#include "usb_hid.h"

static int timeout = 1000;

/*
    Open the indexth USBHID device which match the delection criteria. The
    selection criteria may be any combination of vendor id, product id and
    version number as specified by the flags parameter.

    Returns an open handle on success, or INVALID_USBHIDHANDLE if an error
    occurred.
*/
USBHID OpenUSBHID( int index, int vendorId, int productId, int versionNumber, int flags )
{
	struct usb_bus *bus;
	struct usb_device *dev;
	struct usb_dev_handle *usb = NULL;
	int err,i=0;

	usb_init();

	usb_find_busses();
	usb_find_devices();

	for (bus = usb_get_busses(); bus != NULL; bus = bus->next) {
		for (dev = bus->devices; dev != NULL; dev = dev->next) {
			if (dev->descriptor.idVendor != vendorId ||
			    dev->descriptor.idProduct != productId)
				continue;

			if (i < index)
				continue;

			usb = usb_open(dev);
			if (usb == NULL)
				continue;

			err=usb_claim_interface(usb,1);
			if (err < 0) {
				fprintf(stderr,"Can't claim P5 glove interface: %s\n",strerror(errno));
				usb_close(usb);
				return NULL;
			}

			break;
		}
		if (usb != NULL)
			break;
	}

	return usb;
}


/*
    Close a USB HID handle previously opened with OpenUSBHID()
*/
void CloseUSBHID( USBHID usb )
{
	usb_release_interface(usb,1);
	usb_close(usb);
}


/*
    Read count bytes from the USB HID into dest. Returns the number of bytes
    read. This routine reads from the Kernel HID ring buffer, if it is no called
    frequently enough the data returned may lag behind the latest HID reports
    from the device.

    HidD_GetInputReport will read the most recent report, but is only
    implemented on Windows XP.
*/
#ifndef HAVE_USB_INTERRUPT_READ
# define usb_read(usb,ep,data,len,tm)	usb_bulk_read(usb,ep,data,len,tm)
# define usb_clear_halt(usb,ep) usb_resetep(usb,ep)
#else
//# define usb_read(usb,ep,data,len,tm)	usb_get_input(usb,1,data,len)
# define usb_read(usb,ep,data,len,tm)	usb_interrupt_read(usb,ep,data,len,tm)
#endif

int ReadUSBHID( USBHID usb, void *dest, int count )
{
	return usb_read(usb, 0x81, dest, count, timeout);
}
	

int usb_get_input(USBHID udev, unsigned char *report, int ReportSize)
{
	int err;
retry:
	err=usb_control_msg(udev, 
                         USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                         0x01, /* HID_REPORT_GET */
                         (report[0]&0xf) | (0x01<<8), /* HID_REPORT_TYPE_INPUT */
                         1, report, ReportSize, timeout);
	if (err == -EPIPE)
		goto retry;
	return err;
}


int usb_set_output(USBHID udev, unsigned char *report, int ReportSize )
{
	return usb_control_msg(udev, 
                         USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                         0x09, /* HID_REPORT_SET = 0x09*/
                         (report[0]&0xf) | (0x02<<8), /* HID_REPORT_TYPE_OUPUT */
                         1, report, ReportSize, timeout);
}

int GetUSBHIDFeature( USBHID udev, char *report, int count )
{
	int err;
retry:
	err=usb_control_msg(udev, 
                         USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                         0x01, /* HID_REPORT_GET */
                         (report[0]&0xf) | (0x03<<8), /* HID_REPORT_TYPE_FEATURE */
                         1, report, count, timeout);
	if (err == -EPIPE)
		goto retry;
	return err;
}


int SetUSBHIDFeature( USBHID udev, char *report, int count )
{
	return usb_control_msg(udev, 
                         USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
                         0x09, /* HID_REPORT_SET = 0x09*/
                         (report[0]&0xf) | (0x03<<8), /* HID_REPORT_TYPE_FEATURE */
                         1, report, count, timeout);
}
