// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// VRSim3DGlobeView.h : CVRSim3DGlobeView 类的接口
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
protected: // 仅从序列化创建
	CVRSim3DGlobeView();
	DECLARE_DYNCREATE(CVRSim3DGlobeView)

// 特性
public:
	CVRSim3DGlobeDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	///zqgao 场景创建者指针
	VRSim3D::SceneViewBuilder*	     m_Builder;
	///zqgao 场景管理者指针
	VRSim3D::GlobeSceneViewManager* m_GLManager;
	///时间模拟
	UINT  m_TimeSim;
	///zqgao 是否加载星空
	bool m_isSky;
	osg::ref_ptr<osgEarth::Util::SkyNode> m_Sky;
	///zqgao 海洋
	osg::ref_ptr<osgEarth::Util::OceanNode> m_Ocean;
	osg::ref_ptr<VRSim3D::GlobeBasicFunc::AnimateSkyUpdateCallback> m_AnimateSky;
	///zqgao 天气特效模拟
	osg::ref_ptr<VRSim3D::VRPrecipitationManager>	m_VRPrecipitationManager;
// 实现
public:
	virtual ~CVRSim3DGlobeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	///zqgao 天气模拟
	void setPrecipitation(VRSim3D::VRPrecipitation::PPTType type);
	///zqgao 加载运动模型
	osg::Node* addDynamicModel(const std::string &path,osg::AnimationPath* animation);

protected:

// 生成的消息映射函数
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
	///zqgao 天空模拟控制
	afx_msg void OnSKy();
	///zqgao 海洋模拟控制
	afx_msg void OnOcean();
	afx_msg void OnUpdateSKy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTimeSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOcean(CCmdUI *pCmdUI);
	///zqgao 雨模拟控制
	afx_msg void OnRain();
	///zqgao 雪模拟控制
	afx_msg void OnSnow();
	///zqgao 加载波士顿模型
	afx_msg void OnLoadBSTBuilding();
	///zqgao 雾模拟控制
	afx_msg void OnFog();
	///zqgao 霾模拟控制
	afx_msg void OnHaze();
	///zqgao 船行驶模拟（需要先加载海洋）
	afx_msg void OnLoadShip();
	///zqgao 旋翼溪流模拟（需要先加载海洋）
	afx_msg void OnLoadPlane();
	///zqgao 云模拟
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

	///zqgao 显示节点信息
	void showNodeInfo(osg::Node* node,const float& x,const float &y);
private:
	SatelliteGroupManager *m_satelliteGroupManager;
	///zqgao 节点信息节点callback
	osg::ref_ptr<NodeInfoDrawCallback>		m_NodeInfoCallback;
	///zqgao 当前选中节点
	osg::ref_ptr<osg::Node>							m_NowPickNode;
	osg::ref_ptr<ControlBar>                          m_conBar;
	osg::ref_ptr<ModelPicker>						m_modelPicker;
	VRSim3D::ConBarCallback *                                     m_conBarCallback;
public:
	afx_msg void OnLoadSatellite();
	afx_msg void OnButtonLoadGroundStation();
};

#ifndef _DEBUG  // VRSim3DGlobeView.cpp 中的调试版本
inline CVRSim3DGlobeDoc* CVRSim3DGlobeView::GetDocument() const
   { return reinterpret_cast<CVRSim3DGlobeDoc*>(m_pDocument); }
#endif

