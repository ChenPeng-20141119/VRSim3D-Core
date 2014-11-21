//////////////////////////////////////////////////////////////////////////
///文件名：控制类（雨雪雾）
///创建者：高志清
///创建时间：2014年7月19日
///内容描述： 雨雪雾特效控制
//////////////////////////////////////////////////////////////////////////

#ifndef VR_PRECIPITATION_MANAGER_H
#define VR_PRECIPITATION_MANAGER_H 1

#include "Export.h"

#include "VRPrecipitation.h"

#include "osg/MatrixTransform"
#include "osgViewer/Viewer"

#include "osgEarth/MapNode"


namespace VRSim3D {

class VRSim3D_EXPORT VRPrecipitationManager:public osg::MatrixTransform
{
public:
	VRPrecipitationManager(void);
	VRPrecipitationManager(osgViewer::Viewer* view,osgEarth::MapNode* mapnode);
	~VRPrecipitationManager(void);
public:
	///zqgao
	void setViewer(osgViewer::Viewer* view);
	void setMapnode(osgEarth::MapNode* mapnode);

	///zqgao 特效起始高度（相对地面）
	void setStartHeight(const float& startHeight);

	///zqgao 特效最大密度
	void setMaxIntensity(const float& maxIntensity);
	float getMaxIntensity() const {return m_MaxIntensity;}

	///zqgao 特效雾最大密度
	void setMaxfogtensity(const float& maxfogtensity);
	float getMaxfogtensity()  const {return m_Maxfogtensity;}

	///zqgao 获取特效节点
	VRPrecipitation* getVRPrecipitation(){return m_VrPrecipitation.get();}

	///zqgao 设置风向
	/*将特效放在地球上后，x轴正方向为地理南方向，
	*通过角度和速度，可计算出世界坐标风向
	*/
	void setWindProperty(float heading, float speed);

	virtual void traverse(osg::NodeVisitor& nv);
private:
	///zqgao 初始化
	void init();
	///zqgao 更新
	void updata();
private:
	///zqgao 特效节点
	osg::ref_ptr<VRPrecipitation> m_VrPrecipitation;
	///zqgao 特效起始高度（相对地面）
	float	m_StartHeight;
	///zqgao 特效密度（0-1）
	float m_MaxIntensity;
	///zqgao 特效雾密度
	float m_Maxfogtensity;
	osg::ref_ptr<osgViewer::Viewer> m_View;
	osg::ref_ptr<osgEarth::MapNode> m_Mapnode;
};
}
#endif
