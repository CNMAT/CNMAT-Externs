#include "version.h"

     resource 'vers' (1) {
          0xff,               // Major release: digit before decimal
          0xff,               // Minor release: digits after decimal
          release,               // dev/alpha/beta/release
          0x00,               // Build # (for pre-release)
          0,                  // Region code (US)
          VERSION " (OSX)",        // Major/minor release as short string
          VERSION ",© 2002,3,4,5 CNMAT (OSX Version)"
          };
