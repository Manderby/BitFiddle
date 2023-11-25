cmake_minimum_required(VERSION 3.13)
project(BitFiddle)

set(TARGET_NAME BitFiddle)

if (MSVC)
  add_executable(${TARGET_NAME} WIN32 src/main.c)
else()
  add_executable(${TARGET_NAME})
endif()

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

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

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION/)

endif()



# copying resources
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/BitFiddleChangelog.txt
      \"${OUTPUT_DIRECTORY}BitFiddleChangelog.txt\"
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

set(resourceFiles
  res/BitFiddleChangelog.txt
  res/BitFiddleStrings_deu.h
  res/BitFiddleStrings_eng.h
  res/ascii.png
  res/help.png
  res/prefs.png
)

set(sourceFiles
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
  src/PreferencesController.h
)

source_group("res" FILES ${resourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${resourceFiles})

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})



# ######### Linking to NALib ################

# Define GUI to be in use and hide the CMake GUI options.
add_definitions(-DNA_COMPILE_GUI=1)
set(COMPILE_NALIB_GUI 1 CACHE INTERNAL "GUI required")

set(NALIB_DIR "../../lib/NALib/code/NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)



# ######### Linking to other libraries ################

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
