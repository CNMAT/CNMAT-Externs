/*----------------------------------------------------------------------------NAME TAEHelpers.c -- Tablet Apple Event Helper Functions

 ABOUT:
	These functions will help you communicate with the Wacom Tablet Driver via
	Apple Events. These functions will build a send a proper Apple Event for
	you.

 COPYRIGHT

 Copyright WACOM Technologies, Inc. 2002
 All rights reserved.

 -----------------------------------------------------------------------------*/

#include "TAEHelpers.h"
#include "TabletAEDictionary.h"

#define kDefaultTimeOut 15  //Timeout value in ticks Approx 1/4 second

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
OSErr	GetTabletDriverTarget(AEDesc *driverTarget)
{
   OSErr						err = noErr;
   
//   assert(driverTarget);

	OSType	tdSig = kWacomDriverSig;
	AEInitializeDesc(driverTarget);
	err = AECreateDesc(typeApplSignature,
							(Ptr) &tdSig,
							sizeof(tdSig),
							driverTarget);
	
	
	if(noErr != err)
	{
		AEDisposeDesc(driverTarget);
	}

   return err;
}


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
OSErr	ResendLastTabletEventofType(DescType tabletEventType)
{
   AEDesc 			driverTarget;
   AppleEvent		aeSend;
   OSErr				err;

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      AEDisposeDesc(&driverTarget);
      return err;
   }

   err = AECreateAppleEvent(kAEWacomSuite,		// Create a special Wacom Event
                            eSendTabletEvent,   // Send Last Tablet Event
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);
   if(err)
   {
      AEDisposeDesc(&driverTarget);
      return err;
   }
	
	err = AEPutParamPtr ( &aeSend, keyAEData,
							  typeEnumeration,
							  &tabletEventType,
							  sizeof(tabletEventType)); // Add what type of event to send
   if(err)
   {
      AEDisposeDesc(&driverTarget);
      return err;
   }

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                NULL,
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);
   
   AEDisposeDesc(&aeSend);

   return err;
}



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
OSErr	CreateWacomContextForTablet(UInt32 tabletIdx, UInt32	*contextID)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
   DescType			objectToMake = cContext;
   UInt32			outContextID;
   DescType			outType;
   Size				outSize;
   AppleEvent		aeSend;
   AppleEvent		aeReply;
   OSErr				err;

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAECreateElement,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to create
   err = AEPutParamPtr( &aeSend,
                        keyAEObjectClass,
                        typeType,
                        &objectToMake,
                        sizeof(DescType));

   // Now tell the AE where to create a context
   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeUInt32,
                       &tabletIdx,	// This is the tablet number we want the context for
                       sizeof(tabletIdx),
                       &keyData );

   err = CreateObjSpecifier(cWTDTablet,			// We want info from a tablet
                            &nullDesc,				// This is the last item in the chain
                            formAbsolutePosition,	// use indexing to determine which tablet to get data from
                            &keyData,				// This is the Tablet Number descriptor created above
                            TRUE,					// delete the nullDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want data from tablet number X

   err = AEPutParamDesc( &aeSend, keyAEInsertHere,  &tObjSpecifier);

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                &aeReply,	// The reply event that contains the Context ID.
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);
   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);

   // Get the Context ID out of the reply
   err = AEGetParamPtr(&aeReply,
                       keyDirectObject,
                       typeUInt32,
                       &outType,
                       &outContextID,   // Put the answer into this variable
                       sizeof(long),
                       &outSize);
   if(!err)
   {
      if(contextID)
      {
         *contextID = outContextID;
      }
   }

   AEDisposeDesc(&aeReply);

   return err;
}



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
OSErr	DestoryWacomContext(UInt32	contextID)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
   AppleEvent		aeSend;
   OSErr				err;

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAEDelete,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to destroy
   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeUInt32,
                       &contextID,	// This is the context ID we want to destroy
                       sizeof(contextID),
                       &keyData );

   err = CreateObjSpecifier(cContext,				// We want to destroy a context
                            &nullDesc,				// This is the last item in the chain
                            formUniqueID,			// use id to determine which context to destroy
                            &keyData,				// This is the Context ID descriptor created above
                            TRUE,					// delete the nullDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want to delete Context X

   err = AEPutParamDesc( &aeSend, keyDirectObject,  &tObjSpecifier);

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                NULL,		// Don't need a reply
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);
   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);

   return err;
}



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
                                                   DescType 	attribute)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
   AEDesc			lastDesc;
   AppleEvent		aeSend;
   OSErr				err;


