#ifndef VRSIM3DGLOBE_QT_H
#define VRSIM3DGLOBE_QT_H

#include <QtGui/QMainWindow>
#include "ui_vrsim3dglobe_qt.h"

#include "SingleSceneView.h"
#include "SceneViewBuilder.h"
#include "SingleSceneView-QT.h"
#include "GlobeSceneViewManager.h"
#include "GlobeBasicFunc.h"
#include "ControlBar.h"
#include "VRPrecipitation.h"
#include "VRPrecipitationManager.h"
#include "ModelPicker.h"
#include "NodeInfoDrawCallback.h"
#include "SatelliteGroupManager.h"

//#include <qgraphicssceneevent.h>
//#include <QtGui/QKeyEvent>
//#include <QtGui/QApplication>
//#include <QtOpenGL/QGLWidget>
//#include <QtGui/QMainWindow>
//#include <QtGui/QMdiSubWindow>
//#include <QtGui/QMdiArea>
class GraphicsView : public QGraphicsView
{
public:
	GraphicsView(){}

protected:
	void resizeEvent(QResizeEvent *event) 
	{
		if (scene())
		{
			scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
			VRSim3D::SingleSceneViewQT* view = dynamic_cast<VRSim3D::SingleSceneViewQT*>(scene());
			if(view)
				view->resize(event->size().width(),event->size().height());
		}
		QGraphicsView::resizeEvent(event);
	}
};

class VRSim3DGlobe_QT : public QMainWindow
{
	Q_OBJECT

public:
	VRSim3DGlobe_QT(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VRSim3DGlobe_QT();

public:
	///zqgao 星空设置
	void CreateSKy();
	///设置时间速度
	void OnTimeSet(UINT id);
	///zqgao 海洋设置
	void CreateOcean();
	///zqgao 云模拟
	void CreateClouds();
	///雨效果
	void CreateRain();
	///雪效果
	void CreateSnow();
	///zqgao 加载波士顿城市模型
	void LoadBSTBuilding();
	///雾效果
	void CreateFog();
	///雾霾效果
	void CreateHaze();
	///船行驶模拟（需要先加载海洋）
	void LoadShip();
	///旋翼溪流模拟（需要先加载海洋）
	void LoadPlane();
	// 加载卫星
	//参数1：卫星的名称
	//参数2：卫星的轨迹路径【在程序的执行目录下把给你的3Ddata文件夹放进去，把轨迹路径放进去，这里只需要写轨迹的名字就可以了】
	//参数3：卫星的模型路径【在3Ddata文件夹里有model文件夹把模型放进去，这里传model/模型文件名】
	void AddSatelliteNode( std::string name,std::string satellitePath, std::string satelliteModel);
	// 加载地面物体
	//参数：name模型标识，modelPath表示物体的模型路径【在3Ddata文件夹里有model文件夹把模型放进去，这里传model/模型文件名】
	//参数：scaleX，scaleY，scaleZ表示模型在x轴、y轴、z轴的缩放比例
	//参数：lon，lat，alt表示模型放置的位置，经度、纬度、高度【高度设置为0即可】
	void LoadGroundObject(std::string name,std::string modelPath,double scaleX,
		double scaleY,double scaleZ,double lon,double lat,double alt);
	// 创建威力范围
	//参数1：内部小球半径【如果只画一个将其设为0】
	//参数2：外部大球半径
	//参数3：水平张角起始弧度
	//参数4：水平张角结束弧度
	//参数5：垂直张角起始弧度
	//参数6：垂直张角结束弧度
	//参数7：球网格密度【一般设为40】
	//参数8：表面纬线密度【一般设为10】
	//参数9：表面经线间隔【一般设为30】
	//参数：lon，lat，alt表示模型放置的位置，经度、纬度、高度【高度设置为0即可】
	void CreatePowerRange(double interRadius,double radius,double azMin,double azMax,
		double elevMin,double elevMax,int density,int elevDensity,
		int azDensity,double lon,double lat,double alt);
	//获取当前位置信息点信息
	void GetCurrentSatellitePosInfo(VRSim3D::SatellitePosInfo &currentPosInfo);
	//发射信号接口
	//参数1：name发射信号模型的名字
	//参数2：xAngle,yAngle,zAngle信号x,y,z方向的角度
	//参数3：height信号发射的高度
	//参数4：speed信号发射的速度
	//参数5：length信号的波长
	//参数6：color信号波段的颜色
	void SendSignal(std::string name,float xAngle,float yAngle,float zAngle,float height,
		float speed = 5,float length = 4000,osg::Vec4 color = osg::Vec4(1, 0, 1, 0.4));

	//发射信号接口
	void SendSignal(std::string startName,std::string endName,
		float speed = 5,float length = 4000,osg::Vec4 color = osg::Vec4(1, 0, 1, 0.4));
private:
	///zqgao 天气模拟
	void setPrecipitation( VRSim3D::VRPrecipitation::PPTType type );
	///zqgao 加载运动模型
	osg::Node* addDynamicModel(const std::string &path,osg::AnimationPath* animation);
	void OnControlBarDragger(float value);
	void OnPickEvent(std::string name);
	void OnPickOverEvent(int mask);
	///zqgao 显示节点信息
	void showNodeInfo(osg::Node* node,const float& x,const float &y);
	void addControlBar();
private:
	void initView();
private:
	Ui::VRSim3DGlobe_QTClass ui;
	GraphicsView* m_qview;
	///zqgao 场景创建者指针
	VRSim3D::SceneViewBuilder*	     m_Builder;
	///zqgao 场景管理者指针
	VRSim3D::GlobeSceneViewManager* m_GLManager;
private:
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
	VRSim3D::SatelliteGroupManager *m_satelliteGroupManager;
	///zqgao 节点信息节点callback
	osg::ref_ptr<VRSim3D::NodeInfoDrawCallback>		m_NodeInfoCallback;
	///zqgao 当前选中节点
	osg::ref_ptr<osg::Node>							 m_NowPickNode;
	osg::ref_ptr<VRSim3D::ControlBar>                           m_conBar;
	osg::ref_ptr<VRSim3D::ModelPicker>						 m_modelPicker;
	VRSim3D::ConBarCallback *                       m_conBarCallback;
};

#endif // VRSIM3DGLOBE_QT_H
