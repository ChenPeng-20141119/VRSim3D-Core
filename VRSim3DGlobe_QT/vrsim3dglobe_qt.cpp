#include "vrsim3dglobe_qt.h"
#include "SingleSceneViewBuilder.h"
#include "osgEarthUtil/Fog"
#include "osgEarth/GeoTransform"
#include "CommEffect.h"
#include "SphereSegment.h"
#include "RectEffect.h"
#include "SingleSceneViewBuilder.h"
#include "ShipWake.h"
#include "RotorWashHandler.h"
#include "SilverLiningNode.h"
#include "ModelPicker.h"
#include "CommEffect.h"
#include "osgEarthDrivers/ocean_triton/TritonNode"

VRSim3DGlobe_QT::VRSim3DGlobe_QT(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_GLManager = NULL;
	m_Builder = NULL;
	m_TimeSim = 0;
	m_isSky = false;
	m_Sky = NULL;
	m_Ocean = NULL;
	m_AnimateSky = NULL;
	m_VRPrecipitationManager = NULL;
	//chenpeng
	m_satelliteGroupManager = NULL;
	m_NodeInfoCallback = NULL;
	m_NowPickNode = NULL;
	m_conBar = NULL;
	m_modelPicker = NULL;
	m_conBarCallback = NULL;
	initView();
}

VRSim3DGlobe_QT::~VRSim3DGlobe_QT()
{

}

void VRSim3DGlobe_QT::initView()
{
	m_Builder = new VRSim3D::SingleSceneViewBuilder();
	m_Builder->createSceneView(NULL);

	m_modelPicker = new VRSim3D::ModelPicker();
	m_modelPicker->m_PickEvent.Bind(this,&VRSim3DGlobe_QT::OnPickEvent);
	m_modelPicker->m_PickOverEvent.Bind(this,&VRSim3DGlobe_QT::OnPickOverEvent);
	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>(m_Builder->getViewer());
	if(viewer)
	{
		viewer->addEventHandler(m_modelPicker);
		addControlBar();
	}

	if(m_conBar)
	{
		m_conBarCallback = new VRSim3D::ConBarCallback(m_NowPickNode,m_conBar);
		m_Builder->getSceneData()->addUpdateCallback(m_conBarCallback);
	}

	QGLWidget *widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
	widget->makeCurrent();
	
	m_qview = new GraphicsView;
	m_qview->setViewport(widget);
	m_qview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	QGraphicsScene* ssv = dynamic_cast<QGraphicsScene*>(m_Builder->getSceneView());
	if(ssv)
		m_qview->setScene(ssv);
	m_qview->viewport()->setMouseTracking(true);
	m_qview->showNormal();

	//chenpeng
	//m_qview->showMinimized();

	setCentralWidget(m_qview);


	//ceshi
	m_GLManager = new VRSim3D::GlobeSceneViewManager(m_Builder);
	m_GLManager->Load("D:\\CGGlobe\\Debug\\data\\map\\globe.earth");
	m_GLManager->showViewPosInfoText(false);
	//CreateSKy();
	//CreateOcean();
	//CreateLoadShip();
	AddSatelliteNode("Satellite 1    ",
		"FinalOutput_WX6000km.txt",
		"model/artemis.ive");
	//CreateSnow();
	//CreateRain();
	//CreateClouds();
	//LoadBSTBuilding();
	//LoadGroundObject("leida","model/leida.ive",100,100,100,118.00,25.00,0);
	//LoadGroundObject("leida2","model/leida.ive",100,100,100,118.00,26.00,0);
	//CreatePowerRange(0,10000,0.0f,osg::PI * 2.0, 0,osg::PI / 2.0,40,10,30,118.00,25.00,0);
}

