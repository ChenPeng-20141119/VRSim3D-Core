#ifndef VIEW_POSINFO_TEXT_H
#define VIEW_POSINFO_TEXT_H 1

#include "Export.h"

#include "osg/Referenced"
#include "osg/Node"
#include "osg/Drawable"

#include "osgEarthUtil/EarthManipulator"

namespace VRSim3D
{

class VRSim3D_EXPORT ViewPosInfoText
	:public osg::Referenced
{
public:
	struct ValueTextDrawCallback : public virtual osg::Drawable::DrawCallback
	{
	public:
		ValueTextDrawCallback(osgEarth::Util::EarthManipulator *pManp);
		~ValueTextDrawCallback(){}
		virtual void drawImplementation(osg::RenderInfo& renderInfo, const osg::Drawable* drawable) const;
	private:
		osg::ref_ptr<osgEarth::Util::EarthManipulator>    m_EarthManp;
		//mutable CString        m_TmpText;
	};
	ViewPosInfoText(osgEarth::Util::EarthManipulator* EarthManip);
	~ViewPosInfoText(void);
	///zqgao ������Ϣ�ڵ�
	osg::Node* makeViewPosInfoText(const float &x,const float &y, const float &width,const float &height);
	///zqgao ��������
	void setVisible(bool visible);
	bool getVisible(){return m_Visible;}
	///zqgao ��ȡ��Ϣ�ڵ�
	osg::Node* getInforNode(){return m_InforNode.get();}
private:
	///zqgao ��������ٿ���
	osg::ref_ptr<osgEarth::Util::EarthManipulator> m_EarthManip;
	///zqgao ��Ϣ�ڵ�
	osg::ref_ptr<osg::Node>						   m_InforNode;
	///zqgao �ڵ�����
	bool										   m_Visible;
};
}
#endif
