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
	///zqgao 添加节点
	bool AddNode(osg::Group *group,osg::Node *child);
	///zqgao 删除节点
	bool RemoveNode(osg::Group *group,osg::Node *child);
	///zqgao 添加事件
	bool AddEventHandler(osgGA::GUIEventHandler *handler);
	///zqgao 删除事件
	bool RemoveEventHandler(osgGA::GUIEventHandler *handler);
	/*///zqgao 添加模型
	bool Load(const std::string &path);*/
	///zqgao 设置数据路径
	void setDataPath(const std::string path);
	///zqgao 获取默认数据路径
	std::string getDataPath() const{return m_Dir;}
	///
	osg::Group* getModelRoot()const {return m_ModelRoot.get();}
private:
	///zqgao 获取运行路径
	std::string GetAppPath(void);
	///zqgao 初始化
	void init();
protected:
	///zqgao 场景创建
	SceneViewBuilder*									   m_SceneViewBuilder;
	///zqgao 设备层
	osg::ref_ptr<osg::Group>							   m_ModelRoot;
	///zqgao 全局字体文件
	osg::ref_ptr<osgText::Font>							   m_AllFont;
	///zqgao 数据路径
	std::string											   m_Dir;
	///zqgao 控件面板层
	//osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
};

}
#endif
