// Primary header
#include "ROIWindowClassifier.h"

// System header
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <Commdlg.h>
#include <stdio.h>
#include <cstdio>

// DPTK headers
#include "Algorithm.h"
#include "Image.h"
#include "Global.h"
#include "image/io/Image.h"
#include "image/tile/Factory.h"
#include <geometry/graphic/Polygon.h>

// Poco header needed for the macros below
#include <Poco/ClassLibrary.h>

// Declare that this object has AlgorithmBase subclasses
//  and declare each of those sub-classes
POCO_BEGIN_MANIFEST(sedeen::algorithm::AlgorithmBase)
POCO_EXPORT_CLASS(sedeen::algorithm::ROIWindowClassifier)
POCO_END_MANIFEST

namespace sedeen {
namespace algorithm {

ROIWindowClassifier::ROIWindowClassifier()
	: foreground_results_(),
	target_location_(){
	original_image_ = image();
}

ROIWindowClassifier::~ROIWindowClassifier() {
}

void ROIWindowClassifier::init(const image::ImageHandle& image) {
	if (isNull(image)) return;

	// Create output location option and bind member to UI
	auto fDlgOptions = sedeen::file::FileDialogOptions();
	fDlgOptions.caption = "Select Destination";
	fDlgOptions.startDir = image->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);
	fDlgOptions.flags = sedeen::file::FileDialogFlags::ShowDirsOnly;
	target_location_ = createSaveFileDialogParameter(*this,
		"Output Path",
		"Select output path",
		fDlgOptions,
		false);

	// Bind overlay result to UI
	foreground_results_ = createOverlayResult(*this);

}

void ROIWindowClassifier::run() {
	if (target_location_.isChanged()) {
		// Update results if algorithm parameters have changed
		auto bbox = generateBBox();
		// Draw overlay bounding box
		foreground_results_.clear();
		for (unsigned int i = 0; i < bbox.size(); ++i) {
			Polygon bbox_to_draw = Polygon::Polygon(bbox.at(i));
			foreground_results_.drawPolygon(bbox_to_draw); 
		}
	}
	return;
}

void ROIWindowClassifier::getPythonPath() {
	char* tmp = std::getenv("SedeenPythonHome");
	int len = strlen(tmp);
	if (tmp[len - 1] == ';') {
		tmp[len - 1] = tmp[len];
	}
	const size_t cSize = strlen(tmp) + 1;
	PYTHONHOME = new wchar_t[cSize];
	mbstowcs(PYTHONHOME, tmp, cSize);
}

std::vector<std::vector<PointF>> ROIWindowClassifier::generateBBox() {
	// Select python file to run
	pathToPythonScript_ = openFile();

	// Get python script path and pythonHOME
	getPythonPath();

	// Get image path
	path_to_image = image()->getMetaData()->get(image::StringTags::SOURCE_DESCRIPTION, 0);

	// Get output path
	sedeen::algorithm::parameter::SaveFileDialog::DataType fileDialogDataType = target_location_;
	output_path = fileDialogDataType.getFilename();

	// Create python kernel
	PythonKernel PyKernel(PYTHONHOME, pathToPythonScript_, path_to_image, output_path);
	std::string bboxfilename = PyKernel.doProcessData();

	// Parse contour points in bboxfile to vector of PointF
	auto bboxes = parseBBox(bboxfilename);

	// Remove bbox contour point file
	remove(bboxfilename.c_str());
	return bboxes;
}

std::vector<std::vector<PointF>> ROIWindowClassifier::parseBBox(std::string bboxfilename) {
	std::ifstream bboxfile(bboxfilename);
	std::string cont_line;
	std::vector<std::vector<PointF>> bboxes;
	while (std::getline(bboxfile, cont_line))
	{
		std::stringstream cont_ss(cont_line);
		std::vector<PointF> cont;
		std::string point;
		while (std::getline(cont_ss, point, ' '))
		{
			int x, y;
			char comma, braket;
			std::stringstream point_ss(point);
			point_ss >> braket >> x >> comma >> y;
			cont.push_back(PointF(x, y));
		}
		bboxes.push_back(cont);
	}
	return bboxes;
}

std::string ROIWindowClassifier::openFile(void)
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