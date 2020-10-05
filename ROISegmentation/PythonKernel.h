#include <stdio.h>
#include <fstream>
#include <direct.h>
#include <vector>

#include "Global.h"
#include "Image.h"

#include "Python.h"

class PythonKernel {

public:
	PythonKernel(wchar_t*, std::string, std::vector<std::string>, int);

	virtual ~PythonKernel();

	std::string doProcessData();

private:
	std::string pathToPythonScript_;
	std::string pyModuleName_;
	std::vector<std::string> pathsToImage_;
	int batchsize_;

	wchar_t* PYTHONHOME;

};

