#ifndef _MACHEADERSACAF_H_

#define ACCESSOR_CALLS_ARE_FUNCTIONS	1

/*	#include <ADSP.h> */
	#include <AEDataModel.h>
	#include <AEHelpers.h>
	#include <AEInteraction.h>
/*	#include <AEMach.h> */
	#include <AEObjects.h>
	#include <AEPackObject.h>
	#include <AERegistry.h>
	#include <AEUserTermTypes.h>
/*	#include <AIFF.h> */
	#include <Aliases.h>
	#include <Appearance.h>
	#include <AppleDiskPartitions.h>
	#include <AppleEvents.h> 
/*	#include <AppleGuide.h> */
	#include <AppleHelp.h>
	#include <AppleScript.h>
/*	#include <AppleTalk.h> */
/*	#include <ApplicationServices.h> */
	#include <ASDebugging.h>
	#include <ASRegistry.h>
/*	#include <ATA.h> */
/*	#include <ATSFont.h> */
/*	#include <ATSLayoutTypes.h> */
/*	#include <ATSTypes.h> */
/*	#include <ATSUnicode.h> */
/*	#include <AVComponents.h> */
/*	#include <AVLTree.h> */
/*	#include <Balloons.h> */
/*	#include <Carbon.h> */
/*	#include <CarbonEvents.h> */
/*	#include <CardServices.h> */
/*	#include <CFArray.h> */
/*	#include <CFBag.h> */
/*	#include <CFBase.h> */
/*	#include <CFBundle.h> */
/*	#include <CFCharacterSet.h> */
/*	#include <CFData.h> */
/*	#include <CFDate.h> */
/*	#include <CFDictionary.h> */
/*	#include <CFNumber.h> */
/*	#include <CFPlugIn.h> */
/*	#include <CFPlugInCOM.h> */
/*	#include <CFPreferences.h> */
/*	#include <CFPropertyList.h> */
/*	#include <CFSet.h> */
/*	#include <CFString.h> */
/*	#include <CFStringEncodingExt.h> */
/*	#include <CFTimeZone.h> */
/*	#include <CFTree.h> */
/*	#include <CFURL.h> */
/*	#include <CFURLAccess.h> */
/*	#include <CFUUID.h> */
/*	#include <CFXMLNode.h> */
/*	#include <CFXMLParser.h> */
/*	#include <CGAffineTransform.h> */
/*	#include <CGBase.h> */
/*	#include <CGBitmapContext.h> */
/*	#include <CGColorSpace.h> */
/*	#include <CGContext.h> */
/*	#include <CGDataConsumer.h> */
/*	#include <CGDataProvider.h> */
/*	#include <CGDirectDisplay.h> */
/*	#include <CGDirectPalette.h> */
/*	#include <CGError.h> */
/*	#include <CGFont.h> */
/*	#include <CGGeometry.h> */
/*	#include <CGImage.h> */
/*	#include <CGPDFContext.h> */
/*	#include <CGPDFDocument.h> */
/*	#include <CGRemoteOperation.h> */
/*	#include <CGWindowLevel.h> */
/*	#include <CMAcceleration.h> */
/*	#include <CMApplication.h> */
/*	#include <CMCalibrator.h> */
/*	#include <CMComponent.h> */
/*	#include <CMConversions.h> */
/*	#include <CMDeviceIntegration.h> */
/*	#include <CMICCProfile.h> */
/*	#include <CMMComponent.h> */
/*	#include <CMPRComponent.h> */
/*	#include <CMScriptingPlugin.h> */
/*	#include <CMTypes.h> */
	#include <CodeFragments.h>
	#include <Collections.h>
	#include <ColorPicker.h>
/*	#include <ColorPickerComponents.h> */
/*	#include <CommResources.h> */
	#include <Components.h>
	#include <ConditionalMacros.h>
/*	#include <Connections.h> */
/*	#include <ConnectionTools.h> */
	#include <ControlDefinitions.h>
	#include <Controls.h>
/*	#include <ControlStrip.h> */
/*	#include <CoreServices.h> */
/*	#include <cred.h> */
/*	#include <CRMSerialDevices.h> */
/*	#include <CryptoMessageSyntax.h> */
/*	#include <CTBUtilities.h> */
/*	#include <CursorDevices.h> */
/*	#include <DatabaseAccess.h> */
	#include <DateTimeUtils.h>
/*	#include <Debugging.h> */
/*	#include <Desk.h> */ /* Obsolete */
/*	#include <DeskBus.h> */
/*	#include <DesktopPrinting.h> */
	#include <Devices.h>
	#include <Dialogs.h>
/*	#include <Dictionary.h> */
	#include <DiskInit.h>
/*	#include <Disks.h> */
	#include <Displays.h>
/*	#include <dlpi.h> */
	#include <Drag.h>
/*	#include <DrawSprocket.h> */
	#include <DriverFamilyMatching.h>