void VRSim3DGlobe_QT::CreateSKy()
{
	// TODO: 在此添加命令处理程序代码
	m_isSky = !m_isSky;
	if(!m_Sky.valid())
	{
		if(!m_Builder)
			return;
		osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_Builder->getViewer());
		if(!m_GLManager)
			return;
		m_Sky = VRSim3D::GlobeBasicFunc::createSkymodel(view.get(),m_GLManager->getGlobeNode());
		m_Sky->setMoonVisible(true);
		if(m_AnimateSky == NULL)
		{
			m_AnimateSky = new VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback();
			/*m_Sky->getSunLight()->setAmbient(osg::Vec4f(0.9,0.9,0.9,1.0)); */
			m_Sky->addCullCallback(m_AnimateSky);
			//OnTimeSet(m_TimeSim);
		}
	}
}

void VRSim3DGlobe_QT::CreateOcean()
{
	// TODO: 在此添加命令处理程序代码
	if(!m_Ocean.valid())
	{
		if(!m_Builder || !m_GLManager)
			return;
		std::string  resources = m_GLManager->getDataPath() + "sundog/Resources";
		m_Ocean = VRSim3D::GlobeBasicFunc::createOceanmodel(m_GLManager->getGlobeNode(),resources);
		if(m_Ocean.valid())
		{
			//m_Ocean->setSeaLevel(20);
			//m_Sky->addChild(m_Ocean.get());
			m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),m_Ocean.get());
			//m_GLManager->flyTo(22.57,123.24,20,24.261,42.34425,5);
			//m_Builder->getSceneData()->asGroup()->addChild(m_Ocean.get());
		}
	}else
	{
		m_Ocean->setNodeMask(!m_Ocean->getNodeMask());
	}
}

void VRSim3DGlobe_QT::CreateRain()
{
	// TODO: 在此添加命令处理程序代码
	setPrecipitation(VRSim3D::VRPrecipitation::RAIN);
}

void VRSim3DGlobe_QT::CreateSnow()
{
	// TODO: 在此添加命令处理程序代码
	setPrecipitation(VRSim3D::VRPrecipitation::SNOW);
}

void VRSim3DGlobe_QT::LoadBSTBuilding()
{
	// TODO: 在此添加命令处理程序代码
	static bool isload = false;

	if(!isload && m_GLManager)
	{
		std::string features = "boston_buildings_utm19.shp";
		std::string texture = "resources/textures_us/catalog.xml";
		std::string datapath = m_GLManager->getDataPath();
		osg::ref_ptr<osgEarth::MapNode> mapnode = m_GLManager->getGlobeNode();
		osg::ref_ptr<osgEarth::ModelLayer> modellayer = dynamic_cast<osgEarth::ModelLayer*>
			(VRSim3D::GlobeBasicFunc::loadBuildingsFromShp(datapath+features,datapath+texture));
		if(modellayer.valid())
			mapnode->getMap()->addModelLayer(modellayer.get());
		isload = true;
	}
	m_GLManager->flyTo(42.34425,-71.076262,3450,24.261,42.34425,5);
}

void VRSim3DGlobe_QT::CreateFog()
{
	// TODO: 在此添加命令处理程序代码
	setPrecipitation(VRSim3D::VRPrecipitation::FOG);
}

void VRSim3DGlobe_QT::CreateHaze()
{
	// TODO: 在此添加命令处理程序代码
	setPrecipitation(VRSim3D::VRPrecipitation::HAZE);
}

