/*
    Copyright   (c) 2003 Alan Macek <www.alanmacek.com>
                (c) 2004 Ross Bencina <rossb@audiomulch.com>

    Win32 USB HID I/O routines

    Based on usb.c code be Alan Macek
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

#include "usb_hid.h"
#include <windows.h>
#include <stdio.h>
#include <setupapi.h>

#ifdef HAVE_HIDSDI_DDK_HEADERS

/*
    hidsdi.h and the files it includes are part of the Windows DDK. If you
    don't have the DDK header files, define HAVE_HIDSDI_DDK_HEADERS, otherwise
    the definitions in the #else block will be used.
*/

# include <hidsdi.h>

#else

#include <pshpack4.h>

void __stdcall
HidD_GetHidGuid (
   OUT   LPGUID   HidGuid
   );

typedef struct _HIDD_ATTRIBUTES {
    ULONG   Size; // = sizeof (struct _HIDD_ATTRIBUTES)

    //
    // Vendor ids of this hid device
    //
    USHORT  VendorID;
    USHORT  ProductID;
    USHORT  VersionNumber;

    //
    // Additional fields will be added to the end of this structure.
    //
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

BOOLEAN __stdcall
HidD_GetAttributes (
    IN  HANDLE              HidDeviceObject,
    OUT PHIDD_ATTRIBUTES    Attributes
    );

BOOLEAN __stdcall
HidD_GetSerialNumberString (
   IN    HANDLE   HidDeviceObject,
   OUT   PVOID    Buffer,
   IN    ULONG    BufferLength
   );


BOOLEAN __stdcall
HidD_SetFeature(
    IN HANDLE  HidDeviceObject,
    IN PVOID  ReportBuffer,
    IN ULONG  ReportBufferLength
    );

BOOLEAN __stdcall
HidD_GetFeature(
    IN HANDLE  HidDeviceObject,
    OUT PVOID  ReportBuffer,
    IN ULONG  ReportBufferLength
    );

#include <poppack.h>

#endif

/*
    typedef for enumeration function called by EnumHidDevices for every
    USB HID device. Return non-zero to continue enumeration
*/
typedef BOOL HidEnumProc( PSP_DEVICE_INTERFACE_DATA deviceInterfaceData,
                PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData,
                void *userData );


/*
    call enumProc with enumProcData for every UDB HID device until enumProc
    returns zero or all devices have been enumerated.
*/
void EnumHidDevices( HidEnumProc* enumProc, void *enumProcData )
{
    GUID hidGUID;
    HDEVINFO hardwareDeviceInfoSet;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData;
    ULONG requiredSize;
    DWORD index;
    DWORD result;
    BOOL enumOk;
    BOOL continueEnumerating = TRUE;

    /* Get the HID GUID value - used as mask to get list of devices */
    HidD_GetHidGuid( &hidGUID );

    /* Get a list of devices matching the criteria (hid interface, present) */
    hardwareDeviceInfoSet = SetupDiGetClassDevs( &hidGUID,
                                                  NULL,                      /* no enumerator (global) */
                                                  NULL,                      /* no top level window */
                                                  (DIGCF_PRESENT |           /* Only Devices present */
                                                  DIGCF_DEVICEINTERFACE) );  /* Function class devices. */

    if( hardwareDeviceInfoSet != INVALID_HANDLE_VALUE ){

        deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        index = 0;
        do{
            // enumerate the list and get the interface data
            enumOk = SetupDiEnumDeviceInterfaces( hardwareDeviceInfoSet,
                                                  NULL,             /* infoData */
                                                  &hidGUID,         /* interfaceClassGuid */
                                                  index, 
                                                  &deviceInterfaceData );

            /*
                regarding SetupDiEnumDeviceInterfaces msdn says:
                You should first call this function with the MemberIndex parameter set
                to zero to obtain the first interface. Then, repeatedly increment
                MemberIndex and retrieve an interface until this function fails and
                GetLastError returns ERROR_NO_MORE_ITEMS.

                Rather than checking for ERROR_NO_MORE_ITEMS we halt on any error.
            */

            if( enumOk ){

                /* Get the details with null values to get the required size of the buffer */
                requiredSize = 0;
                SetupDiGetDeviceInterfaceDetail( hardwareDeviceInfoSet,
                                                 &deviceInterfaceData,
                                                 NULL,          /* interfaceDetail */
                                                 0,             /* interfaceDetailSize */
                                                 &requiredSize,
                                                 0 );           /* infoData */
                if( requiredSize != 0 ){
                    /* Allocate the buffer */
                    deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);

                    if( deviceInterfaceDetailData != NULL ){
                        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

                        /* Fill the buffer with the device details */
                        result = SetupDiGetDeviceInterfaceDetail( hardwareDeviceInfoSet,
                                                              &deviceInterfaceData,
                                                              deviceInterfaceDetailData,
                                                              requiredSize,
                                                              &requiredSize,
                                                              NULL );
                        if( result != 0 ){

                            /* call the enumProc */
                            continueEnumerating = enumProc( &deviceInterfaceData, deviceInterfaceDetailData, enumProcData );
                            
                        }

                        free (deviceInterfaceDetailData);
                    }
                }

                ++index;
            }
            
        }while( enumOk && continueEnumerating );

        SetupDiDestroyDeviceInfoList (hardwareDeviceInfoSet);
    }
}


