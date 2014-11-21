//////////////////////////////////////////////////////////////////////////
///�ļ��������������
///�����ߣ���־��
///����ʱ�䣺2013��10��31��
///�����������ṩ������ɾ���ڵ�/�¼���������ڳ�����̬����
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
	///zqgao �ӽڵ����
	typedef std::vector<osg::ref_ptr<osg::Node>> NodeList;
	///zqgao ���������
	typedef std::map<osg::ref_ptr<osg::Group>,NodeList> WaitDispose;
	///zqgao �����������ӽڵ����
	WaitDispose m_WaitAddDispose;
	///zqgao ������ɾ���ӽڵ����
	WaitDispose m_WaitDelDispose;
	
	OpenThreads::Mutex m_requestdelMutex;
public:
	static CommandHandler* instance(osgViewer::Viewer* view);
	OpenThreads::Mutex m_requestaddMutex;
	///zqgao �ӽڵ㴦������
	void childCommand(osg::Group* parent,osg::Node* child,ChildContr ctr);

	///zqgao ����������ӽڵ����
	void disposeWaitChilds(ChildContr ctr);

	///zqgao ���ӽڵ�����
	void addNodeCommand( osg::Group* Parent,osg::Node* child );

	///zqgaoɾ���ڵ�����
	void removeNodeCommand( osg::Group* Parent,osg::Node* child );

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ); 
};
}
#endif