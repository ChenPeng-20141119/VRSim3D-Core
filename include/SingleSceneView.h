//////////////////////////////////////////////////////////////////////////
///文件名：单视图场景
///创建者：高志清
///创建时间：2013年7月26日
///内容描述：单视图场景
//////////////////////////////////////////////////////////////////////////

#ifndef SINGLE_SCENE_VIEW_H
#define SINGLE_SCENE_VIEW_H 1

#include "Export.h"
#include "SceneView.h"

#include "osgViewer/Viewer"

namespace VRSim3D
{

class VRSim3D_EXPORT SingleSceneView :
	public SceneView
{
public:
	//friend class SceneViewBuilder;
	SingleSceneView(void);
	~SingleSceneView(void);
	///zqgao 创建场景，返回true创建成功
	///\param samples(unsigned int)
	// 抗锯齿倍数，请根据使用机显卡设置
	virtual	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);

	///zqgao 获取场景视景器
	virtual osgViewer::ViewerBase* getViewer();
private:
	// zqgao 渲染线程
	static void RenderThread(void* ptr);
	// zqgao 初始化 
	void myinit(void);
	///zqgao 保存基础事件
	virtual void SaveBasicEvents();

protected:
	// zqgao 初始化主相机
	void initCamera(void);
	// zqgao 设置场景
	void setupView(void);
	///zqgao 场景管理器
	osg::ref_ptr<osgViewer::Viewer>						   m_Viewer;
};

}
#endif
