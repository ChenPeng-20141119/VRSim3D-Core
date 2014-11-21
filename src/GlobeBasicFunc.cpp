#include "GlobeBasicFunc.h"

#include "osgEarth/NodeUtils"
#include "osgEarth/DateTime"
#include "osgEarth/GeoData"
#include "osgEarth/ModelLayer"
#include "osgEarth/PhongLightingEffect"
#include "osgEarthDrivers/sky_simple/SimpleSkyOptions"
#include "osgEarthDrivers/feature_ogr/OGRFeatureOptions"
#include "osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions"
#include "osgEarthDrivers/ocean_triton/Tritonoptions"

using namespace VRSim3D::GlobeBasicFunc;

osgEarth::Util::SkyNode* VRSim3D::GlobeBasicFunc::createSkymodel( osgViewer::Viewer* view,osgEarth::MapNode* mapnode )
{
	osgEarth::Util::SkyOptions options;
	//osgEarth::Drivers::SimpleSky::SimpleSkyOptions options;
	if(mapnode->getMapSRS()->isGeographic())
	{
		options.setDriver("simple");
		//options.atmosphericLighting() = false;
	}else
		options.setDriver("gl");
	osg::ref_ptr<osgEarth::Util::SkyNode> skynode = osgEarth::Util::SkyNode::create(options,mapnode);
	if(skynode.valid())
	{
		//// scene lighting
		//osg::StateSet* stateset = skynode->getOrCreateStateSet();
		//osg::ref_ptr<osgEarth::PhongLightingEffect> lighting = new osgEarth::PhongLightingEffect();
		//lighting->setCreateLightingUniform( false );
		//lighting->attach( stateset );

		//// ensure it's depth sorted and draws after the terrain
		//stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		skynode->attach(view,0);
		osgEarth::insertGroup(skynode.get(),mapnode);
	}

	return skynode.release();
}

extern VRSim3D_EXPORT osgEarth::Util::OceanNode* VRSim3D::GlobeBasicFunc::createOceanmodel( osgEarth::MapNode* mapnode,const std::string &resources )
{
	osgEarth::Drivers::Triton::TritonOptions options;
	//options.setDriver("triton");
	options.resourcePath() = resources;
	options.maxAltitude() = 300000;
	osg::ref_ptr<osgEarth::Util::OceanNode> ocean = osgEarth::Util::OceanNode::create(options,mapnode);

	return ocean.release();
}

extern VRSim3D_EXPORT osg::Matrixd VRSim3D::GlobeBasicFunc::getMaxtrixFormLocal( osgEarth::MapNode *mapnode,const float &lat,const float &lon,const float &ele )
{
	osg::Matrixd local2world;
	/*osgEarth::GeoPoint geo(mapnode->getMapSRS()->getGeographicSRS(),lon,lat,ele,osgEarth::ALTMODE_RELATIVE);

	osgEarth::GeoPoint absPos(geo);
	
	if ( !geo.createLocalToWorld(local2world) )
	return local2world;*/
	if(mapnode->getMapSRS()->getGeographicSRS()->createLocalToWorld(osg::Vec3(lon,lat,ele),local2world))
		return local2world;
	return local2world;
}

extern VRSim3D_EXPORT osg::Referenced* VRSim3D::GlobeBasicFunc::loadBuildingsFromShp( const std::string &features,const std::string &texture )
{
	osgEarth::Drivers::OGRFeatureOptions feature_opt;
	feature_opt.name() = "buildings";
	feature_opt.url() = features;
	feature_opt.buildSpatialIndex() = true;

	///zqgao 建筑样式数据
	osgEarth::Symbology::Style buildingStyle;
	buildingStyle.setName( "buildings" );

	osgEarth::Symbology::ExtrusionSymbol* extrusion = buildingStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>();
	extrusion->heightExpression() = osgEarth::Symbology::NumericExpression( "3.5 * max( [story_ht_], 1 )" );
	extrusion->flatten() = true;
	extrusion->wallStyleName() = "building-wall";
	extrusion->roofStyleName() = "building-roof";
	extrusion->wallGradientPercentage() = 0.8;

	osgEarth::Symbology::PolygonSymbol* poly = buildingStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>();
	poly->fill()->color() = osgEarth::Symbology::Color::White;

	///zqgao 设置建筑物贴地
	osgEarth::Symbology::AltitudeSymbol* al = buildingStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	al->clamping() = al->CLAMP_TO_TERRAIN;
	al->technique() = al->TECHNIQUE_MAP;
	al->clampingResolution() = 0.001;

	///zqgao 墙面样式
	osgEarth::Symbology::Style wallStyle;
	wallStyle.setName( "building-wall" );
	osgEarth::Symbology::SkinSymbol* wallSkin = wallStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
	wallSkin->libraryName() = "us_resources";
	wallSkin->addTag( "building" );
	wallSkin->randomSeed() = 1;

	///zqgao 屋顶样式
	osgEarth::Symbology::Style roofStyle;
	roofStyle.setName( "building-roof" );
	osgEarth::Symbology::SkinSymbol* roofSkin = roofStyle.getOrCreate<osgEarth::Symbology::SkinSymbol>();
	roofSkin->libraryName() = "us_resources";
	roofSkin->addTag( "rooftop" );
	roofSkin->randomSeed() = 1;
	roofSkin->isTiled() = true;

	///zqgao 创建样式集，并将样式加入其中
	osgEarth::Symbology::StyleSheet* styleSheet = new osgEarth::Symbology::StyleSheet();
	styleSheet->addStyle( buildingStyle );
	styleSheet->addStyle( wallStyle );
	styleSheet->addStyle( roofStyle );

	///zqgao 加载资源库，其中包含建筑文理
	osgEarth::Symbology::ResourceLibrary* reslib = new osgEarth::Symbology::ResourceLibrary( "us_resources", texture );
	styleSheet->addResourceLibrary( reslib );

	///zqgao 创建分页方式（分块）
	osgEarth::Features::FeatureDisplayLayout layout;
	layout.tileSizeFactor() = 52.0;
	layout.addLevel( osgEarth::Features::FeatureLevel(0.0f, 20000.0f, "buildings") );

	///zqgao 创建模型层来渲染样式集中的建筑 
	osgEarth::Drivers::FeatureGeomModelOptions fgm_opt;
	fgm_opt.featureOptions() = feature_opt;
	fgm_opt.styles() = styleSheet;
	fgm_opt.layout() = layout;

	osg::ref_ptr< osgEarth::ModelLayer> modelLayer =  new osgEarth::ModelLayer( "buildings", fgm_opt ) ;

	return modelLayer.release();
}

