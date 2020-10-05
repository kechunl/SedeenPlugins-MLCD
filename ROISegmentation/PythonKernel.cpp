#include "PythonKernel.h"

#include "PythonObject.h"
#include "PythonException.h"
#include "PythonEmbeddingWrapper.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy\arrayobject.h"

#include <io.h>
#include <direct.h>
#include <ctime>


PythonKernel::PythonKernel(wchar_t* pyHOME, std::string path, std::vector<std::string> image_paths, int batch_size):
	pathsToImage_(image_paths),
	batchsize_(batch_size),
	PYTHONHOME(pyHOME)
	{
		pathToPythonScript_ = path.substr(0, path.find_last_of("/\\") + 1);
		auto p1 = path.find_last_of("/\\");
		auto p2 = path.find_last_of('.') - 1;
		pyModuleName_ = path.substr(p1 + 1, (p2 - p1));
	}


PythonKernel::~PythonKernel(void)
{

}


std::string PythonKernel::doProcessData()
{
	std::string result;
	// Call the python code
	try
	{
		Py_SetPythonHome(PYTHONHOME);
		Py_Initialize();

		// set the python path
		std::shared_ptr<sedeen::python::PythonObject> sys(sedeen::python::PythonObject::py_ImportModule("sys"));
		assert(sys->py_Object() != NULL);
		std::shared_ptr<sedeen::python::PythonObject> path(sys->py_GetAttr("path"));
		assert(path->py_Object() != NULL);
		PyList_Append(path->py_Object(), PyUnicode_FromString(pathToPythonScript_.c_str()));

		// load the module
		std::shared_ptr<sedeen::python::PythonObject> pModule(sedeen::python::PythonObject::py_ImportModule(pyModuleName_.c_str())); // "Sedeen_ROISegmentation"
		assert(pModule->py_Object() != NULL);
		if (pModule == NULL) {
			exit(1);
		}

		// find the class
		std::string python_class_name_ = "ROISegmentation";
		std::shared_ptr<sedeen::python::PythonObject> pClass(pModule->py_GetAttr(python_class_name_.c_str()));
        assert(pClass->py_Object() != NULL);

        // builds the python variable to pass to the function
		PyObject* pROIs = sedeen::python::PythonObject::pyobject_from(pathsToImage_)->py_Object();
        PyObject * pArgs = Py_BuildValue("(O, i)", pROIs, batchsize_);
        assert(pArgs != NULL);

        // create a new instance of the class
        std::shared_ptr<sedeen::python::PythonObject> pInstance(pClass->py_CallObject(sedeen::python::PythonObject(pArgs, true)));
        assert(pInstance->py_Object() != NULL);
        Py_DECREF(pArgs);

        // get the function
        std::shared_ptr<sedeen::python::PythonObject> pMethod(pInstance->py_GetAttr("run"));
        assert(pMethod->py_Object() != NULL);

		// run the function
        pArgs = Py_BuildValue("()");
        assert(pArgs != NULL);
        std::shared_ptr<sedeen::python::PythonObject> pResult(pMethod->py_CallObject(sedeen::python::PythonObject(pArgs, true)));
        assert(pResult->py_Object() != NULL);

		//convert the returning python object to RawImage
        bool conversionIsDone = sedeen::python::PythonObject::pyobject_to(pResult->py_Object(), result);
        assert(conversionIsDone != false);

		/*Py_Finalize();*/

	}
	catch (std::exception& e)
	{
		std::string logfolder = strcat((char*)pathToPythonScript_.c_str(), "Exception Log/");
		if (_access(logfolder.c_str(), 0) == -1)
			_mkdir(logfolder.c_str());
		// Get time stamp
		std::time_t rawtime;
		struct tm* ptminfo;
		std::time(&rawtime);
		ptminfo = localtime(&rawtime);
		char stamp[51];
		sprintf(stamp, "%02d%02d%02d-%02d%02d%02d", ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
			ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
		std::string logname = strcat(stamp, "_ROISegmentation_Exception.txt");
		// Full path
		std::string logpath = logfolder + logname;
		FILE* fp = fopen(logpath.c_str(), "w");
		if (fp != NULL) {
			fprintf(fp, "Exception! %s\n", e.what());
			fclose(fp);
		}
	}

	return result;
}