void VRSim3DGlobe_QT::LoadShip()
{
	// TODO: 在此添加命令处理程序代码
	if(!m_GLManager || !m_Builder)
		return;
	std::string path = "model/chuan_01.ive";  
	osg::ref_ptr<osg::AnimationPath> an = VRSim3D::GlobeBasicFunc::createEarthAnimationPath(
		m_GLManager->getGlobeNode(),
		osg::Vec3(123.24,22.57,-4),osg::Vec3(1,1,1),
		osg::Vec3d(0,0,1),osg::Vec3d(0,-1,0),500,200
		);
	osg::ref_ptr<osg::Node> node = addDynamicModel(path,an.get());

	//m_Builder->getSceneData()->asGroup()->addChild(node.get());
	//m_GLManager->getGlobeNode()->addChild(node.get());
	m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),node.get());

	///zqgao 添加水面运动轨迹
	if (m_Ocean.valid())
	{
		osgEarth::Drivers::Triton::TritonNode* trionNode = 
			dynamic_cast<osgEarth::Drivers::Triton::TritonNode*>(m_Ocean.get());
		if(trionNode)
		{
			osg::Drawable* trionDrawable = trionNode->getChild(0)->asGeode()->getDrawable(0);
			osg::ref_ptr<VRSim3D::ShipWake> ShipWake = new VRSim3D::ShipWake(trionNode->getTritonContext());
			//trionNode->getTritonContext()->EnableGodRays(true);

			node->addUpdateCallback(ShipWake.get());
		}
	}

	m_GLManager->cameraBound(node->asGroup()->getChild(0),5000);
}

void VRSim3DGlobe_QT::LoadPlane()
{
	// TODO: 在此添加命令处理程序代码
	if(!m_GLManager || !m_Builder)
		return;
	std::string path = "model/AH_donghua.ive";
	osg::ref_ptr<osg::AnimationPath> an = VRSim3D::GlobeBasicFunc::createEarthAnimationPath(
		m_GLManager->getGlobeNode(),
		osg::Vec3(123.24,22.57,5),osg::Vec3(1,1,1),osg::Vec3d(0,0,1),osg::Vec3d(-1,0,0),2000,2000
		);

	//osg::ref_ptr<osg::Node> node = addDynamicModel(path,an.get());
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
	if(!node.valid())
		return ;
	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3, 0.3, 0.3, 1.0));
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.5, 0.5, 1.0));
	//node->getOrCreateStateSet()->setAttribute(material.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->addChild(node.get());
	//m_GLManager->AddNode(mt.get(),node.get());
	//m_GLManager->getGlobeNode()->addChild(mt.get());
	m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),mt.get());


	//m_Builder->getSceneData()->asGroup()->addChild(mt.get());
	//m_GLManager->getGlobeNode()->addChild(mt.get());

	///zqgao 获取中心点坐标
	osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), osg::Vec3(123.24,22.57,5),osgEarth::Util::ALTMODE_ABSOLUTE);
	///zqgao 获取上方向
	osg::Vec3d up;
	centerMap.createWorldUpVector(up);
	osg::Matrix m;
	centerMap.createLocalToWorld(m);
	mt->setMatrix(m);
	if (m_Ocean.valid())
	{
		osgEarth::Drivers::Triton::TritonNode* trionNode = 
			dynamic_cast<osgEarth::Drivers::Triton::TritonNode*>(m_Ocean.get());
		if(trionNode)
		{
			osg::Drawable* trionDrawable = trionNode->getChild(0)->asGeode()->getDrawable(0);
			osg::ref_ptr<VRSim3D::RotorWashHandler> rotorWashHandler = new VRSim3D::RotorWashHandler(trionNode->getTritonContext());
			rotorWashHandler->setwindVelocity(6);
			mt->addUpdateCallback(rotorWashHandler.get());
		}
	}

	m_GLManager->cameraBound(mt->asGroup()->getChild(0),2000);
}

void VRSim3DGlobe_QT::CreateClouds()
{
	// TODO: 在此添加命令处理程序代码
	if(!m_GLManager || !m_Builder)
		return;
	if(!m_Sky.valid())
		return;
	static bool isload = false;
	if(isload)
		return;
	std::string  resources = m_GLManager->getDataPath() + "sundog/Resources";
	//resources = "D:/SilverLining SDK/Resources/";
	osg::ref_ptr<VRSim3D::SilverLining::SilverLiningNode> node = 
		new VRSim3D::SilverLining::SilverLiningNode(m_GLManager->getGlobeNode()->getMap(),m_Sky.get(),resources);
	osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_Builder->getViewer());
	m_GLManager->AddNode(m_Sky.get(),node.get());
	//osgEarth::insertGroup(node.get(),m_GLManager->getGlobeNode());
	isload = true;
}

