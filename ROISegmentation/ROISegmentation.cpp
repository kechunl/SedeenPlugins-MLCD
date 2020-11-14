<<<<<<< HEAD
// Primary header
#include "ROISegmentation.h"

// System header
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include <Commdlg.h>
#include <stdio.h>

// DPTK headers
#include "Algorithm.h"
#include "Image.h"
#include "Global.h"
#include "image/io/Image.h"
#include "image/tile/Factory.h"

// Poco header needed for the macros below
#include <Poco/ClassLibrary.h>

#include "PythonObject.h"
#include "PythonException.h"
#include "PythonEmbeddingWrapper.h"

// Declare that this object has AlgorithmBase subclasses
//  and declare each of those sub-classes
POCO_BEGIN_MANIFEST(sedeen::algorithm::AlgorithmBase)
POCO_EXPORT_CLASS(sedeen::algorithm::ROISegmentation)
POCO_END_MANIFEST

namespace sedeen {
	namespace algorithm {

		ROISegmentation::ROISegmentation()
			: output_text_(),
			batch_size_(),
			roi_location_(),
			batchsize(1)
		{
			// Get python script path and pythonHOME
			getPythonPath();
			// Compute default batch size
			default_bs = ComputeDefaultBatchSize();
		}

		ROISegmentation::~ROISegmentation() {
		}

		void ROISegmentation::init(const image::ImageHandle& image) {
			if (isNull(image)) return;

			// Create ROI images option and bind member to UI
			auto openFileDialogOptions = sedeen::file::FileDialogOptions();
			openFileDialogOptions.caption = "Select ROI Images";
			openFileDialogOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
			openFileDialogOptions.flags = sedeen::file::FileDialogFlags::MultiSelect;
			auto roiDialogFilter = sedeen::file::FileDialogFilter();
			roiDialogFilter.name = "ROI images(*.jpg)";
			roiDialogFilter.extensions.push_back("jpg");
			openFileDialogOptions.filters.push_back(roiDialogFilter);
			roi_location_ = createOpenFileDialogParameter(*this,
				"ROI Images",
				"Select ROI Images",
				openFileDialogOptions,
				false);


			// Create output location option and bind member to UI
			//auto fDlgOptions = sedeen::file::FileDialogOptions();
			//fDlgOptions.caption = "Select Destination";
			//fDlgOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
			//fDlgOptions.flags = sedeen::file::FileDialogFlags::ShowDirsOnly;
			//target_location_ = createSaveFileDialogParameter(*this,
			//	"Output Path",
			//	"Select output path",
			//	fDlgOptions,
			//	false);


			// Create batch size parameter slider and bind member to UI
			batch_size_ = createIntegerParameter(*this,
				"Batch Size",
				"Number of samples processed in one iteration",
				default_bs, // Initial Value
				1, // Min Value
				20, // Max Value
				false);

			// Output text box
			output_text_ = createTextResult(*this, "Text Result");


		}

		void ROISegmentation::run() {
			if (batch_size_.isChanged() || roi_location_.isChanged()) {
				auto report = run_seg();
				output_text_.sendText(report);
			}

		}

		void ROISegmentation::getPythonPath() {
			char* tmp = std::getenv("SedeenPythonHome");
			int len = strlen(tmp);
			if (tmp[len - 1] == ';') {
				tmp[len - 1] = tmp[len];
			}
			const size_t cSize = strlen(tmp) + 1;
			PYTHONHOME = new wchar_t[cSize];
			mbstowcs(PYTHONHOME, tmp, cSize);
		}

		std::string ROISegmentation::run_seg() {
			// Select python file to run
			pathToPythonScript_ = openFile();

			// Get image paths
			sedeen::algorithm::parameter::OpenFileDialog::DataType openDialogDataType = roi_location_;
			for (int i = 0; i < openDialogDataType.size(); i++) {
				image_paths.push_back(openDialogDataType[i].getFilename());
			}

			// Get output path
			//sedeen::algorithm::parameter::SaveFileDialog::DataType fileDialogDataType = target_location_;
			//output_path = fileDialogDataType.getFilename();
			
			// Get batch size
			batchsize = batch_size_;

			// Create python kernel
			PythonKernel PyKernel(PYTHONHOME, pathToPythonScript_, image_paths, batchsize);
			std::string report = PyKernel.doProcessData();

			return report;
		}

		std::string ROISegmentation::openFile(void)
		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "*.py";
			ofn.lpstrFile = (LPSTR)szFileName;
			ofn.lpstrTitle = "Executable Python File";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			GetOpenFileName(&ofn);

			return ofn.lpstrFile;
		}

		int ROISegmentation::ComputeDefaultBatchSize(void)
		{
			int memory = 0;
			Py_SetPythonHome(PYTHONHOME);
			Py_Initialize();

			PyRun_SimpleString("import psutil");
			PyObject* main = PyImport_AddModule("__main__");
			PyObject* globalDictionary = PyModule_GetDict(main);
			PyObject* localDictionary = PyDict_New();
			PyRun_String("memory = psutil.virtual_memory().available / 1024 ** 3", Py_file_input, globalDictionary, localDictionary);

			memory = (int)PyLong_AsLong(PyDict_GetItemString(localDictionary, "memory"));
			Py_Finalize();
			if (memory > 8)
				return 5;
			else if (memory > 6)
				return 2;
			else
				return 1;
		}
	}
