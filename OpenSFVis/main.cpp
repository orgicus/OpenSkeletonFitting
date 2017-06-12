/***********************************************************************
*
* OpenSkeletonFitting
* Skeleton fitting by the use of energy minimization
* Copyright (C) 2012 Norman Link <norman.link@gmx.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#include <iostream>
#include "../OpenSF/Logging.h"
#include "../OpenSFInput/InputKinect.h"
#include "../OpenSFInput/InputNumbered.h"
#include "../OpenSFInput/InputPlayerONI.h"
#include "../OpenSFSegmentation/SegmentationBackground.h"
#include "../OpenSFFeatures/Features.h"
#include "../OpenSFitting/Fitting.h"
#include "../OpenSFitting/SkeletonHuman.h"
#include "../OpenSFitting/SkeletonManipulator.h"
#include "../OpenSFLib/System.h"
#include "Visualization2d.h"
#include "Visualization3d.h"
using namespace std;
using namespace osf;

void on_trackbar( int, void* )
{

}

int main(int argc,char** argv)
{
	try {
		System myOSF;
		myOSF.init();
		
		Input* input;
		
		if(argc > 1){
			cout << "trying to load " << argv[1] << endl;
			//InputPlayerONI* input = dynamic_cast<InputPlayerONI*>(myOSF.createInput(InputPlayerONI::getType()));
			input = dynamic_cast<InputPlayerONI*>(myOSF.createInput(InputPlayerONI::getType()));
			((InputPlayerONI*)input)->setFilename(argv[1]);
			// ((InputPlayerONI*)input)->setRepeat(true);
			// input->setPauseMode(true);
			// input->setRepeat(true);
		}else{
			cout << "no .oni file path provided, attempting to connect to sensor via USB" << endl;
			// create input object
			// InputKinect* input = dynamic_cast<InputKinect*>(myOSF.createInput(InputKinect::getType()));
			input = dynamic_cast<InputKinect*>(myOSF.createInput(InputKinect::getType()));
			((InputKinect*)input)->setRegisterDepth(true);
			//input->setMotorAngle(10);
			//input->startRecording("..\\..\\Data\\new_scene.oni");
		}
		
		// resize for better performance
		input->setResizing(cv::Size(320, 240));

		// create segmentation object
		Segmentation* seg = 0;
		seg = myOSF.createSegmentation(SegmentationBackground::getType());

		// create feature detection
		Features* feat = 0;
		feat = myOSF.createFeatures(Features::getType());

		// set parameters for feature detection
		if (feat) {
			feat->setGeoMaxZDistThreshold(0.1f);
			feat->setGeoNeighborPrecision(8);
			feat->setIsoPatchResizing(cv::Size(160, 120));
			feat->setIsoPatchWidth(0.2f);
			feat->setTrSearchRadius(0.3f);
			feat->setTrFtLifespan(10);
			feat->setTrFtPointTempTimespan(20);
			feat->setTrFtKfMeasurementNoise(1e-5);
			feat->setTrFtKfProcessNoise(1e-6);
		}
		
		// create skeleton fitting
		Fitting* fitting = 0;
		fitting = myOSF.createFitting(Fitting::getType());

		// set parameters for skeleton fitting
		if (fitting) {
			fitting->setNNDepthStep(3);
			fitting->setNNDepthMaxLeafSize(15);
			fitting->setNNGeoStep(1);
			fitting->setNNGeoMaxLeafSize(15);
			fitting->setNNGeoCutoffFactor(0.5f);
			fitting->setFitCCDMaxIter(1);
			fitting->setFitCCDChangeThresh(0.001);
			fitting->setFitCCDMinimzeSize(true);
			
			// select skeleton to track
			if(argc > 2){
				if(argv[2][0] == '1')  fitting->createSkeleton<SkeletonUpperBody>();
				if(argv[2][0] == '2')  fitting->createSkeleton<SkeletonLowerBody>();
				if(argv[2][0] == '3')  fitting->createSkeleton<SkeletonFullBody>();
				if(argv[2][0] == '4')  fitting->createSkeleton<SkeletonSimple>();
				if(argv[2][0] == '5')  fitting->createSkeleton<SkeletonManipulator>();
			}else{
				cout << "no skeleton tracking option provided (1-5), using SkeletonManipulator as default" << endl;
				fitting->createSkeleton<SkeletonManipulator>();
			}
			
		}
		
		// init system
		myOSF.prepare();
		

		// create visualization objects
		Visualization2d vis2d(input, seg, feat, fitting);
		// Visualization3d vis3d(input, seg, feat, fitting, cv::Size(800, 600));
		
		//vis2d.recordDepthMap("rec_depthmap.avi");
		//vis2d.recordSegmentation("rec_segmentation.avi");
		//vis2d.recordGeodesicMap("rec_geodesic.avi");
		//vis2d.recordSkeleton("rec_skeleton.avi", 0, 15);

		vis2d.init();
		// vis3d.init();

		/*
		seg
		m_threshold = 0.15f;
		m_erodingSize = 3;
		m_medianBlurSize = 3;
		m_contoursFactor = 300.0f;

		void setThreshold(float);
		void setErodingSize(int);
		void setMedianBlurSize(int);
		void setContoursFactor(float);
		float getThreshold() const;
		int getErodingSize() const;
		int getMedianBlurSize() const;
		float getContoursFactor() const;
		*/
		// createTrackbar("DepthMap", "result", &templScale, 200,on_trackbar);

		// main loop
		bool terminate = false;
		bool paused = false;
		bool step = false;
		do {
			// process
			if (!paused || step) {
				myOSF.process();
				step = false;
			}

			terminate |= myOSF.getTerminate();

			// draw 2d
			bool cvPaused = paused;
			bool cvStep = step;
			terminate |= vis2d.draw(cvPaused, cvStep);
			/*
			// draw 3d
			bool glPaused = paused;
			bool glStep = step;
			terminate |= vis3d.draw(glPaused, glStep);

			bool tempPaused = paused;
			if (cvPaused == !tempPaused)
				paused = cvPaused;
			if (glPaused == !tempPaused)
				paused = glPaused;
			
			bool tempStep = step;
			if (cvStep == !tempStep)
				step = cvStep;
			if (glStep == !tempStep)
				step = glStep;
			//*/
		} while (!terminate);
	}
	catch (Exception& e) {
		ERR << "Exception: \"" <<
			e.what() << "\"" << ENDL;
	}
	catch (std::exception& e) {
		ERR << "std::exception: \"" <<
			e.what() << "\"" << ENDL;
	}
	catch (...) {
		ERR << "unknown exception" << ENDL;
	}
	
	return 0;
}
