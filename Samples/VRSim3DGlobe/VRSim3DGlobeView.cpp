// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// VRSim3DGlobeView.cpp : CVRSim3DGlobeView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "VRSim3DGlobe.h"
#endif

#include "SingleSceneViewBuilder.h"
#include "ShipWake.h"
#include "RotorWashHandler.h"
#include "SilverLiningNode.h"
#include "ModelPicker.h"

#include "VRSim3DGlobeDoc.h"
#include "VRSim3DGlobeView.h"

#include "osgViewer/Viewer"
#include "osgEarthUtil/Fog"
#include "osgEarthDrivers/ocean_triton/TritonNode"
#include "osgEarth/GeoTransform"

#include "CommEffect.h"
#include "SphereSegment.h"
#include "RectEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVRSim3DGlobeView

IMPLEMENT_DYNCREATE(CVRSim3DGlobeView, CView)

BEGIN_MESSAGE_MAP(CVRSim3DGlobeView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND_RANGE(ID_BUTTON26,ID_BUTTON_D,OnTimeSet)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BUTTON26,ID_BUTTON_D, &CVRSim3DGlobeView::OnUpdateTimeSet)
	ON_COMMAND(ID_FILE_OPEN,OnFileOpen)
	ON_COMMAND(ID_CHECK2, &CVRSim3DGlobeView::OnSKy)
	ON_COMMAND(ID_CHECK3, &CVRSim3DGlobeView::OnOcean)
	ON_UPDATE_COMMAND_UI(ID_CHECK2, &CVRSim3DGlobeView::OnUpdateSKy)
	
	ON_UPDATE_COMMAND_UI(ID_CHECK3, &CVRSim3DGlobeView::OnUpdateOcean)
	ON_COMMAND(ID_BUTTON15, &CVRSim3DGlobeView::OnRain)
	ON_COMMAND(ID_BUTTON16, &CVRSim3DGlobeView::OnSnow)
	ON_COMMAND(ID_BUTTON2, &CVRSim3DGlobeView::OnLoadBSTBuilding)
	ON_COMMAND(ID_BUTTON13, &CVRSim3DGlobeView::OnFog)
	ON_COMMAND(ID_BUTTON14, &CVRSim3DGlobeView::OnHaze)
	ON_COMMAND(ID_BUTTON30, &CVRSim3DGlobeView::OnLoadShip)
	ON_COMMAND(ID_BUTTON31, &CVRSim3DGlobeView::OnLoadPlane)
	ON_COMMAND(ID_BUTTON12, &CVRSim3DGlobeView::OnClouds)
	ON_COMMAND(ID_BUTTON34, &CVRSim3DGlobeView::OnLoadSatellite)
	ON_COMMAND(ID_BUTTON35, &CVRSim3DGlobeView::OnButtonLoadGroundStation)
END_MESSAGE_MAP()

// CVRSim3DGlobeView ����/����
//chenpeng
#include "AnimationPathPlayBackCallback.h"
#include "ControlBar.h"

CVRSim3DGlobeView::CVRSim3DGlobeView()
{
	// TODO: �ڴ˴���ӹ������
	m_GLManager = NULL;
	m_Builder = NULL;
	m_TimeSim = ID_BUTTON_M;
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
}

CVRSim3DGlobeView::~CVRSim3DGlobeView()
{
}

BOOL CVRSim3DGlobeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CVRSim3DGlobeView ����

void CVRSim3DGlobeView::OnDraw(CDC* /*pDC*/)
{
	CVRSim3DGlobeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CVRSim3DGlobeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVRSim3DGlobeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVRSim3DGlobeView ���

#ifdef _DEBUG
void CVRSim3DGlobeView::AssertValid() const
{
	CView::AssertValid();
}

void CVRSim3DGlobeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVRSim3DGlobeDoc* CVRSim3DGlobeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVRSim3DGlobeDoc)));
	return (CVRSim3DGlobeDoc*)m_pDocument;
}
#endif //_DEBUG


// CVRSim3DGlobeView ��Ϣ�������


