//////////////////////////////////////////////////////////////////////////
///�ļ�������̬������
///�����ߣ���־��
///����ʱ�䣺2014��7��1��
///�����������ṩ�����ϳ��ú���ʵ��
//////////////////////////////////////////////////////////////////////////

#ifndef GLOBEBASICFUNC_H
#define GLOBEBASICFUNC_H 1

#include "Export.h"

#include "osgViewer/Viewer"
#include "osg/NodeCallback"
#include "osg/AnimationPath"

#include "osgEarthUtil/Sky"
#include "osgEarthUtil/Ocean"
#include "osgEarth/MapNode"

namespace VRSim3D {namespace GlobeBasicFunc
{
	/*using namespace VRSim3D;*/
	///zqgao �����ǿ�
	extern VRSim3D_EXPORT osgEarth::Util::SkyNode* createSkymodel(osgViewer::Viewer* view,osgEarth::MapNode* mapnode);

	///zqgao ��������
	extern VRSim3D_EXPORT osgEarth::Util::OceanNode* createOceanmodel(osgEarth::MapNode* mapnode,const std::string &resources);

	///zqgao ����ʸ�����ݽ���
	extern VRSim3D_EXPORT osg::Referenced* loadBuildingsFromShp(const std::string &features,const std::string &texture);


	///zqgao zqgao ���ݾ�γ�߻�ȡ�����������
	extern VRSim3D_EXPORT osg::Matrixd getMaxtrixFormLocal(osgEarth::MapNode *mapnode,const float &lat,const float &lon,const float &ele);

	///zqgao ��������ĳһ��Ϊ���ģ�����Բ���˶�����·��
	extern VRSim3D_EXPORT osg::AnimationPath* createEarthAnimationPath(osgEarth::MapNode* mapNode, const osg::Vec3& center,
		const osg::Vec3&scale,const osg::Vec3d&Z,const osg::Vec3d&toWard,const float& radius,const double& looptime);

	/// �ǿն���
	struct VRSim3D_EXPORT AnimateSkyUpdateCallback : public osg::NodeCallback
	{
		AnimateSkyUpdateCallback( double rate = 1440 );
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

		///zqgao ����ģ��ʱ���ٶ�
		void setRate(const double &rate);

		///zqgao ���û��������ȣ������ʼ������Ӱ���ֹ�����
		void setAmbientBrightness();

		///zqgao ������һ���ǿձ仯ʱ����
		double m_accumTime;
		///zqgao ��һ����Ⱦ����Ӧʱ��
		double m_prevTime;
		///zqgao ģ��ʱ���ٶȣ�1440��ʾһ�����ʵ��һ�죩
		double m_rate;
		///zqgao �ǿսڵ�
		osgEarth::Util::SkyNode* m_Sky;
	};

}
}
#endif
