//////////////////////////////////////////////////////////////////////////
///�ļ����������ࣨ��ѩ��
///�����ߣ���־��
///����ʱ�䣺2014��7��19��
///���������� ��ѩ����Ч����
//////////////////////////////////////////////////////////////////////////

#ifndef VR_PRECIPITATION_MANAGER_H
#define VR_PRECIPITATION_MANAGER_H 1

#include "Export.h"

#include "VRPrecipitation.h"

#include "osg/MatrixTransform"
#include "osgViewer/Viewer"

#include "osgEarth/MapNode"


namespace VRSim3D {

class VRSim3D_EXPORT VRPrecipitationManager:public osg::MatrixTransform
{
public:
	VRPrecipitationManager(void);
	VRPrecipitationManager(osgViewer::Viewer* view,osgEarth::MapNode* mapnode);
	~VRPrecipitationManager(void);
public:
	///zqgao
	void setViewer(osgViewer::Viewer* view);
	void setMapnode(osgEarth::MapNode* mapnode);

	///zqgao ��Ч��ʼ�߶ȣ���Ե��棩
	void setStartHeight(const float& startHeight);

	///zqgao ��Ч����ܶ�
	void setMaxIntensity(const float& maxIntensity);
	float getMaxIntensity() const {return m_MaxIntensity;}

	///zqgao ��Ч������ܶ�
	void setMaxfogtensity(const float& maxfogtensity);
	float getMaxfogtensity()  const {return m_Maxfogtensity;}

	///zqgao ��ȡ��Ч�ڵ�
	VRPrecipitation* getVRPrecipitation(){return m_VrPrecipitation.get();}

	///zqgao ���÷���
	/*����Ч���ڵ����Ϻ�x��������Ϊ�����Ϸ���
	*ͨ���ǶȺ��ٶȣ��ɼ���������������
	*/
	void setWindProperty(float heading, float speed);

	virtual void traverse(osg::NodeVisitor& nv);
private:
	///zqgao ��ʼ��
	void init();
	///zqgao ����
	void updata();
private:
	///zqgao ��Ч�ڵ�
	osg::ref_ptr<VRPrecipitation> m_VrPrecipitation;
	///zqgao ��Ч��ʼ�߶ȣ���Ե��棩
	float	m_StartHeight;
	///zqgao ��Ч�ܶȣ�0-1��
	float m_MaxIntensity;
	///zqgao ��Ч���ܶ�
	float m_Maxfogtensity;
	osg::ref_ptr<osgViewer::Viewer> m_View;
	osg::ref_ptr<osgEarth::MapNode> m_Mapnode;
};
}
#endif
