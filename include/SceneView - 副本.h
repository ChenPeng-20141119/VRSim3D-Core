#ifndef SCENEVIEW_H
#define SCENEVIEW_H 1

#include "Export.h"
#include <Windows.h>

#include "CommandHandler.h"
#include "ViewPosInfoText.h"

#include "osgText/Font"
#include "osg/GraphicsContext"
#include "osg/Group"
#include "osgGA/KeySwitchMatrixManipulator"
#include "osgViewer/Viewer"

#include "osgEarth/MapNode"
#include "osgEarthUtil/Controls"

namespace VRSim3D
{

class VRSim3D_EXPORT SceneView
{
public:
	SceneView(void);
	~SceneView(void);
	///zqgao 用于嵌入Cwnd窗口
	///\param hwnd(HWND)
	// 嵌入窗口句柄，若为空则创建独立窗口
	SceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f);

	///zqgao 创建场景，返回true创建成功
	///\param samples(unsigned int)
	// 抗锯齿倍数，请根据使用机显卡设置
	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);

	///zqgao 保存基础事件
	virtual void SaveBasicEvents() = 0;
	// zqgao 加载模型
	bool LoadModel(const std::string& path);
	// zqgao 加载地球数据
	bool LoadGlobe(const std::string& path,bool isEarthManip = true);
	///zqgao 删除地球数据
	bool RemoveGlobe();
	///zqgao 鼠标信息控件显隐
	void showMouseInfor(bool isShow);
	//zqgao 屏幕中心位置信息显隐
	void showViewPosInfoText(bool isShow = true);
	///zqgao 删除鼠标信息控件
	bool DelMouseInfor();
	//zqgao 删除屏幕中心位置信息
	bool DelViewPosInfoText();
	///zqgao 增加基础事件基础事件
	bool AddEventHandlers(osgGA::GUIEventHandler* event);
	///zqgao 清空场景
	void clear();
protected:
	// zqgao 创建上下文环境
	void setGraphicsContext(HWND hwnd, const float & width, const float & height,const unsigned int &samples);
 
	// zqgao 初始化 
	void init(void);
	// 获取运行路径
	std::string GetAppPath(void);
	// zqgao 初始化主相机 
	void initCamera(void);
	// zqgao 设置场景
	void setupView(void);
	//zqgao 设置地球操控器
	void setEarthManip();
	//zqgao 显示鼠标位置
	void showMousePosition(bool isShow = true);
	///zqgao 删除非基础事件
	void DelEventHandlers();
	///zqgao 清空根节点
	void clearRoot();
private:
	// zqgao 渲染线程
	static void RenderThread(void* ptr);
private:
	///zqgao 场景管理器
	osg::ref_ptr<osgViewer::Viewer>						   m_Viewer;
	///zqgao Viewer基础事件集（在场景切换时不需要更改事件）
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS;
	BASICEVENTS											   m_BasicEvents;							
	///zqgao 上下文环境
	osg::ref_ptr<osg::GraphicsContext>					   m_gc;
	///zqgao 地球节点
	osg::ref_ptr<osgEarth::MapNode>						   m_MapNode;
	///zqgao 地球相机操控器
	osg::ref_ptr<osgEarth::Util::EarthManipulator>         m_EarthManip;
	///zqgao 根节点
	osg::ref_ptr<osg::Group>							   m_Root;
	///zqgao 设备层
	osg::ref_ptr<osg::Group>							   m_ModelRoot;
	///zqgao 控件面板层
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
	///zqgao 鼠标消息控件
	osg::ref_ptr<osgEarth::Util::Controls::LabelControl>   m_MouseCoords;
	///zqgao 全局字体文件
	osg::ref_ptr<osgText::Font>							   m_AllFont;
	///zqgao 数据路径
	std::string											   m_Dir;
	///zqgao 节点控制
	osg::ref_ptr<VRSim3D::CommandHandler>				   m_CommandHandler;
	///zqao 屏幕中心位置信息
	osg::ref_ptr<ViewPosInfoText>						   m_ViewPosInfoText;
	///zqgao 相机操控器
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
	///zqgao 相机操控器数量
	int													   m_MpNum;
};
}
#endif
