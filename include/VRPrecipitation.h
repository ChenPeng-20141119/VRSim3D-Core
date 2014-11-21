//////////////////////////////////////////////////////////////////////////
///文件名：特效类（雨雪雾）
///创建者：高志清
///创建时间：2014年7月11日
///内容描述： 雨雪雾特效实现（继承自osgParticle::PrecipitationEffect）
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
	///zqgao 雨模拟
	void rain(float intensity,float fogtensity);

	///zqgao 雪模拟
	void snow(float intensity,float fogtensity);

	///zqgao 雾模拟
	void fog(float fogtensity);

	///zqgao 霾模拟
	void haze(float hazetensity);

	///zqgao 设置当前特效类型
	void setCurrentPPTType(PPTType ppt);

	///zqgao 获取当前特效
	PPTType getCurrentPPTType() const {return m_PPTType;}
protected:
	///zqgao 重载
	virtual void update();
private:
	///zqgao 模拟特效类型
	PPTType m_PPTType;
	///zqgao 雨文理
	osg::ref_ptr<osg::Texture2D> m_RainTex;
	///zqgao 雪文理
	osg::ref_ptr<osg::Texture2D> m_SnowTex;
};
}
#endif
