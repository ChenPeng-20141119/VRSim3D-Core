#include "RectEffect.h"
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/CullFace>
#include <osg/Depth>
#include <osgFX/Scribe>

#include <osgEarth/GeoData>

using namespace VRSim3D;

osg::ref_ptr<osg::Image> RectEffect::mEffectTexImage = NULL;

RectEffect::RectEffect(osgEarth::MapNode* earthModel,osg::Vec3 top, osg::Vec3 center, double r,osg::Vec4 tColor)
{

	///zqgao 参数初始化
	mEarthModel = earthModel;
	mfEffectRadius = r;
	mEffectVec0 = top;
	mEffectVec1 = center;
	mdEffectHeight = (mEffectVec0 - mEffectVec1).length();
	mdAspectRatio = 0.2;

	///zqgao 是否启用纹理动画并设置纹理动画速度
	m_bUseDynTex = false;
	mfTexSpeed = 2.0f;
	
	///zqgao 设置扫描效果的颜色并设置Alpha为0.4
	mCurColor = tColor;  mCurColor.a() = 0.4;

	///zqgao 设置纹理波束长度
	mfWaveLength = 200000;

	m_bNeedUpdateBeam = false;

	createBeam(mEffectVec0,mEffectVec1);
	setNumChildrenRequiringUpdateTraversal(1);
}

RectEffect::~RectEffect(void)
{

}

void RectEffect::traverse(osg::NodeVisitor& nv)
{
	
	osg::NodeVisitor::VisitorType vt = nv.getVisitorType();
	if (vt == osg::NodeVisitor::UPDATE_VISITOR
		&&nv.getFrameStamp())
	{
		///zqgao 启用扫描效果动态纹理
		if (m_bUseDynTex)
		{
			const osg::FrameStamp* f = nv.getFrameStamp();
			float dt = f->getSimulationTime();

			float ss = fmod(dt * mfTexSpeed, 1.0f);
			mEffectTexMatrix->setMatrix(osg::Matrix::translate(-ss, 0, 0));
		}
		if (m_bNeedUpdateBeam)
		{
			updateBeam(mEffectVec0, mEffectVec1);
			m_bNeedUpdateBeam = false;
		}
	}
	osg::MatrixTransform::traverse(nv);

}

///zqgao 在动态更新位置时调用，设置轨道点的位置参数，地面点自动计算
void RectEffect::setOneXYZPosition(const osg::Vec3d &vec0)
{
	osg::Vec3d n = vec0;
	n.normalize();  n = n * osg::WGS_84_RADIUS_EQUATOR;
	setXYZPosition(vec0, n);
}

///zqgao 设置波束动态纹理效果
void RectEffect::setUseDynTex(bool bDynTex)
{
	m_bUseDynTex = bDynTex;
}

bool RectEffect::getUesDynTex()
{
	return m_bUseDynTex;
}

///zqgao 光束效果纹理的移动速度
void RectEffect::setTexSpeed(float s)
{
	mfTexSpeed = s;
}

float RectEffect::getTexSpeed()
{
	return mfTexSpeed;
}

///zqgao 设置卫星扫描效果的张角
void RectEffect::setEffectAngle(double dAngle)
{
	///zqgao 保存当前的角度值，注意要转化为弧度
	mdEffectAngle = osg::DegreesToRadians(dAngle);

	///zqgao 计算得到扫描效果地面底盘的半径
	mfEffectRadius = mdEffectHeight * tan(mdEffectAngle);

	m_bNeedUpdateBeam = true;
}

///zqgao 获取卫星扫描效果的张角
double RectEffect::getEffectAngle()
{
	return mdEffectAngle;
}

///zqgao 设置两端的半径
void RectEffect::setEffectRadius(double r)
{
	if (mfEffectRadius != r)
	{
		///zqgao 保存扫描效果地盘半径
		mfEffectRadius = r;

		///zqgao 反向计算角度
		mdEffectAngle = atan2(mfEffectRadius, mdEffectHeight);

		m_bNeedUpdateBeam = true;
	}
}