void VRSim3DGlobe_QT::AddSatelliteNode( std::string name,std::string satellitePath, std::string satelliteModel )
{
	if(!m_satelliteGroupManager)
	{
		if(m_Builder == NULL || m_Builder->getSceneData() == NULL)
			return;
		m_satelliteGroupManager = new VRSim3D::SatelliteGroupManager;
		m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),m_satelliteGroupManager->getGroup());
		///m_Builder->getSceneData()->asGroup()->addChild(m_satelliteGroupManager->getGroup());
	}
	bool useOld = true;
	std::vector<int> cutPositions;
	m_satelliteGroupManager->addNode(name,m_GLManager->getDataPath() + satellitePath,satelliteModel,useOld,cutPositions);
}

void VRSim3DGlobe_QT::addControlBar()
{
	if(m_Builder == NULL && m_Builder->getViewer() == NULL)
		return;
	osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_Builder->getViewer());
	m_conBar = new VRSim3D::ControlBar(view);
	m_conBar->addControlInfor("test");
	m_conBar->addSlider(0,100);
	m_conBar->setSliderRange(100,200,150);
	m_conBar->m_DraggerEvent.Bind(this,&VRSim3DGlobe_QT::OnControlBarDragger);
	m_conBar->showControl(false);
}

void VRSim3DGlobe_QT::LoadGroundObject(std::string name,std::string modelPath,double scaleX,double scaleY,double scaleZ,double lon,double lat,double alt)
{
	 //TODO: 在此添加命令处理程序代码
	if(!m_GLManager || !m_Builder)
		return;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(modelPath);
	if(!node.valid())
		return ;
	node->setName(name);
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform;
	mt1->setMatrix(osg::Matrix::scale(osg::Vec3(scaleX,scaleY,scaleZ)));
	mt1->addChild(node.get());
	osg::ref_ptr<osgEarth::GeoTransform> mt = new osgEarth::GeoTransform();
	mt->addChild(mt1.get());

	///zqgao 获取中心点坐标
	osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), osg::Vec3(lon,lat,0),osgEarth::Util::ALTMODE_RELATIVE);
	mt->setAutoRecomputeHeights(true);
	mt->setTerrain(m_GLManager->getGlobeNode()->getTerrain());
	mt->setPosition(centerMap);

	m_GLManager->AddNode(m_GLManager->getModelRoot(),mt.get());
	m_GLManager->flyTo(lat,lon,20000,24.261,-80.34425,5);
}

