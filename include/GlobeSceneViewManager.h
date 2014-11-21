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
	///zqgao ���ģ��
	bool Load(const std::string &path);
	///zqgao ���ж�λ
	void flyTo(const float& lat,const float &lon,const float &zoom = 5000,
					const float &heading_deg = 73.04,const float &pitch_deg = -20.0,const float &time = 5.0);
	///zqgao ����󶨣�������棬�����˳��ӽǣ�
	void cameraBound(osg::Node* node,const float &distance);
	///zqgao ��������
	void releaseCameraBound();
	///zqgao �����Ϣ�ؼ�����
	void showMouseInfor(bool isShow = true);
	//zqgao ��Ļ����λ����Ϣ����
	void showViewPosInfoText(bool isShow = true);
	///zqgao ɾ�������Ϣ�ؼ�
	bool DelMouseInfor();
	//zqgao ɾ����Ļ����λ����Ϣ
	bool DelViewPosInfoText();
	///zqgao �����������
	bool ClearGlobeData();
	///zqgao ��ȡ����ڵ�
	osgEarth::MapNode* getGlobeNode(){return m_MapNode.get();}
private:
	//zqgao ���õ���ٿ���
	void setEarthManip();
	///zqgao ��������ü�
	void setCameraCull();
	///zqgao ��ʾ���λ��
	void showMousePosition(bool isShow = true);
	//zqgao ��ʾ��Ļ����λ����Ϣ
	void showViewPosition(bool isShow = true);
private:
	///zqgao ����ڵ�
	osg::ref_ptr<osgEarth::MapNode>						   m_MapNode;
	///zqgao ��������ٿ���
	osg::ref_ptr<osgEarth::Util::EarthManipulator>         m_EarthManip;
	///zqgao �ؼ�����
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
	///zqgao �����Ϣ�ؼ�
	osg::ref_ptr<osgEarth::Util::Controls::LabelControl>   m_MouseCoords;
	///zqao ��Ļ����λ����Ϣ
	osg::ref_ptr<ViewPosInfoText>						   m_ViewPosInfoText;
};

}

#endif
