#ifndef ITCR_SRC_PLUGINS_ROISEGMENTATION_H
#define ITCR_SRC_PLUGINS_ROISEGMENTATION_H

// System headers
#include <Windows.h>
#include <vector>

// DPTK headers - a minimal set
#include "algorithm/AlgorithmBase.h"
#include "algorithm/Parameters.h"
#include "algorithm/Results.h"
#include "PythonKernel.h"
#include "Python.h"

namespace sedeen {

	namespace algorithm {

		/// Algorithm for segmenting ROIs into different tissues, which will be 
		/// used as features for diagnosis prediction.

		class ROISegmentation : public AlgorithmBase {
		public:
			// Constructor
			ROISegmentation();

			// Optional Destructor
			virtual ~ROISegmentation();

		private:
			virtual void run();

			virtual void init(const image::ImageHandle& image);

			std::string openFile(void);

			int ComputeDefaultBatchSize(void);

			std::string run_seg();

			void getPythonPath();

		private:
			std::vector<std::string> image_paths;
			std::string output_path;
			int batchsize;
			int default_bs;

			// Batch size
			algorithm::IntegerParameter batch_size_;

			// Parameter for selecting to open the ROI images
			OpenFileDialogParameter roi_location_;

			// Parameter for selecting to save the tiles
			//SaveFileDialogParameter target_location_;

			/// The output result
			algorithm::TextResult output_text_;

			// The embedded Python Script path
			std::string pathToPythonScript_;

			// Python Home
			wchar_t* PYTHONHOME;
		};

	} // namespace algorithm

} // namespace sedeen

#ifndef ITCR_SRC_PLUGINS_ROISEGMENTATION_H
#define ITCR_SRC_PLUGINS_ROISEGMENTATION_H

// System headers
#include <Windows.h>
#include <vector>

// DPTK headers - a minimal set
#include "algorithm/AlgorithmBase.h"
#include "algorithm/Parameters.h"
#include "algorithm/Results.h"
#include "PythonKernel.h"
#include "Python.h"

namespace sedeen {

	namespace algorithm {

		/// Algorithm for segmenting ROIs into different tissues, which will be 
		/// used as features for diagnosis prediction.

		class ROISegmentation : public AlgorithmBase {
		public:
			// Constructor
			ROISegmentation();

			// Optional Destructor
			virtual ~ROISegmentation();

		private:
			virtual void run();

			virtual void init(const image::ImageHandle& image);

			std::string openFile(void);

			int ComputeDefaultBatchSize(void);

			std::string run_seg();

			void getPythonPath();

		private:
			std::vector<std::string> image_paths;
			std::string output_path;
			int batchsize;
			int default_bs;

			// Batch size
			algorithm::IntegerParameter batch_size_;

			// Parameter for selecting to open the ROI images
			OpenFileDialogParameter roi_location_;

			// Parameter for selecting to save the tiles
			//SaveFileDialogParameter target_location_;

			/// The output result
			algorithm::TextResult output_text_;

			// The embedded Python Script path
			std::string pathToPythonScript_;

			// Python Home
			wchar_t* PYTHONHOME;
		};

	} // namespace algorithm

} // namespace sedeen

#endif // ifndef ITCR_SRC_PLUGINS_ROISEGMENTATION_H