/*----------------------------------------------------------------------------
NAME

 TAEHelpers.h -- Tablet Apple Event Helper Functions

 ABOUT:
	These functions will help you communicate with the Wacom Tablet Driver via
	Apple Events. These functions will build a send a proper Apple Event for
	you.

 COPYRIGHT

 Copyright WACOM Technologies, Inc. 2002
 All rights reserved.

 -----------------------------------------------------------------------------*/

#ifndef taeHELPERS_H
#define taeHELPERS_H

#include <Carbon/Carbon.h>
#include "TabletAEDictionary.h"


#ifdef __cplusplus
extern "C" {
#endif

// Tablet Driver Object Tree
//
// cWTDTablet
//   |---> cWTDCustomizedApp
//           |---> cWTDPopItem
//           |---> cWTDMenuItem
//           |---> cWTDTransducer
//                   |---> cWTDButton
//
// In order to reference a particular sub-item you will also need to reference
// it's parent items in the Object Tree. You do not need to reference child
// items if you only care about a parent.

//////////////////////////////////////////////////////////////////////////////
typedef struct TAEObject
{
   // This is the actual object you want to get/set/count data from/to/of
   // Below is the list of accetable objectTypes
   // cWTDDriver, cWTDTablet, cWTDCustomizedApp, cWTDTransducer, cWTDButton,
	// cWTDMenuItem, cWTDButton
   DescType		objectType;

   // The following are used to determine exactly which of the obove object
   // types you want to reference. Not every index is valid for all object
   // types. If the index is not needed for the type, you can ignore it.
   // See the Tablet Driver Object Tree above.
   // !! Don't forget, Indexes into the Tablet Driver via AE start at 1 not 0!
   UInt32		tabletIndex;
   UInt32		applicationIndex;
   UInt32		transducerIndex; 
   UInt32		auxIndex; 			// use for cWTDButton, cWTDMenuItem, or cWTDPopItem
}TAEObject;

//////////////////////////////////////////////////////////////////////////////
// GetTabletDriverTarget
//
// This function creates an AE descriptor that can be used to target Apple
// Events to the Tablet Driver.
//
// parameters: AEDesc *driverTarget - a Pointer to an AEDesc.
//
// returns: driverTarget references the TabletDriver and noErr on success,
//				else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	GetTabletDriverTarget(AEDesc *driverTarget);

//////////////////////////////////////////////////////////////////////////////
// ResendLastTabletEventofType
//
//	This function will instruct the Tablet Driver to resend either the last
// posted proximity event, or pointer event.
//
// parameters: DescType tabletEventType - eEventProximity, eEventPointer
//
// returns: noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	ResendLastTabletEventofType(DescType tabletEventType);

//////////////////////////////////////////////////////////////////////////////
// CreateWacomContextForTablet
//
//	This function will instruct the Tablet Driver to create a context for your
// application that you can then modify. For example, you can modify your
// context to change the tablet mapping or to diconnect the tablet from the
// cursor.
//
// parameters: UInt32 tabletIdx - Tablet Number you want to a context for
//					UInt32 *contextID - On return, the ID used to modify / delete
//											  your context.
//
// returns: a contextID and noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	CreateWacomContextForTablet(UInt32 tabletIdx, UInt32	*contextID);

//////////////////////////////////////////////////////////////////////////////
// DestoryWacomContext
//
//	This function will instruct the Tablet Driver to delete a context that your
// application created. Please be nice and destroy all contexts you create
// before your application quits.
//
// parameters: UInt32 contextID - The ID returned to you when you created the
//											 context.
//
// returns: a contextID and noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	DestoryWacomContext(UInt32	contextID);

//////////////////////////////////////////////////////////////////////////////
// SetData_ofSize_ofType_ofContext_ForAttribute
//
//	This function will modify an attribute of a context your app created. See
// Context Attributes near the bottom of the TabletAEDictionary.h header file.
//
// parameters: const void *inDataPtr - Pointer to the new value.
//					Size inDataSize - Size of the data pointed to above.
//					DescType dataType - Type of data pointed to above.
//					UInt32 contextID - The context to modify.
//					DescType attribute - The attribute you want to modify.
//
// returns: noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	SetData_ofSize_ofType_ofContext_ForAttribute(const void *inDataPtr,
                                                   Size 			inDataSize,
                                                   DescType 	dataType,
                                                   UInt32 		contextID,
                                                   DescType 	attribute);

//////////////////////////////////////////////////////////////////////////////
// GetData_ofSize_ofType_ofContext_ForAttribute
//
//	This function will get an attribute of a context your app created. See
// Context Attributes near the bottom of the TabletAEDictionary.h header file.
//
// parameters: const void *inDataPtr - Location to store the attribute data.
//					Size inDataSize - Size of the data buffer pointed to above.
//					DescType dataType - Type of data bufferpointed to above.
//					UInt32 contextID - The context to get from.
//					DescType attribute - The attribute you want to get.
//
// returns: noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	GetData_ofSize_ofType_ofContext_ForAttribute(void 		*inDataPtr,
                                                   Size 		inDataSize,
                                                   DescType dataType,
                                                   UInt32 	contextID,
                                                   DescType attribute);

//////////////////////////////////////////////////////////////////////////////
// GetData_ofSize_ofType_ofTabletObject_ForAttribute
//
//	This function will get an attribute of a tablet object. See the
// TabletAEDictionary.h header file. Note that these are the global objects
// that are modified by the user with the Wacom Control Panel. You should not
// modify any of these attributes. However, you can use this function to get
// information like the version of the Tablet Driver, size of a tablet, the
// name of a transducer and so on. You might also use this function to get a
// dump of the current setting of each transducer of each tablet for
// diagnostics.
//
// parameters: const void *inDataPtr - Location to store the attribute data.
//					Size inDataSize - Size of the data buffer pointed to above.
//					DescType dataType - Type of data bufferpointed to above.
//					TAEObject *pTabletObject - Pointer to a TAEObject that
//														describes exactly which tablet object
//														you want to get data from.
//					DescType attribute - The attribute you want to get.
//
// returns: noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	GetData_ofSize_ofType_ofTabletObject_ForAttribute(void 		*inDataPtr,
                                                        Size 		inDataSize,
                                                        DescType 	dataType,
                                                        TAEObject *pTabletObject,
                                                        DescType 	attribute);
OSErr	SetData_ofSize_ofType_ofTabletObject_ForAttribute(void 		*inDataPtr,
															Size 		inDataSize,
															DescType 	dataType,
															TAEObject *pTabletObject,
															DescType 	attribute);
															
//////////////////////////////////////////////////////////////////////////////
// CountTabletObjects
//
//	This function will return the number of particular items the Tablet Driver
// knows about. For examle, the number of tablets, the number of transducers
// for a customized application for a particular tablet...
//
// parameters: TAEObject *pTabletObject - Pointer to a TAEObject that
//														describes exactly which tablet object
//														you want to count.
//					 UInt32 *outCount - The number of pTabletObjects the driver knows
//											  about.
//
// returns: noErr on success, else an AE error code
//////////////////////////////////////////////////////////////////////////////
OSErr	CountTabletObjects(TAEObject *pTabletObject, UInt32 *outCount);

#ifdef __cplusplus
}
#endif

#endif taeHELPERS_H