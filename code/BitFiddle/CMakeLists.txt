cmake_minimum_required(VERSION 3.13)
project(BitFiddle)

set(TARGET_NAME BitFiddle)
add_executable(${TARGET_NAME} MACOSX_BUNDLE)

set_target_properties(${TARGET_NAME} PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_BUNDLE_NAME Bit\ Fiddle
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/res/mac/Bit\ Fiddle-Info.plist
)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  add_definitions(-DNA_USE_WINDOWS_COMMON_CONTROLS_6=1)

  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)

  # unicode. Theoretically, needs _MBCS to be undefined, but cmake does that.
  add_definitions(-D_UNICODE)

  # working directory
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $(OutDir))

  set(OUTPUT_DIRECTORY $(OutDir))

else()

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION/${TARGET_NAME}.app/Contents/Resources/)

endif()



# copying the resources
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/ascii.png
      \"${OUTPUT_DIRECTORY}ascii.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/help.png
      \"${OUTPUT_DIRECTORY}help.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/prefs.png
      \"${OUTPUT_DIRECTORY}prefs.png\"
)



# ######### Sources ################

set(bitFiddleResFiles
  res/ascii.png
  res/BitFiddleStrings_deu.h
  res/BitFiddleStrings_eng.h
  res/help.png
  res/prefs.png
)

set(bitFiddleWinResFiles
  res/win/resource.h
  res/win/Bit\ Fiddle.rc
  res/win/bitfiddle.ico
  res/win/icon_small.png
  res/win/icon.png
)

set(bitFiddleMacResFiles
  res/mac/Bit\ Fiddle-Info.plist
  res/mac/Bit\ Fiddle.entitlements
  res/mac/BitFiddle.icns
)

set(bitFiddleMacDeResFiles
  res/mac/de.lproj/InfoPlist.strings
  res/mac/de.lproj/MainMenu.xib
)

set(bitFiddleMacEnResFiles
  res/mac/en.lproj/InfoPlist.strings
  res/mac/en.lproj/MainMenu.xib
)

set(manderAppSrcFiles
  src/PreferencesController.h
  src/ASCIIController.c
  src/ASCIIController.h
  src/BitArray.c
  src/BitArray.h
  src/BitFiddleApplication.c
  src/BitFiddleApplication.h
  src/BitFiddleNSApplication.h
  src/BitFiddleNSApplication.m
  src/BitFiddlePreferences.c
  src/BitFiddlePreferences.h
  src/BitFiddleTranslations.c
  src/BitFiddleTranslations.h
  src/ConverterController.c
  src/ConverterController.h
  src/main.c
  src/PreferencesController.c
)

source_group("res" FILES ${bitFiddleResFiles})
target_sources(${TARGET_NAME} PRIVATE ${bitFiddleResFiles})

source_group("res/win" FILES ${bitFiddleWinResFiles})
target_sources(${TARGET_NAME} PRIVATE ${bitFiddleWinResFiles})

source_group("res/mac" FILES ${bitFiddleMacResFiles})
target_sources(${TARGET_NAME} PRIVATE ${bitFiddleMacResFiles})
source_group("res/mac/de.lproj" FILES ${bitFiddleMacDeResFiles})
target_sources(${TARGET_NAME} PRIVATE ${bitFiddleMacDeResFiles})
source_group("res/mac/en.lproj" FILES ${bitFiddleMacEnResFiles})
target_sources(${TARGET_NAME} PRIVATE ${bitFiddleMacEnResFiles})

source_group("src" FILES ${manderAppSrcFiles})
target_sources(${TARGET_NAME} PRIVATE ${manderAppSrcFiles})




# ######### Linking to NALib ################

# Define GUI and OpenGL to be not in use and hide the CMake GUI options.
add_definitions(-DNA_COMPILE_GUI=1)
set(COMPILE_NALIB_GUI 1 CACHE INTERNAL "GUI required")
set(COMPILE_NALIB_OPENGL 0 CACHE INTERNAL "OpenGL not required")

set(NALIB_DIR "../../lib/NALib/code/NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)



# ######### Linking to other libraries ################

set(MANDERAPP_DIR "../../lib/ManderApp/code/ManderApp")
target_include_directories(${TARGET_NAME} PRIVATE ${MANDERAPP_DIR}/src)
add_subdirectory(${MANDERAPP_DIR} ManderApp)
target_link_libraries(${TARGET_NAME} PRIVATE ManderApp)

if (MSVC)
  target_link_libraries(${TARGET_NAME} PRIVATE ComCtl32.lib)
else()
  FIND_LIBRARY(CARBON_FRAMEWORK Carbon)
  FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
  FIND_LIBRARY(UTYPE_FRAMEWORK UniformTypeIdentifiers)
  target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${COCOA_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${UTYPE_FRAMEWORK})
endif()
