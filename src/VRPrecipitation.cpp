#include "VRPrecipitation.h"
#include "GlobeBasicFunc.h"

#include "osg/ImageUtils"

using namespace VRSim3D;

VRPrecipitation::VRPrecipitation(void)
{
	setNumChildrenRequiringUpdateTraversal(1);
	m_PPTType = RAIN;
	m_RainTex = NULL;
	m_SnowTex = NULL;
	rain(0,0);
}
VRPrecipitation::~VRPrecipitation(void)
{
}
void VRSim3D::VRPrecipitation::rain( float intensity,float fogtensity )
{
	setCurrentPPTType(RAIN);
	//_wind.set(0.0f,0.0f,0.0f);
	_particleSpeed = -2.0f + -5.0f*intensity;
	_particleSize = 0.01 + 0.02*intensity;
	_particleColor = osg::Vec4(0.6, 0.6, 0.6, 1.0) -  osg::Vec4(0.1, 0.1, 0.1, 1.0)* intensity;
	_maximumParticleDensity = intensity * 8.5f;
	_cellSize.set(5.0f / (0.25f+intensity), 5.0f / (0.25f+intensity), 5.0f);
	_nearTransition = 25.f;
	_farTransition = 100.0f - 60.0f*sqrtf(intensity);

	if (!_fog)
	{
		_fog = new osg::Fog;
		_fog->setMode(osg::Fog::LINEAR);
		_fog->setColor(osg::Vec4(0.6, 0.6, 0.6, 1.0));
		_fog->setUseRadialFog(true);
	}
	_fog->setDensity(fogtensity);

	_useFarLineSegments = false;

	_dirty = true;

	if(!m_RainTex.valid())
	{
		m_RainTex = new osg::Texture2D(osgDB::readImageFile("weather/rain.TGA"));
	}
	if(_stateset.valid())
		_stateset->setTextureAttribute(0,m_RainTex.get());
	update();
}

void VRSim3D::VRPrecipitation::snow( float intensity,float fogtensity )
{
	setCurrentPPTType(SNOW);
	//_wind.set(0.0f,0.0f,0.0f);
	_particleSpeed = -0.75f - 0.25f*intensity;
	_particleSize = 0.02f + 0.03f*intensity;
	_particleColor = osg::Vec4(0.85f, 0.85f, 0.85f, 1.0f) -  osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f)* intensity;
	_maximumParticleDensity = intensity * 8.2f;
	_cellSize.set(5.0f / (0.25f+intensity), 5.0f / (0.25f+intensity), 5.0f);
	_nearTransition = 25.f;
	_farTransition = 100.0f - 60.0f*sqrtf(intensity);

	if (!_fog)
	{
		_fog = new osg::Fog;
		_fog->setMode(osg::Fog::LINEAR);
		_fog->setUseRadialFog(true);
		_fog->setColor(osg::Vec4(0.6, 0.6, 0.6, 1.0));
	}
	_fog->setDensity(fogtensity);
	

	_useFarLineSegments = false;

	_dirty = true;
	if(!m_SnowTex.valid())
	{
		m_SnowTex = new osg::Texture2D(osgDB::readImageFile("weather/Snow.TGA"));
	}
	if(_stateset.valid())
		_stateset->setTextureAttribute(0,m_SnowTex.get());
	update();
}

void VRSim3D::VRPrecipitation::update()
{
	_dirty = false;

	OSG_INFO<<"PrecipitationEffect::update()"<<std::endl;

	float length_u = _cellSize.x();
	float length_v = _cellSize.y();
	float length_w = _cellSize.z();

	// time taken to get from start to the end of cycle
	_period = fabsf(_cellSize.z() / _particleSpeed);

	_du.set(length_u, 0.0f, 0.0f);
	_dv.set(0.0f, length_v, 0.0f);
	_dw.set(0.0f, 0.0f, length_w);

	_inverse_du.set(1.0f/length_u, 0.0f, 0.0f);
	_inverse_dv.set(0.0f, 1.0f/length_v, 0.0f);
	_inverse_dw.set(0.0f, 0.0f, 1.0f/length_w);

	OSG_INFO<<"Cell size X="<<length_u<<std::endl;
	OSG_INFO<<"Cell size Y="<<length_v<<std::endl;
	OSG_INFO<<"Cell size Z="<<length_w<<std::endl;


	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_viewDrawableMap.clear();
	}

	// set up state/
	{
		if (!_stateset)
		{
			_stateset = new osg::StateSet;
			_stateset->addUniform(new osg::Uniform("baseTexture",0));

			_stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			_stateset->setMode(GL_BLEND, osg::StateAttribute::ON);

			osg::Texture2D* texture  = new osg::Texture2D(createSpotLightImage(osg::Vec4(1.0f,1.0f,1.0f,1.0f),osg::Vec4(1.0f,1.0f,1.0f,0.0f),32,1.0));
			_stateset->setTextureAttribute(0, texture);
        }

		/*osg::Texture2D* texture = NULL;
		switch(m_PPTType)
		{
		case RAIN:
		texture  = new osg::Texture2D(createSpotLightImage(osg::Vec4(1.0f,1.0f,1.0f,1.0f),osg::Vec4(1.0f,1.0f,1.0f,0.0f),32,1.0));
		break;
		case SNOW:
		texture = new osg::Texture2D(osgDB::readImageFile("G:/zqgao/osg3.2-osgearth2.5/VRSim3D-Core/bin/res/weather/Snowflake.TGA"));
		break;
		default:
		break;
		}
		_stateset->setTextureAttribute(0,texture);*/

		if (!_inversePeriodUniform)
		{
			_inversePeriodUniform = new osg::Uniform("inversePeriod",1.0f/_period);
			_stateset->addUniform(_inversePeriodUniform.get());
		}
		else _inversePeriodUniform->set(1.0f/_period);

		if (!_particleColorUniform)
		{
			_particleColorUniform = new osg::Uniform("particleColour", _particleColor);
			_stateset->addUniform(_particleColorUniform.get());
		}
		else _particleColorUniform->set(_particleColor);

		if (!_particleSizeUniform)
		{
			_particleSizeUniform = new osg::Uniform("particleSize", _particleSize);
			_stateset->addUniform(_particleSizeUniform.get());
		}
		else _particleSizeUniform->set(_particleSize);

	}
}

void VRSim3D::VRPrecipitation::setCurrentPPTType( PPTType ppt )
{
	if(m_PPTType != ppt)
		m_PPTType = ppt;
}

void VRSim3D::VRPrecipitation::haze( float hazetensity )
{
	setCurrentPPTType(HAZE);
	if (!_fog)
	{
		_fog = new osg::Fog;
		_fog->setMode(osg::Fog::LINEAR);
		_fog->setUseRadialFog(true);
		_fog->setColor(osg::Vec4(113/255.0, 113/255.0, 75/255.0, 1.0));
	}
	
	_fog->setDensity(hazetensity);
}

void VRSim3D::VRPrecipitation::fog( float fogtensity )
{
	setCurrentPPTType(FOG);
	if (!_fog)
	{
		_fog = new osg::Fog;
		_fog->setMode(osg::Fog::LINEAR);
		_fog->setUseRadialFog(true);
		_fog->setColor(osg::Vec4(0.6, 0.6, 0.6, 1.0));
	}
	
	_fog->setDensity(fogtensity);
}