typedef struct{
    DWORD currentIndex;
    DWORD deviceIndex;
    USHORT vendorId, productId, versionNumber;
    int flags;
    HANDLE deviceHandle;
} FindHidDeviceEnumData;


BOOL FindHidDeviceEnumProc( PSP_DEVICE_INTERFACE_DATA deviceInterfaceData,
                PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData,
                void *userData )
{
    HANDLE deviceHandle;
    HIDD_ATTRIBUTES deviceAttributes;
    FindHidDeviceEnumData *data = (FindHidDeviceEnumData*)userData;

    (void) deviceInterfaceData; /* unused parameter */
    
    /* Open file on the device */
    deviceHandle = CreateFile( deviceInterfaceDetailData->DevicePath,
                               GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,            /* no SECURITY_ATTRIBUTES structure */
                               OPEN_EXISTING,   /* No special create flags */
                               0,
                               NULL );          /* No template file */

    if( deviceHandle != INVALID_HANDLE_VALUE ){

        deviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
        if( HidD_GetAttributes( deviceHandle, &deviceAttributes ) == TRUE
                && ( !(data->flags & SELECT_VENDOR_ID_FLAG) || deviceAttributes.VendorID == data->vendorId )
                && ( !(data->flags & SELECT_PRODUCT_ID_FLAG) || deviceAttributes.ProductID == data->productId )
                && ( !(data->flags & SELECT_VERSION_NUMBER_FLAG) || deviceAttributes.VersionNumber == data->versionNumber ) ){

            /* found a device which matches the selection criteria */

            if( data->currentIndex == data->deviceIndex ){

                /* found the requested device */
                
                data->deviceHandle = deviceHandle;
                return FALSE; /* stop enumerating */

            }else{
                CloseHandle( deviceHandle );
            }

            ++data->currentIndex;

        }else{
            CloseHandle( deviceHandle );
        }
    }

    return TRUE; /* continue enumerating */
}


USBHID OpenUSBHID( int index, int vendorId, int productId, int versionNumber, int flags )
{
    FindHidDeviceEnumData data;

    data.currentIndex = 0;
    data.deviceIndex = index;
    data.vendorId = (USHORT)vendorId;
    data.productId = (USHORT)productId;
    data.versionNumber = (USHORT)versionNumber;
    data.flags = flags;
    data.deviceHandle = INVALID_HANDLE_VALUE;
    
    EnumHidDevices( FindHidDeviceEnumProc, &data );

    if( data.deviceHandle == INVALID_HANDLE_VALUE )
        return INVALID_USBHID_VALUE;
    else
        return (USBHID)data.deviceHandle; 
}


BOOL DumpHidDeviceInfoEnumProc( PSP_DEVICE_INTERFACE_DATA deviceInterfaceData,
                PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData,
                void *userData )
{
    HANDLE deviceHandle;
    HIDD_ATTRIBUTES deviceAttributes;
    wchar_t s[128];
    
    (void) deviceInterfaceData; /* unused parameter */
    (void) userData;
    
    printf( "Device Path: %s\n", deviceInterfaceDetailData->DevicePath );

    deviceHandle = CreateFile( deviceInterfaceDetailData->DevicePath,
                               GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,            /* no SECURITY_ATTRIBUTES structure */
                               OPEN_EXISTING,   /* No special create flags */
                               0,
                               NULL );          /* No template file */

    if( deviceHandle != INVALID_HANDLE_VALUE ){

        deviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
        if( HidD_GetAttributes( deviceHandle, &deviceAttributes ) ){

            printf( "Vendor ID: %d\n", deviceAttributes.VendorID );
            printf( "Product ID: %d\n", deviceAttributes.ProductID );
            printf( "Version Number: %d\n", deviceAttributes.VersionNumber );
        }

        if( HidD_GetSerialNumberString( deviceHandle, s, 128 ) )
            printf( "Serial Number: `%ls' (len:%d, 1st char:%d)\n", s, wcslen(s), s[0] );


        CloseHandle( deviceHandle );
    }

    printf( "\n" );

    return TRUE; /* continue enumerating */
}


void CloseUSBHID( USBHID handle )
{
    CloseHandle( (HANDLE)handle );
}

int ReadUSBHID( USBHID handle, void *dest, int count )
{
    DWORD bytesRead;

    ReadFile( (HANDLE)handle, dest, count, &bytesRead, 0 );

    return bytesRead;
}

int SetUSBHIDFeature( USBHID handle, char *report, int count )
{
    return HidD_SetFeature( handle, report, count ) == TRUE ? 0 : -EINVAL;
}

void GetUSBHIDFeature( USBHID handle, char *report, int count )
{
    return HidD_GetFeature( handle, report, count ) == TRUE ? 0 : -EINVAL;
}
