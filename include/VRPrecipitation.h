//////////////////////////////////////////////////////////////////////////
///�ļ�������Ч�ࣨ��ѩ��
///�����ߣ���־��
///����ʱ�䣺2014��7��11��
///���������� ��ѩ����Чʵ�֣��̳���osgParticle::PrecipitationEffect��
//////////////////////////////////////////////////////////////////////////

#ifndef VR_PRECIPITATION_H
#define VR_PRECIPITATION_H 1

#include "Export.h"

#include "osgParticle/PrecipitationEffect"
#include "osgViewer/Viewer"
#include "osg/Texture2D"

#include "osgEarth/MapNode"


namespace VRSim3D {

class VRSim3D_EXPORT VRPrecipitation:public osgParticle::PrecipitationEffect
{
public:
	VRPrecipitation(void);
	VRPrecipitation(osgViewer::Viewer* view,osgEarth::MapNode* mapnode);
	~VRPrecipitation(void);
	enum PPTType
	{
		RAIN,
		SNOW,
		FOG,
		HAZE,
		NONE
	};
public:
	///zqgao ��ģ��
	void rain(float intensity,float fogtensity);

	///zqgao ѩģ��
	void snow(float intensity,float fogtensity);

	///zqgao ��ģ��
	void fog(float fogtensity);

	///zqgao ��ģ��
	void haze(float hazetensity);

	///zqgao ���õ�ǰ��Ч����
	void setCurrentPPTType(PPTType ppt);

	///zqgao ��ȡ��ǰ��Ч
	PPTType getCurrentPPTType() const {return m_PPTType;}
protected:
	///zqgao ����
	virtual void update();
private:
	///zqgao ģ����Ч����
	PPTType m_PPTType;
	///zqgao ������
	osg::ref_ptr<osg::Texture2D> m_RainTex;
	///zqgao ѩ����
	osg::ref_ptr<osg::Texture2D> m_SnowTex;
};
}
#endif
