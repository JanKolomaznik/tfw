CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(TFW)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
endif(UNIX )

if(MSVC)
	add_definitions(/MP)
endif(MSVC)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options filesystem system timer chrono)

# Find the QtWidgets library
find_package(Qt5Widgets)

INCLUDE_DIRECTORIES(
	${Boost_INCLUDE_DIRS}
	"include"
	${${PROJECT_NAME}_BINARY_DIR}
	)

SET(${PROJECT_NAME}_HEADERS
	"include/tfw/ATransferFunctionEditor.hpp"
	"include/tfw/CreateEditorVisitor.hpp"
	"include/tfw/CreateEditorVisitor.hpp"
	"include/tfw/data/AStatistics.hpp"
	"include/tfw/data/ATransferFunction.hpp"
	"include/tfw/data/TransferFunctionLoading.hpp"
	"include/tfw/data/TransferFunctionPalette.hpp"
	"include/tfw/ColorChooserButton.hpp"
	"include/tfw/Exceptions.hpp"
	"include/tfw/GraphicsItems.hpp"
	"include/tfw/PaletteWidget.hpp"
	"include/tfw/ShowTFOptionsVisitor.hpp"
	"include/tfw/TF1DOptionsDialog.hpp"
	"include/tfw/TF2DOptionsDialog.hpp"
	"include/tfw/TransferFunctionEditor1D.hpp"
	"include/tfw/TransferFunctionEditor2D.hpp"
	"include/tfw/TransferFunctionView.hpp"
	)

SET(${PROJECT_NAME}_SOURCES
	"src/data/TransferFunctionLoading.cpp"
	"src/data/TransferFunctionPalette.cpp"
	"src/PaletteWidget.cpp"
	"src/TF1DOptionsDialog.cpp"
	"src/TF2DOptionsDialog.cpp"
	"src/GraphicsItems.cpp"
	"src/TransferFunctionEditor1D.cpp"
	"src/TransferFunctionEditor2D.cpp"
	)

qt5_wrap_ui(${PROJECT_NAME}_UIS
	"ui/PaletteWidget.ui"
	"ui/TransferFunctionEditor1D.ui"
	"ui/TransferFunctionEditor2D.ui"
	"ui/TF1DOptionsDialog.ui"
	"ui/TF2DOptionsDialog.ui"
	)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_UIS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d )
IF(WIN32)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS /NODEFAULTLIB)
ENDIF(WIN32)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)

# Use the Widgets module from Qt 5.
qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)