//   assert(inDataPtr);
   if(!inDataPtr)
   {
      return -1;
   }
   
   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAESetData,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to set
   
   // We have to set up a chain here.
   // Property -> Context -> null
   // The thing is, Apple Events are built backwards. So...
   
   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeUInt32,
                       &contextID,	// This is the context ID that we want to modify
                       sizeof(contextID),
                       &keyData );

   err = CreateObjSpecifier(cContext,				// We want to modifiy the property of this context
                            &nullDesc,				// This is the last item in the chain
                            formUniqueID,			// use unique id to determine which context to modify
                            &keyData,				// This is the Context ID descriptor created above
                            TRUE,					// delete the nullDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want to modify Context X
	AEDuplicateDesc(&tObjSpecifier, &lastDesc);
   AEDisposeDesc(&tObjSpecifier);
	
   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeType,
                       &attribute,	// This is the Property we want to set
                       sizeof(DescType),
                       &keyData );

   err = CreateObjSpecifier(cProperty,				// The context's property we want to modify
                            &lastDesc,				// This is the last item in the chain (the Context)
                            formPropertyID,		// use PropertyID to determine which type of Property to set
                            &keyData,				// This is the Property descriptor created above
                            TRUE,					// delete the lastDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want to set Property X
   
	// Add the data to set to the AE here.
   err = AEPutParamPtr ( &aeSend, keyAEData, dataType,
								 inDataPtr, inDataSize);
	// The Object chain (Attribute -> Context -> null)
	err = AEPutParamDesc( &aeSend, keyDirectObject,  &tObjSpecifier);	

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                NULL,		// Don't need a reply
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);
   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);

   return err;
}



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
                                                   DescType attribute)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
	AEDesc			lastDesc;
   AppleEvent		aeSend;
   AppleEvent		aeReply;
   DescType			outType;
   Size				outSize;
   OSErr				err;


//   assert(inDataPtr);
   if(!inDataPtr)
   {
      return -1;
   }

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAEGetData,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to get

   // We have to set up a chain here.
   // Attribute -> Context -> null
   // The thing is, Apple Events are built backwards. So...

   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeUInt32,
                       &contextID,	// This is the context ID that we want to modify
                       sizeof(contextID),
                       &keyData );

   err = CreateObjSpecifier(cContext,				// We want to modifiy a context attribute
                            &nullDesc,				// This is the last item in the chain
                            formUniqueID,			// use id to determine which context to modify
                            &keyData,				// This is the Context ID descriptor created above
                            TRUE,					// delete the nullDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want to modify Context X
	AEDuplicateDesc(&tObjSpecifier, &lastDesc);
   AEDisposeDesc(&tObjSpecifier);
	
   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeType,
                       &attribute,	// This is the Attribute we want to set
                       sizeof(DescType),
                       &keyData );

   err = CreateObjSpecifier(cProperty,				// We want to destroy a context
                            &lastDesc,				// This is the last item in the chain (the Context)
                            formPropertyID,		// use id to determine which context to destroy
                            &keyData,				// This is the Context ID descriptor created above
                            TRUE,					// delete the nullDesc, and KeyData descriptor for us
                            &tObjSpecifier);		// The created descriptor which says that we want to delete Context X
	
	// Add the data to set to the AE here.
   err = AEPutParamPtr ( &aeSend, keyAERequestedType,
								 typeType, &dataType, sizeof(DescType));
	// The Object chain (Attribute -> Context -> null)
	err = AEPutParamDesc( &aeSend, keyDirectObject,  &tObjSpecifier);

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                &aeReply,		
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);

   if(!err)
   {
      err = AEGetParamPtr(&aeReply,
                          keyDirectObject,
                          dataType,
                          &outType,
                          inDataPtr,   // Put the answer into this variable
                          inDataSize,
                          &outSize);
   }
   
   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);
   AEDisposeDesc(&aeReply);

   return err;
}



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
																		  DescType 	attribute)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
   AEDesc			lastDesc;
   AppleEvent		aeSend;
   AppleEvent		aeReply;
   DescType			outType;
   Size				outSize;
   UInt32			tmpIndex = 0;
   OSErr				err;


