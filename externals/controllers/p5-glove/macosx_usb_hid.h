/*
    Copyright (c) 2004 Tim Kreger <tkreger@bigpond.net.au>

    Macintosh OS X USB HID I/O routines

	Created by Tim Kreger on Wed Mar 17 2004.
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


#ifndef INCLUDED_WIN32_USB_HID_H
#define INCLUDED_WIN32_USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	these calls are roughly equivalent to those defined in win32_usb_hid.h
*/

typedef void* USBHIDHandle;

#define INVALID_USBHIDHANDLE_VALUE      (0)


USBHIDHandle OpenUSBHID( int index, int idVendor, int idProduct, int versionNumber, int flags );


void CloseUSBHID( USBHIDHandle usbHidHandle );


int ReadUSBHID( USBHIDHandle usbHidHandle, void *data, int count );


#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_WIN32_USB_HID_H */