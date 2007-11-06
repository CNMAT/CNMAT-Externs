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



#include <stdio.h>
#include <math.h>
#include <stdlib.h>


// Mac Headers

#include <mach/mach.h>
#include <CoreFoundation/CFNumber.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>


#include "macosx_usb_hid.h"



USBHIDHandle OpenUSBHID( int index, int idVendor, int idProduct, int versionNumber, int flags )
{
    // todo: index and flags are currently ignored, see win32 for example implementation.
    
    IOUSBDeviceInterface 	**dev;
    IOCFPlugInInterface 	**iodev;
    kern_return_t		err;
    CFMutableDictionaryRef 	matchingDictionary = 0;		// requires <IOKit/IOKitLib.h>
    CFNumberRef			numberRef;
    UInt8 			inPipeRef;
    io_service_t		usbDeviceRef;
    io_iterator_t 		iterator = 0;
    
    SInt32 				score;
    
    UInt8				numConf;
    IOUSBConfigurationDescriptorPtr	confDesc;
    io_service_t			usbInterfaceRef;
    IOUSBFindInterfaceRequest		interfaceRequest;
    IOUSBInterfaceInterface183 		**intf = 0;
    UInt8				numPipes;
    UInt8				direction, number, transferType, interval;
    int					i;
    UInt16				maxPacketSize;
    
	mach_port_t 	masterPort;				// requires <mach/mach.h>


    err = IOMasterPort(MACH_PORT_NULL, &masterPort);				
    if (err)
    {
        fprintf( stderr, "USBSimpleExample: could not create master port, err = %08x\n", err);
        return 0;
    }
    
    // configure matching dictionary
    
    matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);	// requires <IOKit/usb/IOUSBLib.h>
    if (!matchingDictionary)
    {
        fprintf( stderr, "USBSimpleExample: could not create matching dictionary\n");
        return 0;
    }
    
    if( 1 ){ //todo: use flag to decide whether to add vendor id
    
        numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &idVendor);
        if (!numberRef)
        {
            fprintf( stderr, "USBSimpleExample: could not create CFNumberRef for vendor\n");
            return 0;
        }
        
        CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBVendorID), numberRef);
        CFRelease(numberRef);
        numberRef = 0;
    }
    
    if( 1 ){ // todo: use flag to decide whether to add product id
        numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &idProduct);
        if (!numberRef)
        {
            fprintf( stderr, "USBSimpleExample: could not create CFNumberRef for product\n");
            return 0;
        }
        
        CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBProductID), numberRef);
        CFRelease(numberRef);
        numberRef = 0;
    }
    
    // get iterator to matching devices
    
    err = IOServiceGetMatchingServices(masterPort, matchingDictionary, &iterator);
    matchingDictionary = 0;
    
    // get first matching device
    
   if(usbDeviceRef = IOIteratorNext(iterator))
   {
        // open device
   
        err = IOCreatePlugInInterfaceForService(usbDeviceRef, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, 						&iodev, &score);
        if (err || !iodev)
        {
            fprintf( stderr, "dealWithDevice: unable to create plugin. ret = %08x, iodev = %p\n", err, iodev);
            return 0;
        }
        
         err = (*iodev)->QueryInterface(iodev, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID)&dev);
        (*iodev)->Release(iodev);				// done with this
        if (err || !dev)
        {
            fprintf( stderr, "dealWithDevice: unable to create a device interface. ret = %08x, dev = %p\n", err, dev);
            return 0;
        }

        err = (*dev)->USBDeviceOpen(dev);
        if (err)
        {
            fprintf( stderr, "dealWithDevice: unable to open device. ret = %08x\n", err);
            return 0;
        }
        
        err = (*dev)->GetNumberOfConfigurations(dev, &numConf);
        if (err || !numConf)
        {
            fprintf( stderr, "dealWithDevice: unable to obtain the number of configurations. ret = %08x\n", err);
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            return 0;
        }

        fprintf( stderr, "dealWithDevice: found %d configurations\n", numConf);
        
        err = (*dev)->GetConfigurationDescriptorPtr(dev, 0, &confDesc);			// get the first config desc (index 0)
        if (err)
        {
            fprintf( stderr, "dealWithDevice:unable to get config descriptor for index 0\n");
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            return 0;
        }
        
        err = (*dev)->SetConfiguration(dev, confDesc->bConfigurationValue);
        if (err)
        {
            fprintf( stderr, "dealWithDevice: unable to set the configuration\n");
            (*dev)->USBDeviceClose(dev);
            (*dev)->Release(dev);
            return 0;
        }
        
    }else{
        return intf;
    }
    
    // set criteria for interface iterator

    interfaceRequest.bInterfaceClass = 3;		// requested class (3==hid class)
    interfaceRequest.bInterfaceSubClass = 0;		// requested subclass
    interfaceRequest.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;		// requested protocol
    interfaceRequest.bAlternateSetting = kIOUSBFindInterfaceDontCare;		// requested alt setting
    
    err = (*dev)->CreateInterfaceIterator(dev, &interfaceRequest, &iterator);
    if (err)
    {
        fprintf( stderr, "dealWithDevice: unable to create interface iterator\n");
        (*dev)->USBDeviceClose(dev);
        (*dev)->Release(dev);
        return 0;
    }
    
    // get first interface matching criteria
    
    if( (usbInterfaceRef = IOIteratorNext(iterator)) )
    {
        // open interface
        
        fprintf( stderr, "found interface: %p\n", (void*)usbInterfaceRef);
                
        err = IOCreatePlugInInterfaceForService(usbInterfaceRef, kIOUSBInterfaceUserClientTypeID, kIOCFPlugInInterfaceID, &iodev, &score);
        if (err || !iodev)
        {
            fprintf( stderr, "dealWithInterface: unable to create plugin. ret = %08x, iodev = %p\n", err, iodev);
            return 0;
        }
        
        err = (*iodev)->QueryInterface(iodev, CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID183), (LPVOID)&intf);
        (*iodev)->Release(iodev);				// done with this
        if (err || !intf)
        {
            fprintf( stderr, "dealWithInterface: unable to create a device interface. ret = %08x, intf = %p\n", err, intf);
            return 0;
        }
    
        err = (*intf)->USBInterfaceOpenSeize(intf);
        if (err)
        {
            (*intf)->Release( intf );
            fprintf( stderr, "dealWithInterface: unable to open interface. ret = %08x, score = %d \n", err, (int)score);
            return 0;
        }

    }else{
        return 0;
    }  
        
    
    err = (*intf)->GetNumEndpoints(intf, &numPipes);
    if (err)
    {
        fprintf( stderr, "dealWithInterface: unable to get number of endpoints. ret = %08x\n", err);
        (*intf)->USBInterfaceClose(intf);
        (*intf)->Release(intf);
        return 0;
    }
    
    fprintf( stderr, "dealWithInterface: found %d pipes\n", numPipes);
    
    // find pipe with transfer type kUSBInterrupt and direction kUSBIn
    // store pipe number in inPipeRef
    
    for (i=1; i <= numPipes; i++)
    {
        err = (*intf)->GetPipeProperties(intf, i, &direction, &number, &transferType, &maxPacketSize, &interval);
        if (err)
        {
            (*intf)->USBInterfaceClose(intf);
            (*intf)->Release(intf);
            fprintf( stderr, "dealWithPipes: unable to get pipe properties for pipe %d, err = %08x\n", i, err);
            return 0;
        }
        
        if (transferType != kUSBInterrupt)
        {
            fprintf( stderr, "dealWithPipes: skipping pipe %d because it is not an interupt pipe\n", i);
            fprintf( stderr, "Pipe is type %d\n", transferType);
            
            continue;
        }
        
        if (direction == kUSBIn)
        {
            fprintf( stderr, "dealWithPipes: grabbing BULK IN pipe index %d, number %d, inteveral %d \n",i, number, interval);
            inPipeRef = i;
        }
    }

    return intf;
}




