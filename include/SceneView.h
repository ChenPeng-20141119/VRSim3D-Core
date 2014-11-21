//////////////////////////////////////////////////////////////////////////
///�ļ�������ͼ����
///�����ߣ���־��
///����ʱ�䣺2013��7��26��
///������������ͼ����
//////////////////////////////////////////////////////////////////////////
#ifndef SCENEVIEW_H
#define SCENEVIEW_H 1

#include "Export.h"

#include "osg/GraphicsContext"
#include "osg/Group"
#include "osgGA/KeySwitchMatrixManipulator"

#include "osgViewer/ViewerBase"

#include "osgEarthUtil/Controls"

#include <Windows.h>

#ifdef _QTWIND
#include <QtGui/QGraphicsView>
//#include <graphicsview/qgraphicssceneevent>
#include <qgraphicssceneevent.h>
#include <QString>
#include <QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMdiArea>

#endif

namespace VRSim3D
{
class VRSim3D_EXPORT SceneView : public osg::Referenced
{
public:
	friend class SceneViewBuilder;
	SceneView(void);
	~SceneView(void);
	
	///zqgao ����Ƕ��Cwnd����
	///\param hwnd(HWND)
	// Ƕ�봰�ھ������Ϊ���򴴽���������
	SceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f);
	
	///zqgao ��������������true�����ɹ�
	///\param samples(unsigned int)
	// ����ݱ����������ʹ�û��Կ�����
	virtual bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16) = 0;
	
	///zqgao ��������¼�
	virtual void SaveBasicEvents() = 0;
	
	///zqgao ��ȡ�����Ӿ���
	virtual osgViewer::ViewerBase* getViewer() = 0;

	///zqgao ��ȡ�����Ļ���
	osg::GraphicsContext* getGraphicsContext() const;

	///zqgao ��ȡ����ٿ���
	osgGA::KeySwitchMatrixManipulator* getSwitchMatrixManipulator();

protected:
	// zqgao ���������Ļ���
	void setGraphicsContext(HWND hwnd, const float & width, const float & height,const unsigned int &samples);
	// zqgao ��ʼ�� 
	void init(void);
#if 0
private:
	// zqgao ��Ⱦ�߳�
	static void RenderThread(void* ptr);
#endif
protected:	
	///zqgao ���ڵ�
	osg::ref_ptr<osg::Group>							   m_Root;
	///zqgao ����ٿ���
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
	///zqgao ����ٿ�������
	int													   m_MpNum;
	///zqgao Viewer�����¼������ڳ����л�ʱ����Ҫ�����¼���
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS ;
	BASICEVENTS											   m_BasicEvents;

///zqgao �����Ļ���
#ifdef _QTWIND
	osg::ref_ptr<osgViewer::GraphicsWindow>	m_gWindow;
#else
	osg::ref_ptr<osg::GraphicsContext>					   m_gc;
#endif
		
};
}
#endif