///zqgao 获取波束两个端点处的半径
double RectEffect::getEffectRadius()
{
	return mfEffectRadius;
}

///zqgao 设置卫星扫描效果的高度
void RectEffect::setEffectHeight(double h)
{
	if (mdEffectHeight != h)
	{
		///zqgao 保存扫描效果的高度
		mdEffectHeight = h;

		///zqgao 反向计算角度
		mdEffectAngle = atan2(mfEffectRadius, mdEffectHeight);

		m_bNeedUpdateBeam = true;
	}
}

///zqgao 获取卫星扫描效果的高度
double RectEffect::getEffectHeight()
{
	return mdEffectHeight;
}

///zqgao 设置扫描效果地盘的横纵比
void RectEffect::setAspectRatio(double ratio)
{
	mdAspectRatio = ratio;

	m_bNeedUpdateBeam = true;
}

///zqgao 获取扫描效果地盘的横纵比
double RectEffect::getAspectRatio(double ratio)
{
	return mdAspectRatio;
}

void RectEffect::setXYZPosition(const osg::Vec3d &vec0, const osg::Vec3d &vec1)
{
	if (mEffectVec0 != vec0)
	{
		mEffectVec0 = vec0;
		m_bNeedUpdateBeam = true;
	}
	if (mEffectVec1 != vec1)
	{
		mEffectVec1 = vec1;
		m_bNeedUpdateBeam = true;
	}
}

///zqgao 设置颜色
void RectEffect::setColor(osg::Vec4 vec)
{
	mCurColor = vec;
	if (mGeoColors.valid())
	{
		(*mGeoColors)[0] = mCurColor;
		mGeoColors->dirty();
	}
}
///zqgao 获取条带的颜色
osg::Vec4& RectEffect::getColor()
{
	return mCurColor;
}

///zqgao 设置纹理波束长度
void RectEffect::setWaveLength(float l)
{
	if (mfWaveLength != l)
	{
		mfWaveLength = l;
		m_bNeedUpdateBeam = true;
	}
}

float RectEffect::getWaveLength()
{
	return mfWaveLength;
}

