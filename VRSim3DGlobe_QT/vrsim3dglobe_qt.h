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
	///zqgao �ǿ�����
	void CreateSKy();
	///����ʱ���ٶ�
	void OnTimeSet(UINT id);
	///zqgao ��������
	void CreateOcean();
	///zqgao ��ģ��
	void CreateClouds();
	///��Ч��
	void CreateRain();
	///ѩЧ��
	void CreateSnow();
	///zqgao ���ز�ʿ�ٳ���ģ��
	void LoadBSTBuilding();
	///��Ч��
	void CreateFog();
	///����Ч��
	void CreateHaze();
	///����ʻģ�⣨��Ҫ�ȼ��غ���
	void LoadShip();
	///����Ϫ��ģ�⣨��Ҫ�ȼ��غ���
	void LoadPlane();
	// ��������
	//����1�����ǵ�����
	//����2�����ǵĹ켣·�����ڳ����ִ��Ŀ¼�°Ѹ����3Ddata�ļ��зŽ�ȥ���ѹ켣·���Ž�ȥ������ֻ��Ҫд�켣�����־Ϳ����ˡ�
	//����3�����ǵ�ģ��·������3Ddata�ļ�������model�ļ��а�ģ�ͷŽ�ȥ�����ﴫmodel/ģ���ļ�����
	void AddSatelliteNode( std::string name,std::string satellitePath, std::string satelliteModel);
	// ���ص�������
	//������nameģ�ͱ�ʶ��modelPath��ʾ�����ģ��·������3Ddata�ļ�������model�ļ��а�ģ�ͷŽ�ȥ�����ﴫmodel/ģ���ļ�����
	//������scaleX��scaleY��scaleZ��ʾģ����x�ᡢy�ᡢz������ű���
	//������lon��lat��alt��ʾģ�ͷ��õ�λ�ã����ȡ�γ�ȡ��߶ȡ��߶�����Ϊ0���ɡ�
	void LoadGroundObject(std::string name,std::string modelPath,double scaleX,
		double scaleY,double scaleZ,double lon,double lat,double alt);
	// ����������Χ
	//����1���ڲ�С��뾶�����ֻ��һ��������Ϊ0��
	//����2���ⲿ����뾶
	//����3��ˮƽ�Ž���ʼ����
	//����4��ˮƽ�Žǽ�������
	//����5����ֱ�Ž���ʼ����
	//����6����ֱ�Žǽ�������
	//����7���������ܶȡ�һ����Ϊ40��
	//����8������γ���ܶȡ�һ����Ϊ10��
	//����9�����澭�߼����һ����Ϊ30��
	//������lon��lat��alt��ʾģ�ͷ��õ�λ�ã����ȡ�γ�ȡ��߶ȡ��߶�����Ϊ0���ɡ�
	void CreatePowerRange(double interRadius,double radius,double azMin,double azMax,
		double elevMin,double elevMax,int density,int elevDensity,
		int azDensity,double lon,double lat,double alt);
	//��ȡ��ǰλ����Ϣ����Ϣ
	void GetCurrentSatellitePosInfo(VRSim3D::SatellitePosInfo &currentPosInfo);
	//�����źŽӿ�
	//����1��name�����ź�ģ�͵�����
	//����2��xAngle,yAngle,zAngle�ź�x,y,z����ĽǶ�
	//����3��height�źŷ���ĸ߶�
	//����4��speed�źŷ�����ٶ�
	//����5��length�źŵĲ���
	//����6��color�źŲ��ε���ɫ
	void SendSignal(std::string name,float xAngle,float yAngle,float zAngle,float height,
		float speed = 5,float length = 4000,osg::Vec4 color = osg::Vec4(1, 0, 1, 0.4));

	//�����źŽӿ�
	void SendSignal(std::string startName,std::string endName,
		float speed = 5,float length = 4000,osg::Vec4 color = osg::Vec4(1, 0, 1, 0.4));
private:
	///zqgao ����ģ��
	void setPrecipitation( VRSim3D::VRPrecipitation::PPTType type );
	///zqgao �����˶�ģ��
	osg::Node* addDynamicModel(const std::string &path,osg::AnimationPath* animation);
	void OnControlBarDragger(float value);
	void OnPickEvent(std::string name);
	void OnPickOverEvent(int mask);
	///zqgao ��ʾ�ڵ���Ϣ
	void showNodeInfo(osg::Node* node,const float& x,const float &y);
	void addControlBar();
private:
	void initView();
private:
	Ui::VRSim3DGlobe_QTClass ui;
	GraphicsView* m_qview;
	///zqgao ����������ָ��
	VRSim3D::SceneViewBuilder*	     m_Builder;
	///zqgao ����������ָ��
	VRSim3D::GlobeSceneViewManager* m_GLManager;
private:
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
	VRSim3D::SatelliteGroupManager *m_satelliteGroupManager;
	///zqgao �ڵ���Ϣ�ڵ�callback
	osg::ref_ptr<VRSim3D::NodeInfoDrawCallback>		m_NodeInfoCallback;
	///zqgao ��ǰѡ�нڵ�
	osg::ref_ptr<osg::Node>							 m_NowPickNode;
	osg::ref_ptr<VRSim3D::ControlBar>                           m_conBar;
	osg::ref_ptr<VRSim3D::ModelPicker>						 m_modelPicker;
	VRSim3D::ConBarCallback *                       m_conBarCallback;
};

#endif // VRSIM3DGLOBE_QT_H
