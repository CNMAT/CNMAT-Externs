# Copyright (c) 2008.  The Regents of the University of California (Regents).
# All Rights Reserved.

# Permission to use, copy, modify, and distribute this software and its
# documentation for educational, research, and not-for-profit purposes, without
# fee and without a signed licensing agreement, is hereby granted, provided that
# the above copyright notice, this paragraph and the following two paragraphs
# appear in all copies, modifications, and distributions.  Contact The Office of
# Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
# CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

# Written by John MacCallum, The Center for New Music and
# Audio Technologies, University of California, Berkeley.

#     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
#     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
#     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
#     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
#     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
#     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
#     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
#     ENHANCEMENTS, OR MODIFICATIONS.

SET(cmmjl_PATH_TO_SDIFLIB ../../../sdif/lib)
SET(cmmjl_PATH_TO_MAXSDK ../../SDK/MaxSDK-5)
SET(cmmjl_PATH_TO_SDIF-BUFFER ../../externals/SDIF/SDIF-buffer)
SET(cmmjl_PATH_TO_LIBOSC ../../../OSC/libOSC)

SET(cmmjl_SOURCES 
	cmmjl.c 
	cmmjl_sdif.c 
	cmmjl_osc.c 
	cmmjl_osc_pattern.c
	cmmjl_error.c 
	cmmjl_strerror.c
	cmmjl_commonsymbols.c
	cmmjl_profile.c
	${cmmjl_PATH_TO_LIBOSC}/OSC-client.c
	${cmmjl_PATH_TO_LIBOSC}/OSC-timetag.c)

SET(cmmjl_HEADERS 
	cmmjl.h 
	cmmjl_sdif.h 
	cmmjl_osc.h 
	cmmjl_osc_pattern.h
	cmmjl_error.h 
	cmmjl_errno.h
	cmmjl_commonsymbols.h 
	cmmjl_types.h
	cmmjl_profile.h
	${cmmjl_PATH_TO_LIBOSC}/OSC-client.h
	${cmmjl_PATH_TO_LIBOSC}/OSC-timetag.h)

SET(cmmjl_EXTRA_FILES ${PROJECT_SOURCE_DIR}/README.txt ${PROJECT_SOURCE_DIR}/COPYRIGHT.txt)

INCLUDE_DIRECTORIES(. 
	${cmmjl_PATH_TO_MAXSDK}/c74support/max-includes 
	../../include
	${cmmjl_PATH_TO_SDIF-BUFFER}
	${cmmjl_PATH_TO_SDIFLIB})

LINK_DIRECTORIES(../../lib)
#SET(OSC_LIBRARY OSC)

#SET(EXTRA_LIBS ${EXTRA_LIBS} OSC)

IF(APPLE)
	SET(CMAKE_FRAMEWORK_PATH ${cmmjl_PATH_TO_MAXSDK}/c74support/max-includes)
	INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
	FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
	FIND_LIBRARY(MAX_LIBRARY MaxAPI )
	MARK_AS_ADVANCED (CARBON_LIBRARY)
	MARK_AS_ADVANCED (QUICKTIME_LIBRARY)
	MARK_AS_ADVANCED (MAX_LIBRARY)
	SET(EXTRA_LIBS ${EXTRA_LIBS} ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${MAX_LIBRARY})
ENDIF(APPLE)

ADD_CUSTOM_COMMAND(OUTPUT cmmjl_commonsymbols.h
	PRE_BUILD
	COMMAND ${PROJECT_SOURCE_DIR}/src/cmmjl_commonsymbols.pl ARGS ${PROJECT_SOURCE_DIR}/src
	DEPENDS cmmjl_commonsymbols.c)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/cmmjl_commonsymbols.h)

ADD_LIBRARY(cmmjl STATIC ${cmmjl_SOURCES} ${cmmjl_HEADERS} ${cmmjl_PATH_TO_MAXSDK}/c74support/max-includes/macho-prefix.pch)
TARGET_LINK_LIBRARIES(cmmjl ${EXTRA_LIBS})

INSTALL(TARGETS cmmjl
	ARCHIVE DESTINATION lib)

INSTALL(FILES ${cmmjl_HEADERS}
	DESTINATION include/cmmjl)


