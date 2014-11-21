#ifndef SCENE_VIEW_MANAGER_H
#define SCENE_VIEW_MANAGER_H 1

#include "Export.h"

#include "SceneViewBuilder.h"

#include "osg/Referenced"

namespace VRSim3D
{

class VRSim3D_EXPORT SceneViewManager /*:
	public osg::Referenced*/
{
public:
	SceneViewManager(void);
	SceneViewManager(SceneViewBuilder *sceneViewBuilder);
	~SceneViewManager(void);
	///zqgao ��ӽڵ�
	bool AddNode(osg::Group *group,osg::Node *child);
	///zqgao ɾ���ڵ�
	bool RemoveNode(osg::Group *group,osg::Node *child);
	///zqgao ����¼�
	bool AddEventHandler(osgGA::GUIEventHandler *handler);
	///zqgao ɾ���¼�
	bool RemoveEventHandler(osgGA::GUIEventHandler *handler);
	/*///zqgao ���ģ��
	bool Load(const std::string &path);*/
	///zqgao ��������·��
	void setDataPath(const std::string path);
	///zqgao ��ȡĬ������·��
	std::string getDataPath() const{return m_Dir;}
	///
	osg::Group* getModelRoot()const {return m_ModelRoot.get();}
private:
	///zqgao ��ȡ����·��
	std::string GetAppPath(void);
	///zqgao ��ʼ��
	void init();
protected:
	///zqgao ��������
	SceneViewBuilder*									   m_SceneViewBuilder;
	///zqgao �豸��
	osg::ref_ptr<osg::Group>							   m_ModelRoot;
	///zqgao ȫ�������ļ�
	osg::ref_ptr<osgText::Font>							   m_AllFont;
	///zqgao ����·��
	std::string											   m_Dir;
	///zqgao �ؼ�����
	//osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
};

}
#endif
