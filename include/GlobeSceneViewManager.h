#ifndef GLOBE_SCENE_VIEW_MANAGER_H
#define GLOBE_SCENE_VIEW_MANAGER_H 1

#include "Export.h"
#include "SceneViewManager.h"
#include "ViewPosInfoText.h"

#include "osgEarth/MapNode"
#include "osgEarthUtil/EarthManipulator"
#include "osgEarthUtil/Controls"

namespace VRSim3D
{
class VRSim3D_EXPORT GlobeSceneViewManager :
	public VRSim3D::SceneViewManager
{
public:
	GlobeSceneViewManager(void);
	GlobeSceneViewManager(SceneViewBuilder *sceneViewBuilder);
	~GlobeSceneViewManager(void);
	///zqgao 添加模型
	bool Load(const std::string &path);
	///zqgao 飞行定位
	void flyTo(const float& lat,const float &lon,const float &zoom = 5000,
					const float &heading_deg = 73.04,const float &pitch_deg = -20.0,const float &time = 5.0);
	///zqgao 相机绑定（相机跟随，第三人称视角）
	void cameraBound(osg::Node* node,const float &distance);
	///zqgao 解除相机绑定
	void releaseCameraBound();
	///zqgao 鼠标信息控件显隐
	void showMouseInfor(bool isShow = true);
	//zqgao 屏幕中心位置信息显隐
	void showViewPosInfoText(bool isShow = true);
	///zqgao 删除鼠标信息控件
	bool DelMouseInfor();
	//zqgao 删除屏幕中心位置信息
	bool DelViewPosInfoText();
	///zqgao 清除地球数据
	bool ClearGlobeData();
	///zqgao 获取地球节点
	osgEarth::MapNode* getGlobeNode(){return m_MapNode.get();}
private:
	//zqgao 设置地球操控器
	void setEarthManip();
	///zqgao 设置相机裁剪
	void setCameraCull();
	///zqgao 显示鼠标位置
	void showMousePosition(bool isShow = true);
	//zqgao 显示屏幕中心位置信息
	void showViewPosition(bool isShow = true);
private:
	///zqgao 地球节点
	osg::ref_ptr<osgEarth::MapNode>						   m_MapNode;
	///zqgao 地球相机操控器
	osg::ref_ptr<osgEarth::Util::EarthManipulator>         m_EarthManip;
	///zqgao 控件面板层
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
	///zqgao 鼠标消息控件
	osg::ref_ptr<osgEarth::Util::Controls::LabelControl>   m_MouseCoords;
	///zqao 屏幕中心位置信息
	osg::ref_ptr<ViewPosInfoText>						   m_ViewPosInfoText;
};

}

#endif
