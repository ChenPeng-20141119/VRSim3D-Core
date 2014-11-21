#define _CRT_SECURE_NO_WARNINGS

#include "ShipWake.h"
#include "osgEarthDrivers/ocean_triton/TritonContext"


using namespace VRSim3D;

ShipWake::ShipWake(osgEarth::Drivers::Triton::TritonContext* triton)
    :_TRITON(triton)
	,_ship(0)
    ,_shipPosition( 0,0,0 )
    ,_shipDirection( 0,0,0 )
    ,_shipVelocity( 0.f )
{
   
}

// Clean up our resources
ShipWake::~ShipWake()
{
    Cleanup();
}

void ShipWake::UpdateShipPos( double x, double y, double z, double dx, double dy, double dz, double velocity )
{
    _shipPosition.set(x, y, z);
    _shipDirection.set(dx, dy, dz);
    _shipVelocity = velocity;
}

void ShipWake::Setup( )
{
    if( !_ship && _TRITON->getOcean() ) {
        Triton::WakeGeneratorParameters params;
		params.bowSprayOffset = 110.0;
		params.bowWaveOffset = 110.0;
		params.beamWidth = 20.0;
		params.sternWaveOffset = -50;
		params.propWashOffset = 50;
        params.sprayEffects = true;
        params.length = 110.0;
        params.propWash = true;
		/*params.propWashOffset = 1;
		params.propWashWidthMultiplier = 5;*/
        /*params.numHullSprays = 10;
		params.hullSprayStartOffset = 110.0;
		params.hullSprayEndOffset = 120.0;*/
		params.hullSprayScale = 10.0;
		params.hullSpraySizeScale = 1.0;
		params.hullSprayVerticalOffset = 0.0;
        _ship = new Triton::WakeGenerator( _TRITON->getOcean(), params);
    }
}

// Clean up our resources
void ShipWake::Cleanup()
{
    if( _ship ) {
        delete _ship;
        _ship = NULL;
    }
}

void VRSim3D::ShipWake::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	if (nv->getVisitorType() != osg::NodeVisitor::UPDATE_VISITOR && 
		!nv->getFrameStamp())
		return;
	update(node,nv);
	if( _ship )
		_ship->Update
		( Triton::Vector3( _shipPosition[0],_shipPosition[1],_shipPosition[2] ),
		Triton::Vector3( _shipDirection[0],_shipDirection[1],_shipDirection[2]),
		_shipVelocity, nv->getFrameStamp()->getSimulationTime() );

	traverse( node, nv );
}

void VRSim3D::ShipWake::update( osg::Node* node,osg::NodeVisitor* nv )
{
	osg::ComputeBoundsVisitor cbbv;
	node->accept(cbbv);
	m_bb = cbbv.getBoundingBox();
	if( !_ship && _TRITON->getOcean() ) 
	{
		_TRITON->getOcean()->EnableGodRays(true);
		float lengh = m_bb.yMax() - m_bb.yMin();
		float width = m_bb.xMax() - m_bb.xMin();

        Triton::WakeGeneratorParameters params;
		params.bowSprayOffset = lengh*1.1;
		params.bowWaveOffset = lengh*1.1;
		params.beamWidth = width/4.0;
		params.sternWaveOffset = -lengh*1.1;
		params.propWashOffset = lengh*1.1;
        params.sprayEffects = true;
        params.length = lengh*1.1;
		params.spraySizeScale = 10;
        params.propWash = true;
		/*params.propWashWidthMultiplier = 5;*/
		params.numHullSprays = 1;
		params.hullSprayStartOffset = lengh/2.0;
		params.hullSprayEndOffset = -lengh/2.0;
		params.hullSprayScale = 2.0;
		params.hullSpraySizeScale = 1.0;
		params.hullSprayVerticalOffset = 0.0;
        _ship = new Triton::WakeGenerator( _TRITON->getOcean(), params);
		//_shipVelocity = lengh;
    }
	if (m_bb.valid())
	{
		static double prevTime = -1;
		static double accumTime = 0;
		double time = nv->getFrameStamp()->getSimulationTime();
		static osg::Vec3d prePosition = osg::Vec3d(0,0,0);
		if(1/*prevTime >0*/)
		{
			_shipPosition = m_bb.center();
			double delta = /*ceil*/((time - prevTime));
			accumTime += delta;
			if(/*accumTime > 1.0  &&*/(_shipPosition - prePosition).length() > 20)
			{
				double deltaS = floor(accumTime );
				_shipDirection = _shipPosition - prePosition;
				_shipVelocity = /*1000**/_shipDirection.length()/delta;
				_shipDirection.normalize();
				//accumTime -= delta;
				prePosition = _shipPosition;
				prevTime = time;
			}
		}
		
	}
	
}
