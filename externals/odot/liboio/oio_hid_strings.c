#include "oio_hid_usage_strings.h"
#include "oio_obj.h"

CFStringRef oio_hid_strings_getUsagePageString(CFDictionaryRef dict, uint32_t usage_page){

}

CFStringRef oio_hid_strings_getUsageString(CFDictionaryRef dict, uint32_t usage){

}

uint32_t oio_hid_strings_getUsagePageString(CFDictionaryRef dict, uint32_t usage_page){

}

uint32_t oio_hid_strings_getUsageString(CFDictionaryRef dict, uint32_t usage){

}

void oio_hid_usage_strings(uint32_t usage_page, uint32_t usage, char *usage_page_string, char *usage_string){
	switch(usage_page){
	case 0x0001:
		strcpy(usage_page_string, "Generic-Desktop");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Pointer"); break;
		case 0x0002:
			strcpy(usage_string, "Mouse"); break;
		case 0x0004:
			strcpy(usage_string, "Joystick"); break;
		case 0x0005:
			strcpy(usage_string, "GamePad"); break;
		case 0x0006:
			strcpy(usage_string, "Keyboard"); break;
		case 0x0007:
			strcpy(usage_string, "Keypad"); break;
		case 0x0008:
			strcpy(usage_string, "MultiAxisController"); break;
		case 0x0030:
			strcpy(usage_string, "X"); break;
		case 0x0031:
			strcpy(usage_string, "Y"); break;
		case 0x0032:
			strcpy(usage_string, "Z"); break;
		case 0x0033:
			strcpy(usage_string, "Rx"); break;
		case 0x0034:
			strcpy(usage_string, "Ry"); break;
		case 0x0035:
			strcpy(usage_string, "Rz"); break;
		case 0x0036:
			strcpy(usage_string, "Slider"); break;
		case 0x0037:
			strcpy(usage_string, "Dial"); break;
		case 0x0038:
			strcpy(usage_string, "Wheel"); break;
		case 0x0039:
			strcpy(usage_string, "Hatswitch"); break;
		case 0x003A:
			strcpy(usage_string, "Counted-Buffer"); break;
		case 0x003B:
			strcpy(usage_string, "Byte-Count"); break;
		case 0x003C:
			strcpy(usage_string, "Motion-Wakeup"); break;
		case 0x003D:
			strcpy(usage_string, "Start"); break;
		case 0x003E:
			strcpy(usage_string, "Select"); break;
		case 0x0040:
			strcpy(usage_string, "Vx"); break;
		case 0x0041:
			strcpy(usage_string, "Vy"); break;
		case 0x0042:
			strcpy(usage_string, "Vz"); break;
		case 0x0043:
			strcpy(usage_string, "Vbrx"); break;
		case 0x0044:
			strcpy(usage_string, "Vbry"); break;
		case 0x0045:
			strcpy(usage_string, "Vbrz"); break;
		case 0x0046:
			strcpy(usage_string, "Vno"); break;
		case 0x0080:
			strcpy(usage_string, "System-Control"); break;
		case 0x0081:
			strcpy(usage_string, "System-Power-Down"); break;
		case 0x0082:
			strcpy(usage_string, "System-Sleep"); break;
		case 0x0083:
			strcpy(usage_string, "System-Wake-Up"); break;
		case 0x0084:
			strcpy(usage_string, "System-Context-Menu"); break;
		case 0x0085:
			strcpy(usage_string, "System-Main-Menu"); break;
		case 0x0086:
			strcpy(usage_string, "System-App-Menu"); break;
		case 0x0087:
			strcpy(usage_string, "System-Menu-help"); break;
		case 0x0088:
			strcpy(usage_string, "System-Menu-Exit"); break;
		case 0x0089:
			strcpy(usage_string, "System-Menu"); break;
		case 0x008A:
			strcpy(usage_string, "System-Menu-Right"); break;
		case 0x008B:
			strcpy(usage_string, "System-Menu-Left"); break;
		case 0x008C:
			strcpy(usage_string, "System-Menu-Up"); break;
		case 0x008D:
			strcpy(usage_string, "System-Menu-Down"); break;
		case 0x0090:
			strcpy(usage_string, "DPad-Up"); break;
		case 0x0091:
			strcpy(usage_string, "DPad-Down"); break;
		case 0x0092:
			strcpy(usage_string, "DPad-Right"); break;
		case 0x0093:
			strcpy(usage_string, "DPad-Left"); break;
		}
		break;
	case 0x0002:
		strcpy(usage_page_string, "Simulation");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Flight-Simulation-Device"); break;
		case 0x0002:
			strcpy(usage_string, "Automobile-Simulation-Device"); break;
		case 0x0003:
			strcpy(usage_string, "Tank-Simulation-Device"); break;
		case 0x0004:
			strcpy(usage_string, "Spaceship-Simulation-Device"); break;
		case 0x0005:
			strcpy(usage_string, "Submarine-Simulation-Device"); break;
		case 0x0006:
			strcpy(usage_string, "Sailing-Simulation-Device"); break;
		case 0x0007:
			strcpy(usage_string, "Motorcycle-Simulation-Device"); break;
		case 0x0008:
			strcpy(usage_string, "Sports-Simulation-Device"); break;
		case 0x0009:
			strcpy(usage_string, "Airplane-Simulation-Device"); break;
		case 0x000A:
			strcpy(usage_string, "Helicopter-Simulation-Device"); break;
		case 0x000B:
			strcpy(usage_string, "Magic-Carpet-Simulation-Device"); break;
		case 0x000C:
			strcpy(usage_string, "Bicycle-Simulation-Device"); break;
		case 0x0020:
			strcpy(usage_string, "Flight-Control-Stick"); break;
		case 0x0021:
			strcpy(usage_string, "Flight-Stick"); break;
		case 0x0022:
			strcpy(usage_string, "Cyclic-Control"); break;
		case 0x0023:
			strcpy(usage_string, "Cyclic-Trim"); break;
		case 0x0024:
			strcpy(usage_string, "Flight-Yoke"); break;
		case 0x0025:
			strcpy(usage_string, "Track-Control"); break;
		case 0x00B0:
			strcpy(usage_string, "Aileron"); break;
		case 0x00B1:
			strcpy(usage_string, "Aileron-Trim"); break;
		case 0x00B2:
			strcpy(usage_string, "Anti-Torque-Control"); break;
		case 0x00B5:
			strcpy(usage_string, "Collective-Control"); break;
		case 0x00B6:
			strcpy(usage_string, "Dive-Brake"); break;
		case 0x00B7:
			strcpy(usage_string, "Electronic-Countermeasures"); break;
		case 0x00B8:
			strcpy(usage_string, "Elevator"); break;
		case 0x00B9:
			strcpy(usage_string, "Elevator-Trim"); break;
		case 0x00BA:
			strcpy(usage_string, "Rudder"); break;
		case 0x00BB:
			strcpy(usage_string, "Throttle"); break;
		case 0x00BC:
			strcpy(usage_string, "Flight-Communications"); break;
		case 0x00BD:
			strcpy(usage_string, "Flare-Release"); break;
		case 0x00BE:
			strcpy(usage_string, "Landing-Gear"); break;
		case 0x00BF:
			strcpy(usage_string, "Toe-Brake"); break;
		case 0x00C0:
			strcpy(usage_string, "Trigger"); break;
		case 0x00C1:
			strcpy(usage_string, "Weapons-Arm"); break;
		case 0x00C2:
			strcpy(usage_string, "Weapons"); break;
		case 0x00C3:
			strcpy(usage_string, "Wing-Flaps"); break;
		case 0x00C4:
			strcpy(usage_string, "Accelerator"); break;
		case 0x00C5:
			strcpy(usage_string, "Brake"); break;
		case 0x00C6:
			strcpy(usage_string, "Clutch"); break;
		case 0x00C7:
			strcpy(usage_string, "Shifter"); break;
		case 0x00C8:
			strcpy(usage_string, "Steering"); break;
		case 0x00C9:
			strcpy(usage_string, "Turret-Direction"); break;
		case 0x00CA:
			strcpy(usage_string, "Barrel-Elevation"); break;
		case 0x00CB:
			strcpy(usage_string, "Dive-Plane"); break;
		case 0x00CC:
			strcpy(usage_string, "Ballast"); break;
		case 0x00CD:
			strcpy(usage_string, "Bicycle-Crank"); break;
		case 0x00CE:
			strcpy(usage_string, "Handle-Bars"); break;
		case 0x00CF:
			strcpy(usage_string, "Front-Brake"); break;
		case 0x00D0:
			strcpy(usage_string, "Rear-Brake"); break;
		}
		break;
	case 0x0003:
		strcpy(usage_page_string, "Virtual-Reality");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Belt"); break;
		case 0x0002:
			strcpy(usage_string, "Body-Suit"); break;
		case 0x0003:
			strcpy(usage_string, "Flexor"); break;
		case 0x0004:
			strcpy(usage_string, "Glove"); break;
		case 0x0005:
			strcpy(usage_string, "Head-Tracker"); break;
		case 0x0006:
			strcpy(usage_string, "Head-Mounted-Display"); break;
		case 0x0007:
			strcpy(usage_string, "Hand-Tracker"); break;
		case 0x0008:
			strcpy(usage_string, "Oculometer"); break;
		case 0x0009:
			strcpy(usage_string, "Vest"); break;
		case 0x000A:
			strcpy(usage_string, "Animatronic-Device"); break;
		case 0x0020:
			strcpy(usage_string, "Stereo-Enable"); break;
		case 0x0021:
			strcpy(usage_string, "Display-Enable"); break;
		}
		break;
	case 0x0004:
		strcpy(usage_page_string, "Sport");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Baseball-Bat"); break;
		case 0x0002:
			strcpy(usage_string, "Golf-Club"); break;
		case 0x0003:
			strcpy(usage_string, "Rowing-Machine"); break;
		case 0x0004:
			strcpy(usage_string, "Treadmill"); break;
		case 0x0030:
			strcpy(usage_string, "Oar"); break;
		case 0x0031:
			strcpy(usage_string, "Slope"); break;
		case 0x0032:
			strcpy(usage_string, "Rate"); break;
		case 0x0033:
			strcpy(usage_string, "Stick-Speed"); break;
		case 0x0034:
			strcpy(usage_string, "Stick-Face-Angle"); break;
		case 0x0035:
			strcpy(usage_string, "Stick-Heel-Or-Toe"); break;
		case 0x0036:
			strcpy(usage_string, "Stick-Follow-Through"); break;
		case 0x0037:
			strcpy(usage_string, "Stick-Tempo"); break;
		case 0x0038:
			strcpy(usage_string, "Stick-Type"); break;
		case 0x0039:
			strcpy(usage_string, "Stick-Height"); break;
		case 0x0050:
			strcpy(usage_string, "Putter"); break;
		case 0x0051:
			strcpy(usage_string, "1-Iron"); break;
		case 0x0052:
			strcpy(usage_string, "2-Iron"); break;
		case 0x0053:
			strcpy(usage_string, "3-Iron"); break;
		case 0x0054:
			strcpy(usage_string, "4-Iron"); break;
		case 0x0055:
			strcpy(usage_string, "5-Iron"); break;
		case 0x0056:
			strcpy(usage_string, "6-Iron"); break;
		case 0x0057:
			strcpy(usage_string, "7-Iron"); break;
		case 0x0058:
			strcpy(usage_string, "8-Iron"); break;
		case 0x0059:
			strcpy(usage_string, "9-Iron"); break;
		case 0x005A:
			strcpy(usage_string, "10-Iron"); break;
		case 0x005B:
			strcpy(usage_string, "11-Iron"); break;
		case 0x005C:
			strcpy(usage_string, "Sand-Wedge"); break;
		case 0x005D:
			strcpy(usage_string, "Loft-Wedge"); break;
		case 0x005E:
			strcpy(usage_string, "Power-Wedge"); break;
		case 0x005F:
			strcpy(usage_string, "1-Wood"); break;
		case 0x0060:
			strcpy(usage_string, "3-Wood"); break;
		case 0x0061:
			strcpy(usage_string, "5-Wood"); break;
		case 0x0062:
			strcpy(usage_string, "7-Wood"); break;
		case 0x0063:
			strcpy(usage_string, "9-Wood"); break;
		}
		break;
	case 0x0005:
		strcpy(usage_page_string, "Game");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "3D-Game-Controller"); break;
		case 0x0002:
			strcpy(usage_string, "Pinball-Device"); break;
		case 0x0003:
			strcpy(usage_string, "Gun"); break;
		case 0x0020:
			strcpy(usage_string, "Point-of-View"); break;
		case 0x0021:
			strcpy(usage_string, "Turn-Right-Or-Left"); break;
		case 0x0022:
			strcpy(usage_string, "Pitch-Up-Or-Down"); break;
		case 0x0023:
			strcpy(usage_string, "Roll-Right-Or-Left"); break;
		case 0x0024:
			strcpy(usage_string, "Move-Right-Or-Left"); break;
		case 0x0025:
			strcpy(usage_string, "Move-Forward-Or-Backward"); break;
		case 0x0026:
			strcpy(usage_string, "Move-Up-Or-Down"); break;
		case 0x0027:
			strcpy(usage_string, "Lean-Right-Or-Left"); break;
		case 0x0029:
			strcpy(usage_string, "Height-Of-POV"); break;
		case 0x002A:
			strcpy(usage_string, "Flipper"); break;
		case 0x002B:
			strcpy(usage_string, "Secondary-Flipper"); break;
		case 0x002C:
			strcpy(usage_string, "Bump"); break;
		case 0x002D:
			strcpy(usage_string, "New-Game"); break;
		case 0x002E:
			strcpy(usage_string, "Shoot-Ball"); break;
		case 0x002F:
			strcpy(usage_string, "Player"); break;
		case 0x0030:
			strcpy(usage_string, "Gun-Bolt"); break;
		case 0x0031:
			strcpy(usage_string, "Gun-Clip"); break;
		case 0x0032:
			strcpy(usage_string, "Gun"); break;
		case 0x0033:
			strcpy(usage_string, "Gun-Single-Shot"); break;
		case 0x0034:
			strcpy(usage_string, "Gun-Burst"); break;
		case 0x0035:
			strcpy(usage_string, "Gun-Automatic"); break;
		case 0x0036:
			strcpy(usage_string, "Gun-Safety"); break;
		case 0x0037:
			strcpy(usage_string, "Gamepad-Fire-Or-Jump"); break;
		case 0x0039:
			strcpy(usage_string, "Gamepad-Trigger"); break;
		}
		break;
	case 0x0007:
		strcpy(usage_page_string, "Keyboard-or-Keypad"); 
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Error-Roll-Over"); break;
		case 0x0002:
			strcpy(usage_string, "POST-Fail"); break;
		case 0x0003:
			strcpy(usage_string, "Error-Undefined"); break;
		case 0x0004:
			strcpy(usage_string, "A"); break;
		case 0x0005:
			strcpy(usage_string, "B"); break;
		case 0x0006:
			strcpy(usage_string, "C"); break;
		case 0x0007:
			strcpy(usage_string, "D"); break;
		case 0x0008:
			strcpy(usage_string, "E"); break;
		case 0x0009:
			strcpy(usage_string, "F"); break;
		case 0x000A:
			strcpy(usage_string, "G"); break;
		case 0x000B:
			strcpy(usage_string, "H"); break;
		case 0x000C:
			strcpy(usage_string, "I"); break;
		case 0x000D:
			strcpy(usage_string, "J"); break;
		case 0x000E:
			strcpy(usage_string, "K"); break;
		case 0x000F:
			strcpy(usage_string, "L"); break;
		case 0x0010:
			strcpy(usage_string, "M"); break;
		case 0x0011:
			strcpy(usage_string, "N"); break;
		case 0x0012:
			strcpy(usage_string, "O"); break;
		case 0x0013:
			strcpy(usage_string, "P"); break;
		case 0x0014:
			strcpy(usage_string, "Q"); break;
		case 0x0015:
			strcpy(usage_string, "R"); break;
		case 0x0016:
			strcpy(usage_string, "S"); break;
		case 0x0017:
			strcpy(usage_string, "T"); break;
		case 0x0018:
			strcpy(usage_string, "U"); break;
		case 0x0019:
			strcpy(usage_string, "V"); break;
		case 0x001A:
			strcpy(usage_string, "W"); break;
		case 0x001B:
			strcpy(usage_string, "X"); break;
		case 0x001C:
			strcpy(usage_string, "Y"); break;
		case 0x001D:
			strcpy(usage_string, "Z"); break;
		case 0x001E:
			strcpy(usage_string, "1"); break;
		case 0x001F:
			strcpy(usage_string, "2"); break;
		case 0x0020:
			strcpy(usage_string, "3"); break;
		case 0x0021:
			strcpy(usage_string, "4"); break;
		case 0x0022:
			strcpy(usage_string, "5"); break;
		case 0x0023:
			strcpy(usage_string, "6"); break;
		case 0x0024:
			strcpy(usage_string, "7"); break;
		case 0x0025:
			strcpy(usage_string, "8"); break;
		case 0x0026:
			strcpy(usage_string, "9"); break;
		case 0x0027:
			strcpy(usage_string, "0"); break;
		case 0x0028:
			strcpy(usage_string, "Return"); break;
		case 0x0029:
			strcpy(usage_string, "Escape"); break;
		case 0x002A:
			strcpy(usage_string, "Delete"); break;
		case 0x002B:
			strcpy(usage_string, "Tab"); break;
		case 0x002C:
			strcpy(usage_string, "Spacebar"); break;
		case 0x002D:
			strcpy(usage_string, "Dash"); break;
		case 0x002E:
			strcpy(usage_string, "Equal"); break;
		case 0x002F:
			strcpy(usage_string, "Left-Square-Bracket"); break;
		case 0x0030:
			strcpy(usage_string, "Right-Square-Bracket"); break;
		case 0x0031:
			strcpy(usage_string, "Slash"); break;
		case 0x0032:
			strcpy(usage_string, "Non-US-#"); break;
		case 0x0033:
			strcpy(usage_string, "Semi-Colan"); break;
		case 0x0034:
			strcpy(usage_string, "Single-Quote"); break;
		case 0x0035:
			strcpy(usage_string, "Grave-Accent"); break;
		case 0x0036:
			strcpy(usage_string, "Comma"); break;
		case 0x0037:
			strcpy(usage_string, "Period"); break;
		case 0x0038:
			strcpy(usage_string, "Slash"); break;
		case 0x0039:
			strcpy(usage_string, "Caps-Lock"); break;
		case 0x003A:
			strcpy(usage_string, "F1"); break;
		case 0x003B:
			strcpy(usage_string, "F2"); break;
		case 0x003C:
			strcpy(usage_string, "F3"); break;
		case 0x003D:
			strcpy(usage_string, "F4"); break;
		case 0x003E:
			strcpy(usage_string, "F5"); break;
		case 0x003F:
			strcpy(usage_string, "F6"); break;
		case 0x0040:
			strcpy(usage_string, "F7"); break;
		case 0x0041:
			strcpy(usage_string, "F8"); break;
		case 0x0042:
			strcpy(usage_string, "F9"); break;
		case 0x0043:
			strcpy(usage_string, "F10"); break;
		case 0x0044:
			strcpy(usage_string, "F11"); break;
		case 0x0045:
			strcpy(usage_string, "F12"); break;
		case 0x0046:
			strcpy(usage_string, "Print-Screen"); break;
		case 0x0047:
			strcpy(usage_string, "Scroll-Lock"); break;
		case 0x0048:
			strcpy(usage_string, "Pause"); break;
		case 0x0049:
			strcpy(usage_string, "Insert"); break;
		case 0x004A:
			strcpy(usage_string, "Home"); break;
		case 0x004B:
			strcpy(usage_string, "Page-Up"); break;
		case 0x004C:
			strcpy(usage_string, "Delete-Forward"); break;
		case 0x004D:
			strcpy(usage_string, "End"); break;
		case 0x004E:
			strcpy(usage_string, "Page-Down"); break;
		case 0x004F:
			strcpy(usage_string, "Right-Arrow"); break;
		case 0x0050:
			strcpy(usage_string, "Left-Arrow"); break;
		case 0x0051:
			strcpy(usage_string, "Down-Arrow"); break;
		case 0x0052:
			strcpy(usage_string, "Up-Arrow"); break;
		case 0x0053:
			strcpy(usage_string, "Keypad-NumLock"); break;
		case 0x0054:
			strcpy(usage_string, "Keypad-Slash"); break;
		case 0x0055:
			strcpy(usage_string, "Keypad-Asterisk"); break;
		case 0x0056:
			strcpy(usage_string, "Keypad-Dash"); break;
		case 0x0057:
			strcpy(usage_string, "Keypad-Plus"); break;
		case 0x0058:
			strcpy(usage_string, "Keypad-Enter"); break;
		case 0x0059:
			strcpy(usage_string, "Keypad-1"); break;
		case 0x005A:
			strcpy(usage_string, "Keypad-2"); break;
		case 0x005B:
			strcpy(usage_string, "Keypad-3"); break;
		case 0x005C:
			strcpy(usage_string, "Keypad-4"); break;
		case 0x005D:
			strcpy(usage_string, "Keypad-5"); break;
		case 0x005E:
			strcpy(usage_string, "Keypad-6"); break;
		case 0x005F:
			strcpy(usage_string, "Keypad-7"); break;
		case 0x0060:
			strcpy(usage_string, "Keypad-8"); break;
		case 0x0061:
			strcpy(usage_string, "Keypad-9"); break;
		case 0x0062:
			strcpy(usage_string, "Keypad-0"); break;
		case 0x0063:
			strcpy(usage_string, "Keypad-Period"); break;
		case 0x0064:
			strcpy(usage_string, "Non-US-Backslash"); break;
		case 0x0065:
			strcpy(usage_string, "-Application-"); break;
		case 0x0066:
			strcpy(usage_string, "-Power-"); break;
		case 0x0067:
			strcpy(usage_string, "Keypad-Equal"); break;
		case 0x0068:
			strcpy(usage_string, "F13"); break;
		case 0x0069:
			strcpy(usage_string, "F14"); break;
		case 0x006A:
			strcpy(usage_string, "F15"); break;
		case 0x006B:
			strcpy(usage_string, "F16"); break;
		case 0x006C:
			strcpy(usage_string, "F17"); break;
		case 0x006D:
			strcpy(usage_string, "F18"); break;
		case 0x006E:
			strcpy(usage_string, "F19"); break;
		case 0x006F:
			strcpy(usage_string, "F20"); break;
		case 0x0070:
			strcpy(usage_string, "F21"); break;
		case 0x0071:
			strcpy(usage_string, "F22"); break;
		case 0x0072:
			strcpy(usage_string, "F23"); break;
		case 0x0073:
			strcpy(usage_string, "F24"); break;
		case 0x0074:
			strcpy(usage_string, "Execute"); break;
		case 0x0075:
			strcpy(usage_string, "Help"); break;
		case 0x0076:
			strcpy(usage_string, "Menu"); break;
		case 0x0077:
			strcpy(usage_string, "Select"); break;
		case 0x0078:
			strcpy(usage_string, "Stop"); break;
		case 0x0079:
			strcpy(usage_string, "Again"); break;
		case 0x007A:
			strcpy(usage_string, "Undo"); break;
		case 0x007B:
			strcpy(usage_string, "Cut"); break;
		case 0x007C:
			strcpy(usage_string, "Copy"); break;
		case 0x007D:
			strcpy(usage_string, "Paste"); break;
		case 0x007E:
			strcpy(usage_string, "Find"); break;
		case 0x007F:
			strcpy(usage_string, "Mute"); break;
		case 0x0080:
			strcpy(usage_string, "Volume-Up"); break;
		case 0x0081:
			strcpy(usage_string, "Volume-Down"); break;
		case 0x0082:
			strcpy(usage_string, "Locking-Caps-Lock"); break;
		case 0x0083:
			strcpy(usage_string, "Locking-Num-Lock"); break;
		case 0x0084:
			strcpy(usage_string, "Locking-Scroll-Lock"); break;
		case 0x0085:
			strcpy(usage_string, "Keypad-Comma"); break;
		case 0x0086:
			strcpy(usage_string, "Keypad-Equal-Sign-for-AS-400"); break;
		case 0x0087:
			strcpy(usage_string, "International1"); break;
		case 0x0088:
			strcpy(usage_string, "International2"); break;
		case 0x0089:
			strcpy(usage_string, "International3"); break;
		case 0x008A:
			strcpy(usage_string, "International4"); break;
		case 0x008B:
			strcpy(usage_string, "International5"); break;
		case 0x008C:
			strcpy(usage_string, "International6"); break;
		case 0x008D:
			strcpy(usage_string, "International7"); break;
		case 0x008E:
			strcpy(usage_string, "International8"); break;
		case 0x008F:
			strcpy(usage_string, "International9"); break;
		case 0x0090:
			strcpy(usage_string, "LANG1"); break;
		case 0x0091:
			strcpy(usage_string, "LANG2"); break;
		case 0x0092:
			strcpy(usage_string, "LANG3"); break;
		case 0x0093:
			strcpy(usage_string, "LANG4"); break;
		case 0x0094:
			strcpy(usage_string, "LANG5"); break;
		case 0x0095:
			strcpy(usage_string, "LANG6"); break;
		case 0x0096:
			strcpy(usage_string, "LANG7"); break;
		case 0x0097:
			strcpy(usage_string, "LANG8"); break;
		case 0x0098:
			strcpy(usage_string, "LANG9"); break;
		case 0x0099:
			strcpy(usage_string, "Alternate-Erase"); break;
		case 0x009A:
			strcpy(usage_string, "SysReq-or-Attention"); break;
		case 0x009B:
			strcpy(usage_string, "Cancel"); break;
		case 0x009C:
			strcpy(usage_string, "Clear"); break;
		case 0x009D:
			strcpy(usage_string, "Prior"); break;
		case 0x009E:
			strcpy(usage_string, "Return"); break;
		case 0x009F:
			strcpy(usage_string, "-Separator-"); break;
		case 0x00A0:
			strcpy(usage_string, "Out"); break;
		case 0x00A1:
			strcpy(usage_string, "Oper"); break;
		case 0x00A2:
			strcpy(usage_string, "Clear-or-Again"); break;
		case 0x00A3:
			strcpy(usage_string, "CrSel-or-Props"); break;
		case 0x00A4:
			strcpy(usage_string, "ExSel"); break;
		case 0x00E0:
			strcpy(usage_string, "Left-Control"); break;
		case 0x00E1:
			strcpy(usage_string, "Left-Shift"); break;
		case 0x00E2:
			strcpy(usage_string, "Left-Alt"); break;
		case 0x00E3:
			strcpy(usage_string, "Left-GUI"); break;
		case 0x00E4:
			strcpy(usage_string, "Right-Control"); break;
		case 0x00E5:
			strcpy(usage_string, "Right-Shift"); break;
		case 0x00E6:
			strcpy(usage_string, "Right-Alt"); break;
		case 0x00E7:
			strcpy(usage_string, "Right-GUI"); break;
		}
		break;
	case 0x0008:
		strcpy(usage_page_string, "LED"); 
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Num-Lock"); break;
		case 0x0002:
			strcpy(usage_string, "Caps-Lock"); break;
		case 0x0003:
			strcpy(usage_string, "Scroll-Lock"); break;
		case 0x0004:
			strcpy(usage_string, "Compose"); break;
		case 0x0005:
			strcpy(usage_string, "Kana"); break;
		case 0x0006:
			strcpy(usage_string, "Power"); break;
		case 0x0007:
			strcpy(usage_string, "Shift"); break;
		case 0x0008:
			strcpy(usage_string, "DoNotDisturb"); break;
		case 0x0009:
			strcpy(usage_string, "Mute"); break;
		case 0x000A:
			strcpy(usage_string, "Tone-Enable"); break;
		case 0x000B:
			strcpy(usage_string, "High-Cut-Filter"); break;
		case 0x000C:
			strcpy(usage_string, "Low-Cut-Filter"); break;
		case 0x000D:
			strcpy(usage_string, "Equalizer-Enable"); break;
		case 0x000E:
			strcpy(usage_string, "Sound-Field-On"); break;
		case 0x000F:
			strcpy(usage_string, "Surround-On"); break;
		case 0x0010:
			strcpy(usage_string, "Repeat"); break;
		case 0x0011:
			strcpy(usage_string, "Stereo"); break;
		case 0x0012:
			strcpy(usage_string, "Sampling-Rate-Detect"); break;
		case 0x0013:
			strcpy(usage_string, "Spinning"); break;
		case 0x0014:
			strcpy(usage_string, "CAV"); break;
		case 0x0015:
			strcpy(usage_string, "CLV"); break;
		case 0x0016:
			strcpy(usage_string, "Recording-Format-Detect"); break;
		case 0x0017:
			strcpy(usage_string, "Off-Hook"); break;
		case 0x0018:
			strcpy(usage_string, "Ring"); break;
		case 0x0019:
			strcpy(usage_string, "Message-Waiting"); break;
		case 0x001A:
			strcpy(usage_string, "Data-Mode"); break;
		case 0x001B:
			strcpy(usage_string, "Battery-Operation"); break;
		case 0x001C:
			strcpy(usage_string, "Battery-OK"); break;
		case 0x001D:
			strcpy(usage_string, "Battery-Low"); break;
		case 0x001E:
			strcpy(usage_string, "Speaker"); break;
		case 0x001F:
			strcpy(usage_string, "Headset"); break;
		case 0x0020:
			strcpy(usage_string, "Hold"); break;
		case 0x0021:
			strcpy(usage_string, "Microphone"); break;
		case 0x0022:
			strcpy(usage_string, "Coverage"); break;
		case 0x0023:
			strcpy(usage_string, "Night-Mode"); break;
		case 0x0024:
			strcpy(usage_string, "Send-Calls"); break;
		case 0x0025:
			strcpy(usage_string, "Call-Pickup"); break;
		case 0x0026:
			strcpy(usage_string, "Conference"); break;
		case 0x0027:
			strcpy(usage_string, "Stand-By"); break;
		case 0x0028:
			strcpy(usage_string, "Camera-On"); break;
		case 0x0029:
			strcpy(usage_string, "Camera-Off"); break;
		case 0x002A:
			strcpy(usage_string, "On-Line"); break;
		case 0x002B:
			strcpy(usage_string, "Off-Line"); break;
		case 0x002C:
			strcpy(usage_string, "Busy"); break;
		case 0x002D:
			strcpy(usage_string, "Ready"); break;
		case 0x002E:
			strcpy(usage_string, "Paper-Out"); break;
		case 0x002F:
			strcpy(usage_string, "Paper-Jam"); break;
		case 0x0030:
			strcpy(usage_string, "Remote"); break;
		case 0x0031:
			strcpy(usage_string, "Forward"); break;
		case 0x0032:
			strcpy(usage_string, "Reverse"); break;
		case 0x0033:
			strcpy(usage_string, "Stop"); break;
		case 0x0034:
			strcpy(usage_string, "Rewind"); break;
		case 0x0035:
			strcpy(usage_string, "Fast-Forward"); break;
		case 0x0036:
			strcpy(usage_string, "Play"); break;
		case 0x0037:
			strcpy(usage_string, "Pause"); break;
		case 0x0038:
			strcpy(usage_string, "Record"); break;
		case 0x0039:
			strcpy(usage_string, "Error"); break;
		case 0x003A:
			strcpy(usage_string, "Usage"); break;
		case 0x003B:
			strcpy(usage_string, "Usage-In-Use-Indicator"); break;
		case 0x003C:
			strcpy(usage_string, "Usage-Multi-Mode-indicator"); break;
		case 0x003D:
			strcpy(usage_string, "Indicator-On"); break;
		case 0x003E:
			strcpy(usage_string, "Indicator-Flash"); break;
		case 0x003F:
			strcpy(usage_string, "Indicator-Slow-Blink"); break;
		case 0x0040:
			strcpy(usage_string, "Indicator-Fast-Blink"); break;
		case 0x0041:
			strcpy(usage_string, "Indicator-Off"); break;
		case 0x0042:
			strcpy(usage_string, "Flash-On-Time"); break;
		case 0x0043:
			strcpy(usage_string, "Slow-Blink-On-Time"); break;
		case 0x0044:
			strcpy(usage_string, "Slow-Blink-Off-Time"); break;
		case 0x0045:
			strcpy(usage_string, "Fast-Blink-On-Time"); break;
		case 0x0046:
			strcpy(usage_string, "Fast-Blink-Off-Time"); break;
		case 0x0047:
			strcpy(usage_string, "Usage-Indicator-Color"); break;
		case 0x0048:
			strcpy(usage_string, "Indicator-Red"); break;
		case 0x0049:
			strcpy(usage_string, "Indicator-Green"); break;
		case 0x004A:
			strcpy(usage_string, "Indicator-Amber"); break;
		case 0x004B:
			strcpy(usage_string, "Generic-Indicator"); break;
		case 0x004C:
			strcpy(usage_string, "System-Suspend"); break;
		case 0x004D:
			strcpy(usage_string, "External-Power-Connected"); break;
		}
		break;
	case 0x0009:
		*usage_string = '\0';
		strcpy(usage_page_string, "Button"); 
		break;
	case 0x000C:
		strcpy(usage_page_string, "Consumer");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Consumer-Control"); break;
		case 0x0002:
			strcpy(usage_string, "Numeric-KeyPad"); break;
		case 0x0003:
			strcpy(usage_string, "Programmable-Buttons"); break;
		case 0x0020:
			strcpy(usage_string, "Plus-10"); break;
		case 0x0021:
			strcpy(usage_string, "Plus-100"); break;
		case 0x0022:
			strcpy(usage_string, "AM-Or-PM"); break;
		case 0x0030:
			strcpy(usage_string, "Power"); break;
		case 0x0031:
			strcpy(usage_string, "Reset"); break;
		case 0x0032:
			strcpy(usage_string, "Sleep"); break;
		case 0x0033:
			strcpy(usage_string, "Sleep-After"); break;
		case 0x0034:
			strcpy(usage_string, "Sleep-Mode"); break;
		case 0x0035:
			strcpy(usage_string, "Illumination"); break;
		case 0x0036:
			strcpy(usage_string, "Function-Buttons"); break;
		case 0x0040:
			strcpy(usage_string, "Menu"); break;
		case 0x0041:
			strcpy(usage_string, "Menu-Pick"); break;
		case 0x0042:
			strcpy(usage_string, "Menu-Up"); break;
		case 0x0043:
			strcpy(usage_string, "Menu-Down"); break;
		case 0x0044:
			strcpy(usage_string, "Menu-Left"); break;
		case 0x0045:
			strcpy(usage_string, "Menu-Right"); break;
		case 0x0046:
			strcpy(usage_string, "Menu-Escape"); break;
		case 0x0047:
			strcpy(usage_string, "Menu-Value-Increase"); break;
		case 0x0048:
			strcpy(usage_string, "Menu-Value-Decrease"); break;
		case 0x0060:
			strcpy(usage_string, "Data-On-Screen"); break;
		case 0x0061:
			strcpy(usage_string, "Closed-Caption"); break;
		case 0x0062:
			strcpy(usage_string, "Closed-Caption-Select"); break;
		case 0x0063:
			strcpy(usage_string, "VCR-Or-TV"); break;
		case 0x0064:
			strcpy(usage_string, "Broadcast-Mode"); break;
		case 0x0065:
			strcpy(usage_string, "Snapshot"); break;
		case 0x0066:
			strcpy(usage_string, "Still"); break;
		case 0x0080:
			strcpy(usage_string, "Selection"); break;
		case 0x0081:
			strcpy(usage_string, "Assign"); break;
		case 0x0082:
			strcpy(usage_string, "Mode-Step"); break;
		case 0x0083:
			strcpy(usage_string, "Recall-Last"); break;
		case 0x0084:
			strcpy(usage_string, "Enter-Channel"); break;
		case 0x0085:
			strcpy(usage_string, "Order-Movie"); break;
		case 0x0086:
			strcpy(usage_string, "Channel"); break;
		case 0x0087:
			strcpy(usage_string, "Media-Selection"); break;
		case 0x0088:
			strcpy(usage_string, "Media-Select-Computer"); break;
		case 0x0089:
			strcpy(usage_string, "Media-Select-TV"); break;
		case 0x008A:
			strcpy(usage_string, "Media-Select-WWW"); break;
		case 0x008B:
			strcpy(usage_string, "Media-Select-DVD"); break;
		case 0x008E:
			strcpy(usage_string, "Media-Select-Video-Phone"); break;
		case 0x008F:
			strcpy(usage_string, "Media-Select-Games"); break;
		case 0x0090:
			strcpy(usage_string, "Media-Select-Messages"); break;
		case 0x0091:
			strcpy(usage_string, "Media-Select-CD"); break;
		case 0x0092:
			strcpy(usage_string, "Media-Select-VCR"); break;
		case 0x0093:
			strcpy(usage_string, "Media-Select-Tuner"); break;
		case 0x0094:
			strcpy(usage_string, "Quit"); break;
		case 0x0095:
			strcpy(usage_string, "Help"); break;
		case 0x0096:
			strcpy(usage_string, "Media-Select-Tape"); break;
		case 0x0097:
			strcpy(usage_string, "Media-Select-Cable"); break;
		case 0x0098:
			strcpy(usage_string, "Media-Select-Satellite"); break;
		case 0x0099:
			strcpy(usage_string, "Media-Select-Security"); break;
		case 0x009A:
			strcpy(usage_string, "Media-Select-Home"); break;
		case 0x009B:
			strcpy(usage_string, "Media-Select-Call"); break;
		case 0x009C:
			strcpy(usage_string, "Channel-Increment"); break;
		case 0x009D:
			strcpy(usage_string, "Channel-Decrement"); break;
		case 0x009E:
			strcpy(usage_string, "Media"); break;
		case 0x00A0:
			strcpy(usage_string, "VCR-Plus"); break;
		case 0x00A1:
			strcpy(usage_string, "Once"); break;
		case 0x00A2:
			strcpy(usage_string, "Daily"); break;
		case 0x00A3:
			strcpy(usage_string, "Weekly"); break;
		case 0x00A4:
			strcpy(usage_string, "Monthly"); break;
		case 0x00B0:
			strcpy(usage_string, "Play"); break;
		case 0x00B1:
			strcpy(usage_string, "Pause"); break;
		case 0x00B2:
			strcpy(usage_string, "Record"); break;
		case 0x00B3:
			strcpy(usage_string, "Fast-Forward"); break;
		case 0x00B4:
			strcpy(usage_string, "Rewind"); break;
		case 0x00B5:
			strcpy(usage_string, "Scan-Next-Track"); break;
		case 0x00B6:
			strcpy(usage_string, "Scan-Previous-Track"); break;
		case 0x00B7:
			strcpy(usage_string, "Stop"); break;
		case 0x00B8:
			strcpy(usage_string, "Eject"); break;
		case 0x00B9:
			strcpy(usage_string, "Random-Play"); break;
		case 0x00BA:
			strcpy(usage_string, "Select-Disc"); break;
		case 0x00BB:
			strcpy(usage_string, "Enter-Disc"); break;
		case 0x00BC:
			strcpy(usage_string, "Repeat"); break;
		case 0x00BD:
			strcpy(usage_string, "Tracking"); break;
		case 0x00BE:
			strcpy(usage_string, "Track-Normal"); break;
		case 0x00BF:
			strcpy(usage_string, "Slow-Tracking"); break;
		case 0x00C0:
			strcpy(usage_string, "Frame-Forward"); break;
		case 0x00C1:
			strcpy(usage_string, "Frame-Back"); break;
		case 0x00C2:
			strcpy(usage_string, "Mark"); break;
		case 0x00C3:
			strcpy(usage_string, "Clear-Mark"); break;
		case 0x00C4:
			strcpy(usage_string, "Repeat-From-Mark"); break;
		case 0x00C5:
			strcpy(usage_string, "Return-To-Mark"); break;
		case 0x00C6:
			strcpy(usage_string, "Search-Mark-Forward"); break;
		case 0x00C7:
			strcpy(usage_string, "Search-Mark-Backwards"); break;
		case 0x00C8:
			strcpy(usage_string, "Counter-Reset"); break;
		case 0x00C9:
			strcpy(usage_string, "Show-Counter"); break;
		case 0x00CA:
			strcpy(usage_string, "Tracking-Increment"); break;
		case 0x00CB:
			strcpy(usage_string, "Tracking-Decrement"); break;
		case 0x00CC:
			strcpy(usage_string, "Stop-Or-Eject"); break;
		case 0x00CD:
			strcpy(usage_string, "Play-Or-Pause"); break;
		case 0x00CE:
			strcpy(usage_string, "Play-Or-Skip"); break;
		case 0x00E0:
			strcpy(usage_string, "Volume"); break;
		case 0x00E1:
			strcpy(usage_string, "Balance"); break;
		case 0x00E2:
			strcpy(usage_string, "Mute"); break;
		case 0x00E3:
			strcpy(usage_string, "Bass"); break;
		case 0x00E4:
			strcpy(usage_string, "Treble"); break;
		case 0x00E5:
			strcpy(usage_string, "Bass-Boost"); break;
		case 0x00E6:
			strcpy(usage_string, "Surround-Mode"); break;
		case 0x00E7:
			strcpy(usage_string, "Loudness"); break;
		case 0x00E8:
			strcpy(usage_string, "MPX"); break;
		case 0x00E9:
			strcpy(usage_string, "Volume-Increment"); break;
		case 0x00EA:
			strcpy(usage_string, "Volume-Decrement"); break;
		case 0x00F0:
			strcpy(usage_string, "Speed"); break;
		case 0x00F1:
			strcpy(usage_string, "Playback-Speed"); break;
		case 0x00F2:
			strcpy(usage_string, "Standard-Play"); break;
		case 0x00F3:
			strcpy(usage_string, "Long-Play"); break;
		case 0x00F4:
			strcpy(usage_string, "Extended-Play"); break;
		case 0x00F5:
			strcpy(usage_string, "Slow"); break;
		case 0x0100:
			strcpy(usage_string, "Fan-Enable"); break;
		case 0x0101:
			strcpy(usage_string, "Fan-Speed"); break;
		case 0x0102:
			strcpy(usage_string, "Light-Enable"); break;
		case 0x0103:
			strcpy(usage_string, "Light-Illumination-Level"); break;
		case 0x0104:
			strcpy(usage_string, "Climate-Control-Enable"); break;
		case 0x0105:
			strcpy(usage_string, "Room-Temperature"); break;
		case 0x0106:
			strcpy(usage_string, "Security-Enable"); break;
		case 0x0107:
			strcpy(usage_string, "Fire-Alarm"); break;
		case 0x0108:
			strcpy(usage_string, "Police-Alarm"); break;
		case 0x0150:
			strcpy(usage_string, "Balance-Right"); break;
		case 0x0151:
			strcpy(usage_string, "Balance-Left"); break;
		case 0x0152:
			strcpy(usage_string, "Bass-Increment"); break;
		case 0x0153:
			strcpy(usage_string, "Bass-Decrement"); break;
		case 0x0154:
			strcpy(usage_string, "Treble-Increment"); break;
		case 0x0155:
			strcpy(usage_string, "Treble-Decrement"); break;
		case 0x0160:
			strcpy(usage_string, "Speaker-System"); break;
		case 0x0161:
			strcpy(usage_string, "Channel-Left"); break;
		case 0x0162:
			strcpy(usage_string, "Channel-Right"); break;
		case 0x0164:
			strcpy(usage_string, "Channel-Front"); break;
		case 0x0165:
			strcpy(usage_string, "Channel-Center-Front"); break;
		case 0x0166:
			strcpy(usage_string, "Channel-Side"); break;
		case 0x0167:
			strcpy(usage_string, "Channel-Surround"); break;
		case 0x0168:
			strcpy(usage_string, "Channel-Low-Frequency-Enhancement"); break;
		case 0x0169:
			strcpy(usage_string, "Channel-Top"); break;
		case 0x016A:
			strcpy(usage_string, "Channel-Unknown"); break;
		case 0x0170:
			strcpy(usage_string, "Sub-Channel"); break;
		case 0x0171:
			strcpy(usage_string, "Sub-Channel-Increment"); break;
		case 0x0172:
			strcpy(usage_string, "Sub-Channel-Decrement"); break;
		case 0x0173:
			strcpy(usage_string, "Alternate-Audio-Increment"); break;
		case 0x0174:
			strcpy(usage_string, "Alternate-Audio-Decrement"); break;
		case 0x0180:
			strcpy(usage_string, "Application-Launch-Buttons"); break;
		case 0x0181:
			strcpy(usage_string, "AL-Launch-Button-Configuration-Tool"); break;
		case 0x0182:
			strcpy(usage_string, "AL-Programmable-Button-Configuration"); break;
		case 0x0183:
			strcpy(usage_string, "AL-Consumer-Control-Configuration"); break;
		case 0x0184:
			strcpy(usage_string, "AL-Word-Processor"); break;
		case 0x0185:
			strcpy(usage_string, "AL-Text-Editor"); break;
		case 0x0186:
			strcpy(usage_string, "AL-Spreadsheet"); break;
		case 0x0187:
			strcpy(usage_string, "AL-Graphics-Editor"); break;
		case 0x0188:
			strcpy(usage_string, "AL-Presentation-App"); break;
		case 0x0189:
			strcpy(usage_string, "AL-Database-App"); break;
		case 0x018A:
			strcpy(usage_string, "AL-Email-Reader"); break;
		case 0x018B:
			strcpy(usage_string, "AL-Newsreader"); break;
		case 0x018C:
			strcpy(usage_string, "AL-Voicemail"); break;
		case 0x018D:
			strcpy(usage_string, "AL-Contacts-Or-AddressBook"); break;
		case 0x018E:
			strcpy(usage_string, "AL-Calendar-Or-Schedule"); break;
		case 0x018F:
			strcpy(usage_string, "AL-Task-Or-Project-Manager"); break;
		case 0x0190:
			strcpy(usage_string, "AL-Log-Or-Journal-Or-Timecard"); break;
		case 0x0191:
			strcpy(usage_string, "AL-Checkbook-Or-Finance"); break;
		case 0x0192:
			strcpy(usage_string, "AL-Calculator"); break;
		case 0x0193:
			strcpy(usage_string, "AL-A-Or-V-Capture-Or-Playback"); break;
		case 0x0194:
			strcpy(usage_string, "AL-Local-Machine-Browser"); break;
		case 0x0195:
			strcpy(usage_string, "AL-LAN-Or-WAN-Browser"); break;
		case 0x0196:
			strcpy(usage_string, "AL-Internet-Browser"); break;
		case 0x0197:
			strcpy(usage_string, "AL-Remote-Networking-Or-ISP-Connect"); break;
		case 0x0198:
			strcpy(usage_string, "AL-Network-Conference"); break;
		case 0x0199:
			strcpy(usage_string, "AL-Network-Chat"); break;
		case 0x019A:
			strcpy(usage_string, "AL-Telephony-Or-Dialer"); break;
		case 0x019B:
			strcpy(usage_string, "AL-Logon"); break;
		case 0x019C:
			strcpy(usage_string, "AL-Logoff"); break;
		case 0x019D:
			strcpy(usage_string, "AL-Logon-Or-Logoff"); break;
		case 0x019E:
			strcpy(usage_string, "AL-Terminal-Lock-Or-Screensaver"); break;
		case 0x019F:
			strcpy(usage_string, "AL-Control-Panel"); break;
		case 0x01A0:
			strcpy(usage_string, "AL-Command-Line-Processor-Or-Run"); break;
		case 0x01A1:
			strcpy(usage_string, "AL-Process-Or-Task-Manager"); break;
		case 0x01A2:
			strcpy(usage_string, "AL"); break;
		case 0x01A3:
			strcpy(usage_string, "AL-Next-Task-Or-Application"); break;
		case 0x01A4:
			strcpy(usage_string, "AL-Previous-Task-Or-Application"); break;
		case 0x01A5:
			strcpy(usage_string, "AL-Preemptive-Halt-Task-Or-Application"); break;
		case 0x0200:
			strcpy(usage_string, "Generic-GUI-Application-Controls"); break;
		case 0x0201:
			strcpy(usage_string, "AC-New"); break;
		case 0x0202:
			strcpy(usage_string, "AC-Open"); break;
		case 0x0203:
			strcpy(usage_string, "AC-Close"); break;
		case 0x0204:
			strcpy(usage_string, "AC-Exit"); break;
		case 0x0205:
			strcpy(usage_string, "AC-Maximize"); break;
		case 0x0206:
			strcpy(usage_string, "AC-Minimize"); break;
		case 0x0207:
			strcpy(usage_string, "AC-Save"); break;
		case 0x0208:
			strcpy(usage_string, "AC-Print"); break;
		case 0x0209:
			strcpy(usage_string, "AC-Properties"); break;
		case 0x021A:
			strcpy(usage_string, "AC-Undo"); break;
		case 0x021B:
			strcpy(usage_string, "AC-Copy"); break;
		case 0x021C:
			strcpy(usage_string, "AC-Cut"); break;
		case 0x021D:
			strcpy(usage_string, "AC-Paste"); break;
		case 0x021E:
			strcpy(usage_string, "AC"); break;
		case 0x021F:
			strcpy(usage_string, "AC-Find"); break;
		case 0x0220:
			strcpy(usage_string, "AC-Find-and-Replace"); break;
		case 0x0221:
			strcpy(usage_string, "AC-Search"); break;
		case 0x0222:
			strcpy(usage_string, "AC-Go-To"); break;
		case 0x0223:
			strcpy(usage_string, "AC-Home"); break;
		case 0x0224:
			strcpy(usage_string, "AC-Back"); break;
		case 0x0225:
			strcpy(usage_string, "AC-Forward"); break;
		case 0x0226:
			strcpy(usage_string, "AC-Stop"); break;
		case 0x0227:
			strcpy(usage_string, "AC-Refresh"); break;
		case 0x0228:
			strcpy(usage_string, "AC-Previous-Link"); break;
		case 0x0229:
			strcpy(usage_string, "AC-Next-Link"); break;
		case 0x022A:
			strcpy(usage_string, "AC-Bookmarks"); break;
		case 0x022B:
			strcpy(usage_string, "AC-History"); break;
		case 0x022C:
			strcpy(usage_string, "AC-Subscriptions"); break;
		case 0x022D:
			strcpy(usage_string, "AC-Zoom-In"); break;
		case 0x022E:
			strcpy(usage_string, "AC-Zoom-Out"); break;
		case 0x022F:
			strcpy(usage_string, "AC-Zoom"); break;
		case 0x0230:
			strcpy(usage_string, "AC-Full-Screen-View"); break;
		case 0x0231:
			strcpy(usage_string, "AC-Normal-View"); break;
		case 0x0232:
			strcpy(usage_string, "AC-View-Toggle"); break;
		case 0x0233:
			strcpy(usage_string, "AC-Scroll-Up"); break;
		case 0x0234:
			strcpy(usage_string, "AC-Scroll-Down"); break;
		case 0x0235:
			strcpy(usage_string, "AC-Scroll"); break;
		case 0x0236:
			strcpy(usage_string, "AC-Pan-Left"); break;
		case 0x0237:
			strcpy(usage_string, "AC-Pan-Right"); break;
		case 0x0238:
			strcpy(usage_string, "AC-Pan"); break;
		case 0x0239:
			strcpy(usage_string, "AC-New-Window"); break;
		case 0x023A:
			strcpy(usage_string, "AC-Tile-Horizontally"); break;
		case 0x023B:
			strcpy(usage_string, "AC-Tile-Vertically"); break;
		case 0x023C:
			strcpy(usage_string, "AC-Format"); break;
		}
		break;
	case 0x0084:
		strcpy(usage_page_string, "Power-Device-Page");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Name-Index"); break;
		case 0x0002:
			strcpy(usage_string, "Present-Status"); break;
		case 0x0003:
			strcpy(usage_string, "Changed-Status"); break;
		case 0x0004:
			strcpy(usage_string, "Uninterruptible-Power-Supply"); break;
		case 0x0005:
			strcpy(usage_string, "Power-Supply"); break;
		case 0x0010:
			strcpy(usage_string, "Battery-System-power-module"); break;
		case 0x0011:
			strcpy(usage_string, "Battery-System-ID"); break;
		case 0x0012:
			strcpy(usage_string, "Battery"); break;
		case 0x0013:
			strcpy(usage_string, "Battery-ID"); break;
		case 0x0014:
			strcpy(usage_string, "Charger"); break;
		case 0x0015:
			strcpy(usage_string, "Charger-ID"); break;
		case 0x0016:
			strcpy(usage_string, "Power-Converter-power-module"); break;
		case 0x0017:
			strcpy(usage_string, "Power-Converter-ID"); break;
		case 0x0018:
			strcpy(usage_string, "Outlet-System-power-module"); break;
		case 0x0019:
			strcpy(usage_string, "Outlet-System-ID"); break;
		case 0x001A:
			strcpy(usage_string, "Input"); break;
		case 0x001B:
			strcpy(usage_string, "Input-ID"); break;
		case 0x001C:
			strcpy(usage_string, "Output"); break;
		case 0x001D:
			strcpy(usage_string, "Output-ID"); break;
		case 0x001E:
			strcpy(usage_string, "Flow"); break;
		case 0x001F:
			strcpy(usage_string, "Flow-ID"); break;
		case 0x0020:
			strcpy(usage_string, "Outlet"); break;
		case 0x0021:
			strcpy(usage_string, "Outlet-ID"); break;
		case 0x0022:
			strcpy(usage_string, "Gang"); break;
		case 0x0023:
			strcpy(usage_string, "Gang-ID"); break;
		case 0x0024:
			strcpy(usage_string, "Power-Summary"); break;
		case 0x0025:
			strcpy(usage_string, "Power-Summary-ID"); break;
		case 0x0030:
			strcpy(usage_string, "Voltage"); break;
		case 0x0031:
			strcpy(usage_string, "Current"); break;
		case 0x0032:
			strcpy(usage_string, "Frequency"); break;
		case 0x0033:
			strcpy(usage_string, "Apparent-Power"); break;
		case 0x0034:
			strcpy(usage_string, "RMS-Power"); break;
		case 0x0035:
			strcpy(usage_string, "Percent-Load"); break;
		case 0x0036:
			strcpy(usage_string, "Temperature"); break;
		case 0x0037:
			strcpy(usage_string, "Humidity"); break;
		case 0x0038:
			strcpy(usage_string, "Bad-Condition-Count"); break;
		case 0x0040:
			strcpy(usage_string, "Nominal-Voltage"); break;
		case 0x0041:
			strcpy(usage_string, "Nominal-Current"); break;
		case 0x0042:
			strcpy(usage_string, "Nominal-Frequency"); break;
		case 0x0043:
			strcpy(usage_string, "Nominal-Apparent-Power"); break;
		case 0x0044:
			strcpy(usage_string, "Nominal-RMS-Power"); break;
		case 0x0045:
			strcpy(usage_string, "Nominal-Percent-Load"); break;
		case 0x0046:
			strcpy(usage_string, "Nominal-Temperature"); break;
		case 0x0047:
			strcpy(usage_string, "Nominal-Humidity"); break;
		case 0x0050:
			strcpy(usage_string, "Switch-On-Control"); break;
		case 0x0051:
			strcpy(usage_string, "Switch-Off-Control"); break;
		case 0x0052:
			strcpy(usage_string, "Toogle-Sequence-Control"); break;
		case 0x0053:
			strcpy(usage_string, "Min-Transfer-Voltage"); break;
		case 0x0054:
			strcpy(usage_string, "Max-Transfer-Voltage"); break;
		case 0x0055:
			strcpy(usage_string, "Delay-Before-Reboot"); break;
		case 0x0056:
			strcpy(usage_string, "Delay-Before-Startup"); break;
		case 0x0057:
			strcpy(usage_string, "Delay-Before-Shutdown"); break;
		case 0x0058:
			strcpy(usage_string, "Test-Request/Result"); break;
		case 0x0059:
			strcpy(usage_string, "Reset-Request/Result"); break;
		case 0x005A:
			strcpy(usage_string, "Audible-Alarm-Control"); break;
		case 0x0060:
			strcpy(usage_string, "Present"); break;
		case 0x0061:
			strcpy(usage_string, "Good"); break;
		case 0x0062:
			strcpy(usage_string, "Internal-Failure"); break;
		case 0x0063:
			strcpy(usage_string, "Voltage-Out-Of-Range"); break;
		case 0x0064:
			strcpy(usage_string, "Frequency-Out-Of-Range"); break;
		case 0x0065:
			strcpy(usage_string, "Overload"); break;
		case 0x0066:
			strcpy(usage_string, "Over-Charged"); break;
		case 0x0067:
			strcpy(usage_string, "Over-Temperature"); break;
		case 0x0068:
			strcpy(usage_string, "Shutdown-Requested"); break;
		case 0x0069:
			strcpy(usage_string, "Shutdown-Imminent"); break;
		case 0x006B:
			strcpy(usage_string, "On/Off-Switch-Status"); break;
		case 0x006C:
			strcpy(usage_string, "Switchable"); break;
		case 0x006D:
			strcpy(usage_string, "Used"); break;
		case 0x006E:
			strcpy(usage_string, "Boosted"); break;
		case 0x006F:
			strcpy(usage_string, "Bucked"); break;
		case 0x0070:
			strcpy(usage_string, "Initialized"); break;
		case 0x0071:
			strcpy(usage_string, "Tested"); break;
		case 0x0072:
			strcpy(usage_string, "Awaiting-Power"); break;
		case 0x0073:
			strcpy(usage_string, "Communication-Lost"); break;
		case 0x00FD:
			strcpy(usage_string, "Manufacturer-String-Index"); break;
		case 0x00FE:
			strcpy(usage_string, "Product-String-Index"); break;
		case 0x00FF:
			strcpy(usage_string, "Serial-Number-String-Index"); break;
		}
		break;
	case 0x0085:
		strcpy(usage_page_string, "Battery-System-Page");
		switch(usage){
		case 0x0000:
			strcpy(usage_string, "Undefined-Usage"); break;
		case 0x0001:
			strcpy(usage_string, "Mode"); break;
		case 0x0002:
			strcpy(usage_string, "Status"); break;
		case 0x0003:
			strcpy(usage_string, "Alarm-Warning"); break;
		case 0x0004:
			strcpy(usage_string, "Charger-Mode"); break;
		case 0x0005:
			strcpy(usage_string, "Charger-Status"); break;
		case 0x0006:
			strcpy(usage_string, "Charger-Extended-Status"); break;
		case 0x0007:
			strcpy(usage_string, "Selector-State"); break;
		case 0x0008:
			strcpy(usage_string, "Selector-Presets"); break;
		case 0x0009:
			strcpy(usage_string, "Selector-Info"); break;
		case 0x0010:
			strcpy(usage_string, "Mfg-Function-1"); break;
		case 0x0011:
			strcpy(usage_string, "Mfg-Function-2"); break;
		case 0x0012:
			strcpy(usage_string, "Mfg-Function-3"); break;
		case 0x0013:
			strcpy(usage_string, "Mfg-Function-4"); break;
		case 0x0014:
			strcpy(usage_string, "Mfg-Function-5"); break;
		case 0x0015:
			strcpy(usage_string, "Connection-To-System-Management-Bus"); break;
		case 0x0016:
			strcpy(usage_string, "Output-Connection-Status"); break;
		case 0x0017:
			strcpy(usage_string, "Charger-Connection"); break;
		case 0x0018:
			strcpy(usage_string, "Battery-Insertion"); break;
		case 0x0019:
			strcpy(usage_string, "Use-Next"); break;
		case 0x001A:
			strcpy(usage_string, "OK-To-Use"); break;
		case 0x001B:
			strcpy(usage_string, "Battery-Supported"); break;
		case 0x001C:
			strcpy(usage_string, "Selector-Revision"); break;
		case 0x001D:
			strcpy(usage_string, "Charging-Indicator"); break;
		case 0x0028:
			strcpy(usage_string, "Manufacturer-Access"); break;
		case 0x0029:
			strcpy(usage_string, "Remaining-Capacity-Limit"); break;
		case 0x002A:
			strcpy(usage_string, "Remaining-Time-Limit"); break;
		case 0x002B:
			strcpy(usage_string, "At-Rate..."); break;
		case 0x002C:
			strcpy(usage_string, "Capacity-Mode"); break;
		case 0x002D:
			strcpy(usage_string, "Broadcast-To-Charger"); break;
		case 0x002E:
			strcpy(usage_string, "Primary-Battery"); break;
		case 0x002F:
			strcpy(usage_string, "Charge-Controller"); break;
		case 0x0040:
			strcpy(usage_string, "Terminate-Charge"); break;
		case 0x0041:
			strcpy(usage_string, "Terminate-Discharge"); break;
		case 0x0042:
			strcpy(usage_string, "Below-Remaining-Capacity-Limit"); break;
		case 0x0043:
			strcpy(usage_string, "Remaining-Time-Limit-Expired"); break;
		case 0x0044:
			strcpy(usage_string, "Charging"); break;
		case 0x0045:
			strcpy(usage_string, "Discharging"); break;
		case 0x0046:
			strcpy(usage_string, "Fully-Charged"); break;
		case 0x0047:
			strcpy(usage_string, "Fully-Discharged"); break;
		case 0x0048:
			strcpy(usage_string, "Conditioning-Flag"); break;
		case 0x0049:
			strcpy(usage_string, "At-Rate-OK"); break;
		case 0x004A:
			strcpy(usage_string, "SMB-Error-Code"); break;
		case 0x004B:
			strcpy(usage_string, "Need-Replacement"); break;
		case 0x0060:
			strcpy(usage_string, "At-Rate-Time-To-Full"); break;
		case 0x0061:
			strcpy(usage_string, "At-Rate-Time-To-Empty"); break;
		case 0x0062:
			strcpy(usage_string, "Average-Current"); break;
		case 0x0063:
			strcpy(usage_string, "Max-Error"); break;
		case 0x0065:
			strcpy(usage_string, "Absolute-State-Of-Charge"); break;
		case 0x0066:
			strcpy(usage_string, "Remaining-Capacity"); break;
		case 0x0067:
			strcpy(usage_string, "Full-Charge-Capacity"); break;
		case 0x0068:
			strcpy(usage_string, "Run-Time-To-Empty"); break;
		case 0x0069:
			strcpy(usage_string, "Average-Time-To-Empty"); break;
		case 0x006A:
			strcpy(usage_string, "Average-Time-To-Full"); break;
		case 0x006B:
			strcpy(usage_string, "Cycle-Count"); break;
		case 0x0080:
			strcpy(usage_string, "Batt-Pack-Model-Level"); break;
		case 0x0081:
			strcpy(usage_string, "Internal-Charge-Controller"); break;
		case 0x0082:
			strcpy(usage_string, "Primary-Battery-Support"); break;
		case 0x0083:
			strcpy(usage_string, "Design-Capacity"); break;
		case 0x0084:
			strcpy(usage_string, "Specification-Info"); break;
		case 0x0085:
			strcpy(usage_string, "Manufacturer-Date"); break;
		case 0x0086:
			strcpy(usage_string, "Serial-Number"); break;
		case 0x0087:
			strcpy(usage_string, "Manufacturer-Name-Index"); break;
		case 0x0088:
			strcpy(usage_string, "Device-Name-Index"); break;
		case 0x0089:
			strcpy(usage_string, "Device-Chemistry-Index"); break;
		case 0x008A:
			strcpy(usage_string, "Manufacturer-Data"); break;
		case 0x008B:
			strcpy(usage_string, "Rechargable"); break;
		case 0x008C:
			strcpy(usage_string, "Warning-Capacity-Limit"); break;
		case 0x008D:
			strcpy(usage_string, "Capacity-Granularity-1"); break;
		case 0x008E:
			strcpy(usage_string, "Capacity-Granularity-2"); break;
		case 0x008F:
			strcpy(usage_string, "OEM-Information-Index"); break;
		case 0x00C0:
			strcpy(usage_string, "Inhibit-Charge"); break;
		case 0x00C1:
			strcpy(usage_string, "Enable-Polling"); break;
		case 0x00C2:
			strcpy(usage_string, "Reset-To-Zero"); break;
		case 0x00D0:
			strcpy(usage_string, "AC-Present"); break;
		case 0x00D1:
			strcpy(usage_string, "Battery-Present"); break;
		case 0x00D2:
			strcpy(usage_string, "Power-Fail"); break;
		case 0x00D3:
			strcpy(usage_string, "Alarm-Inhibited"); break;
		case 0x00D4:
			strcpy(usage_string, "Thermistor-Under-Range"); break;
		case 0x00D5:
			strcpy(usage_string, "Thermistor-Hot"); break;
		case 0x00D6:
			strcpy(usage_string, "Thermistor-Cold"); break;
		case 0x00D7:
			strcpy(usage_string, "Thermistor-Over-Range"); break;
		case 0x00D8:
			strcpy(usage_string, "Voltage-Out-Of-Range"); break;
		case 0x00D9:
			strcpy(usage_string, "Current-Out-Of-Range"); break;
		case 0x00DA:
			strcpy(usage_string, "Current-Not-Regulated"); break;
		case 0x00DB:
			strcpy(usage_string, "Voltage-Not-Regulated"); break;
		case 0x00DC:
			strcpy(usage_string, "Master-Mode"); break;
		case 0x00F0:
			strcpy(usage_string, "Charger-Support-Selector"); break;
		case 0x00F1:
			strcpy(usage_string, "Charger-Specification"); break;
		case 0x00F2:
			strcpy(usage_string, "Charger-Level-2"); break;
		case 0x00F3:
			strcpy(usage_string, "Charger-Level-3"); break;
		}
		break;
	case 0xFF00:
		*usage_string = '\0';
		strcpy(usage_page_string, "Vendor"); 
		break;
	}
}
