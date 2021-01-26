// Primary header
#include "Diagnosis.h"

// System header
#include <algorithm>
#include <vector>
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
POCO_EXPORT_CLASS(sedeen::algorithm::Diagnosis)
POCO_END_MANIFEST

namespace sedeen {
	namespace algorithm {

		Diagnosis::Diagnosis()
			: output_text_(),
			csv_location_(),
			PYTHONHOME(){
		}

		Diagnosis::~Diagnosis() {
		}

		void Diagnosis::init(const image::ImageHandle& image) {
			if (isNull(image)) return;

			// Create CSV file option and bind member to UI
			auto openFileDialogOptions = sedeen::file::FileDialogOptions();
			openFileDialogOptions.caption = "Select Co-occurence Features (.csv) or segmentation label image for prediction";
			openFileDialogOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
			openFileDialogOptions.flags = sedeen::file::FileDialogFlags::MultiSelect;
			auto csvDialogFilter1 = sedeen::file::FileDialogFilter();
			csvDialogFilter1.name = "Co-occurence Features(*SuperpixelCooccurrence.csv)";
			csvDialogFilter1.extensions.push_back("csv");
			openFileDialogOptions.filters.push_back(csvDialogFilter1);
			auto csvDialogFilter2 = sedeen::file::FileDialogFilter();
			csvDialogFilter2.name = "Segmentation label(*_seg_label.png)";
			csvDialogFilter2.extensions.push_back("png");
			openFileDialogOptions.filters.push_back(csvDialogFilter2);
			csv_location_ = createOpenFileDialogParameter(*this,
				"Co-occurence Features / Seg Label Image Paths",
				"Select Co-occurence Features or Segmentation Label Image for Diagnosis",
				openFileDialogOptions,
				false);

			// Output text box
			output_text_ = createTextResult(*this, "Text Result");

		}

		void Diagnosis::run() {
			if (csv_location_.isChanged()) {
				// Update results if algorithm parameters have changed
				auto report = generateDiagnosis();
				output_text_.sendText(report);
			}

		}

		std::string Diagnosis::generateDiagnosis() {
			// Select python file to run
			pathToPythonScript_ = openFile();

			// Get python script path and pythonHOME
			getPythonPath();

			// Get features paths
			sedeen::algorithm::parameter::OpenFileDialog::DataType openDialogDataType = csv_location_;
			for (int i = 0; i < openDialogDataType.size(); i++) {
				csv_paths.push_back(openDialogDataType[i].getFilename());
			}

			PythonKernel PyKernel(PYTHONHOME, pathToPythonScript_, csv_paths);
			std::string report = PyKernel.doProcessData();

			return report;
		}

		void Diagnosis::getPythonPath() {
			char* tmp = std::getenv("SedeenPythonHome");
			int len = strlen(tmp);
			if (tmp[len - 1] == ';') {
				tmp[len - 1] = tmp[len];
			}
			const size_t cSize = strlen(tmp) + 1;
			PYTHONHOME = new wchar_t[cSize];
			mbstowcs(PYTHONHOME, tmp, cSize);


			/// If more than one path is set
			//std::string pathvar = std::getenv("SedeenPythonHome");
			//std::vector<std::string> SedeenPaths;
			//std::string::size_type pos1, pos2;
			//pos2 = pathvar.find(delim);
			//pos1 = 0;
			//while (std::string::npos != pos2)
			//{
			//	SedeenPaths.push_back(pathvar.substr(pos1, pos2 - pos1));
			//	pos1 = pos2 + sizeof(char);
			//	pos2 = pathvar.find(delim, pos1);
			//}
			//if (pos1 != pathvar.length())
			//	SedeenPaths.push_back(pathvar.substr(pos1));

			//std::vector<std::string>::iterator iter = std::find_if(SedeenPaths.begin(), SedeenPaths.end(), [](const std::string& str) {return str.find("cancer_env") != std::string::npos; });
			//if (iter != SedeenPaths.end())
			//{
			//	const char* tmp = (*iter).c_str();
			//	const size_t cSize = strlen(tmp) + 1;
			//	PYTHONHOME = new wchar_t[cSize];
			//	mbstowcs(PYTHONHOME, tmp, cSize);
			//}
			//iter = std::find_if(SedeenPaths.begin(), SedeenPaths.end(), [](const std::string& str) {return str.find("cancer_diagnosis") != std::string::npos; });
			//if (iter != SedeenPaths.end())
			//	pathToPythonScript_ = *iter + "\\Sedeen Scripts\\Sedeen_Diagnosis.py";
		}

		std::string Diagnosis::openFile(void)
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
	}
}
