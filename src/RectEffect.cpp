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

	///zqgao ������ʼ��
	mEarthModel = earthModel;
	mfEffectRadius = r;
	mEffectVec0 = top;
	mEffectVec1 = center;
	mdEffectHeight = (mEffectVec0 - mEffectVec1).length();
	mdAspectRatio = 0.2;

	///zqgao �Ƿ������������������������ٶ�
	m_bUseDynTex = false;
	mfTexSpeed = 2.0f;
	
	///zqgao ����ɨ��Ч������ɫ������AlphaΪ0.4
	mCurColor = tColor;  mCurColor.a() = 0.4;

	///zqgao ��������������
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
		///zqgao ����ɨ��Ч����̬����
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

///zqgao �ڶ�̬����λ��ʱ���ã����ù�����λ�ò�����������Զ�����
void RectEffect::setOneXYZPosition(const osg::Vec3d &vec0)
{
	osg::Vec3d n = vec0;
	n.normalize();  n = n * osg::WGS_84_RADIUS_EQUATOR;
	setXYZPosition(vec0, n);
}

///zqgao ���ò�����̬����Ч��
void RectEffect::setUseDynTex(bool bDynTex)
{
	m_bUseDynTex = bDynTex;
}

bool RectEffect::getUesDynTex()
{
	return m_bUseDynTex;
}

///zqgao ����Ч��������ƶ��ٶ�
void RectEffect::setTexSpeed(float s)
{
	mfTexSpeed = s;
}

float RectEffect::getTexSpeed()
{
	return mfTexSpeed;
}

///zqgao ��������ɨ��Ч�����Ž�
void RectEffect::setEffectAngle(double dAngle)
{
	///zqgao ���浱ǰ�ĽǶ�ֵ��ע��Ҫת��Ϊ����
	mdEffectAngle = osg::DegreesToRadians(dAngle);

	///zqgao ����õ�ɨ��Ч��������̵İ뾶
	mfEffectRadius = mdEffectHeight * tan(mdEffectAngle);

	m_bNeedUpdateBeam = true;
}

///zqgao ��ȡ����ɨ��Ч�����Ž�
double RectEffect::getEffectAngle()
{
	return mdEffectAngle;
}

///zqgao �������˵İ뾶
void RectEffect::setEffectRadius(double r)
{
	if (mfEffectRadius != r)
	{
		///zqgao ����ɨ��Ч�����̰뾶
		mfEffectRadius = r;

		///zqgao �������Ƕ�
		mdEffectAngle = atan2(mfEffectRadius, mdEffectHeight);

		m_bNeedUpdateBeam = true;
	}
}

///zqgao ��ȡ���������˵㴦�İ뾶
double RectEffect::getEffectRadius()
{
	return mfEffectRadius;
}

///zqgao ��������ɨ��Ч���ĸ߶�
void RectEffect::setEffectHeight(double h)
{
	if (mdEffectHeight != h)
	{
		///zqgao ����ɨ��Ч���ĸ߶�
		mdEffectHeight = h;

		///zqgao �������Ƕ�
		mdEffectAngle = atan2(mfEffectRadius, mdEffectHeight);

		m_bNeedUpdateBeam = true;
	}
}

///zqgao ��ȡ����ɨ��Ч���ĸ߶�
double RectEffect::getEffectHeight()
{
	return mdEffectHeight;
}

///zqgao ����ɨ��Ч�����̵ĺ��ݱ�
void RectEffect::setAspectRatio(double ratio)
{
	mdAspectRatio = ratio;

	m_bNeedUpdateBeam = true;
}

///zqgao ��ȡɨ��Ч�����̵ĺ��ݱ�
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

///zqgao ������ɫ
void RectEffect::setColor(osg::Vec4 vec)
{
	mCurColor = vec;
	if (mGeoColors.valid())
	{
		(*mGeoColors)[0] = mCurColor;
		mGeoColors->dirty();
	}
}
///zqgao ��ȡ��������ɫ
osg::Vec4& RectEffect::getColor()
{
	return mCurColor;
}

///zqgao ��������������
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

///zqgao ����ͨ��Ч����Բ�ιܵ�
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


	///zqgao Ϊ���������������ռ䣬��Ϊ�������㸳ֵ
	if (!mGeoVertices.valid())  mGeoVertices = new osg::Vec3dArray();
	mGeoVertices->resize(6);
	(*mGeoVertices)[0] = top;
	(*mGeoVertices)[1] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[2] = center + left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[3] = center + left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[4] = center - left*mfEffectRadius + front*mfEffectRadius * mdAspectRatio;
	(*mGeoVertices)[5] = center - left*mfEffectRadius - front*mfEffectRadius * mdAspectRatio;
	///zqgao ��������֮������˹�ϵ
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
	///zqgao Ϊ��������ɫ�������ռ䲢��ֵ
	if (!mGeoColors.valid())  mGeoColors = new osg::Vec4Array(1);
	(*mGeoColors)[0] = mCurColor;

	///zqgao ����ɨ��Ч�������壬�����ý���������
	osg::ref_ptr<osg::Geometry> geom  =  new osg::Geometry;	
	
	geom->setVertexArray(mGeoVertices.get());
	geom->setColorArray(mGeoColors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN,0,mGeoVertices->size()));
	//geom->addPrimitiveSet(_triangles);	
	geom->setUseVertexBufferObjects(true);
	geom->setUseDisplayList(false);
	geom->setDataVariance(osg::Object::DYNAMIC);

	///zqgao ����ɨ��Ч��ʵ��ڵ㣬�����ýڵ�������Ϣ
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
	///zqgao ����Ҫע��
	geode->setCullingActive(false);

	///zqgao ���ƽڵ�߿�
	osg::ref_ptr<osgFX::Scribe> sc = new osgFX::Scribe();
	sc->setWireframeColor(mCurColor);
	//sc->setWireframeLineWidth(2.0f);
	sc->addChild(geode.get());
	///zqgao ���������
	addChild(sc.get());
	
}

///zqgao �����µĶ˵������ɨ��Ч�����и���
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