void CVRSim3DGlobeView::OnTimeSet(UINT id)
{
	// TODO: �ڴ���������������
	m_TimeSim = id;
	if(m_AnimateSky)
	{
		switch (id)
		{
		case ID_BUTTON_M:
			m_AnimateSky->setRate(1440);
			break;
		case ID_BUTTON_H:
			m_AnimateSky->setRate(60);
			break;
		case ID_BUTTON_D:
			m_AnimateSky->setRate(1);
			break;
		default:
			break;
		}
	}
}


BOOL CVRSim3DGlobeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CVRSim3DGlobeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	if(!m_Builder)
	{
		RECT rect;
		::GetWindowRect(m_hWnd,&rect);
		float width = rect.right - rect.left;
		float height =  rect.bottom - rect.top;
		m_Builder = new VRSim3D::SingleSceneViewBuilder();
		m_Builder->createSceneView(m_hWnd,width,height,16);

		m_modelPicker = new VRSim3D::ModelPicker();
		m_modelPicker->m_PickEvent.Bind(this,&CVRSim3DGlobeView::OnPickEvent);
		m_modelPicker->m_PickOverEvent.Bind(this,&CVRSim3DGlobeView::OnPickOverEvent);
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

		/*if(!m_GLManager)
		{
			m_GLManager = new VRSim3D::GlobeSceneViewManager(m_Builder);
		}
		std::string path = "G:/zqgao/google/s/boston_buildings.earth";
		m_GLManager->Load(path);*/
	}
	
}

void CVRSim3DGlobeView::OnFileOpen()
{
	CString filename;
	CFileDialog opendlg(true,_T("*.earth"),NULL,OFN_OVERWRITEPROMPT,_T("earth�ļ�(*.earth)|*.earth|ģ���ļ�(*.ive)|*.ive|�����ļ�(*.*)|*.*||"),NULL);
	if(opendlg.DoModal() == IDOK)
	{
		filename = opendlg.GetPathName();

		if(!m_GLManager)
		{
			m_GLManager = new VRSim3D::GlobeSceneViewManager(m_Builder);
		}
		std::string path;
		path.append(filename);
		m_GLManager->Load(path);
	}
}

///zqgao �ǿ�����
void CVRSim3DGlobeView::OnSKy()
{
	// TODO: �ڴ���������������
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
			OnTimeSet(m_TimeSim);
		}
	}
}

///zqgao ��������
void CVRSim3DGlobeView::OnOcean()
{
	// TODO: �ڴ���������������
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

///zqgao �ǿ�ѡ�б�ʾ����
void CVRSim3DGlobeView::OnUpdateSKy(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	//CButton *check = (CButton*)GetDlgItem(ID_CHECK2);
	//int a = check->GetCheck();
	pCmdUI->SetCheck(m_isSky);
	if(m_Sky.valid()) 
	{
		if(!m_isSky)
		{
			///zqgao ��skynode�½ڵ�ת�Ƶ�mapnode�£�ԭ������mapnode�£���ɾ��skynode
			VRSim3D::UtileMethod::transferChilds(m_Sky.get(),m_Sky->getParent(0));
			m_GLManager->RemoveNode(m_Sky->getParent(0),m_Sky.get());
			m_Sky = NULL;
			m_AnimateSky = NULL;
		}
	}

}

///zqgao ����ѡ�е�ǰʱ��ģ��button
void CVRSim3DGlobeView::OnUpdateTimeSet(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_isSky);
	pCmdUI->SetRadio(m_TimeSim == pCmdUI->m_nID);
}

///zqgao ����ѡ�б�ʾ����
void CVRSim3DGlobeView::OnUpdateOcean(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Ocean.valid())
	{
		if(m_Ocean->getNodeMask())
			pCmdUI->SetCheck(true);
		else
			pCmdUI->SetCheck(false);
	}else
	{
		pCmdUI->SetCheck(false);
	}
}


void CVRSim3DGlobeView::OnRain()
{
	// TODO: �ڴ���������������
	setPrecipitation(VRSim3D::VRPrecipitation::RAIN);
}

