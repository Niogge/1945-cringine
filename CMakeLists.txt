cmake_minimum_required(VERSION 3.14)
project(SDLPrj LANGUAGES C)

#CMAKE_MODULE_PATH

find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message(SDL2 Trovata)
    message(${SDL2_INCLUDE_DIR})
    message(${SDL2_LIBRARY})
endif()

find_package(SDL2_image REQUIRED)

if(SDL2_IMAGE_FOUND)
    message(SDLImage Trovata)
    message( ${SDL2_IMAGE_INCLUDE_DIRS} )
    message( ${SDL2_IMAGE_LIBRARIES} )
endif()

find_package(SDL2TTF REQUIRED)

if(SDL2TTF_FOUND)
    message(SDL2TTF Trovata)
    message( ${SDL2TTF_INCLUDE_DIR} )
    message( ${SDL2TTF_LIBRARY} )
endif()

find_package(SDL2_mixer REQUIRED)

if(SDL2_MIXER_FOUND)
    message(SDL2_MIXER Trovata)
    message( ${SDL2_MIXER_INCLUDE_DIRS} )
    message( ${SDL2_MIXER_LIBRARIES} )
endif()

file(GLOB SOURCES "src/*.c" "src/Engine/*.c" "src/Math/*.c" "src/Collections/*.c" "src/Components/*.c" )
file(GLOB HEADERS "include/*.h" "include/Engine/*.h" "include/Math/*.h" "include/Collections/*.h" "include/Components/*.h")

add_executable(SDLApp ${SOURCES} ${HEADERS})
#In automatico carica gli include e la library (dipendenza .lib)
target_link_libraries(SDLApp ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARIES})
target_include_directories(SDLApp PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS} include include/Engine include/Math include/Collections include/Components)


include(GNUInstallDirs)
set(SDL2_DLL ${SDL2_PATH}/lib/x64/SDL2.dll)
set(SDL2_IMAGE_DLL ${SDL2_IMAGE_PATH}/lib/x64/SDL2_image.dll)
set(SDL2_IMAGE_PNG_DLL ${SDL2_IMAGE_PATH}/lib/x64/libpng16-16.dll)
set(SDL2_IMAGE_JPEG_DLL ${SDL2_IMAGE_PATH}/lib/x64/libjpeg-9.dll)
set(SDL2_TTF_DLL ${SDL2_TTF_PATH}/lib/x64/SDL2_ttf.dll)
set(ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/resources)
add_custom_command(TARGET SDLApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:SDLApp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_IMAGE_DLL} $<TARGET_FILE_DIR:SDLApp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_IMAGE_PNG_DLL} $<TARGET_FILE_DIR:SDLApp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_IMAGE_JPEG_DLL} $<TARGET_FILE_DIR:SDLApp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:SDLApp>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ResourcesPath} $<TARGET_FILE_DIR:SDLApp>/resources
)