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
		///dbs ���캯��������xyz �˵�뾶���������� bDyn�Ƿ��в���Ч��
		CCommEffect(const osg::Vec3d &vec1, float r1, const osg::Vec3d &vec2, float r2, bool bDyn = true);
		~CCommEffect(void);

	protected:
		///dbs ���������Ĺܵ�Ч��
		osg::Node* createPipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
		///dbs ��̬�Բ���Ч�����и���
		void updatePipe(const osg::Vec3d& XYZ0, const osg::Vec3d& XYZ1);
		///dbs ����ʱ���е���
		virtual void traverse(osg::NodeVisitor& nv);

	public:
		///dbs ���õ�����Բģ�ͽڵ㣬���ڽ��пռ�����;�γ�������ת��
		void setEarthModel(osgEarth::MapNode* pEarthModel);

		///dbs �ڶ�̬����λ��ʱ���ã�������ֹ��λ��xyz����λ��
		void setXYZPosition(const osg::Vec3d &vec1, const osg::Vec3d &vec2);
		///dbs �ڶ�̬����λ��ʱ���ã�������ֹ��λ��lla��ά�Ⱦ��ȸ߶�
		void setLLAPosition(const osg::Vec3d &lla1, const osg::Vec3d &lla2);

		///dbs ����ͨ�Ų��ε��ƶ��ٶȣ���λ����
		void setSpeed(float s);
		///dbs ���ò����ĳ��ȣ���λ����
		void setLength(float l); 
		///dbs ���ò��������˵㴦�İ뾶
		void setRadius(float r0, float r1);
		///dbs ����\��ȡ��������ɫ������͸����
		void setColor(osg::Vec4 vec);
		osg::Vec4& getColor();
		///dbs ���ò����Ƿ���ͣ������������
		void setDynStop(bool bStop);

	public:
		bool m_bDynEffect, m_bStop;

		///dbs ��ʶͨ��Ч�����ƶ��ٶ�
		float mfSpeed, mfLength;

		///dbs ͨ��Ч�����˵İ뾶
		float mfRadius[2];
		///dbs ͨ��Ч���������յ�
		osg::Vec3d mVecPos1, mVecPos2;

		///dbs �����������
		osg::ref_ptr<osg::TexMat>  mTexMatrix;
		///dbs �����嶥������
		osg::ref_ptr<osg::Vec3Array> mGeoVertices;
		///dbs ������������
		osg::ref_ptr<osg::Vec2Array> mTexCoords;
		///dbs ��ǰ���õ���ɫ���Լ���������ɫ����ָ��
		osg::Vec4 mCurColor;
		osg::ref_ptr<osg::Vec4Array> mGeoColors;
		///dbs ��ʶ�Ƿ���Ҫ�������ɹܵ����νṹ
		bool m_bNeedUpdate;

	protected:
		///dbs ����ģ�͵�ָ��
		osg::ref_ptr<osgEarth::MapNode>  mEarthModel;

		///dbs ��Ч����Ҫ����������
		static osg::ref_ptr<osg::Image>  mCommTexImage;
	};
}

#endif