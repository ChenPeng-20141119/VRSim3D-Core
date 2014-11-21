#include "SceneViewManager.h"

#include "CommandHandler.h"
#include "osgDB/FileUtils"

using namespace VRSim3D;

SceneViewManager::SceneViewManager(void)
{
	m_SceneViewBuilder = NULL;
	m_ModelRoot = NULL;
	init();
}

SceneViewManager::~SceneViewManager(void)
{
	/*if(m_SceneViewBuilder)
	{
		delete m_SceneViewBuilder;
		m_SceneViewBuilder = NULL;
	}*/
	if(m_ModelRoot.get())
	{
		m_ModelRoot.release();
		m_ModelRoot = NULL;
	}
	if(!m_Dir.empty())
		m_Dir = "";
	if(m_AllFont.get())
	{
		m_AllFont.release();
		m_AllFont = NULL;
	}
}

VRSim3D::SceneViewManager::SceneViewManager( SceneViewBuilder *sceneViewBuilder ):
m_SceneViewBuilder(sceneViewBuilder)
{
	if(!m_SceneViewBuilder/*.get()*/)
		return;
	init();
	m_ModelRoot = new osg::Group();
	m_ModelRoot->setName("设备层");
	osg::ref_ptr<osg::Group> root = m_SceneViewBuilder->getSceneData()->asGroup();
	AddNode(root.get(),m_ModelRoot.get());
}

bool VRSim3D::SceneViewManager::AddNode( osg::Group *group,osg::Node *child )
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.valid())
		return false;
	osg::ref_ptr<VRSim3D::CommandHandler> cm = VRSim3D::CommandHandler::instance(viewer.get());
	cm->addNodeCommand(group,child);

	return true;
}

bool VRSim3D::SceneViewManager::RemoveNode( osg::Group *group,osg::Node *child )
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.valid())
		return false;
	osg::ref_ptr<VRSim3D::CommandHandler> cm = VRSim3D::CommandHandler::instance(viewer.get());
	cm->removeNodeCommand(group,child);

	return true;
}

//bool VRSim3D::SceneViewManager::Load( const std::string &path )
//{
//	
//}

std::string VRSim3D::SceneViewManager::GetAppPath( void )
{
	char   path_buffer[_MAX_PATH];       
	char   drive[_MAX_DRIVE];       
	char   dir[_MAX_DIR]; 
	char   fname[_MAX_FNAME];       
	char   ext[_MAX_EXT]; 

	/*GetModuleFileName(AfxGetInstanceHandle(),path_buffer,_MAX_PATH*2); */
	///zqgao 这里暂时写死，获取DLL路径
	HINSTANCE hinstance = (HINSTANCE)GetModuleHandle("VRSim3DAPI.dll");
	GetModuleFileName(hinstance,path_buffer,_MAX_PATH);
	//GetCurrentDirectory(_MAX_PATH,drive); 
	_splitpath(path_buffer,   drive,   dir,   fname,   ext); 

	

	std::string   sPath(drive); 
	//sPath=drive; 
	sPath+=dir;
	//MessageBox(NULL,sPath.c_str(),"123",1);
	return   sPath;
	
}

void VRSim3D::SceneViewManager::init()
{
	m_Dir = GetAppPath()  + "3Ddata" + "/";
	///zqgao 设置默认数据路径，用于相对目录
	setDataPath(m_Dir);
	m_AllFont = osgText::readFontFile(m_Dir+"fonts/simhei.ttf");
}

void VRSim3D::SceneViewManager::setDataPath( const std::string path )
{
	osgDB::getDataFilePathList().push_back(path);
}
