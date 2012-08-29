#include "VERSION.h"

     resource 'vers' (1) {
          0xff,               // Major release: digit before decimal
          0xff,               // Minor release: digits after decimal
          release,               // dev/alpha/beta/release
          0x00,               // Build # (for pre-release)
          0,                  // Region code (US)
          VERSION " (Classic or OSX)",        // Major/minor release as short string
          VERSION ", © 2000-04 CNMAT (Classic/OSX Version)"
          };