//   assert(inDataPtr);
   if(!inDataPtr)
   {
      return -1;
   }

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAEGetData,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to get

   // We have to set up a chain here.
   // Attribute -> Button -> Transducer -> Application -> Tablet -> null
   // The thing is, Apple Events are built backwards. So...

   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   // set up the driver desc
   AEInitializeDesc(&keyData);
   tmpIndex = 1;
   err = AECreateDesc( typeUInt32,
                       &tmpIndex,	// This is the Tablet Driver Index. Always 1
                       sizeof(tmpIndex),
                       &keyData );

   err = CreateObjSpecifier(cWTDDriver,
                            &nullDesc,	
                            formAbsolutePosition,
                            &keyData,	
                            TRUE,
                            &tObjSpecifier);
	AEDuplicateDesc(&tObjSpecifier,&lastDesc);
   AEDisposeDesc(&tObjSpecifier);
   
   if (pTabletObject->objectType != cWTDDriver)
   {
      // set up the tablet desc
      AEInitializeDesc(&keyData);
      err = AECreateDesc( typeUInt32,
                          &pTabletObject->tabletIndex,
                          sizeof(UInt32),
                          &keyData );

      err = CreateObjSpecifier(cWTDTablet,
                               &lastDesc,
                               formAbsolutePosition,
                               &keyData,
                               TRUE,
                               &tObjSpecifier);
      AEDuplicateDesc(&tObjSpecifier,&lastDesc);
		AEDisposeDesc(&tObjSpecifier);

      if (pTabletObject->objectType != cWTDTablet)
      {
         // set up the cutom app desc
         AEInitializeDesc(&keyData);
         err = AECreateDesc( typeUInt32,
                             &pTabletObject->applicationIndex,
                             sizeof(UInt32),
                             &keyData );

         err = CreateObjSpecifier(cWTDCustomizedApp,
                                  &lastDesc,
                                  formAbsolutePosition,
                                  &keyData,
                                  TRUE,
                                  &tObjSpecifier);
         AEDuplicateDesc(&tObjSpecifier,&lastDesc);
			AEDisposeDesc(&tObjSpecifier);

         if (pTabletObject->objectType != cWTDCustomizedApp)
         {
            if (pTabletObject->objectType == cWTDButton)
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->transducerIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(cWTDTransducer,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
   
            if (pTabletObject->objectType == cWTDTransducer)
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->transducerIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(cWTDTransducer,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
            else // cWTDButton || cWTDMenuItem || cWTDButton
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->auxIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(pTabletObject->objectType,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
         }
      }
   }

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeType,
                       &attribute,	// This is the Attribute we want to get
                       sizeof(DescType),
                       &keyData );

   err = CreateObjSpecifier(cProperty,	
                            &lastDesc,
                            formPropertyID,
                            &keyData,	
                            TRUE,
                            &tObjSpecifier);	

	// Add the data to set to the AE here.
   err = AEPutParamPtr ( &aeSend, keyAERequestedType,
								 typeType, &dataType, sizeof(DescType)); 
   err = AEPutParamDesc( &aeSend, keyDirectObject,  &tObjSpecifier);	// The Object chain 
   
   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                &aeReply,
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);

   if(!err)
   {
      err = AEGetParamPtr(&aeReply,
                          keyDirectObject,
                          dataType,
                          &outType,
                          inDataPtr,   // Put the answer into this variable
                          inDataSize,
                          &outSize);
   }

   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);
   AEDisposeDesc(&aeReply);

   return err;
   
}


