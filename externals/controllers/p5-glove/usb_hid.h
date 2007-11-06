/*
    Copyright (c) 2004 Ross Bencina <rossb@audiomulch.com>
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
#ifndef USB_HID_H
#define USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct USBHID_struct *USBHID;

#define INVALID_USBHID_VALUE      (0)


#define SELECT_VENDOR_ID_FLAG           (0x01)
#define SELECT_PRODUCT_ID_FLAG          (0x02)
#define SELECT_VERSION_NUMBER_FLAG      (0x04)


/*
    Open the indexth USBHID device which match the delection criteria. The
    selection criteria may be any combination of vendor id, product id and
    version number as specified by the flags parameter.

    Returns an open handle on success, or INVALID_USBHIDHANDLE if an error
    occurred.
*/
USBHID OpenUSBHID( int index, int vendorId, int productId, int versionNumber, int flags );


/*
    Close a USB HID handle previously opened with OpenUSBHID()
*/
void CloseUSBHID( USBHID handle );


/*
    Read count bytes from the USB HID into dest. Returns the number of bytes
    read. This routine reads from the Kernel HID ring buffer, if it is no called
    frequently enough the data returned may lag behind the latest HID reports
    from the device.

    HidD_GetInputReport will read the most recent report, but is only
    implemented on Windows XP.
*/
int ReadUSBHID( USBHID handle, void *dest, int count );


int SetUSBHIDFeature( USBHID handle, char *report, int count );

int GetUSBHIDFeature( USBHID handle, char *report, int count );


#ifdef __cplusplus
}
#endif

#endif /* USB_HID_H */