void VRSim3DGlobe_QT::setPrecipitation( VRSim3D::VRPrecipitation::PPTType type )
{
		if(!m_Builder || !m_GLManager)
		return;
	float maxDensity = 0.5;
	float maxFogDensity = 0.00025;//0.000125
	if(!m_VRPrecipitationManager.valid())
	{
		float startHeight = 10000.0f;
		osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_Builder->getViewer());
		osg::ref_ptr<osgEarth::MapNode> mapnode = m_GLManager->getGlobeNode();
		///zqgao mapnode 接受雾特效
		osgEarth::Util::FogEffect* fogEffect = new osgEarth::Util::FogEffect;
		fogEffect->attach( mapnode->getOrCreateStateSet() ); 
		m_VRPrecipitationManager = new VRSim3D::VRPrecipitationManager(view.get(),mapnode.get());
		m_VRPrecipitationManager->setStartHeight(startHeight);
		
		mapnode->getOrCreateStateSet()->setAttributeAndModes(m_VRPrecipitationManager->getVRPrecipitation()->getFog(),osg::StateAttribute::ON);

		osg::Vec4 fogColor(0.66f, 0.7f, 0.81f, 1.0f);
		m_VRPrecipitationManager->getVRPrecipitation()->getFog()->setColor(fogColor);
		m_GLManager->AddNode(mapnode.get(),m_VRPrecipitationManager.get());
		//m_Builder->getSceneData()->asGroup()->addChild(m_VRPrecipitationManager.get());
	}else if(type == m_VRPrecipitationManager->getVRPrecipitation()->getCurrentPPTType())
	{
		/*if(type == VRSim3D::VRPrecipitation::FOG ||
		type == VRSim3D::VRPrecipitation::HAZE)
		{
		if(m_VRPrecipitationManager->getMaxfogtensity() > 0)
		maxFogDensity = 0.0f;
		}*/
		
		if(m_VRPrecipitationManager->getMaxIntensity() > 0)
			maxDensity = 0.0f;
	}

	if(m_VRPrecipitationManager.valid())
	{
		switch (type)
		{
		case VRSim3D::VRPrecipitation::RAIN:
			m_VRPrecipitationManager->setMaxIntensity(maxDensity);
			break;
		case VRSim3D::VRPrecipitation::SNOW:
			//maxDensity = 0.8;
			if(m_VRPrecipitationManager->getMaxfogtensity() > 0)
				m_VRPrecipitationManager->setMaxfogtensity(m_VRPrecipitationManager->getMaxfogtensity()*2);
			m_VRPrecipitationManager->setMaxIntensity(maxDensity);
			break;
		case VRSim3D::VRPrecipitation::FOG:
			if(type == m_VRPrecipitationManager->getVRPrecipitation()->getCurrentPPTType() && 
				m_VRPrecipitationManager->getMaxfogtensity() > 0)
				maxFogDensity = 0.0f;
			m_VRPrecipitationManager->setMaxfogtensity(maxFogDensity);
			m_VRPrecipitationManager->getVRPrecipitation()->getFog()->setColor(osg::Vec4(0.66f, 0.7f, 0.81f, 1.0f));
			break;
		case VRSim3D::VRPrecipitation::HAZE:
			if(type == m_VRPrecipitationManager->getVRPrecipitation()->getCurrentPPTType() && 
				m_VRPrecipitationManager->getMaxfogtensity() > 0)
				maxFogDensity = 0.0f;
			m_VRPrecipitationManager->setMaxfogtensity(maxFogDensity);
			m_VRPrecipitationManager->getVRPrecipitation()->getFog()->setColor(osg::Vec4(113/255.0, 113/255.0, 75/255.0, 1.0));
			break;
		default:
			break;
		}
		
		m_VRPrecipitationManager->getVRPrecipitation()->setCurrentPPTType(type);
	}
}

osg::Node* VRSim3DGlobe_QT::addDynamicModel( const std::string &path,osg::AnimationPath* animation )
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
	if(!node.valid())
		return NULL;
	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3, 0.3, 0.3, 1.0));
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.5, 0.5, 1.0));
	node->getOrCreateStateSet()->setAttribute(material.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->addChild(node.get());
	//m_GLManager->AddNode(mt.get(),node.get());
	mt->addUpdateCallback(new osg::AnimationPathCallback(animation));

	return mt.release();
}

void VRSim3DGlobe_QT::OnControlBarDragger( float value )
{
	///zqgao 滑块拖动事件，value为滑块当前位置
	///zqgao 下面添加所需实现
	if(!m_NowPickNode.valid())
		return;
	osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(m_NowPickNode.get());
	if(mt == NULL) return;
	std::string name = mt->getName();
	osg::NodeCallback *callback = mt->getUpdateCallback();
	if(callback == NULL) return;
	VRSim3D::AnimationPathPlayBackCallback *pbCallback = 
		dynamic_cast<VRSim3D::AnimationPathPlayBackCallback *>(callback);
	if(pbCallback == NULL) return;
	pbCallback->setPalyBackTime(value);

	//测试获取当前位置信息点
	//VRSim3D::SatellitePosInfo posInfo;
	//GetCurrentSatellitePosInfo(posInfo);
	//SendSignal("leida",0.0,45.0,0.0,100000);
	//SendSignal("leida","leida2");
}

