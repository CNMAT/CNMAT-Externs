#include "version.h"

     resource 'vers' (1) {
          0xff,               // Major release: digit before decimal
          0xff,               // Minor release: digits after decimal
          release,               // dev/alpha/beta/release
          0x00,               // Build # (for pre-release)
          0,                  // Region code (US)
          VERSION " (Classic or OSX)",        // Major/minor release as short string
          VERSION ",© " COPYRIGHT_YEARS " CNMAT (Classic/OSX Version)"
          };
