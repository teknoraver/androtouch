cmake_minimum_required(VERSION 3.19)

project(androtouch LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(androtouch WIN32 MACOSX_BUNDLE
	main.cpp
	androtouch.cpp androtouch.h
	androtouchwidget.ui
	androtouch.qrc
)

if(WIN32)
	target_sources(androtouch PRIVATE androtouch.rc)
endif()

# the generated ui header includes androtouch.h for ClickableLabel
target_include_directories(androtouch PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(androtouch PRIVATE Qt6::Widgets)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(androtouch PRIVATE -Wall -Wextra)
endif()

install(TARGETS androtouch
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