void VRSim3DGlobe_QT::OnPickEvent( std::string name )
{
	osg::Node* node = NULL;
	if(!name.empty())
	{
		VRSim3D::UtileMethod::findChild(m_Builder->getSceneData()->asGroup(),name,&node);
		if(!node)
			return;
		if(m_NowPickNode.valid() && m_NowPickNode != node)
			VRSim3D::UtileMethod::wipeHighLight(m_NowPickNode.get());
		VRSim3D::UtileMethod::highLight(node);
		osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(node);
		if(mt == NULL) return;
		std::string name = mt->getName();
		osg::NodeCallback *callback = mt->getUpdateCallback();
		if(callback == NULL) return;
		VRSim3D::AnimationPathPlayBackCallback *pbCallback = 
			dynamic_cast<VRSim3D::AnimationPathPlayBackCallback *>(callback);
		if(pbCallback == NULL) return;
		if(m_conBar.valid())
		{
			m_conBar->showControl(true);
			m_conBar->setControlName(name);
			m_conBar->setSliderRange(pbCallback->getMinTime(),pbCallback->getMaxTime(),pbCallback->getNowTime());
		}
		m_NowPickNode = node;
		m_conBarCallback->setNowPickNode(m_NowPickNode.get());
		showNodeInfo(node,10,10);
	}
	else if(m_NowPickNode.valid())
	{
		int nodemask = m_modelPicker->getNowMask();
		if(nodemask == NM_GUI_MANAGER)
		{
			///zqao 拣选到滑块
			m_conBarCallback->setUpdate(false);
			m_conBar->setIsDragger(true);
		}else
		{
			VRSim3D::UtileMethod::wipeHighLight(m_NowPickNode.get());
			m_NowPickNode = NULL;
			m_conBarCallback->setNowPickNode(m_NowPickNode.get());
			m_conBar->showControl(false);
			showNodeInfo(node,10,10);
		}
	}
}

void VRSim3DGlobe_QT::OnPickOverEvent( int mask )
{
	if(mask == NM_GUI_MANAGER)
	{
		m_conBar->setIsDragger(false);
		m_conBarCallback->setUpdate(true);
	}
}

void VRSim3DGlobe_QT::showNodeInfo( osg::Node* node,const float& x,const float &y )
{
	///dbs 首先创建用于放置Text的叶子节点
	if(!m_NodeInfoCallback.valid())
	{
		osg::ref_ptr<osg::Camera> hudCamera  = new osg::Camera();
		VRSim3D::UtileMethod::HudCamera(hudCamera,
			m_Builder->getSceneView()->getGraphicsContext()->getTraits()->width, 
			m_Builder->getSceneView()->getGraphicsContext()->getTraits()->height);
		hudCamera->setName("nodehudCamera");
		m_Builder->getSceneData()->asGroup()->addChild(hudCamera.get());
		osg::ref_ptr<osg::Geode> textNode = new osg::Geode();
		hudCamera->addChild(textNode.get());

		///dbs 屏幕上的提示信息
		osg::ref_ptr<osgText::Text> nodeInfoText = new osgText::Text;
		nodeInfoText->setFont(m_GLManager->getDataPath() +"fonts/simhei.ttf");
		nodeInfoText->setColor(osg::Vec4(0, 1, 0, 1));
		nodeInfoText->setCharacterSize(20);
		nodeInfoText->setCharacterSizeMode(osgText::Text::CharacterSizeMode::SCREEN_COORDS);
		nodeInfoText->setAlignment(osgText::TextBase::LEFT_TOP);

		nodeInfoText->setPosition(osg::Vec3(x, m_Builder->getSceneView()->getGraphicsContext()->getTraits()->height-y, 0));
		nodeInfoText->setDataVariance(osg::Object::DYNAMIC);
		nodeInfoText->setText("Hello World");

		///dbs 设置文字信息的更新回调函数
		m_NodeInfoCallback = new VRSim3D::NodeInfoDrawCallback(m_GLManager->getGlobeNode(),node);
		nodeInfoText->setDrawCallback(m_NodeInfoCallback.get());

		///dbs 设置信息提示节点的属性，并关闭光照和深度测试
		osg::StateSet* stateset = nodeInfoText->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

		///dbs 将文本添加到绘制列表中
		textNode->addDrawable(nodeInfoText.get());

	}else
	{
		m_NodeInfoCallback->setIfoNode(node);
	}
}

