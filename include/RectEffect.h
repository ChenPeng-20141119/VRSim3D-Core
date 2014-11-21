//////////////////////////////////////////////////////////////////////////
///文件名：特效几何体
///创建者：高志清
///创建时间：2014年10月26日
///内容描述：特殊效果几何体类
//////////////////////////////////////////////////////////////////////////

#ifndef RECT_EFFECT_H
#define RECT_EFFECT_H 1

#include "Export.h"
#include <osg/MatrixTransform>
#include <osg/TexMat>
#include "osgEarth/MapNode"

namespace VRSim3D
{
class VRSim3D_EXPORT RectEffect : public osg::MatrixTransform
{
public:
	///zqgao 视景体顶点坐标，下方中心点坐标，下方半径（矩形边长，可在后面接口中设置更改），颜色
	RectEffect(osgEarth::MapNode* earthModel,osg::Vec3 top, osg::Vec3 center, double r,osg::Vec4 tColor);
	~RectEffect(void);

protected:
	///zqgao 创建波束的管道效果
	virtual void createBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
	///zqgao 动态对波束效果进行更新
	virtual void updateBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);

public:
	///zqgao 设置卫星扫描效果的张角
	void setEffectAngle(double dAngle);
	///zqgao 获取卫星扫描效果的张角
	double getEffectAngle();
	///zqgao 设置波束两个端点处的半径
	void setEffectRadius(double r);
	///zqgao 获取波束两个端点处的半径
	double getEffectRadius();
	///zqgao 设置卫星扫描效果的高度
	void setEffectHeight(double h);
	///zqgao 获取卫星扫描效果的高度
	double getEffectHeight();
	///zqgao 设置扫描效果地盘的横纵比
	void setAspectRatio(double ratio);
	///zqgao 获取扫描效果地盘的横纵比
	double getAspectRatio(double ratio);

	///zqgao 在动态更新位置时调用，设置起止点位置xyz，单位米
	void setXYZPosition(const osg::Vec3d &vec0, const osg::Vec3d &vec1);
	///zqgao 在动态更新位置时调用，设置轨道点的位置参数，地面点自动计算
	void setOneXYZPosition(const osg::Vec3d &vec0);

	///zqgao 设置探测效果是否可视
	void setEffectBodyVisible(bool bSet);
	
	///zqgao 设置\获取波束的颜色，包括透明度
	void setColor(osg::Vec4 vec);
	osg::Vec4& getColor();
	///zqgao 设置纹理波束的长度，单位：米
	void setWaveLength(float l); 
	float getWaveLength();
	///zqgao 设置光束纹理移动速度，单位：米
	void setTexSpeed(float s);
	float getTexSpeed();
	///zqgao 设置波束动态纹理效果
	void setUseDynTex(bool bDynTex);
	bool getUesDynTex();

public:
	///zqgao 遍历时进行调用
	virtual void traverse(osg::NodeVisitor& nv);
	/*///zqgao 地球模型的指针
	osg::ref_ptr<osg::EllipsoidModel>  mEarthModel;*/

protected:
	///zqgao 光束效果纹理的移动速度；光束效果纹理的波长效果
	float mfTexSpeed, mfWaveLength;

	///zqgao 扫描效果的底端半径，即条带宽度
	double mfEffectRadius;
	///zqgao 扫描效果的相对高度，即对地距离
	double mdEffectHeight;
	///zqgao 扫描效果的张角，通过高度和张角可以计算出底端半径
	double mdEffectAngle;
	///zqgao 扫描效果地盘的横纵比
	double mdAspectRatio;
	///zqgao 扫描效果的起点和终点，这里与半径和高度是相互制约的
	osg::Vec3d mEffectVec0, mEffectVec1;

	///zqgao 设置纹理矩阵
	osg::ref_ptr<osg::TexMat>  mEffectTexMatrix;
	///zqgao 几何体顶点数组
	osg::ref_ptr<osg::Vec3dArray> mGeoVertices;
	///zqgao 纹理坐标数组
	osg::ref_ptr<osg::Vec2Array> mTexCoords;
	osg::ref_ptr<osgEarth::MapNode>  mEarthModel;

	///zqgao 当前设置的颜色，以及几何体颜色数组指针
	osg::Vec4 mCurColor;
	osg::ref_ptr<osg::Vec4Array> mGeoColors;


protected:

	///zqgao 使用动态纹理效果
	bool m_bUseDynTex;
	///zqgao 共用纹理，所以设置为静态变量
	static osg::ref_ptr<osg::Image>  mEffectTexImage;

	///zqgao 标识是否需要重新生成管道几何结构
	bool m_bNeedUpdateBeam;
};
}
#endif

