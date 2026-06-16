cmake_minimum_required (VERSION 3.8)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Tetoris")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем лишние предупреждения
if(MSVC)
    add_compile_options(/W4 /windows-1251)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Путь к SFML (укажите ваш путь)
set(SFML_ROOT "C:/Users/yusef/libc++/SFML-3.0.2")

set(SOURCES
    src/Tetoris.cpp
)

set(HEADERS
    include/stb_image.h
)

add_executable (Tetoris ${SOURCES} ${HEADERS})

set_target_properties(Tetoris PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# ЯВНО указываем пути к include директориям
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SFML_ROOT}/include          # <-- Путь к заголовкам SFML
)

# ЯВНО указываем пути к библиотекам
target_link_directories(${PROJECT_NAME} PRIVATE
    ${SFML_ROOT}/lib
)

# Линкуем библиотеки
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Копируем assets
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets to build directory"
)

# Копируем DLL SFML
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SFML_ROOT}/bin/sfml-graphics-3.dll"
    "${SFML_ROOT}/bin/sfml-window-3.dll"
    "${SFML_ROOT}/bin/sfml-system-3.dll"
    "${SFML_ROOT}/bin/sfml-audio-3.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)