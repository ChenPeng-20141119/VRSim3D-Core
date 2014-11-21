#include "MyRotorWash.h"


using namespace VRSim3D;

VRSim3D::MyRotorWash::MyRotorWash( Triton::Ocean *pOcean, double pRotorDiameter, bool pSprayEffects /*= false*/, bool pUseDecals /*= false*/ )
:Triton::RotorWash(pOcean,pRotorDiameter,pSprayEffects,pUseDecals)
{
	//wakeNumber = 1;
}

void VRSim3D::MyRotorWash::setmaxVelocity( const double &velocity )
{
	maxVelocity = velocity;
}

void VRSim3D::MyRotorWash::setwindScale( const double &scale )
{
	windScale = scale;
}

MyRotorWash::~MyRotorWash()
{
}