void CloseUSBHID( USBHIDHandle usbHidHandle )
{
    IOUSBInterfaceInterface183 **intf = usbHidHandle;
    
    (*intf)->USBInterfaceClose(intf);
    (*intf)->Release(intf);
}



int ReadUSBHID( USBHIDHandle usbHidHandle, void *data, int count )
{
    IOUSBInterfaceInterface183 **intf = usbHidHandle;
    UInt32 readSize = count;
    
    (*intf)->ReadPipe(intf, 1,  data, &readSize ); // 1 should be inPipeRef
    
    return readSize;
}


int HIrequest(void *usbHidHandle, UInt8 RequestType, UInt16 Request, UInt16 ReportId, int index, unsigned char *raw_buf, int ReportSize )
{
    IOUSBDevRequest 			request;
    IOUSBInterfaceInterface183 	**intf = usbHidHandle;
    IOUSBDeviceInterface 		**device;
    io_service_t				usbDeviceRef;
    IOReturn 					success = 0 ;
    char 						test[2];

    // 0x21 for set, 0xA1for get
    request.bmRequestType = RequestType;
    request.bRequest = Request;
    request.wValue = ReportId;
    request.wIndex = index;
    request.wLength = ReportSize;
    request.pData = raw_buf;

  //   (*theDev)->DeviceRequest(theDev, &request);    
    success = (*intf)->ControlRequest(intf, 0, &request);
     
    if(success != kIOReturnSuccess)
        printf("Hi Request Failed \n");
    return success;
 
}


int usb_get_report(void *usbHidHandle,int ReportId, unsigned char *raw_buf, int ReportSize )
{
	int err;

	err =	HIrequest(usbHidHandle, 
                         0xA1, /* Request Type */
                         0x01, /* HID_REPORT_GET */
                         ReportId+(0x03<<8), /* HID_REPORT_TYPE_FEATURE */
                         1, /* Index */
                         raw_buf, ReportSize);
//	if (err == -EPIPE)
//		goto retry;
	return err;
}


int usb_set_report(void *usbHidHandle,int ReportId, unsigned char *raw_buf, int ReportSize )
{
	return 	HIrequest(usbHidHandle, 
                         0x21, /* Request Type */
                         0x09, /* HID_REPORT_SET */
                         ReportId+(0x03<<8), /* HID_REPORT_TYPE_FEATURE */
                         1, /* Index */
                         raw_buf, ReportSize);

}

int SetUSBHIDFeature( USBHIDHandle handle, char *report, int count )
{
    usb_set_report(handle, report[0]&0xf, report, count );

}

int GetUSBHIDFeature( USBHIDHandle handle, char *report, int count )
{
    usb_get_report(handle, report[0]&0xf, report, count );

}

