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
	///zqgao ����Ƕ��Cwnd����
	///\param hwnd(HWND)
	// Ƕ�봰�ھ������Ϊ���򴴽���������
	SceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f);

	///zqgao ��������������true�����ɹ�
	///\param samples(unsigned int)
	// ����ݱ����������ʹ�û��Կ�����
	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);

	///zqgao ��������¼�
	virtual void SaveBasicEvents() = 0;
	// zqgao ����ģ��
	bool LoadModel(const std::string& path);
	// zqgao ���ص�������
	bool LoadGlobe(const std::string& path,bool isEarthManip = true);
	///zqgao ɾ����������
	bool RemoveGlobe();
	///zqgao �����Ϣ�ؼ�����
	void showMouseInfor(bool isShow);
	//zqgao ��Ļ����λ����Ϣ����
	void showViewPosInfoText(bool isShow = true);
	///zqgao ɾ�������Ϣ�ؼ�
	bool DelMouseInfor();
	//zqgao ɾ����Ļ����λ����Ϣ
	bool DelViewPosInfoText();
	///zqgao ���ӻ����¼������¼�
	bool AddEventHandlers(osgGA::GUIEventHandler* event);
	///zqgao ��ճ���
	void clear();
protected:
	// zqgao ���������Ļ���
	void setGraphicsContext(HWND hwnd, const float & width, const float & height,const unsigned int &samples);
 
	// zqgao ��ʼ�� 
	void init(void);
	// ��ȡ����·��
	std::string GetAppPath(void);
	// zqgao ��ʼ������� 
	void initCamera(void);
	// zqgao ���ó���
	void setupView(void);
	//zqgao ���õ���ٿ���
	void setEarthManip();
	//zqgao ��ʾ���λ��
	void showMousePosition(bool isShow = true);
	///zqgao ɾ���ǻ����¼�
	void DelEventHandlers();
	///zqgao ��ո��ڵ�
	void clearRoot();
private:
	// zqgao ��Ⱦ�߳�
	static void RenderThread(void* ptr);
private:
	///zqgao ����������
	osg::ref_ptr<osgViewer::Viewer>						   m_Viewer;
	///zqgao Viewer�����¼������ڳ����л�ʱ����Ҫ�����¼���
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS;
	BASICEVENTS											   m_BasicEvents;							
	///zqgao �����Ļ���
	osg::ref_ptr<osg::GraphicsContext>					   m_gc;
	///zqgao ����ڵ�
	osg::ref_ptr<osgEarth::MapNode>						   m_MapNode;
	///zqgao ��������ٿ���
	osg::ref_ptr<osgEarth::Util::EarthManipulator>         m_EarthManip;
	///zqgao ���ڵ�
	osg::ref_ptr<osg::Group>							   m_Root;
	///zqgao �豸��
	osg::ref_ptr<osg::Group>							   m_ModelRoot;
	///zqgao �ؼ�����
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas>  m_ControlCanvas;
	///zqgao �����Ϣ�ؼ�
	osg::ref_ptr<osgEarth::Util::Controls::LabelControl>   m_MouseCoords;
	///zqgao ȫ�������ļ�
	osg::ref_ptr<osgText::Font>							   m_AllFont;
	///zqgao ����·��
	std::string											   m_Dir;
	///zqgao �ڵ����
	osg::ref_ptr<VRSim3D::CommandHandler>				   m_CommandHandler;
	///zqao ��Ļ����λ����Ϣ
	osg::ref_ptr<ViewPosInfoText>						   m_ViewPosInfoText;
	///zqgao ����ٿ���
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
	///zqgao ����ٿ�������
	int													   m_MpNum;
};
}
#endif