///zqgao 创建通信效果的圆形管道
void RectEffect::createBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1)
{
	osg::Vec3d top,center,front,left,temp;
	osgEarth::GeoPoint centerMap(mEarthModel->getMapSRS(),XYZ0,osgEarth::ALTMODE_ABSOLUTE);
	centerMap.toWorld(top);
	centerMap.set(mEarthModel->getMapSRS(), XYZ1,osgEarth::ALTMODE_ABSOLUTE);
	centerMap.toWorld(center);

	temp = osg::Vec3d(XYZ1.x() - 1,XYZ1.y(),XYZ1.z());
	centerMap.set(mEarthModel->getMapSRS(), temp,osgEarth::ALTMODE_ABSOLUTE);
	centerMap.toWorld(temp);

	left = temp - center;
	left.normalize();

	temp = osg::Vec3d(XYZ1.x(),XYZ1.y()+1,XYZ1.z());
	centerMap.set(mEarthModel->getMapSRS(), temp,osgEarth::ALTMODE_ABSOLUTE);
	centerMap.toWorld(temp);

	front = temp - center;
	front.normalize();


	///zqgao 为顶点坐标数组分配空间，并为各个顶点赋值
	if (!mGeoVertices.valid())  mGeoVertices = new osg::Vec3dArray();
	mGeoVertices->resize(6);
	(*mGeoVertices)[0] = top;
	(*mGeoVertices)[1] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[2] = center + left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[3] = center + left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[4] = center - left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[5] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	///zqgao 建立顶点之间的拓扑关系
	osg::ref_ptr<osg::DrawElementsUInt> _triangles = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	_triangles->resize(12);
	(*_triangles)[0] = 0; (*_triangles)[1] = 1;(*_triangles)[2] = 2;
	(*_triangles)[3] = 0; (*_triangles)[4] = 2;(*_triangles)[5] = 3;
	(*_triangles)[6] = 0; (*_triangles)[7] = 3;(*_triangles)[8] = 4;
	(*_triangles)[9] = 0; (*_triangles)[10] = 4;(*_triangles)[11] = 1;
	/*_triangles->push_back(0);  _triangles->push_back(1);  _triangles->push_back(2);
	_triangles->push_back(0);  _triangles->push_back(2);  _triangles->push_back(3);
	_triangles->push_back(0);  _triangles->push_back(3);  _triangles->push_back(4);
	_triangles->push_back(0);  _triangles->push_back(4);  _triangles->push_back(1);*/
	///zqgao 为几何体颜色数组分配空间并赋值
	if (!mGeoColors.valid())  mGeoColors = new osg::Vec4Array(1);
	(*mGeoColors)[0] = mCurColor;

	///zqgao 创建扫描效果几何体，并设置结合体的属性
	osg::ref_ptr<osg::Geometry> geom  =  new osg::Geometry;	
	
	geom->setVertexArray(mGeoVertices.get());
	geom->setColorArray(mGeoColors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN,0,mGeoVertices->size()));
	//geom->addPrimitiveSet(_triangles);	
	geom->setUseVertexBufferObjects(true);
	geom->setUseDisplayList(false);
	geom->setDataVariance(osg::Object::DYNAMIC);

	///zqgao 创建扫描效果实体节点，并设置节点属性信息
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	geode->setNodeMask(0xF0000002);

	osg::StateSet * ss = geode->getOrCreateStateSet();
	/*ss->setMode(GL_CULL_FACE, true);
	ss->setAttribute(new osg::CullFace(osg::CullFace::FRONT));*/
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	ss->setMode(GL_LINE_SMOOTH, true);
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setWriteMask(false);
	ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
	ss->setMode(GL_NORMALIZE, true);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	ss->setRenderBinDetails(9999, "RenderBins");
	ss->setMode(GL_BLEND, true);
	ss->setMode(GL_DEPTH_TEST, true);
	///zqgao 这里要注意
	geode->setCullingActive(false);

	///zqgao 绘制节点边框
	osg::ref_ptr<osgFX::Scribe> sc = new osgFX::Scribe();
	sc->setWireframeColor(mCurColor);
	//sc->setWireframeLineWidth(2.0f);
	sc->addChild(geode.get());
	///zqgao 创建根结点
	addChild(sc.get());
	
}

///zqgao 根据新的端点对卫星扫描效果进行更新
void RectEffect::updateBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1)
{
	if (mGeoVertices.valid())
	{
		osg::Vec3d top,center,front,left,temp;
		osgEarth::GeoPoint centerMap(mEarthModel->getMapSRS(),XYZ0,osgEarth::ALTMODE_ABSOLUTE);
		centerMap.toWorld(top);
		centerMap.set(mEarthModel->getMapSRS(), XYZ1,osgEarth::ALTMODE_ABSOLUTE);
		centerMap.toWorld(center);

		temp = osg::Vec3d(XYZ1.x() - 1,XYZ1.y(),XYZ1.z());
		centerMap.set(mEarthModel->getMapSRS(), temp,osgEarth::ALTMODE_ABSOLUTE);
		centerMap.toWorld(temp);

		left = temp - center;
		left.normalize();

		temp = osg::Vec3d(XYZ1.x(),XYZ1.y()+1,XYZ1.z());
		centerMap.set(mEarthModel->getMapSRS(), temp,osgEarth::ALTMODE_ABSOLUTE);
		centerMap.toWorld(temp);

		front = temp - center;
		front.normalize();

		(*mGeoVertices)[0] = top;
		(*mGeoVertices)[1] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
		(*mGeoVertices)[2] = center + left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
		(*mGeoVertices)[3] = center + left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
		(*mGeoVertices)[4] = center - left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
		(*mGeoVertices)[5] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
		mGeoVertices->dirty();
	}
}