OSErr	SetData_ofSize_ofType_ofTabletObject_ForAttribute(void 		*inDataPtr,
																		  Size 		inDataSize,
																		  DescType 	dataType,
																		  TAEObject *pTabletObject,
																		  DescType 	attribute)
{
   AEDesc 			driverTarget,nullDesc,keyData,tObjSpecifier;
   AEDesc			lastDesc;
   AppleEvent		aeSend;
   UInt32			tmpIndex = 0;
   OSErr				err;


//   assert(inDataPtr);
   if(!inDataPtr)
   {
      return -1;
   }

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAESetData,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to set

   // We have to set up a chain here.
   // Attribute -> Button -> Transducer -> Application -> Tablet -> null
   // The thing is, Apple Events are built backwards. So...

   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&nullDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &nullDesc );

   // set up the driver desc
   AEInitializeDesc(&keyData);
   tmpIndex = 1;
   err = AECreateDesc( typeUInt32,
                       &tmpIndex,	// This is the Tablet Driver Index. Always 1
                       sizeof(tmpIndex),
                       &keyData );

   err = CreateObjSpecifier(cWTDDriver,
                            &nullDesc,	
                            formAbsolutePosition,
                            &keyData,	
                            TRUE,
                            &tObjSpecifier);
	AEDuplicateDesc(&tObjSpecifier,&lastDesc);
   AEDisposeDesc(&tObjSpecifier);
   
   if (pTabletObject->objectType != cWTDDriver)
   {
      // set up the tablet desc
      AEInitializeDesc(&keyData);
      err = AECreateDesc( typeUInt32,
                          &pTabletObject->tabletIndex,
                          sizeof(UInt32),
                          &keyData );

      err = CreateObjSpecifier(cWTDTablet,
                               &lastDesc,
                               formAbsolutePosition,
                               &keyData,
                               TRUE,
                               &tObjSpecifier);
      AEDuplicateDesc(&tObjSpecifier,&lastDesc);
		AEDisposeDesc(&tObjSpecifier);

      if (pTabletObject->objectType != cWTDTablet)
      {
         // set up the cutom app desc
         AEInitializeDesc(&keyData);
         err = AECreateDesc( typeUInt32,
                             &pTabletObject->applicationIndex,
                             sizeof(UInt32),
                             &keyData );

         err = CreateObjSpecifier(cWTDCustomizedApp,
                                  &lastDesc,
                                  formAbsolutePosition,
                                  &keyData,
                                  TRUE,
                                  &tObjSpecifier);
         AEDuplicateDesc(&tObjSpecifier,&lastDesc);
			AEDisposeDesc(&tObjSpecifier);

         if (pTabletObject->objectType != cWTDCustomizedApp)
         {
            if (pTabletObject->objectType == cWTDButton)
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->transducerIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(cWTDTransducer,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
   
            if (pTabletObject->objectType == cWTDTransducer)
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->transducerIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(cWTDTransducer,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
            else // cWTDButton || cWTDMenuItem || cWTDButton
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                 &pTabletObject->auxIndex,
                                 sizeof(UInt32),
                                 &keyData );
   
               err = CreateObjSpecifier(pTabletObject->objectType,
                                       &lastDesc,
                                       formAbsolutePosition,
                                       &keyData,
                                       TRUE,
                                       &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
         }
      }
   }

   AEInitializeDesc(&keyData);
   err = AECreateDesc( typeType,
                       &attribute,	// This is the Attribute we want to set
                       sizeof(DescType),
                       &keyData );

   err = CreateObjSpecifier(cProperty,	
                            &lastDesc,
                            formPropertyID,
                            &keyData,	
                            TRUE,
                            &tObjSpecifier);	

	// Add the data to set to the AE here.
   err = AEPutParamPtr ( &aeSend, keyAEData,
								 dataType, inDataPtr, inDataSize); 
   err = AEPutParamDesc( &aeSend, keyDirectObject,  &tObjSpecifier);	// The Object chain 
   
   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                NULL,		// Don't need a reply
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);

   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);

   return err;
   
}



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
OSErr	CountTabletObjects(TAEObject *pTabletObject, UInt32 *outCount)
{
   AEDesc 			driverTarget,keyData,tObjSpecifier;
   AEDesc			lastDesc;
   AppleEvent		aeSend;
   AppleEvent		aeReply;
   DescType			outType;
   Size				outSize;
   UInt32			tmpIndex = 0;
   UInt32			tmpCount = 0;
   OSErr				err;


//   assert(outCount);
   if(!outCount)
   {
      return -1;
   }

   err = GetTabletDriverTarget(&driverTarget);
   if(err)
   {
      return err;
   }

   err = AECreateAppleEvent(kAECoreSuite,
                            kAECountElements,
                            &driverTarget,
                            kAutoGenerateReturnID,
                            kAnyTransactionID,
                            &aeSend);

   // Now tell the AE what to Count

   // We have to set up a chain here.
   // [Button ->] [Transducer ->] [Application ->] Tablet -> null
   // The thing is, Apple Events are built backwards. So...

   //Create  NULL AEDesc, this will signify the end of the AEDesc Chain
   AEInitializeDesc(&lastDesc);
   err = AECreateDesc( typeNull, NULL, NULL, &lastDesc );

   if (pTabletObject->objectType != cWTDDriver)
   {
      // set up the driver desc
      AEInitializeDesc(&keyData);
      tmpIndex = 1;
      err = AECreateDesc( typeUInt32,
                        &tmpIndex,	// This is the Tablet Driver Index. Always 1
                        sizeof(tmpIndex),
                        &keyData );
   
      err = CreateObjSpecifier(cWTDDriver,
                              &lastDesc,
                              formAbsolutePosition,
                              &keyData,
                              TRUE,
                              &tObjSpecifier);
      AEDuplicateDesc(&tObjSpecifier,&lastDesc);
		AEDisposeDesc(&tObjSpecifier);

      if (pTabletObject->objectType != cWTDTablet)
      {
         // set up the tablet desc
         AEInitializeDesc(&keyData);
         err = AECreateDesc( typeUInt32,
                           &pTabletObject->tabletIndex,
                           sizeof(UInt32),
                           &keyData );
   
         err = CreateObjSpecifier(cWTDTablet,
                                 &lastDesc,
                                 formAbsolutePosition,
                                 &keyData,
                                 TRUE,
                                 &tObjSpecifier);
         AEDuplicateDesc(&tObjSpecifier,&lastDesc);
			AEDisposeDesc(&tObjSpecifier);
   
         if (pTabletObject->objectType != cWTDCustomizedApp)
         {
            // set up the cutom app desc
            AEInitializeDesc(&keyData);
            err = AECreateDesc( typeUInt32,
                              &pTabletObject->applicationIndex,
                              sizeof(UInt32),
                              &keyData );
   
            err = CreateObjSpecifier(cWTDCustomizedApp,
                                    &lastDesc,
                                    formAbsolutePosition,
                                    &keyData,
                                    TRUE,
                                    &tObjSpecifier);
            AEDuplicateDesc(&tObjSpecifier,&lastDesc);
				AEDisposeDesc(&tObjSpecifier);

            if (pTabletObject->objectType == cWTDButton)
            {
               // set up the transducer desc
               AEInitializeDesc(&keyData);
               err = AECreateDesc( typeUInt32,
                                   &pTabletObject->transducerIndex,
                                   sizeof(UInt32),
                                   &keyData );

               err = CreateObjSpecifier(cWTDTransducer,
                                        &lastDesc,
                                        formAbsolutePosition,
                                        &keyData,
                                        TRUE,
                                        &tObjSpecifier);
               AEDuplicateDesc(&tObjSpecifier,&lastDesc);
					AEDisposeDesc(&tObjSpecifier);
            }
         }
      }
   }


   // Now we actually tell it to count the number of the tablet objects
   err = AEPutParamPtr( &aeSend,
                        keyAEObjectClass,
                        typeType,
                        &pTabletObject->objectType,	// This is the class of objects to count
                        sizeof(DescType) );
   err = AEPutParamDesc( &aeSend, keyDirectObject, &lastDesc);	// The Object chain

   // Finally send the event
   err = AESend(&aeSend,	// The complete AE we created above
                &aeReply,
                kAEWaitReply,
                kAEHighPriority,
                kDefaultTimeOut,
                NULL,
                NULL);

   if(!err)
   {
      // Get the answer from the Reply
      err = AEGetParamPtr(&aeReply,
                          keyDirectObject,
                          typeUInt32,
                          &outType,
                          &tmpCount,   // Put the answer into this variable
                          sizeof(long),
                          &outSize);
      if(noErr != err)
      {
         tmpCount = 0;
      }
      else
      {
         *outCount = tmpCount;
      }
   }

   AEDisposeDesc(&tObjSpecifier);
   AEDisposeDesc(&aeSend);
   AEDisposeDesc(&aeReply);

   return err;

}