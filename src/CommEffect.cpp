#include "CommEffect.h"

#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgDB/ReadFile>
//#include <osgEarthUtil/ObjectPlacer>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/CullFace>
#include <osg/Texture2D>
#include <osg/TexEnv>

using namespace VRSim3D;
osg::ref_ptr<osg::Image> CCommEffect::mCommTexImage = NULL;

///dbs 波束效果的构造函数
CCommEffect::CCommEffect(const osg::Vec3d &vec1, float r1, const osg::Vec3d &vec2, float r2, bool bDyn)
{
	m_bDynEffect = bDyn;
	m_bStop = false;
	mfSpeed = 1.0f;
	mfRadius[0] = r1;
	mfRadius[1] = r2;
	mVecPos1 = vec1;
	mVecPos2 = vec2;

	mCurColor = osg::Vec4(1, 0, 1, 0.4);

	mfLength = 200000;

	m_bNeedUpdate = false;

	setNumChildrenRequiringUpdateTraversal(1);

	addChild(createPipe(vec1, vec2));
}

CCommEffect::~CCommEffect(void)
{
}

///dbs 设置地球椭圆模型节点，用于进行空间坐标和经纬度坐标的转换
void CCommEffect::setEarthModel(osgEarth::MapNode* pEarthModel)
{
	mEarthModel = pEarthModel;
}

void CCommEffect::setDynStop(bool bStop)
{
	m_bStop = bStop;
}

void CCommEffect::setSpeed(float s)
{
	mfSpeed = s;
}

void CCommEffect::setXYZPosition(const osg::Vec3d &vec1, const osg::Vec3d &vec2)
{
	if (mVecPos1 != vec1)
	{
		mVecPos1 = vec1;
		m_bNeedUpdate = true;
	}
	if (mVecPos2 != vec2)
	{
		mVecPos2 = vec2;
		m_bNeedUpdate = true;
	}
}

///dbs 在动态更新位置时调用，设置起止点位置lla，维度经度高度
void CCommEffect::setLLAPosition(const osg::Vec3d &lla1, const osg::Vec3d &lla2)
{
	osg::Vec3d vec1, vec2;
	mEarthModel->getMapSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::inDegrees(lla1.x()),osg::inDegrees(lla1.y()), lla1.z(), vec1.x(), vec1.y(), vec1.z());
	mEarthModel->getMapSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::inDegrees(lla2.x()), osg::inDegrees(lla2.y()), lla2.z(), vec2.x(), vec2.y(), vec2.z());
	osg::Vec3d vec3;
	mEarthModel->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(vec1.x(), vec1.y(), vec1.z(),vec3.x(), vec3.y(), vec3.z());

	setXYZPosition(vec1, vec2);
}

///dbs 设置波束长度
void CCommEffect::setLength(float l)
{
	if (mfLength != l)
	{
		mfLength = l;
		m_bNeedUpdate = true;
	}
}

///dbs 设置两端的半径
void CCommEffect::setRadius(float r0, float r1)
{
	if (mfRadius[0] != r0 || mfRadius[1] != r1)
	{
		mfRadius[0] = r0;
		mfRadius[1] = r1;
		m_bNeedUpdate = true;
	}
}

///dbs 设置颜色
void CCommEffect::setColor(osg::Vec4 vec)
{
	mCurColor = vec;
	if (mGeoColors.valid())
	{
		(*mGeoColors)[0] = mCurColor;
		mGeoColors->dirty();
	}
}
///dbs 获取条带的颜色
osg::Vec4& CCommEffect::getColor()
{
	return mCurColor;
}

