/* ----- ext_midi.h -------------- */

typedef struct {
	short usingCalamari;
	short nInPorts;
	short nOutPorts;
	short inRefNum[16];
	short outRefNum[16];
	short intTimeRefNum;
	short timeInRefNum;
	short timeOutRefNum;
	OSType applSig;
} OldMidiInfoRec;

typedef struct midiinforec {
	short usingCalamari;
	short nInPorts;
	short nOutPorts;
	short inRefNum[16];
	short outRefNum[14];
	Byte *inPorts;
	short intTimeRefNum;
	short timeInRefNum;
	short timeOutRefNum;
	Byte *outPorts;
} MidiInfoRec;
