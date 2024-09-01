cmake_minimum_required(VERSION 3.13)
project(BitFiddle)



# ######### define target ################

set(TARGET_NAME BitFiddle)

set(VERSION_STRING 1.4.3)
set(BUILD_NUMBER_STRING 1.4.5)

if (MSVC)
  add_executable(${TARGET_NAME} WIN32 src/main.c)
else()
  add_executable(${TARGET_NAME})
endif()



# ######### compiler settings ################

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# ######### Define package settings ################

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



# ######### Copying resources ################

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

if(MSVC)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/win/icon.png
        \"${OUTPUT_DIRECTORY}icon.png\"
  )
endif()

if(APPLE)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/BitFiddle.icns
        \"${OUTPUT_DIRECTORY}BitFiddle.icns\"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/de.lproj/InfoPlist.strings
        \"${OUTPUT_DIRECTORY}de.lproj/InfoPlist.strings\"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/en.lproj/InfoPlist.strings
        \"${OUTPUT_DIRECTORY}en.lproj/InfoPlist.strings\"
    COMMAND ibtool --compile
        \"${OUTPUT_DIRECTORY}MainMenu.nib\"
        ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/MainMenu.xib
  )

  set(BUNDLE_IDENTIFIER "ch.manderim.Bit-Fiddle")

  set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE YES
    MACOSX_BUNDLE_BUNDLE_NAME $(PRODUCT_NAME)
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BUNDLE_IDENTIFIER}
    MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION_STRING}
    MACOSX_BUNDLE_BUNDLE_VERSION ${BUILD_NUMBER_STRING}
    MACOSX_BUNDLE_ICON_FILE "BitFiddle.icns"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
  )
endif()


# ######### Sources ################

set(resourceFiles
  res/BitFiddleChangelog.txt
  res/BitFiddleStrings_deu.h
  res/BitFiddleStrings_eng.h
  res/ascii.png
  res/help.png
  res/prefs.png
)

set(macResourceFiles
  #res/mac/de.lproj/InfoPlist.strings
  #res/mac/en.lproj/InfoPlist.strings
  res/mac/BitFiddle.icns
  res/mac/MainMenu.xib
)

set(winResourceFiles
  res/win/icon.png
)

set(sourceFiles
  src/ASCIIController.c
  src/ASCIIController.h
  src/BitArray.c
  src/BitArray.h
  src/BitFiddleAboutController.c
  src/BitFiddleAboutController.h
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



# ######### Macro definitions ################

add_definitions(-DBIT_VERSION_STRING=\"${VERSION_STRING}\")
add_definitions(-DBIT_BUILD_NUMBER_STRING=\"${BUILD_NUMBER_STRING}\")



# ######### Group sources in project browser ################

source_group("res" FILES ${resourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${resourceFiles})

source_group("res/mac" FILES ${macResourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${macResourceFiles})

source_group("res/win" FILES ${winResourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${winResourceFiles})

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



# this hack removes the xxx-CMakeForceLinker.cxx dummy file
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