void CVRSim3DGlobeView::setPrecipitation( VRSim3D::VRPrecipitation::PPTType type )
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
		///zqgao mapnode ��������Ч
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


void CVRSim3DGlobeView::OnSnow()
{
	// TODO: �ڴ���������������
	setPrecipitation(VRSim3D::VRPrecipitation::SNOW);
}

///zqgao ���ز�ʿ�ٳ���ģ��
void CVRSim3DGlobeView::OnLoadBSTBuilding()
{
	// TODO: �ڴ���������������
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


void CVRSim3DGlobeView::OnFog()
{
	// TODO: �ڴ���������������
	setPrecipitation(VRSim3D::VRPrecipitation::FOG);
}


void CVRSim3DGlobeView::OnHaze()
{
	// TODO: �ڴ���������������
	setPrecipitation(VRSim3D::VRPrecipitation::HAZE);
}

osg::Node* CVRSim3DGlobeView::addDynamicModel( const std::string &path,osg::AnimationPath* animation )
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


void CVRSim3DGlobeView::OnLoadShip()
{
	// TODO: �ڴ���������������
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

	///zqgao ���ˮ���˶��켣
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


void CVRSim3DGlobeView::OnLoadPlane()
{
	// TODO: �ڴ���������������
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
	
	///zqgao ��ȡ���ĵ�����
	osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), osg::Vec3(123.24,22.57,5),osgEarth::Util::ALTMODE_ABSOLUTE);
	///zqgao ��ȡ�Ϸ���
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


void CVRSim3DGlobeView::OnClouds()
{
	// TODO: �ڴ���������������
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

void CVRSim3DGlobeView::addSatelliteNode( std::string name,std::string satellitePath, std::string satelliteName,bool useOld,std::vector<int> cutPositions )
{
	if(!m_satelliteGroupManager)
	{
		if(m_Builder == NULL || m_Builder->getSceneData() == NULL)
			return;
		m_satelliteGroupManager = new SatelliteGroupManager;
		m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),m_satelliteGroupManager->getGroup());
		///m_Builder->getSceneData()->asGroup()->addChild(m_satelliteGroupManager->getGroup());
	}
	m_satelliteGroupManager->addNode(name,m_GLManager->getDataPath() + satellitePath,satelliteName,useOld,cutPositions);
}

void CVRSim3DGlobeView::addControlBar()
{
	if(m_Builder == NULL && m_Builder->getViewer() == NULL)
		return;
	osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_Builder->getViewer());
	m_conBar = new ControlBar(view);
	m_conBar->addControlInfor("test");
	m_conBar->addSlider(0,100);
	m_conBar->setSliderRange(100,200,150);
	m_conBar->m_DraggerEvent.Bind(this,&CVRSim3DGlobeView::OnControlBarDragger);
	m_conBar->showControl(false);
}

void CVRSim3DGlobeView::OnControlBarDragger( float value )
{
	///zqgao �����϶��¼���valueΪ���鵱ǰλ��
	///zqgao �����������ʵ��
	if(!m_NowPickNode.valid())
		return;
	osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(m_NowPickNode.get());
	if(mt == NULL) return;
	std::string name = mt->getName();
	osg::NodeCallback *callback = mt->getUpdateCallback();
	if(callback == NULL) return;
	AnimationPathPlayBackCallback *pbCallback = 
		dynamic_cast<AnimationPathPlayBackCallback *>(callback);
	if(pbCallback == NULL) return;
	pbCallback->setPalyBackTime(value);
}

void CVRSim3DGlobeView::OnPickEvent( std::string name )
{
	osg::Node* node = NULL;
	if(!name.empty())
	{
		UtileMethod::findChild(m_Builder->getSceneData()->asGroup(),name,&node);
		if(!node)
			return;
		if(m_NowPickNode.valid() && m_NowPickNode != node)
			UtileMethod::wipeHighLight(m_NowPickNode.get());
		UtileMethod::highLight(node);
		osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(node);
		if(mt == NULL) return;
		std::string name = mt->getName();
		osg::NodeCallback *callback = mt->getUpdateCallback();
		if(callback == NULL) return;
		AnimationPathPlayBackCallback *pbCallback = 
			dynamic_cast<AnimationPathPlayBackCallback *>(callback);
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
			///zqao ��ѡ������
			m_conBarCallback->setUpdate(false);
			m_conBar->setIsDragger(true);
		}else
		{
			UtileMethod::wipeHighLight(m_NowPickNode.get());
			m_NowPickNode = NULL;
			m_conBarCallback->setNowPickNode(m_NowPickNode.get());
			m_conBar->showControl(false);
			showNodeInfo(node,10,10);
		}
	}
}

