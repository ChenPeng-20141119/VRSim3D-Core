//////////////////////////////////////////////////////////////////////////
///�ļ�������Ч������
///�����ߣ���־��
///����ʱ�䣺2014��10��26��
///��������������Ч����������
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
	///zqgao �Ӿ��嶥�����꣬�·����ĵ����꣬�·��뾶�����α߳������ں���ӿ������ø��ģ�����ɫ
	RectEffect(osgEarth::MapNode* earthModel,osg::Vec3 top, osg::Vec3 center, double r,osg::Vec4 tColor);
	~RectEffect(void);

protected:
	///zqgao ���������Ĺܵ�Ч��
	virtual void createBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
	///zqgao ��̬�Բ���Ч�����и���
	virtual void updateBeam(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);

public:
	///zqgao ��������ɨ��Ч�����Ž�
	void setEffectAngle(double dAngle);
	///zqgao ��ȡ����ɨ��Ч�����Ž�
	double getEffectAngle();
	///zqgao ���ò��������˵㴦�İ뾶
	void setEffectRadius(double r);
	///zqgao ��ȡ���������˵㴦�İ뾶
	double getEffectRadius();
	///zqgao ��������ɨ��Ч���ĸ߶�
	void setEffectHeight(double h);
	///zqgao ��ȡ����ɨ��Ч���ĸ߶�
	double getEffectHeight();
	///zqgao ����ɨ��Ч�����̵ĺ��ݱ�
	void setAspectRatio(double ratio);
	///zqgao ��ȡɨ��Ч�����̵ĺ��ݱ�
	double getAspectRatio(double ratio);

	///zqgao �ڶ�̬����λ��ʱ���ã�������ֹ��λ��xyz����λ��
	void setXYZPosition(const osg::Vec3d &vec0, const osg::Vec3d &vec1);
	///zqgao �ڶ�̬����λ��ʱ���ã����ù�����λ�ò�����������Զ�����
	void setOneXYZPosition(const osg::Vec3d &vec0);

	///zqgao ����̽��Ч���Ƿ����
	void setEffectBodyVisible(bool bSet);
	
	///zqgao ����\��ȡ��������ɫ������͸����
	void setColor(osg::Vec4 vec);
	osg::Vec4& getColor();
	///zqgao �����������ĳ��ȣ���λ����
	void setWaveLength(float l); 
	float getWaveLength();
	///zqgao ���ù��������ƶ��ٶȣ���λ����
	void setTexSpeed(float s);
	float getTexSpeed();
	///zqgao ���ò�����̬����Ч��
	void setUseDynTex(bool bDynTex);
	bool getUesDynTex();

public:
	///zqgao ����ʱ���е���
	virtual void traverse(osg::NodeVisitor& nv);
	/*///zqgao ����ģ�͵�ָ��
	osg::ref_ptr<osg::EllipsoidModel>  mEarthModel;*/

protected:
	///zqgao ����Ч��������ƶ��ٶȣ�����Ч������Ĳ���Ч��
	float mfTexSpeed, mfWaveLength;

	///zqgao ɨ��Ч���ĵ׶˰뾶�����������
	double mfEffectRadius;
	///zqgao ɨ��Ч������Ը߶ȣ����Եؾ���
	double mdEffectHeight;
	///zqgao ɨ��Ч�����Žǣ�ͨ���߶Ⱥ��Žǿ��Լ�����׶˰뾶
	double mdEffectAngle;
	///zqgao ɨ��Ч�����̵ĺ��ݱ�
	double mdAspectRatio;
	///zqgao ɨ��Ч���������յ㣬������뾶�͸߶����໥��Լ��
	osg::Vec3d mEffectVec0, mEffectVec1;

	///zqgao �����������
	osg::ref_ptr<osg::TexMat>  mEffectTexMatrix;
	///zqgao �����嶥������
	osg::ref_ptr<osg::Vec3dArray> mGeoVertices;
	///zqgao ������������
	osg::ref_ptr<osg::Vec2Array> mTexCoords;
	osg::ref_ptr<osgEarth::MapNode>  mEarthModel;

	///zqgao ��ǰ���õ���ɫ���Լ���������ɫ����ָ��
	osg::Vec4 mCurColor;
	osg::ref_ptr<osg::Vec4Array> mGeoColors;


protected:

	///zqgao ʹ�ö�̬����Ч��
	bool m_bUseDynTex;
	///zqgao ����������������Ϊ��̬����
	static osg::ref_ptr<osg::Image>  mEffectTexImage;

	///zqgao ��ʶ�Ƿ���Ҫ�������ɹܵ����νṹ
	bool m_bNeedUpdateBeam;
};
}
#endif

