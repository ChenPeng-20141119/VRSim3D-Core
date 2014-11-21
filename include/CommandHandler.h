//////////////////////////////////////////////////////////////////////////
///文件名：命令控制类
///创建者：高志清
///创建时间：2013年10月31日
///内容描述：提供添加添加删除节点/事件等命令，用于场景动态更新
//////////////////////////////////////////////////////////////////////////
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H 1


#include "Export.h"

#include "osg/Group"
#include "osgGA/GUIEventHandler"
#include "osgViewer/Viewer"

#include <vector>
#include <map>

namespace VRSim3D
{
class VRSim3D_EXPORT CommandHandler:public osgGA::GUIEventHandler
{
public: 
	CommandHandler();
	~CommandHandler();
	enum ChildContr
	{
		NONE,
		ADD,
		DEL
	}; 
private:
	///zqgao 子节点队列
	typedef std::vector<osg::ref_ptr<osg::Node>> NodeList;
	///zqgao 待处理队列
	typedef std::map<osg::ref_ptr<osg::Group>,NodeList> WaitDispose;
	///zqgao 待处理增加子节点队列
	WaitDispose m_WaitAddDispose;
	///zqgao 待处理删除子节点队列
	WaitDispose m_WaitDelDispose;
	
	OpenThreads::Mutex m_requestdelMutex;
public:
	static CommandHandler* instance(osgViewer::Viewer* view);
	OpenThreads::Mutex m_requestaddMutex;
	///zqgao 子节点处理命令
	void childCommand(osg::Group* parent,osg::Node* child,ChildContr ctr);

	///zqgao 处理待处理子节点队列
	void disposeWaitChilds(ChildContr ctr);

	///zqgao 增加节点命令
	void addNodeCommand( osg::Group* Parent,osg::Node* child );

	///zqgao删除节点命令
	void removeNodeCommand( osg::Group* Parent,osg::Node* child );

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ); 
};
}
#endif