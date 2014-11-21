#include "RotorWashHandler.h"

#include "osgEarthDrivers/ocean_triton/TritonContext"

using namespace VRSim3D;

RotorWashHandler::RotorWashHandler(osgEarth::Drivers::Triton::TritonContext* triton)
    :m_TRITON(triton)
	,m_wash(0)
    ,m_rotorPosition( 0,0,0 )
    ,m_windDirection( 0,0,0 )
    ,m_windVelocity( 10.f )
{
   
}

// Clean up our resources
RotorWashHandler::~RotorWashHandler()
{
    Cleanup();
}

void RotorWashHandler::UpdateShipPos( double x, double y, double z, double dx, double dy, double dz, double velocity )
{
    m_rotorPosition.set(x, y, z);
    m_windDirection.set(dx, dy, dz);
    m_windVelocity = velocity;
}

void RotorWashHandler::Setup( )
{
    if( !m_wash && m_TRITON->getOcean() ) {
        m_wash = new VRSim3D::MyRotorWash( m_TRITON->getOcean(),30,true);
    }
}

// Clean up our resources
void RotorWashHandler::Cleanup()
{
    if( m_wash ) {
        delete m_wash;
        m_wash = NULL;
    }
}

void VRSim3D::RotorWashHandler::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	/*if (nv->getVisitorType() != osg::NodeVisitor::UPDATE_VISITOR && 
		!nv->getFrameStamp())
		return;*/
	update(node,nv);
	if( m_wash )
		m_wash->Update
		( Triton::Vector3( m_rotorPosition[0],m_rotorPosition[1],m_rotorPosition[2] ),
		Triton::Vector3( m_windDirection[0],m_windDirection[1],m_windDirection[2]),
		m_windVelocity, nv->getFrameStamp()->getSimulationTime() );

	traverse( node, nv );
}

void VRSim3D::RotorWashHandler::update( osg::Node* node,osg::NodeVisitor* nv )
{
	osg::ComputeBoundsVisitor cbbv;
	node->accept(cbbv);
	m_bb = cbbv.getBoundingBox();
	
	if (m_bb.valid())
	{
		if(/* !m_wash &&*/ m_TRITON->getOcean() ) 
		{
			float diameter = m_bb.radius();
			m_wash = new VRSim3D::MyRotorWash( m_TRITON->getOcean(),diameter,true,false);
			m_windDirection = computeWindDir();
			m_wash->setmaxVelocity(200);
			/*m_wash->setwindScale(4);*/
		}
		m_rotorPosition = m_bb.center();
		//m_windDirection = computeWindDir();
	}
	
}

void VRSim3D::RotorWashHandler::setwindVelocity( const double &velocity )
{
	m_windVelocity = velocity;
}

osg::Vec3d VRSim3D::RotorWashHandler::computeWindDir()
{
	osg::Vec3d out_down;
	
	if ( m_TRITON->getSRS()->isGeographic() )
	{
		double lat,lon,ele;
		m_TRITON->getSRS()->getEllipsoid()->convertXYZToLatLongHeight(
			m_rotorPosition.x(),m_rotorPosition.y(),m_rotorPosition.z(),
			lat,lon,ele);
		double a = osg::RadiansToDegrees(lon);
		double b = osg::RadiansToDegrees(lat);
		double coslon = cos( lon );
		double coslat = cos( lat );
		double sinlon = sin( lon );
		double sinlat = sin( lat );

		out_down.set( -coslon*coslat, -sinlon*coslat, -sinlat );
	}
	else
	{
		out_down = -m_TRITON->getSRS()->getEllipsoid()->computeLocalUpVector( m_rotorPosition.x(), m_rotorPosition.y(), m_rotorPosition.z() );
	}

	return out_down;
}