///dbs 创建通信效果的圆形管道
osg::Node* CCommEffect::createPipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1)
{
	int nStackCount = 2; //长度分段
	int nSlices = 50; //圆周分段
	osg::Vec3 N = XYZ1 - XYZ0;
	float dDistance = N.length();
	N.normalize();

	float tu = dDistance / mfLength;

	float fDisStep = dDistance / (nStackCount - 1);

	osg::Vec3 A = N ^ XYZ0;
	A.normalize();

	float stepangle = 2 * osg::PI / (nSlices);
	float mat[9];

	osg::ref_ptr<osg::Vec3Array> normalary = new osg::Vec3Array;
	normalary->resize(nSlices);
	osg::Vec3 temp;
	for (int i = 0; i < nSlices; i++)
	{
		float angle = i * stepangle;
		float cosangle = cos(angle);
		float mincosangle = 1 - cosangle;
		float sinangle = sin(angle);

		mat[0] = cosangle + mincosangle * N.x() * N.x();
		mat[1] = mincosangle * N.x() * N.y() - N.z() * sinangle;
		mat[2] = mincosangle * N.x() * N.z() + N.y() * sinangle;
		mat[3] = mincosangle * N.x() * N.y() + N.z() * sinangle;
		mat[4] = cosangle + mincosangle * N.y() * N.y();
		mat[5] = mincosangle * N.y() * N.z() - N.x() * sinangle;
		mat[6] = mincosangle * N.x() * N.z() - N.y() * sinangle;
		mat[7] = mincosangle * N.y() * N.z() + N.x() * sinangle;
		mat[8] = cosangle + mincosangle * N.z() * N.z();

		temp.x() = mat[0] * A.x() + mat[1] * A.y() + mat[2] * A.z();
		temp.y() = mat[3] * A.x() + mat[4] * A.y() + mat[5] * A.z();
		temp.z() = mat[6] * A.x() + mat[7] * A.y() + mat[8] * A.z();

		temp.normalize();

		(*normalary)[i] = temp;
	}

	int vertCount = (nSlices + 1) * nStackCount;	

	///dbs 为顶点坐标数组分配空间
	if (!mGeoVertices.valid())  mGeoVertices = new osg::Vec3Array;	

	///dbs 为纹理坐标数组分配空间
	if (!mTexCoords.valid())  mTexCoords = new osg::Vec2Array;

	mGeoVertices->resize(vertCount);	
	mTexCoords->resize(vertCount);

	for (int i = 0; i < nStackCount; i++)
	{
		float d0 = i * fDisStep;
		float d1 = dDistance - d0;

		for (int j = 0; j < nSlices; j++)
		{
			(*mGeoVertices)[i * (nSlices + 1) + j] = XYZ0 + N * d0 + (*normalary)[j] * mfRadius[i];
			(*mTexCoords)[i * (nSlices + 1) + j].set(i * tu, 1.0f - float(j) / (nSlices));
		}
		(*mGeoVertices)[i * (nSlices + 1) + nSlices] = XYZ0 + N * d0 + (*normalary)[0] * mfRadius[i];;
		(*mTexCoords)[i * (nSlices + 1) + nSlices].set(i * tu, 0.0f);
	}

	int nIndexCount = 2 * (nStackCount - 1) * nSlices * 3;
	osg::ref_ptr<osg::DrawElementsUInt> _triangles = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	_triangles->resize(nIndexCount);

	for (int i = 0; i < nStackCount - 1; i++)
	{
		for (int j = 0; j < nSlices; j++)
		{
			(*_triangles)[(2*3*i*nSlices) + 6*j] = (i*(nSlices + 1) + j);
			(*_triangles)[(2*3*i*nSlices) + 6*j + 1] = ((i+1)*(nSlices + 1) + j);			
			(*_triangles)[(2*3*i*nSlices) + 6*j + 2] = (i*(nSlices + 1) + (j+1));


			(*_triangles)[(2*3*i*nSlices) + 6*j + 3] = (i*(nSlices + 1) + (j+1));
			(*_triangles)[(2*3*i*nSlices) + 6*j + 4] = ((i+1)*(nSlices + 1) + j);
			(*_triangles)[(2*3*i*nSlices) + 6*j + 5] = ((i+1)*(nSlices + 1) + (j+1));
		}
	}

	///dbs 为几何体颜色数组分配空间并赋值
	if (!mGeoColors.valid())  mGeoColors = new osg::Vec4Array(1);
	(*mGeoColors)[0] = mCurColor;

	osg::ref_ptr<osg::Geometry>   geom = new osg::Geometry;	
	geom->setVertexArray(mGeoVertices.get());
	//geom->setNormalArray(_normals.get());
	//geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);	
	geom->setColorArray(mGeoColors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	if (m_bDynEffect)
	{
		geom->setTexCoordArray(0, mTexCoords);
	}
	geom->addPrimitiveSet(_triangles.get());	
	geom->setUseVertexBufferObjects(true);
	geom->setUseDisplayList(false);
	geom->setDataVariance(osg::Object::DYNAMIC);
	////////////////////////////////////////////////////////////////////

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	///dbs 这里注意
	geode->setCullingActive(false);

	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	//material->setAlpha(osg::Material::FRONT_AND_BACK, 0.8);
	osg::ref_ptr<osg::StateSet> st = new osg::StateSet;

	if (m_bDynEffect)
	{
		///dbs 纹理只是声明一次
		if (!mCommTexImage.valid())
		{
			///dbs 从图象文件中读取纹理数据
			mCommTexImage = osgDB::readImageFile("Images/pipe.tga");
		}

		///dbs  创建纹理并设置纹理属性
		osg::Texture2D* commTexture = new osg::Texture2D;
		commTexture->setImage(mCommTexImage.get());
		commTexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
		commTexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);

		///dbs 如果纹理加载成功则设置纹理属性
		st->setTextureAttributeAndModes(0, commTexture, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		st->setTextureAttribute(0, new osg::TexEnv(osg::TexEnv::MODULATE));

		mTexMatrix = new osg::TexMat;
		//mTexMatrix->setMatrix(osg::Matrix::translate(0, 0, -10000));
		st->setTextureAttributeAndModes(0, mTexMatrix.get(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
		st->setAttributeAndModes(material.get(), true);
	}

	st->setMode(GL_BLEND, true);
	//st->setMode(GL_DEPTH_TEST, false);
	st->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	st->setMode(GL_LINE_SMOOTH, true);
	st->setMode(GL_NORMALIZE, true);
	st->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	st->setRenderBinDetails(9999, "RenderBins");

	geode->setStateSet(st.get());

	osg::Group * backnode= new Group;

	osg::StateSet* stateset = new osg::StateSet;
	stateset->setMode(GL_CULL_FACE, true);
	stateset->setMode(GL_BLEND, true);
	stateset->setAttribute(new osg::CullFace(osg::CullFace::BACK));

	backnode->setStateSet(stateset);
	backnode->addChild(geode);

	osg::Group * frontnode= new Group;

	stateset = new osg::StateSet;
	stateset->setMode(GL_CULL_FACE, true);
	stateset->setAttribute(new osg::CullFace(osg::CullFace::FRONT));

	frontnode->setStateSet(stateset);
	frontnode->addChild(geode);

	osg::ref_ptr<osg::Group> modelnode= new Group;

	modelnode->addChild(backnode);
	modelnode->addChild(frontnode);

	stateset = new osg::StateSet;
	stateset->setMode(GL_BLEND, true);
	stateset->setMode(GL_DEPTH_TEST, true);

	modelnode->setStateSet(stateset);

	return modelnode.release();
}

void CCommEffect::updatePipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1)
{
	int nStackCount = 2; //长度分段
	int nSlices = 50; //圆周分段
	osg::Vec3 N = XYZ1 - XYZ0;
	float dDistance = N.length();
	N.normalize();

	float tu = dDistance / mfLength;

	float fDisStep = dDistance / (nStackCount - 1);

	osg::Vec3 A = N ^ XYZ0;
	A.normalize();

	float stepangle = 2 * osg::PI / (nSlices);
	float mat[9];

	osg::ref_ptr<osg::Vec3Array> normalary = new osg::Vec3Array;
	normalary->resize(nSlices);
	osg::Vec3 temp;
	for (int i = 0; i < nSlices; i++)
	{
		float angle = i * stepangle;
		float cosangle = cos(angle);
		float mincosangle = 1 - cosangle;
		float sinangle = sin(angle);

		mat[0] = cosangle + mincosangle * N.x() * N.x();
		mat[1] = mincosangle * N.x() * N.y() - N.z() * sinangle;
		mat[2] = mincosangle * N.x() * N.z() + N.y() * sinangle;
		mat[3] = mincosangle * N.x() * N.y() + N.z() * sinangle;
		mat[4] = cosangle + mincosangle * N.y() * N.y();
		mat[5] = mincosangle * N.y() * N.z() - N.x() * sinangle;
		mat[6] = mincosangle * N.x() * N.z() - N.y() * sinangle;
		mat[7] = mincosangle * N.y() * N.z() + N.x() * sinangle;
		mat[8] = cosangle + mincosangle * N.z() * N.z();

		temp.x() = mat[0] * A.x() + mat[1] * A.y() + mat[2] * A.z();
		temp.y() = mat[3] * A.x() + mat[4] * A.y() + mat[5] * A.z();
		temp.z() = mat[6] * A.x() + mat[7] * A.y() + mat[8] * A.z();

		temp.normalize();

		(*normalary)[i] = temp;
	}

	int vertCount = (nSlices + 1) * nStackCount;	

	for (int i = 0; i < nStackCount; i++)
	{
		float d0 = i * fDisStep;
		float d1 = dDistance - d0;
		for (int j = 0; j < nSlices; j++)
		{
			(*mGeoVertices)[i * (nSlices + 1) + j] = XYZ0 + N * d0 + (*normalary)[j] * mfRadius[i];
			(*mTexCoords)[i * (nSlices + 1) + j].set(i * tu, 1.0f - float(j) / (nSlices));
		}
		(*mGeoVertices)[i * (nSlices + 1) + nSlices] = XYZ0 + N * d0 + (*normalary)[0] * mfRadius[i];;
		(*mTexCoords)[i * (nSlices + 1) + nSlices].set(i * tu, 0.0f);
	}
	mGeoVertices->dirty();
}

void CCommEffect::traverse(osg::NodeVisitor& nv)
{
	osg::NodeVisitor::VisitorType vt = nv.getVisitorType();
	if (vt == osg::NodeVisitor::UPDATE_VISITOR)
	{
		///dbs 判断是否需要重新生成管道几何结构
		if (m_bNeedUpdate)
		{
			updatePipe(mVecPos1, mVecPos2);
			m_bNeedUpdate = false;
		}
		///dbs 注意这里和前段代码的顺序
		const osg::FrameStamp* f = nv.getFrameStamp();
		float dt = f->getSimulationTime();

		if (m_bDynEffect && !m_bStop)
		{
			float ss = fmod(dt * mfSpeed, 1.0f);
			mTexMatrix->setMatrix(osg::Matrix::translate(-ss,0, 0));
		}
	}

	osg::Group::traverse(nv);
}