#ifndef COMMEFFECT_H
#define COMMEFFECT_H 1

#include "Export.h"
#include "osg/Group"
#include "osg/TexMat"
#include "osg/CoordinateSystemNode"
#include "osgEarth/MapNode"

namespace VRSim3D
{
	class VRSim3D_EXPORT CCommEffect : public osg::Group
	{
	public:
		///dbs 构造函数包括：xyz 端点半径，度数、米 bDyn是否有波束效果
		CCommEffect(const osg::Vec3d &vec1, float r1, const osg::Vec3d &vec2, float r2, bool bDyn = true);
		~CCommEffect(void);

	protected:
		///dbs 创建波束的管道效果
		osg::Node* createPipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
		///dbs 动态对波束效果进行更新
		void updatePipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
		///dbs 遍历时进行调用
		virtual void traverse(osg::NodeVisitor& nv);

	public:
		///dbs 设置地球椭圆模型节点，用于进行空间坐标和经纬度坐标的转换
		void setEarthModel(osgEarth::MapNode* pEarthModel);

		///dbs 在动态更新位置时调用，设置起止点位置xyz，单位米
		void setXYZPosition(const osg::Vec3d &vec1, const osg::Vec3d &vec2);
		///dbs 在动态更新位置时调用，设置起止点位置lla，维度经度高度
		void setLLAPosition(const osg::Vec3d &lla1, const osg::Vec3d &lla2);

		///dbs 设置通信波段的移动速度，单位：米
		void setSpeed(float s);
		///dbs 设置波束的长度，单位：米
		void setLength(float l); 
		///dbs 设置波束两个端点处的半径
		void setRadius(float r0, float r1);
		///dbs 设置\获取波束的颜色，包括透明度
		void setColor(osg::Vec4 vec);
		osg::Vec4& getColor();
		///dbs 设置波束是否暂停或者正常播放
		void setDynStop(bool bStop);

	public:
		bool m_bDynEffect, m_bStop;

		///dbs 标识通信效果的移动速度
		float mfSpeed, mfLength;

		///dbs 通信效果两端的半径
		float mfRadius[2];
		///dbs 通信效果的起点和终点
		osg::Vec3d mVecPos1, mVecPos2;

		///dbs 设置纹理矩阵
		osg::ref_ptr<osg::TexMat>  mTexMatrix;
		///dbs 几何体顶点数组
		osg::ref_ptr<osg::Vec3Array> mGeoVertices;
		///dbs 纹理坐标数组
		osg::ref_ptr<osg::Vec2Array> mTexCoords;
		///dbs 当前设置的颜色，以及几何体颜色数组指针
		osg::Vec4 mCurColor;
		osg::ref_ptr<osg::Vec4Array> mGeoColors;
		///dbs 标识是否需要重新生成管道几何结构
		bool m_bNeedUpdate;

	protected:
		///dbs 地球模型的指针
		osg::ref_ptr<osgEarth::MapNode>  mEarthModel;

		///dbs 特效所需要的纹理数据
		static osg::ref_ptr<osg::Image>  mCommTexImage;
	};
}

#endif