/*	#include <DriverGestalt.h> */
/*	#include <DriverServices.h> */
/*	#include <DriverSupport.h> */
/*	#include <DriverSynchronization.h> */
/*	#include <Editions.h> */
	#include <Endian.h>
/*	#include <ENET.h> */
	#include <EPPC.h>
/*	#include <Errors.h> */ /* Obsolete */
	#include <Events.h>
/*	#include <fenv.h> */
/*	#include <FileMapping.h> */
	#include <Files.h>
/*	#include <FileSigning.h> */
/*	#include <FileTransfers.h> */
/*	#include <FileTransferTools.h> */
	#include <FileTypesAndCreators.h>
/*	#include <FindByContent.h> */
	#include <Finder.h>
	#include <FinderRegistry.h>
	#include <FixMath.h>
	#include <Folders.h>
	#include <Fonts.h>
/*	#include <FontSync.h> */
/*	#include <fp.h> */
/*	#include <FragLoad.h> */ /* Obsolete */
/*	#include <FSM.h> */
	#include <Gestalt.h>
/*	#include <GestaltEqu.h> */ /* Obsolete */
/*	#include <GXEnvironment.h> */
/*	#include <GXErrors.h> */
/*	#include <GXFonts.h> */
/*	#include <GXGraphics.h> */
/*	#include <GXLayout.h> */
/*	#include <GXMath.h> */
/*	#include <GXTypes.h> */
	#include <HFSVolumes.h>
/*	#include <HID.h> */
/*	#include <HTMLRendering.h> */
/*	#include <HyperXCmd.h> */
/*	#include <IAExtractor.h> */
/*	#include <IBCarbonRuntime.h> */
/*	#include <ICAApplication.h> */
/*	#include <ICACamera.h> */
/*	#include <ICADevice.h> */
	#include <Icons.h>
	#include <ImageCodec.h> 
	#include <ImageCompression.h>
/*	#include <InputSprocket.h> */
/*	#include <InternetConfig.h> */
/*	#include <Interrupts.h> */
	#include <IntlResources.h>
/*	#include <IsochronousDataHandler.h> */
/*	#include <JManager.h> */
/*	#include <Kernel.h> */
/*	#include <Keyboards.h> */
/*	#include <Keychain.h> */
/*	#include <KeychainCore.h> */
/*	#include <KeychainHI.h> */
/*	#include <Language.h> */
/*	#include <LanguageAnalysis.h> */
/*	#include <LaunchServices.h> */
	#include <Lists.h>
/*	#include <LocationManager.h> */
	#include <LowMem.h>
/*	#include <MacApplication.h> */
	#include <MacErrors.h>
	#include <MacHelp.h>
/*	#include <MachineExceptions.h> */
/*	#include <MacLocales.h> */
	#include <MacMemory.h>
/*	#include <MacTCP.h> */
/*	#include <MacTextEditor.h> */
	#include <MacTypes.h>
	#include <MacWindows.h>
/*	#include <Math64.h> */
/*	#include <MediaHandlers.h> */
/*	#include <Memory.h> */ /* Obsolete */
	#include <Menus.h>
/*	#include <MIDI.h> */
/*	#include <miioccom.h> */
/*	#include <mistream.h> */
	#include <MixedMode.h>
/*	#include <modnames.h> */
	#include <Movies.h>
/*	#include <MoviesFormat.h> */
/*	#include <MP.h> */ /* Obsolete */
/*	#include <Multiprocessing.h> */
/*	#include <MultiprocessingInfo.h> */
	#include <NameRegistry.h>
	#include <Navigation.h>
/*	#include <NetSprocket.h> */
/*	#include <NetworkSetup.h> */
	#include <Notification.h>
/*	#include <NSL.h> */
/*	#include <NSLCore.h> */
	#include <NumberFormatting.h>
/*	#include <OpenTptAppleTalk.h> */
/*	#include <OpenTptClient.h> */
/*	#include <OpenTptCommon.h> */
/*	#include <OpenTptConfig.h> */
/*	#include <OpenTptDevLinks.h> */
/*	#include <OpenTptGlobalNew.h> */
/*	#include <OpenTptInternet.h> */
/*	#include <OpenTptISDN.h> */
/*	#include <OpenTptLinks.h> */
/*	#include <OpenTptModule.h> */
/*	#include <OpenTptPCISupport.h> */
/*	#include <OpenTptSerial.h> */
/*	#include <OpenTptXTI.h> */
/*	#include <OpenTransport.h> */
/*	#include <OpenTransportKernel.h> */
/*	#include <OpenTransportProtocol.h> */
/*	#include <OpenTransportProviders.h> */
/*	#include <OpenTransportUNIX.h> */
	#include <OSA.h>
	#include <OSAComp.h>
	#include <OSAGeneric.h>
/*	#include <OSEvents.h> */ /* Obsolete */
	#include <OSUtils.h>
/*	#include <OTDebug.h> */
/*	#include <OTSharedLibs.h> */
/*	#include <Packages.h> */
	#include <Palettes.h>
	#include <Patches.h>
