#include "CommandHandler.h"

using namespace VRSim3D;

CommandHandler::CommandHandler()
{

}

CommandHandler::~CommandHandler()
{

}

bool CommandHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if ( ea.getEventType() !=osgGA::GUIEventAdapter::FRAME ) 
		return false;
	///zqgao 处理待增加子节点
	disposeWaitChilds(ADD);

	///zqgao 处理待删除子节点
	disposeWaitChilds(DEL);

#if 0	
	WaitDispose::iterator itor = m_WaitAddDispose.begin();
	WaitDispose::iterator delitor = m_WaitDelDispose.begin();
	if(itor == m_WaitAddDispose.end() && delitor == m_WaitDelDispose.end())
		return false;
	///zqgao 处理待增加子节点
	{
		while(itor != m_WaitAddDispose.end())
		{
			NodeList childList = itor->second;
			NodeList localchildList;
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestaddMutex);
			localchildList.swap(childList);
			NodeList::iterator itor1 = localchildList.begin();
			while(itor1 != localchildList.end())
			{
				itor->first->addChild((*itor1).get());
				++itor1;
			}
			m_WaitAddDispose.erase(itor++);
		}
	}
	
	///zqgao 处理待删除子节点
	{
		while(delitor != m_WaitDelDispose.end())
		{
			NodeList delchildList = delitor->second;
			NodeList dellocalchildList;
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestdelMutex);
			dellocalchildList.swap(delchildList);
			NodeList::iterator delitor1 = dellocalchildList.begin();
			while(delitor1 != dellocalchildList.end())
			{
				delitor->first->removeChild((*delitor1).get());
				++delitor1;
			}
			m_WaitDelDispose.erase(delitor++);
		}
	}

	std::vector<osg::ref_ptr<osg::Node>> localChildList;
	std::vector<osg::Group*> localParentList;
	{ 
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestaddMutex); 
		localChildList.swap( m_ChildList ); 
		//localParentList.swap(m_ParentList);
	} 
	for (std::vector<osg::ref_ptr<osg::Node>>::iterator itr=localChildList.begin();
		itr!=localChildList.end(); ++itr)
	{
		osg::Node* node = itr->get();
		m_tempParent->addChild(node);
	}
	m_tempParent = NULL;

#endif
	return true;
}

void CommandHandler::addNodeCommand( osg::Group* Parent,osg::Node* child )
{
	if(!Parent || !child)
		return;
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestaddMutex);
	WaitDispose::iterator itor = m_WaitAddDispose.find(Parent);
	NodeList childlist;
	if(itor != m_WaitAddDispose.end())
	{
		///zqgao 父节点已在待处理队列，增加待处理子节点队列
		childlist = itor->second;
		NodeList::iterator itor1 = childlist.begin();
		///zqgao 子节点队列中是否已存在
		bool isIn = false;
		while(itor1 != childlist.end())
		{
			if(child == (*itor1).get())
			{
				isIn = true;
				break;
			}
			++itor1;
		}
		///zqgao 将节点放入待处理子节点队列
		if(!isIn)
		{
			childlist.push_back(child);
			itor->second = childlist;
		}
	}else
	{
		///zqgao 新增待处理队列
		childlist.push_back(child);
		m_WaitAddDispose.insert(std::make_pair(Parent,childlist));
	}
}

void CommandHandler::removeNodeCommand( osg::Group* Parent,osg::Node* child )
{
	if(!Parent || !child)
		return;
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestdelMutex);
	WaitDispose::iterator itor = m_WaitDelDispose.find(Parent);
	
	NodeList childlist;
	if(itor != m_WaitDelDispose.end())
	{
		///zqgao 父节点已在待处理队列，增加待处理子节点队列
		childlist = itor->second;
		NodeList::iterator itor1 = childlist.begin();
		///zqgao 子节点队列中是否已存在
		bool isIn = false;
		while(itor1 != childlist.end())
		{
			if(child == (*itor1).get())
			{
				isIn = true;
				break;
			}
			++itor1;
		}
		///zqgao 将节点放入待处理子节点队列
		if(!isIn)
		{
			childlist.push_back(child);
			itor->second = childlist;
		}
	}else
	{
		///zqgao 新增待处理队列
		childlist.push_back(child);
		m_WaitDelDispose.insert(std::make_pair(Parent,childlist));
	}
}

void CommandHandler::childCommand( osg::Group* parent,osg::Node* child,ChildContr ctr)
{
	if(!parent || !child)
		return;
	switch(ctr)
	{
	case ADD:
		{
			addNodeCommand(parent,child);
		}
		
		break;
	case DEL:
		{
			removeNodeCommand(parent,child);
		}
		break;
	case NONE:
		{
			return;
		}
		break;
	default:
		{
			return;
		}
		break;
	}
}

void CommandHandler::disposeWaitChilds( ChildContr ctr )
{
	WaitDispose *waitDispose = NULL;
	switch(ctr)
	{
	case ADD:
		{
			if(m_WaitAddDispose.begin() == m_WaitAddDispose.end())
				return;
			waitDispose = &m_WaitAddDispose;
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestaddMutex);
		}

		break;
	case DEL:
		{
			if(m_WaitDelDispose.begin() == m_WaitDelDispose.end())
				return;
			waitDispose = &m_WaitDelDispose;
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestdelMutex);
		}
		break;
	case NONE:
		{
			return;
		}
		break;
	default:
		{
			return;
		}
		break;
	}
	WaitDispose::iterator itor = waitDispose->begin();
	///zqgao 处理待处理子节点
	{
		while(itor != waitDispose->end())
		{
			NodeList childList = itor->second;
			NodeList localchildList;
			//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_requestdelMutex);
			localchildList.swap(childList);
			NodeList::iterator itor1 = localchildList.begin();
			while(itor1 != localchildList.end())
			{
				if(ADD == ctr)
				{
					itor->first->addChild((*itor1).get());
				}else 
				if(DEL == ctr)
				{
					itor->first->removeChild((*itor1).get());
				}
				++itor1;
			}
			waitDispose->erase(itor++);
		}
	}
}


CommandHandler* VRSim3D::CommandHandler::instance(osgViewer::Viewer* view)
{
	static CommandHandler* cm = 0L;

	if(!cm)
	{
		cm = new CommandHandler();
		view->addEventHandler(cm);
	}

	return cm;
}
