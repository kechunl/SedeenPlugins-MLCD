PROJECT( ROIWindowClassifier ) 
CMAKE_MINIMUM_REQUIRED( VERSION 3.18.0 )

# Load the Sedeen dependencies
FIND_PACKAGE( SEDEENSDK REQUIRED HINTS "C:/Program Files/Sedeen Viewer SDK/v5.4.4.20191024/msvc2017" )
FIND_PACKAGE( Python COMPONENTS Interpreter Development NumPy)
INCLUDE_DIRECTORIES( "${SEDEENSDK_INCLUDE_DIR}" )
#"C:/Qt/Qt5.14.1/5.14.1/msvc2017_64/include" 
	#"C:/ProgramData/Anaconda3/Lib/site-packages" "C:/ProgramData/Anaconda3/include" 
	#"C:/ProgramData/Anaconda3/Lib/site-packages/numpy/core/include"
LINK_DIRECTORIES( "${SEDEENSDK_LIBRARY_DIR}")

#LINK_LIBRARIES( "C:/ProgramData/Anaconda3/libs/python37.lib" )

# Build the code into a module library
ADD_LIBRARY( ROIWindowClassifier SHARED
	ROIWindowClassifier.cpp
	ROIWindowClassifier.h
	PythonEmbeddingWrapper.cpp
	PythonEmbeddingWrapper.h
	PythonException.cpp
	PythonException.h
	PythonKernel.cpp
	PythonKernel.h
	PythonObject.cpp
	PythonObject.h)

target_compile_features(ROIWindowClassifier PUBLIC cxx_std_17)

# Link the library against the Sedeen libraries
TARGET_LINK_LIBRARIES( ROIWindowClassifier ${SEDEENSDK_LIBRARIES} )

TARGET_LINK_LIBRARIES(ROIWindowClassifier Python::Python Python::NumPy)
# Install the plugin in the sedeen plugins directory
INSTALL( TARGETS ROIWindowClassifier LIBRARY DESTINATION
"${PATHCORE_DIR}/plugins" ) 