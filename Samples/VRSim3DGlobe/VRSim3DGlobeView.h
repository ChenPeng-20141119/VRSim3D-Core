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

// VRSim3DGlobeView.h : CVRSim3DGlobeView ��Ľӿ�
//

#pragma once

#include "SceneViewBuilder.h"
#include "GlobeSceneViewManager.h"

#include "GlobeBasicFunc.h"
#include "VRPrecipitationManager.h"

#include "osgEarthUtil/Sky"
#include "osgEarthUtil/ocean"

//chenpeng
#include "SatelliteGroupManager.h"
#include "UtileMethod.h"
#include "ControlBar.h"
#include "ModelPicker.h"
#include "NodeInfoDrawCallback.h"
class ConBarCallback;
using namespace VRSim3D;

class CVRSim3DGlobeView : public CView
{
protected: // �������л�����
	CVRSim3DGlobeView();
	DECLARE_DYNCREATE(CVRSim3DGlobeView)

// ����
public:
	CVRSim3DGlobeDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	///zqgao ����������ָ��
	VRSim3D::SceneViewBuilder*	     m_Builder;
	///zqgao ����������ָ��
	VRSim3D::GlobeSceneViewManager* m_GLManager;
	///ʱ��ģ��
	UINT  m_TimeSim;
	///zqgao �Ƿ�����ǿ�
	bool m_isSky;
	osg::ref_ptr<osgEarth::Util::SkyNode> m_Sky;
	///zqgao ����
	osg::ref_ptr<osgEarth::Util::OceanNode> m_Ocean;
	osg::ref_ptr<VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback> m_AnimateSky;
	///zqgao ������Чģ��
	osg::ref_ptr<VRSim3D::VRPrecipitationManager>	m_VRPrecipitationManager;
// ʵ��
public:
	virtual ~CVRSim3DGlobeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	///zqgao ����ģ��
	void setPrecipitation(VRSim3D::VRPrecipitation::PPTType type);
	///zqgao �����˶�ģ��
	osg::Node* addDynamicModel(const std::string &path,osg::AnimationPath* animation);

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimeSet(UINT id);
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	///zqgao ���ģ�����
	afx_msg void OnSKy();
	///zqgao ����ģ�����
	afx_msg void OnOcean();
	afx_msg void OnUpdateSKy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTimeSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOcean(CCmdUI *pCmdUI);
	///zqgao ��ģ�����
	afx_msg void OnRain();
	///zqgao ѩģ�����
	afx_msg void OnSnow();
	///zqgao ���ز�ʿ��ģ��
	afx_msg void OnLoadBSTBuilding();
	///zqgao ��ģ�����
	afx_msg void OnFog();
	///zqgao ��ģ�����
	afx_msg void OnHaze();
	///zqgao ����ʻģ�⣨��Ҫ�ȼ��غ���
	afx_msg void OnLoadShip();
	///zqgao ����Ϫ��ģ�⣨��Ҫ�ȼ��غ���
	afx_msg void OnLoadPlane();
	///zqgao ��ģ��
	afx_msg void OnClouds();

	//chenpeng
public:
	void addSatelliteNode(std::string name,std::string satellitePath,
		std::string satelliteName,bool useOld,std::vector<int> cutPositions);
	void addControlBar();
private:
	void OnControlBarDragger(float value);
	void OnPickEvent(std::string name);
	void OnPickOverEvent(int mask);

	///zqgao ��ʾ�ڵ���Ϣ
	void showNodeInfo(osg::Node* node,const float& x,const float &y);
private:
	SatelliteGroupManager *m_satelliteGroupManager;
	///zqgao �ڵ���Ϣ�ڵ�callback
	osg::ref_ptr<NodeInfoDrawCallback>		m_NodeInfoCallback;
	///zqgao ��ǰѡ�нڵ�
	osg::ref_ptr<osg::Node>							m_NowPickNode;
	osg::ref_ptr<ControlBar>                          m_conBar;
	osg::ref_ptr<ModelPicker>						m_modelPicker;
	VRSim3D::ConBarCallback *                                     m_conBarCallback;
public:
	afx_msg void OnLoadSatellite();
	afx_msg void OnButtonLoadGroundStation();
};

#ifndef _DEBUG  // VRSim3DGlobeView.cpp �еĵ��԰汾
inline CVRSim3DGlobeDoc* CVRSim3DGlobeView::GetDocument() const
   { return reinterpret_cast<CVRSim3DGlobeDoc*>(m_pDocument); }
#endif

