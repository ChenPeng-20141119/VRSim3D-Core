#include "VRPrecipitationManager.h"
#include "GlobeBasicFunc.h"

#include "osgGA/KeySwitchMatrixManipulator"

#include "osgEarthUtil/EarthManipulator"

using namespace VRSim3D;

VRPrecipitationManager::VRPrecipitationManager(void)
{
	setNumChildrenRequiringUpdateTraversal(1);
	m_View = NULL;
	m_Mapnode = NULL;
	m_VrPrecipitation = NULL;
	init();
}
VRSim3D::VRPrecipitationManager::VRPrecipitationManager( osgViewer::Viewer* view,osgEarth::MapNode* mapnode ):
m_View(view),
m_Mapnode(mapnode)
{
	setNumChildrenRequiringUpdateTraversal(1);
	m_VrPrecipitation = NULL;
	init();
}
VRPrecipitationManager::~VRPrecipitationManager(void)
{
}

void VRSim3D::VRPrecipitationManager::setViewer( osgViewer::Viewer* view )
{
	m_View = view;
}

void VRSim3D::VRPrecipitationManager::setMapnode( osgEarth::MapNode* mapnode )
{
	m_Mapnode = mapnode;
}

void VRSim3D::VRPrecipitationManager::traverse( osg::NodeVisitor& nv )
{
	if (nv.getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
	{
		updata();
	}
	osg::MatrixTransform::traverse(nv);
}

void VRSim3D::VRPrecipitationManager::setStartHeight( const float& startHeight )
{
	m_StartHeight = startHeight;
}

void VRSim3D::VRPrecipitationManager::setMaxIntensity( const float& maxIntensity )
{
	m_MaxIntensity = maxIntensity;
}

void VRSim3D::VRPrecipitationManager::setMaxfogtensity( const float& maxfogtensity )
{
	m_Maxfogtensity = maxfogtensity;
}

void VRSim3D::VRPrecipitationManager::init()
{
	m_VrPrecipitation = new VRPrecipitation();
	this->addChild(m_VrPrecipitation.get());
	m_MaxIntensity = 0.0f;
	m_Maxfogtensity = 0.0f;
}

void VRSim3D::VRPrecipitationManager::updata()
{
	if(!m_View.valid() || !m_Mapnode.valid() || !m_VrPrecipitation.valid())
		return;
	osg::Vec3d eye, center, up;
	
	m_View->getCamera()->getViewMatrixAsLookAt(eye,center, up);
	osgGA::KeySwitchMatrixManipulator*   swthmanp = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(m_View->getCameraManipulator());
	osgEarth::Util::EarthManipulator* earthManp = dynamic_cast<osgEarth::Util::EarthManipulator*>(swthmanp->getCurrentMatrixManipulator());
	/*if(osgEarth::Util::EarthManipulator::PROJ_ORTHOGRAPHIC != earthManp->getSettings()->getCameraProjection())
		earthManp->getSettings()->setCameraProjection(osgEarth::Util::EarthManipulator::PROJ_ORTHOGRAPHIC);*/
	/*osgEarth::Util::Viewpoint vp = earthManp->getViewpoint();
	osgEarth::GeoPoint map(m_Mapnode->getMapSRS(),vp.x(),vp.y(),vp.getRange(),osgEarth::Util::ALTMODE_ABSOLUTE);*/
	osgEarth::GeoPoint map;
	osg::Vec3d dir;
	map.fromWorld( m_Mapnode->getMapSRS(), eye ); 
	map.makeRelative(m_Mapnode->getTerrain());

	map.createWorldUpVector(dir);
	dir = -dir;

	

	///zqgao 更新特效位置及方向，保证特效运动方向与地面垂直
	float ele = map.z();
	/*if(ele < 2000)
		ele = 2000;*/
	osg::Matrixd mat = VRSim3D::GlobeBasicFunc::getMaxtrixFormLocal(m_Mapnode.get(),map.y(),map.x(),ele);
	osg::Matrixd mat1;
	mat1.makeRotate(mat.getRotate());
	this->setMatrix(mat1);

	///zqgao 基于相机高度计算特效密度 
	float ratio1 = ((m_StartHeight - ele) / m_MaxIntensity);
	ratio1 = osg::clampBetween(ratio1, 0.0f, 1.0f);
	float density1 = ratio1 * m_MaxIntensity;

	float ratio2 = ((m_StartHeight - ele) / m_Maxfogtensity);
	ratio2 = osg::clampBetween(ratio2, 0.0f, 1.0f);
	float density2 = ratio2 * m_Maxfogtensity;
	switch(m_VrPrecipitation->getCurrentPPTType()) 
	{
	case VRPrecipitation::RAIN:
		m_VrPrecipitation->rain(density1,density2);
		break;
	case VRPrecipitation::SNOW:
		m_VrPrecipitation->snow(density1,density2);
		break;
	case VRPrecipitation::FOG:
		m_VrPrecipitation->fog(density2);
		break;
	case VRPrecipitation::HAZE:
		m_VrPrecipitation->haze(density2);
		break;
	default:
		break;
	}
	//setWindProperty(map.x(),1.0);
}

void VRSim3D::VRPrecipitationManager::setWindProperty( float heading, float speed )
{
	float x,y,z;
	heading = osg::DegreesToRadians(heading + 180);
	speed = speed * 0.3048;

	x = -cos(heading) * speed;
	y = sin(heading) * speed;
	z = 0;

	m_VrPrecipitation->setWind(osg::Vec3(x,y,z));
}


