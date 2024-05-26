#----------------------------------------------------------------------------
# Set up colors
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(HILData)

#----------------------------------------------------------------------------
# find the git package (needed to determine the version of the code)
find_package(Git REQUIRED)

#----------------------------------------------------------------------------
# find the root package (and add COMPONENTS)
#find_package(ROOT REQUIRED COMPONENTS Gui GuiHtml Minuit Spectrum)
find_package(ROOT REQUIRED XMLParser)

if(${ROOT_xml_FOUND})
	add_compile_options(-DHAS_XML)
	set(CLING_OPTIONS "-DHAS_XML")
else()
	message("${Red}XML feature for ROOT not found (ROOT_xml_FOUND = ${ROOT_xml_FOUND})!${ColourReset}")
endif()

#----------------------------------------------------------------------------
# set c++ standard to the one used for root
if(DEFINED ROOT_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
else()
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

#----------------------------------------------------------------------------
# configure the include directories and set other variables
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_BINDIR ${CMAKE_BINARY_DIR}/bin)

#----------------------------------------------------------------------------
# define all libraries and root dictionaries
# for some we also create dependencies on other libraries to remove linking errors later on

add_library(TRlmd SHARED
	${PROJECT_SOURCE_DIR}/libraries/TRlmd/TRlmdEvent.cxx
	${PROJECT_SOURCE_DIR}/libraries/TRlmd/TRlmdFile.cxx
	)
root_generate_dictionary(G__TRlmd TRlmdEvent.h TRlmdFile.h MODULE TRlmd LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TRlmd/LinkDef.h OPTIONS ${CLING_OPTIONS})
target_link_libraries(TRlmd THILFormat ${ROOT_LIBRARIES})
add_dependencies(TRlmd HILDataVersion)

add_library(THILFormat SHARED
	${PROJECT_SOURCE_DIR}/libraries/THILFormat/THILDetectorInformation.cxx
	${PROJECT_SOURCE_DIR}/libraries/THILFormat/THILMnemonic.cxx
	)
root_generate_dictionary(G__THILFormat THILDetectorInformation.h THILMnemonic.h MODULE THILFormat LINKDEF ${PROJECT_SOURCE_DIR}/libraries/THILFormat/LinkDef.h)
target_link_libraries(THILFormat TEagle TEaglePulser)

add_library(THILDataParser SHARED
	${PROJECT_SOURCE_DIR}/libraries/THILDataParser/THILDataParser.cxx
	${PROJECT_SOURCE_DIR}/libraries/THILDataParser/THILDataParserException.cxx
	)
target_link_libraries(THILDataParser THILFormat)

add_library(TEagle SHARED
	${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEagle/TEagle.cxx
	${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEagle/TEagleHit.cxx
	${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEagle/TPinDiodeHit.cxx
	${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEagle/THILFragment.cxx
	)
root_generate_dictionary(G__TEagle TEagle.h TEagleHit.h TPinDiodeHit.h THILFragment.h MODULE TEagle LINKDEF ${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEagle/LinkDef.h)
target_link_libraries(TEagle ${GRSI_LIBRARIES})

add_library(TEaglePulser SHARED
	${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEaglePulser/TEaglePulser.cxx
	)
root_generate_dictionary(G__TEaglePulser TEaglePulser.h MODULE TEaglePulser LINKDEF ${PROJECT_SOURCE_DIR}/libraries/THILAnalysis/TEaglePulser/LinkDef.h)

##----------------------------------------------------------------------------
# add the global library with the functions to create files and parsers
set(HILDATA_LIBRARIES TRlmd THILFormat THILDataParser TEagle TEaglePulser)

add_library(HILData SHARED
	${PROJECT_SOURCE_DIR}/src/HILDataLibrary.cxx
	)
target_link_libraries(HILData ${HILDATA_LIBRARIES})
add_dependencies(HILData HILDataVersion)

##----------------------------------------------------------------------------
# custom target to generate the version header
# CMAKE_SOURCE_DIR is the GRSISort directory
# PROJECT_SOURCE_DIR is the _deps/grsidata-src directory
add_custom_target(HILDataVersion
	${CMAKE_COMMAND} -D SRC=${PROJECT_SOURCE_DIR}/include/HILDataVersion.h.in
	-D DST=${PROJECT_SOURCE_DIR}/include/HILDataVersion.h
                    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                    -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )

