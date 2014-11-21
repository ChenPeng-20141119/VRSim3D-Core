//////////////////////////////////////////////////////////////////////////
///文件名：多视图场景
///创建者：高志清
///创建时间：2013年7月26日
///内容描述：多视图场景
//////////////////////////////////////////////////////////////////////////

#ifndef COMPOSITE_SCENE_VIEW_H
#define COMPOSITE_SCENE_VIEW_H 1

#include "Export.h"
#include "sceneview.h"

#include "osgViewer/CompositeViewer"

namespace VRSim3D
{

class VRSim3D CompositeSceneView :
	public VRSim3D::SceneView
{
public:
	enum Style
	{
		ST_AUTOMATIC,
		ST_VERTICAL,
		ST_HORIZONTAL

	};
	CompositeSceneView(void);
	~CompositeSceneView(void);
	///zqgao 创建场景，返回true创建成功
	///\param samples(unsigned int)
	// 抗锯齿倍数，请根据使用机显卡设置
	virtual	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);
private:
	// zqgao 渲染线程
	static void RenderThread(void* ptr);
	// zqgao 初始化 
	void myinit(void);
	// zqgao 设置场景
	void setupView(const int &index);
	// zqgao 初始化主相机
	void initCamera(void);
	///zqgao 设置相机操控器
	void setCmaraMP(osgViewer::View* view,osgGA::CameraManipulator* cm,std::string name);
	///zqgao 视口布局
	void setCompostViewers(Style style = ST_AUTOMATIC);
	///zqgao 分割
	void partition(Style style);
	///zqgao 设置相机属性（viewPort等）
	void setCameraPPT(int numview,int allview,Style style);
	//zqgao 自动布局设置
	void setViewPos(const int& numview,const int& allview,float& x, float& y, float& width, float& hight);
private:
	///zqgao 场景管理器
	osg::ref_ptr<osgViewer::CompositeViewer>		m_Viewer;
	///zqgao 相机操控器
	std::vector<osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>> m_KeySwitchMatrixManipulatorMap;
	///zqgao 相机操控器数量
	int													   m_MpNum;
};
}
#endif