extern VRSim3D_EXPORT osg::AnimationPath* VRSim3D::GlobeBasicFunc::createEarthAnimationPath( 
																osgEarth::MapNode* mapNode, const osg::Vec3& center, 
																const osg::Vec3&scale,const osg::Vec3d&Z,const osg::Vec3d&toWard,
																const float& radius,
																const double& looptime )
{
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;

	double delta = osg::PI * 2.0 / (double)numSamples;

	///zqgao 获取中心点坐标
	osgEarth::GeoPoint centerMap(mapNode->getMapSRS(), center,osgEarth::Util::ALTMODE_ABSOLUTE);
	///zqgao 中心点世界坐标
	osg::Vec3d centerWorld;
	centerMap.toWorld(centerWorld);
	///zqgao 获取上方向
	osg::Vec3d up;
	centerMap.createWorldUpVector(up);
	///zqgao 获取运动前方向（圆切线方向）,需保证运动模型模型坐标Z轴正方向为其上方向且Y轴负方向为其头朝向
	osg::Vec3d side = up ^ Z;
	osg::Vec3d dir = osg::Vec3d(0,0,0);


	double time=0.0f;
	double time_delta = looptime/(double)numSamples;

	osg::Vec3d firstPosition;
	osg::Quat firstRotation;

	for (unsigned int i = 0; i < (unsigned int)numSamples; i++)
	{
		double angle = delta * (double)i;
		osg::Quat quat(angle, up );
		osg::Vec3d spoke = quat * (side * radius);
		osg::Vec3d end = centerWorld + spoke;
		spoke.normalize();

		osg::Quat makeUp;
		makeUp.makeRotate(Z, up);
		dir = up ^ spoke;

		osg::Quat rot;
		rot.makeRotate(makeUp*toWard,dir);
		animationPath->insert(time,osg::AnimationPath::ControlPoint(end,makeUp*rot,scale));
		if (i == 0)
		{
			firstPosition = end;
			firstRotation = makeUp*rot; 
		}
		time += time_delta;            
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(firstPosition, firstRotation,scale));

	return animationPath.release();
}



VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback::AnimateSkyUpdateCallback( double rate /*= 1440 */ ):
m_rate( rate ),
m_prevTime( -1 ),
m_accumTime( 0.0 ),
m_Sky(NULL)
{

}

void VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	m_Sky = dynamic_cast< osgEarth::Util::SkyNode* >( node );
	if (m_Sky)
	{ 
		setAmbientBrightness();
		double time = nv->getFrameStamp()->getSimulationTime();            
		if (m_prevTime > 0)
		{                
			osgEarth::TimeStamp t = m_Sky->getDateTime().asTimeStamp();       
			double delta = ceil((time - m_prevTime) * m_rate);
			m_accumTime += delta;
			/// 当时间变化大于一秒时才会对星空时间进行更改
			if (m_accumTime > 1.0)
			{
				double deltaS = floor(m_accumTime );                    
				m_accumTime -= deltaS;
				t += deltaS;
				m_Sky->setDateTime( t );            
			}                
		}            
		m_prevTime = time;
	}
	traverse( node, nv );
}

void VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback::setRate( const double &rate )
{
	if(abs(m_rate - rate) < 0.00000001f)
		return;
	if(m_Sky != NULL)
	{
		osgEarth::DateTime t;
		m_Sky->setDateTime(t);
	}
	m_rate = rate;
}

void VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback::setAmbientBrightness()
{
	float value = m_Sky->getSunLight()->getAmbient().r();
	if(0 < value && 0.8 > value)
	{
		value += 0.001;
		m_Sky->getSunLight()->setAmbient(osg::Vec4f(value,value,value,1.0));
	}
}
