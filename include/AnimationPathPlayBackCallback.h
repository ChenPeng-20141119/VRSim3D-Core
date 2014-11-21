#ifndef ANIMATIONPATHPLAYCALLBACK_H
#define ANIMATIONPATHPLAYCALLBACK_H 1

#include "osg/AnimationPath"
#include "Export.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT AnimationPathPlayBackCallback : public osg::AnimationPathCallback
	{
	public:
		AnimationPathPlayBackCallback(osg::AnimationPath* ap,
			double timeOffset=0.0,double timeMultiplier=1.0);

		double getMinTime();

		double getMaxTime();

		double getNowTime();

		void setPalyBackTime(double time);
	};
}

#endif