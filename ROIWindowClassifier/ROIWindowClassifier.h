<<<<<<< HEAD
#ifndef ITCR_SRC_PLUGINS_ROIWINDOWCLASSIFIER_H
#define ITCR_SRC_PLUGINS_ROIWINDOWCLASSIFIER_H

// System headers
#include <Windows.h>

// DPTK headers - a minimal set
#include "algorithm/AlgorithmBase.h"
#include "algorithm/Parameters.h"
#include "algorithm/Results.h"
#include "PythonKernel.h"

namespace sedeen {

namespace algorithm {

/// Algorithm for detecting relevant regions of interest in breast biopsy whole slide images and 
/// pointing out image regions that tend to cause misdiagnosis and produce suitable warnings 
/// as to why such regions may either be distractors or indicate cancer.

class ROIWindowClassifier : public AlgorithmBase {
public:
	// Constructor
	ROIWindowClassifier();

	// Optional Destructor
	virtual ~ROIWindowClassifier();

private:
	virtual void run();

	virtual void init(const image::ImageHandle& image);

	std::string openFile(void);

	std::vector<std::vector<PointF>> generateBBox();

	std::vector<std::vector<PointF>> parseBBox(std::string bboxfile);

	void getPythonPath();

private:
	std::string path_to_image;
	std::string output_path;

	// The embedded Python Script path
	std::string pathToPythonScript_;

	// Parameter for selecting to save the tiles
	SaveFileDialogParameter target_location_;

	// The input image handle
	image::ImageHandle original_image_;

	/// Overlay reporter through which bounding boxes are displayed
	OverlayResult foreground_results_;

	// Python Home
	wchar_t* PYTHONHOME;
};

} // namespace algorithm

} // namespace sedeen

=======
#ifndef ITCR_SRC_PLUGINS_ROIWINDOWCLASSIFIER_H
#define ITCR_SRC_PLUGINS_ROIWINDOWCLASSIFIER_H

// System headers
#include <Windows.h>

// DPTK headers - a minimal set
#include "algorithm/AlgorithmBase.h"
#include "algorithm/Parameters.h"
#include "algorithm/Results.h"
#include "PythonKernel.h"

namespace sedeen {

namespace algorithm {

/// Algorithm for detecting relevant regions of interest in breast biopsy whole slide images and 
/// pointing out image regions that tend to cause misdiagnosis and produce suitable warnings 
/// as to why such regions may either be distractors or indicate cancer.

class ROIWindowClassifier : public AlgorithmBase {
public:
	// Constructor
	ROIWindowClassifier();

	// Optional Destructor
	virtual ~ROIWindowClassifier();

private:
	virtual void run();

	virtual void init(const image::ImageHandle& image);

	std::string openFile(void);

	std::vector<std::vector<PointF>> generateBBox();

	std::vector<std::vector<PointF>> parseBBox(std::string bboxfile);

	void getPythonPath();

private:
	std::string path_to_image;
	std::string output_path;

	// The embedded Python Script path
	std::string pathToPythonScript_;

	// Parameter for selecting to save the tiles
	SaveFileDialogParameter target_location_;

	// The input image handle
	image::ImageHandle original_image_;

	/// Overlay reporter through which bounding boxes are displayed
	OverlayResult foreground_results_;

	// Python Home
	wchar_t* PYTHONHOME;
};

} // namespace algorithm

} // namespace sedeen

>>>>>>> 6c9cecd... Initial commit
#endif // ifndef ITCR_SRC_PLUGINS_ROIWINDOWCLASSIFIER_H