void VRSim3DGlobe_QT::OnTimeSet( UINT id )
{
	// TODO: 在此添加命令处理程序代码
	m_TimeSim = id;
	if(m_AnimateSky)
	{
		switch (id)
		{
		case 0://ID_BUTTON_M
			m_AnimateSky->setRate(1440);
			break;
		case 1://ID_BUTTON_H
			m_AnimateSky->setRate(60);
			break;
		case 2://ID_BUTTON_D
			m_AnimateSky->setRate(1);
			break;
		default:
			break;
		}
	}
}

void VRSim3DGlobe_QT::CreatePowerRange(double interRadius,double radius,double azMin,double azMax,
	double elevMin,double elevMax,int density,int elevDensity,
	int azDensity,double lon,double lat,double alt)
{
	osg::ref_ptr<VRSim3D::CSphereSegment> ssg = new VRSim3D::CSphereSegment(
		osg::Vec3(0,0,0),interRadius,radius,azMin,azMax, elevMin,elevMax,density,elevDensity,azDensity);

	osg::StateSet *stateset = ssg->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	//设置所有颜色
	ssg->setAllColors(osg::Vec4(1.0f,1.0f,0.0f,0.5f)); 
	//设置表面面颜色
	ssg->setSurfaceLineColor(osg::Vec4(1.0f,1.0f,1.0f,0.9f));
	//设置表面面颜色
	ssg->setSideColor(osg::Vec4(0.0f,1.0f,1.0f,0.5f));
	//设置边界线颜色
	ssg->setEdgeLineColor(osg::Vec4(17/255.0,255/255.0,255/255.0,0.9));
	//设置切面面颜色
	ssg->setSpokeColor(osg::Vec4(17/255.0,255/255.0,255/255.0,0.9));
	//设置切面边线宽度
	ssg->setSpokeLineWidth(3);
	//设置表面网格线宽度
	ssg->setSurfaceLineWidth(3);

	osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), 
		osg::Vec3(lon,lat,4),osgEarth::Util::ALTMODE_ABSOLUTE);

	osg::Vec3d centerWorld;
	centerMap.toWorld(centerWorld);
	osg::Vec3d up;
	centerMap.createWorldUpVector(up);
	osg::Quat t;
	t.makeRotate(osg::Vec3d(0,0,1),up);
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->setMatrix(osg::Matrix::rotate(t)*osg::Matrix::translate(centerWorld));
	mt->addChild(ssg.get());
	m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),mt.get());
	m_GLManager->flyTo(lat,lon,20000,24.261,-80.34425,5);
}