=======
// Primary header
#include "ROISegmentation.h"

// System header
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include <Commdlg.h>
#include <stdio.h>

// DPTK headers
#include "Algorithm.h"
#include "Image.h"
#include "Global.h"
#include "image/io/Image.h"
#include "image/tile/Factory.h"

// Poco header needed for the macros below
#include <Poco/ClassLibrary.h>

#include "PythonObject.h"
#include "PythonException.h"
#include "PythonEmbeddingWrapper.h"

// Declare that this object has AlgorithmBase subclasses
//  and declare each of those sub-classes
POCO_BEGIN_MANIFEST(sedeen::algorithm::AlgorithmBase)
POCO_EXPORT_CLASS(sedeen::algorithm::ROISegmentation)
POCO_END_MANIFEST

namespace sedeen {
	namespace algorithm {

		ROISegmentation::ROISegmentation()
			: output_text_(),
			batch_size_(),
			roi_location_(),
			batchsize(1)
		{
			// Get python script path and pythonHOME
			getPythonPath();
			// Compute default batch size
			default_bs = ComputeDefaultBatchSize();
		}

		ROISegmentation::~ROISegmentation() {
		}

		void ROISegmentation::init(const image::ImageHandle& image) {
			if (isNull(image)) return;

			// Create ROI images option and bind member to UI
			auto openFileDialogOptions = sedeen::file::FileDialogOptions();
			openFileDialogOptions.caption = "Select ROI Images";
			openFileDialogOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
			openFileDialogOptions.flags = sedeen::file::FileDialogFlags::MultiSelect;
			auto roiDialogFilter = sedeen::file::FileDialogFilter();
			roiDialogFilter.name = "ROI images(*.jpg)";
			roiDialogFilter.extensions.push_back("jpg");
			openFileDialogOptions.filters.push_back(roiDialogFilter);
			roi_location_ = createOpenFileDialogParameter(*this,
				"ROI Images",
				"Select ROI Images",
				openFileDialogOptions,
				false);


			// Create output location option and bind member to UI
			//auto fDlgOptions = sedeen::file::FileDialogOptions();
			//fDlgOptions.caption = "Select Destination";
			//fDlgOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
			//fDlgOptions.flags = sedeen::file::FileDialogFlags::ShowDirsOnly;
			//target_location_ = createSaveFileDialogParameter(*this,
			//	"Output Path",
			//	"Select output path",
			//	fDlgOptions,
			//	false);


			// Create batch size parameter slider and bind member to UI
			batch_size_ = createIntegerParameter(*this,
				"Batch Size",
				"Number of samples processed in one iteration",
				default_bs, // Initial Value
				1, // Min Value
				20, // Max Value
				false);

			// Output text box
			output_text_ = createTextResult(*this, "Text Result");


		}

		void ROISegmentation::run() {
			if (batch_size_.isChanged() || roi_location_.isChanged()) {
				auto report = run_seg();
				output_text_.sendText(report);
			}

		}

		void ROISegmentation::getPythonPath() {
			char* tmp = std::getenv("SedeenPythonHome");
			int len = strlen(tmp);
			if (tmp[len - 1] == ';') {
				tmp[len - 1] = tmp[len];
			}
			const size_t cSize = strlen(tmp) + 1;
			PYTHONHOME = new wchar_t[cSize];
			mbstowcs(PYTHONHOME, tmp, cSize);
		}

		std::string ROISegmentation::run_seg() {
			// Select python file to run
			pathToPythonScript_ = openFile();

			// Get image paths
			sedeen::algorithm::parameter::OpenFileDialog::DataType openDialogDataType = roi_location_;
			for (int i = 0; i < openDialogDataType.size(); i++) {
				image_paths.push_back(openDialogDataType[i].getFilename());
			}

			// Get output path
			//sedeen::algorithm::parameter::SaveFileDialog::DataType fileDialogDataType = target_location_;
			//output_path = fileDialogDataType.getFilename();
			
			// Get batch size
			batchsize = batch_size_;

			// Create python kernel
			PythonKernel PyKernel(PYTHONHOME, pathToPythonScript_, image_paths, batchsize);
			std::string report = PyKernel.doProcessData();

			return report;
		}

		std::string ROISegmentation::openFile(void)
		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "*.py";
			ofn.lpstrFile = (LPSTR)szFileName;
			ofn.lpstrTitle = "Executable Python File";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			GetOpenFileName(&ofn);

			return ofn.lpstrFile;
		}

		int ROISegmentation::ComputeDefaultBatchSize(void)
		{
			int memory = 0;
			Py_SetPythonHome(PYTHONHOME);
			Py_Initialize();

			PyRun_SimpleString("import psutil");
			PyObject* main = PyImport_AddModule("__main__");
			PyObject* globalDictionary = PyModule_GetDict(main);
			PyObject* localDictionary = PyDict_New();
			PyRun_String("memory = psutil.virtual_memory().available / 1024 ** 3", Py_file_input, globalDictionary, localDictionary);

			memory = (int)PyLong_AsLong(PyDict_GetItemString(localDictionary, "memory"));
			Py_Finalize();
			if (memory > 8)
				return 5;
			else if (memory > 6)
				return 2;
			else
				return 1;
		}
	}
>>>>>>> 6c9cecd... Initial commit
}