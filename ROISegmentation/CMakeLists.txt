PROJECT( ROISegmentation ) 
CMAKE_MINIMUM_REQUIRED( VERSION 3.18.0 )

# Load the Sedeen dependencies
FIND_PACKAGE( SEDEENSDK REQUIRED HINTS "C:/Program Files/Sedeen Viewer SDK/v5.4.4.20191024/msvc2017" )
FIND_PACKAGE( Python COMPONENTS Interpreter Development NumPy)
INCLUDE_DIRECTORIES( "${SEDEENSDK_INCLUDE_DIR}" )
LINK_DIRECTORIES( "${SEDEENSDK_LIBRARY_DIR}")

# Build the code into a module library
ADD_LIBRARY( ROISegmentation SHARED
	ROISegmentation.cpp
	ROISegmentation.h
	PythonEmbeddingWrapper.cpp
	PythonEmbeddingWrapper.h
	PythonException.cpp
	PythonException.h
	PythonKernel.cpp
	PythonKernel.h
	PythonObject.cpp
	PythonObject.h)

target_compile_features(ROISegmentation PUBLIC cxx_std_17)

# Link the library against the Sedeen libraries
TARGET_LINK_LIBRARIES( ROISegmentation ${SEDEENSDK_LIBRARIES} )

TARGET_LINK_LIBRARIES(ROISegmentation Python::Python Python::NumPy)
# Install the plugin in the sedeen plugins directory
INSTALL( TARGETS ROISegmentation LIBRARY DESTINATION
"${PATHCORE_DIR}/plugins" ) 