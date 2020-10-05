#include <stdio.h>
#include <fstream>
#include <direct.h>

#include "Global.h"
#include "Image.h"

#include "Python.h"

class PythonKernel {

public:
	PythonKernel(wchar_t*, std::string, std::string, std::string);

	virtual ~PythonKernel();

	std::string doProcessData();

private:
	std::string pathToPythonScript_;
	std::string pyModuleName_;
	std::string pathToImage_;
	std::string pathToSaveFile_;

	wchar_t* PYTHONHOME;
};

