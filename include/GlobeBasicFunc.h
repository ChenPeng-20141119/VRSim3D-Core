//////////////////////////////////////////////////////////////////////////
///文件名：静态方法类
///创建者：高志清
///创建时间：2014年7月1日
///内容描述：提供地球上常用函数实现
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
	///zqgao 创建星空
	extern VRSim3D_EXPORT osgEarth::Util::SkyNode* createSkymodel(osgViewer::Viewer* view,osgEarth::MapNode* mapnode);

	///zqgao 创建海洋
	extern VRSim3D_EXPORT osgEarth::Util::OceanNode* createOceanmodel(osgEarth::MapNode* mapnode,const std::string &resources);

	///zqgao 加载矢量数据建筑
	extern VRSim3D_EXPORT osg::Referenced* loadBuildingsFromShp(const std::string &features,const std::string &texture);


	///zqgao zqgao 根据经纬高获取世界坐标矩阵
	extern VRSim3D_EXPORT osg::Matrixd getMaxtrixFormLocal(osgEarth::MapNode *mapnode,const float &lat,const float &lon,const float &ele);

	///zqgao 在球上以某一点为中心，计算圆周运动动画路径
	extern VRSim3D_EXPORT osg::AnimationPath* createEarthAnimationPath(osgEarth::MapNode* mapNode, const osg::Vec3& center,
		const osg::Vec3&scale,const osg::Vec3d&Z,const osg::Vec3d&toWard,const float& radius,const double& looptime);

	/// 星空动画
	struct VRSim3D_EXPORT AnimateSkyUpdateCallback : public osg::NodeCallback
	{
		AnimateSkyUpdateCallback( double rate = 1440 );
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

		///zqgao 设置模拟时间速度
		void setRate(const double &rate);

		///zqgao 设置环境光亮度（解决初始地球阴影部分过暗）
		void setAmbientBrightness();

		///zqgao 距离上一次星空变化时间间隔
		double m_accumTime;
		///zqgao 上一针渲染所对应时间
		double m_prevTime;
		///zqgao 模拟时间速度（1440表示一秒代表实际一天）
		double m_rate;
		///zqgao 星空节点
		osgEarth::Util::SkyNode* m_Sky;
	};

}
}
#endif