/*	#include <PCCard.h>
/*	#include <PCCardAdapterPlugin.h> */
/*	#include <PCCardEnablerPlugin.h> */
/*	#include <PCCardTuples.h> */
/*	#include <PCI.h> */
/*	#include <PEFBinaryFormat.h> */
/*	#include <Picker.h> */ /* Obsolete */
/*	#include <PictUtil.h> */ /* Obsolete */
/*	#include <PictUtils.h> */
/*	#include <PLStringFuncs.h> */
/*	#include <PMApplication.h> */
/*	#include <PMCore.h> */
/*	#include <PMDefinitions.h> */
/*	#include <Power.h> */
	#include <PPCToolbox.h>
	#include <Printing.h>
	#include <Processes.h>
/*	#include <QD3D.h> */
/*	#include <QD3DAcceleration.h> */
/*	#include <QD3DCamera.h> */
/*	#include <QD3DController.h> */
/*	#include <QD3DCustomElements.h> */
/*	#include <QD3DDrawContext.h> */
/*	#include <QD3DErrors.h> */
/*	#include <QD3DExtension.h> */
/*	#include <QD3DGeometry.h> */
/*	#include <QD3DGroup.h> */
/*	#include <QD3DIO.h> */
/*	#include <QD3DLight.h> */
/*	#include <QD3DMath.h> */
/*	#include <QD3DPick.h> */
/*	#include <QD3DRenderer.h> */
/*	#include <QD3DSet.h> */
/*	#include <QD3DShader.h> */
/*	#include <QD3DStorage.h> */
/*	#include <QD3DString.h> */
/*	#include <QD3DStyle.h> */
/*	#include <QD3DTransform.h> */
/*	#include <QD3DView.h> */
/*	#include <QD3DViewer.h> */
/*	#include <QD3DWinViewer.h> */
	#include <QDOffscreen.h>
/*	#include <QTML.h> */
/*	#include <QTSMovie.h> */
/*	#include <QTStreamingComponents.h> */
	#include <Quickdraw.h>
	#include <QuickdrawText.h>
	#include <QuickTime.h> 
	#include <QuickTimeComponents.h> 
/*	#include <QuickTimeMusic.h> */
/*	#include <QuickTimeStreaming.h> */
/*	#include <QuickTimeVR.h> */
/*	#include <QuickTimeVRFormat.h> */
/*	#include <RAVE.h> */
/*	#include <RAVESystem.h> */
	#include <Resources.h>
/*	#include <Retrace.h> */
/*	#include <ROMDefs.h> */
/*	#include <ScalerStreamTypes.h> */
/*	#include <ScalerTypes.h> */
	#include <Scrap.h>
	#include <Script.h>
/*	#include <SCSI.h> */
	#include <SegLoad.h>
/*	#include <Serial.h> */
/*	#include <SFNTLayoutTypes.h> */
/*	#include <SFNTTypes.h> */
/*	#include <ShutDown.h> */
/*	#include <Slots.h> */
/*	#include <SocketServices.h> */
	#include <Sound.h>
/*	#include <SoundComponents.h> */
/*	#include <SoundInput.h> */
/*	#include <SoundSprocket.h> */
/*	#include <Speech.h> */
/*	#include <SpeechRecognition.h> */
/*	#include <SpeechSynthesis.h> */
	#include <StandardFile.h>
/*	#include <Start.h> */
	#include <StringCompare.h>
/*	#include <Strings.h> */
/*	#include <strlog.h> */
/*	#include <stropts.h> */
/*	#include <strstat.h> */
/*	#include <TargetConditionals.h> */
/*	#include <Telephones.h> */
/*	#include <Terminals.h> */
/*	#include <TerminalTools.h> */
	#include <TextCommon.h>
	#include <TextEdit.h>
/*	#include <TextEncodingConverter.h> */
/*	#include <TextEncodingPlugin.h> */
/*	#include <TextServices.h> */
	#include <TextUtils.h>
	#include <Threads.h>
/*	#include <tihdr.h> */
	#include <Timer.h>
	#include <ToolUtils.h>
	#include <Translation.h>
	#include <TranslationExtensions.h>
	#include <Traps.h>
/*	#include <TSMTE.h> */
/*	#include <Types.h> */ /* Obsolete */
/*	#include <TypeSelect.h> */
/*	#include <Unicode.h> */
/*	#include <UnicodeConverter.h> */
/*	#include <UnicodeUtilities.h> */
/*	#include <URLAccess.h> */
/*	#include <USB.h> */
	#include <UTCUtils.h>
/*	#include <vBasicOps.h> */
/*	#include <vBigNum.h> */
/*	#include <vBLAS.h> */
/*	#include <vDSP.h> */
/*	#include <vectorOps.h> */
/*	#include <vfp.h> */
/*	#include <Video.h> */
/*	#include <VideoServices.h> */
/*	#include <Windows.h> */ /* Obsolete */
/*	#include <WorldScript.h> */
/*	#include <ZoomedVideo.h> */




#endif /* _MACHEADERSACAF_H_ */