void CVRSim3DGlobeView::OnPickOverEvent( int mask )
{
	if(mask == NM_GUI_MANAGER)
	{
		m_conBar->setIsDragger(false);
		m_conBarCallback->setUpdate(true);
	}
}

void CVRSim3DGlobeView::showNodeInfo( osg::Node* node,const float& x,const float &y )
{

	///dbs ���ȴ������ڷ���Text��Ҷ�ӽڵ�
	if(!m_NodeInfoCallback.valid())
	{
		osg::ref_ptr<osg::Camera> hudCamera  = new osg::Camera();
		UtileMethod::HudCamera(hudCamera,
			m_Builder->getSceneView()->getGraphicsContext()->getTraits()->width, 
			m_Builder->getSceneView()->getGraphicsContext()->getTraits()->height);
		hudCamera->setName("nodehudCamera");
		m_Builder->getSceneData()->asGroup()->addChild(hudCamera.get());
		osg::ref_ptr<osg::Geode> textNode = new osg::Geode();
		hudCamera->addChild(textNode.get());

		///dbs ��Ļ�ϵ���ʾ��Ϣ
		osg::ref_ptr<osgText::Text> nodeInfoText = new osgText::Text;
		nodeInfoText->setFont(m_GLManager->getDataPath() +"fonts/simhei.ttf");
		nodeInfoText->setColor(osg::Vec4(0, 1, 0, 1));
		nodeInfoText->setCharacterSize(20);
		nodeInfoText->setCharacterSizeMode(osgText::Text::CharacterSizeMode::SCREEN_COORDS);
		nodeInfoText->setAlignment(osgText::TextBase::LEFT_TOP);

		nodeInfoText->setPosition(osg::Vec3(x, m_Builder->getSceneView()->getGraphicsContext()->getTraits()->height-y, 0));
		nodeInfoText->setDataVariance(osg::Object::DYNAMIC);
		nodeInfoText->setText("Hello World");

		///dbs ����������Ϣ�ĸ��»ص�����
		m_NodeInfoCallback = new NodeInfoDrawCallback(m_GLManager->getGlobeNode(),node);
		nodeInfoText->setDrawCallback(m_NodeInfoCallback.get());

		///dbs ������Ϣ��ʾ�ڵ�����ԣ����رչ��պ���Ȳ���
		osg::StateSet* stateset = nodeInfoText->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

		///dbs ���ı���ӵ������б���
		textNode->addDrawable(nodeInfoText.get());

	}else
	{
		m_NodeInfoCallback->setIfoNode(node);
	}
}


void CVRSim3DGlobeView::OnLoadSatellite()
{
	// TODO: �ڴ���������������
	std::vector<int> cutPositionsSatellite1;
	addSatelliteNode("Satellite 1    ",
		"FinalOutput_WX6000km.txt",
		"model/artemis.ive",true,cutPositionsSatellite1);
}


