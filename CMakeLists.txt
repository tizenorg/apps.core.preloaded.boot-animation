CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(boot-animation C)

IF("$ENV{CFLAGS}" MATCHES "-DMICRO_ANI")
	OPTION(USE_MICRO_ANI "Use Micro Ani" ON)
ENDIF()

SET(SRCS
	src/boot.c
	src/animation.c
)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
	elementary
	mm-bootsound
	#utilX
	vconf
	#ecore-x
	evas
	ecore-evas
	edje
	#x11
	capi-appfw-preference
	capi-system-info
	capi-system-system-settings
	)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIE")
SET(CMAKE_C_FLAGS_PROFILING "-O0 -g -pg -fPIE")
SET(CMAKE_CXX_FLAGS_PROFILING "-O0 -std=c++0x -g -pg -fPIE")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -fPIE")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -std=c++0x -g -fPIE")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -g -fPIE")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 std=c++0x -g -fPIE")
SET(CMAKE_C_FLAGS_CCOV "-O2 -g --coverage -fPIE")
SET(CMAKE_CXX_FLAGS_CCOV "-O2 std=c++0x -g --coverage -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})

#IF("${TIZEN_PROFILE_NAME}" STREQUAL "mobile")
ADD_CUSTOM_COMMAND(OUTPUT 360x480_PowerOn.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOn.edc ${CMAKE_BINARY_DIR}/360x480_PowerOn.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOn.edc
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOn_image.h
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOn_part.h
)
ADD_CUSTOM_COMMAND(OUTPUT 360x480_PowerOff.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOff.edc ${CMAKE_BINARY_DIR}/360x480_PowerOff.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOff.edc
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOff_image.h
			${CMAKE_SOURCE_DIR}/data/edje/360x480/360x480_PowerOff_part.h
)
ADD_CUSTOM_COMMAND(OUTPUT 480x800_PowerOn.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOn.edc ${CMAKE_BINARY_DIR}/480x800_PowerOn.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOn.edc
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOn_image.h
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOn_part.h
)
ADD_CUSTOM_COMMAND(OUTPUT 480x800_PowerOff.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOff.edc ${CMAKE_BINARY_DIR}/480x800_PowerOff.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOff.edc
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOff_image.h
			${CMAKE_SOURCE_DIR}/data/edje/480x800/480x800_PowerOff_part.h
)
ADD_CUSTOM_COMMAND(OUTPUT 720x1280_PowerOn.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOn.edc ${CMAKE_BINARY_DIR}/720x1280_PowerOn.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOn.edc
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOn_image.h
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOn_part.h
)
ADD_CUSTOM_COMMAND(OUTPUT 720x1280_PowerOff.edj
		COMMAND edje_cc -id ${CMAKE_SOURCE_DIR}/data/image/
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOff.edc ${CMAKE_BINARY_DIR}/720x1280_PowerOff.edj
		DEPENDS ${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOff.edc
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOff_image.h
			${CMAKE_SOURCE_DIR}/data/edje/720x1280/720x1280_PowerOff_part.h
)
ADD_CUSTOM_TARGET(edj_build DEPENDS 360x480_PowerOn.edj 360x480_PowerOff.edj 480x800_PowerOn.edj 480x800_PowerOff.edj 720x1280_PowerOn.edj 720x1280_PowerOff.edj)
ADD_DEPENDENCIES(${PROJECT_NAME} edj_build)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)

INSTALL(FILES ${CMAKE_BINARY_DIR}/360x480_PowerOn.edj DESTINATION share/edje)
INSTALL(FILES ${CMAKE_BINARY_DIR}/360x480_PowerOff.edj DESTINATION share/edje)
INSTALL(FILES ${CMAKE_BINARY_DIR}/480x800_PowerOn.edj DESTINATION share/edje)
INSTALL(FILES ${CMAKE_BINARY_DIR}/480x800_PowerOff.edj DESTINATION share/edje)
INSTALL(FILES ${CMAKE_BINARY_DIR}/720x1280_PowerOn.edj DESTINATION share/edje)
INSTALL(FILES ${CMAKE_BINARY_DIR}/720x1280_PowerOff.edj DESTINATION share/edje)
IF("${ARCH}" STREQUAL "emulator")
ADD_DEFINITIONS("-DEMULATOR_TYPE")
INSTALL(FILES ${CMAKE_SOURCE_DIR}/data/sound/SDK_poweron/poweron.wav DESTINATION share/keysound)
ELSEIF("${ARCH}" STREQUAL "arm")
ADD_DEFINITIONS("-DARM_TYPE")
IF("${USE_MICRO_ANI}" STREQUAL "ON")
INSTALL(FILES ${CMAKE_SOURCE_DIR}/data/sound/B2_poweron/poweron.ogg DESTINATION share/keysound)
ELSEIF("${USE_MICRO_ANI}" STREQUAL "OFF")
INSTALL(FILES ${CMAKE_SOURCE_DIR}/data/sound/poweron.ogg DESTINATION share/keysound)
ENDIF()
ENDIF()
