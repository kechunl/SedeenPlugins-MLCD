#ifndef ITCR_SRC_PLUGINS_DIAGNOSIS_H
#define ITCR_SRC_PLUGINS_DIAGNOSIS_H

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

		/// Algorithm for providing diagnosis prediction per ROI.

		class Diagnosis : public AlgorithmBase {
		public:
			// Constructor
			Diagnosis();

			// Optional Destructor
			virtual ~Diagnosis();

		private:
			virtual void run();

			virtual void init(const image::ImageHandle& image);

			std::string openFile(void);

			std::string generateDiagnosis();

			void getPythonPath();

		private:
			std::vector<std::string> csv_paths;

			// Parameter for selecting csv file
			OpenFileDialogParameter csv_location_;

			/// The output result
			algorithm::TextResult output_text_;

			// The embedded Python Script path
			std::string pathToPythonScript_;

			// Python Home
			wchar_t* PYTHONHOME;
		};

	} // namespace algorithm

} // namespace sedeen

#endif // ifndef ITCR_SRC_PLUGINS_DIAGNOSIS_H
