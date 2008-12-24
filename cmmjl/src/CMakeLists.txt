SET(cmmjl_SOURCES cmmjl.c cmmjl_sdif.c cmmjl_osc.c cmmjl_error.c cmmjl_commonsymbols.c ../../../OSC/libOSC/OSC-client.c ../../../OSC/libOSC/OSC-timetag.c)
SET(cmmjl_HEADERS cmmjl.h cmmjl_sdif.h cmmjl_osc.h cmmjl_error.h cmmjl_commonsymbols.h cmmjl_types.h ../../../OSC/libOSC/OSC-client.h ../../../OSC/libOSC/OSC-timetag.h)
SET(cmmjl_EXTRA_FILES ../README.txt ../COPYRIGHT.txt)


INCLUDE_DIRECTORIES(. ../../SDK/MaxSDK-5/c74support/max-includes ../../../OSC/libOSC ../../externals/SDIF/SDIF-buffer ../../../sdif/lib)

IF(APPLE)
	SET(CMAKE_FRAMEWORK_PATH ../../SDK/MaxSDK-5/c74support/max-includes)
	INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
	FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
	FIND_LIBRARY(MAX_LIBRARY MaxAPI )
	MARK_AS_ADVANCED (CARBON_LIBRARY)
	MARK_AS_ADVANCED (QUICKTIME_LIBRARY)
	MARK_AS_ADVANCED (MAX_LIBRARY)
	SET(EXTRA_LIBS ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${MAX_LIBRARY})
	#find_library(MaxAPI_path MaxAPI)
	#LINK_LIBRARIES(MaxAPI_path)
	#INCLUDE_DIRECTORIES(/Developer/SDKs/MacOSX10.5.sdk/Developer/Headers/FlatCarbon)
ENDIF(APPLE)

ADD_CUSTOM_COMMAND(OUTPUT cmmjl_commonsymbols.h
			PRE_BUILD
			COMMAND ${PROJECT_SOURCE_DIR}/src/cmmjl_commonsymbols.pl ARGS ${PROJECT_SOURCE_DIR}/src
			DEPENDS cmmjl_commonsymbols.c)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/cmmjl_commonsymbols.h)

ADD_LIBRARY(cmmjl STATIC ${cmmjl_SOURCES} ${cmmjl_HEADERS} ../../SDK/MaxSDK-5/c74support/max-includes/macho-prefix.pch)
TARGET_LINK_LIBRARIES(cmmjl ${EXTRA_LIBS})

INSTALL(TARGETS cmmjl
	ARCHIVE DESTINATION lib)

INSTALL(FILES ${cmmjl_HEADERS}
	DESTINATION include/cmmjl)


