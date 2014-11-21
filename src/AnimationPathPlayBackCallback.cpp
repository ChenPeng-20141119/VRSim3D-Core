#include "AnimationPathPlayBackCallback.h"

using namespace VRSim3D;

AnimationPathPlayBackCallback::AnimationPathPlayBackCallback( osg::AnimationPath* ap,
	double timeOffset/*=0.0*/,double timeMultiplier/*=1.0*/ ) : 
osg::AnimationPathCallback(ap,timeOffset,timeMultiplier)
{
}

void AnimationPathPlayBackCallback::setPalyBackTime( double time )
{
	_firstTime = (_latestTime-time)-_timeOffset;
}

double AnimationPathPlayBackCallback::getMinTime()
{
	osg::AnimationPath::TimeControlPointMap::iterator it = _animationPath->getTimeControlPointMap().begin();
	if(it != _animationPath->getTimeControlPointMap().end())
		return it->first;
}

double AnimationPathPlayBackCallback::getMaxTime()
{
	osg::AnimationPath::TimeControlPointMap::reverse_iterator it = _animationPath->getTimeControlPointMap().rbegin();
	if(it != _animationPath->getTimeControlPointMap().rend())
		return it->first;
}

double AnimationPathPlayBackCallback::getNowTime()
{
	return _latestTime;
}