void VRSim3DGlobe_QT::GetCurrentSatellitePosInfo( VRSim3D::SatellitePosInfo &currentPosInfo )
{
	if(!m_NowPickNode.valid())
		return;
	osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(m_NowPickNode.get());
	if(mt == NULL) return;
	std::string name = mt->getName();
	osg::NodeCallback *callback = mt->getUpdateCallback();
	if(callback == NULL) return;
	VRSim3D::AnimationPathPlayBackCallback *pbCallback = 
		dynamic_cast<VRSim3D::AnimationPathPlayBackCallback *>(callback);
	if(pbCallback == NULL) return;
	std::map<std::string,VRSim3D::SatelliteAnimation*> satelliteAnimations = 
		m_satelliteGroupManager->GetSatelliteAnimations();
	std::map<std::string,VRSim3D::SatelliteAnimation*>::iterator it = satelliteAnimations.begin();
	it = satelliteAnimations.find(name);
	if(it == satelliteAnimations.end())
		return;
	int nowTime = (int)(pbCallback->getNowTime() + 0.5);
	std::map<int,VRSim3D::SatellitePosInfo> satellitePosInfo = it->second->GetSatellitePosInfo();
	std::map<int,VRSim3D::SatellitePosInfo>::iterator it2 = satellitePosInfo.begin();
	it2 = satellitePosInfo.find(nowTime);
	if(it2 == satellitePosInfo.end())
		return;
	currentPosInfo = it2->second;
}

void VRSim3DGlobe_QT::SendSignal( std::string name,float xAngle,float yAngle,float zAngle,
	float height,float speed,float length,osg::Vec4 color )
{
	osg::Node *modelNode = NULL;
	VRSim3D::UtileMethod::findChild(m_GLManager->getModelRoot(),name,&modelNode);
	if(modelNode == NULL)
		return;
	osg::MatrixTransform *mt1 = dynamic_cast<osg::MatrixTransform *>(modelNode->getParent(0));
	if(mt1 == NULL)
		return;
	osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform *>(mt1->getParent(0));
	if(mt == NULL)
		return;
	osg::Vec3f angle = osg::Vec3(xAngle,yAngle,zAngle);
	osg::Quat quat;
	quat.makeRotate(osg::inDegrees(angle.x()),osg::X_AXIS,osg::inDegrees(angle.y()),osg::Y_AXIS,osg::inDegrees(angle.z()),osg::Z_AXIS);
	osg::Vec3f hight = osg::Vec3f(0,0,height);
	osg::Vec3f hight1 =  quat*hight;
	osg::ref_ptr<VRSim3D::CCommEffect> commEffect = new VRSim3D::CCommEffect(mt1->getBound().center(),200,hight1,3000);
	commEffect->setEarthModel(m_GLManager->getGlobeNode());
	commEffect->setSpeed(speed);
	commEffect->setLength(length);
	commEffect->setColor(color);
	mt->addChild(commEffect.get());
}

void VRSim3DGlobe_QT::SendSignal( std::string name,std::string name2,float speed,float length,osg::Vec4 color )
{
	osg::Node *modelNode = NULL;
	osg::Node *modelNode2 = NULL;
	VRSim3D::UtileMethod::findChild(m_GLManager->getModelRoot(),name,&modelNode);
	VRSim3D::UtileMethod::findChild(m_GLManager->getModelRoot(),name2,&modelNode2);
	if(modelNode == NULL || modelNode2 == NULL)
		return;
	osg::MatrixTransform *mt1 = dynamic_cast<osg::MatrixTransform *>(modelNode->getParent(0));
	osg::MatrixTransform *mt2 = dynamic_cast<osg::MatrixTransform *>(modelNode2->getParent(0));
	if(mt1 == NULL || mt2 == NULL)
		return;
	osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform *>(mt1->getParent(0));
	osg::MatrixTransform *mt3 = dynamic_cast<osg::MatrixTransform *>(mt2->getParent(0));
	if(mt == NULL || mt3 == NULL)
		return;
	osg::ref_ptr<VRSim3D::CCommEffect> commEffect = new VRSim3D::CCommEffect(mt->getBound().center(),200,mt3->getBound().center(),3000);
	commEffect->setEarthModel(m_GLManager->getGlobeNode());
	commEffect->setSpeed(speed);
	commEffect->setLength(length);
	commEffect->setColor(color);
	m_GLManager->AddNode(m_GLManager->getModelRoot(),commEffect);
}
