CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(cmmjl)
ADD_SUBDIRECTORY(src)

SET(CMAKE_OSX_ARCHITECTURES "ppc;i386" CACHE STRING "Build architectures for OSX" FORCE) 

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library of useful tools for writing Max/MSP/Jitter externals")
SET(CPACK_PACKAGE_VENDOR "The Center for New Music and Audio Technologies, Department of Music, University of California, Berkeley")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
INCLUDE(CPack)

INCLUDE(FindDoxygen)
IF(DOXYGEN)
	MESSAGE(STATUS 
		"Documentation is not built by default but can be bulit by typing\n\tmake doc\n")

	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                      ${CMAKE_BINARY_DIR}/Doxyfile)

	ADD_CUSTOM_TARGET(doc ${DOXYGEN} ${CMAKE_BINARY_DIR}/Doxyfile
				DEPENDS ${CMAKE_BINARY_DIR}/Doxyfile)
	#ADD_CUSTOM_TARGET(doc 
	#	${DOXYGEN} ${CMAKE_HOME_DIRECTORY}/Doxyfile
        #     	DEPENDS ${CMAKE_HOME_DIRECTORY}/Doxyfile)
ELSE(DOXYGEN)
	MESSAGE(STATUS "WARNING: Doxygen not found--documentation will not be generated.  To build the documentation, install Doxygen (www.doxygen.org")
ENDIF(DOXYGEN)