void CVRSim3DGlobeView::OnButtonLoadGroundStation()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	if(!m_GLManager || !m_Builder)
		return;
	//std::string path = "model/leida.ive";
	//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
	//if(!node.valid())
	//	return ;
	//osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform;
	//mt1->setMatrix(osg::Matrix::scale(osg::Vec3(500,500,500)));
	//mt1->addChild(node.get());
	//osg::ref_ptr<osgEarth::GeoTransform> mt = new osgEarth::GeoTransform();
	//mt->addChild(mt1.get());

	/////zqgao ��ȡ���ĵ�����
	//osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), osg::Vec3(118.00,25.00,0),osgEarth::Util::ALTMODE_RELATIVE);
	//mt->setAutoRecomputeHeights(true);
	//mt->setTerrain(m_GLManager->getGlobeNode()->getTerrain());
	//mt->setPosition(centerMap);
	//
	//osg::Vec3d vec1;
	//m_GLManager->getGlobeNode()->getMapSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::inDegrees(25.0),osg::inDegrees(118.5), 100000, vec1.x(), vec1.y(), vec1.z());
	//osg::ref_ptr<CCommEffect> commEffect = new CCommEffect(mt->getBound().center(),200,vec1,3000);
	//commEffect->setEarthModel(m_GLManager->getGlobeNode());
	//commEffect->setSpeed(5);
	//commEffect->setLength(4000);
	//commEffect->setColor(osg::Vec4(1, 0, 1, 0.4));

	//osg::ref_ptr<CCommEffect> commEffect1 = new CCommEffect(vec1,3000,mt->getBound().center(),200);
	//commEffect1->setEarthModel(m_GLManager->getGlobeNode());
	//commEffect1->setSpeed(5);
	//commEffect1->setLength(4000);
	//commEffect1->setColor(osg::Vec4(1, 1, 0, 0.4));
	//m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),commEffect.get());
	//m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),commEffect1.get());

	//m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),mt.get());
	//m_GLManager->flyTo(25.00,118.00,20000,24.261,-80.34425,5);


	/////////////////////////////////////////////////////////_part2
	////���������Χ
	//osg::ref_ptr<VRSim3D::CSphereSegment> ssg = new VRSim3D::CSphereSegment(osg::Vec3(0,0,0),50000,100000,0.0f,osg::PI * 2.0, 0,osg::PI/5.0,40,10,30);

	//osg::StateSet *stateset = ssg->getOrCreateStateSet();
	//stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	////����������ɫ
	//ssg->setAllColors(osg::Vec4(1.0f,1.0f,0.0f,0.5f)); 
	////���ñ�������ɫ
	//ssg->setSurfaceLineColor(osg::Vec4(1.0f,1.0f,1.0f,0.9f));
	////���ñ�������ɫ
	//ssg->setSideColor(osg::Vec4(0.0f,1.0f,1.0f,0.5f));
	////���ñ߽�����ɫ
	//ssg->setEdgeLineColor(osg::Vec4(17/255.0,255/255.0,255/255.0,0.9));
	////������������ɫ
	//ssg->setSpokeColor(osg::Vec4(17/255.0,255/255.0,255/255.0,0.9));
	////����������߿��
	//ssg->setSpokeLineWidth(3);
	////���ñ��������߿��
	//ssg->setSurfaceLineWidth(3);

	//osgEarth::GeoPoint centerMap(m_GLManager->getGlobeNode()->getMapSRS(), 
	//	osg::Vec3(122.14,23.08,4),osgEarth::Util::ALTMODE_ABSOLUTE);

	//osg::Vec3d centerWorld;
	//centerMap.toWorld(centerWorld);
	//osg::Vec3d up;
	//centerMap.createWorldUpVector(up);
	//osg::Quat t;
	//t.makeRotate(osg::Vec3d(0,0,1),up);
	//osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	//mt->setMatrix(osg::Matrix::rotate(t)*osg::Matrix::translate(centerWorld));
	//mt->addChild(ssg.get());
	//m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),mt.get());
	m_GLManager->flyTo(23.08,122.14,20000,24.261,-80.34425,5);

	/////////////////////////////////////////////////////////////_part3
	osg::ref_ptr<VRSim3D::RectEffect>rect=new VRSim3D::RectEffect(m_GLManager->getGlobeNode(),
		osg::Vec3(122.14,23.08,50000),osg::Vec3(122.14,23.08,0),30000,osg::Vec4(0.1,1.0, 0.5, 0.8));
	///zqgao ���ú��ݱ���
	rect->setAspectRatio(0.4);
	m_GLManager->AddNode(m_Builder->getSceneData()->asGroup(),rect.get());
}
