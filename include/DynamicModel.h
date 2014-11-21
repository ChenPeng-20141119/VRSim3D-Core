#ifndef DYNAMICMODEL_H
#define DYNAMICMODEL_H 1

#include "Export.h"
#include "map"
#include "osg/MatrixTransform"
#include "osgEarth/MapNode"
#include "CommEffect.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT DynamicModel :
		public osg::MatrixTransform
	{
	public:
		struct CEFTATT
		{
			std::string movePoint;
			osg::Vec3d start;
			osg::Vec3d end;
		};
		DynamicModel(void);
		///zqgao �������������
		DynamicModel(const osg::Vec3f &pos,const std::string &filePath);
		///zqgao �þ�γ���������
		DynamicModel(osgEarth::MapNode* globeNode,const float &lat,const float &lon,const float &ele,const std::string &filePath);
		~DynamicModel(void);
	protected:
		///zqgao ����ģ��
		osg::Node* LoadModel(const std::string &filePath);
		///zqgao ��ʼ��
		void init();
		///zqgao ����ʱ���е���
		virtual void traverse(osg::NodeVisitor& nv);
	public:
		///zqgao ��������
		void setPosition(const osg::Vec3f &pos);
		void setPosition(const float &lat,const float &lon,const float &ele,bool parallel = true);
		///zqgao ��ȡ����
		osg::Vec3f getPosition(){return m_Position;}
		///����ģ���ļ�·��
		void setFilePath(const std::string &filePath);
		///zqgao ��ȡģ���ļ�·��
		std::string getFilePath(){return m_FilePath;}
		///zqgao ����ź�
		void addCommEffect(osg::Vec3f start,osg::Vec3f end,float speed,float length,osg::Vec4f color,const std::string &movePoint = "NoMove");
		///zqgao ��������
		void setScale(const osg::Vec3f &scale);
		///zqgao ������ת
		void setRotation(const osg::Quat &quat);
		///zqgao �����źŸ���
		void OpenCftFollow();
		osg::ref_ptr<osg::MatrixTransform> m_dyGroup;
	private:
		///zqgao ��ڵ�
		osg::ref_ptr<osgEarth::MapNode> m_GlobeNode;
		///zqgao ��������
		osg::Vec3f						m_Position;
		///zqgao ģ������
		std::string						m_FilePath;
		///zqgao ģ���źż�
		std::map<CEFTATT*,CCommEffect*>  m_CommEffectMap;
		///zqgao �źŽ�����
		osg::Vec3						m_CommEnd;
		///zqgao �ź��Ƿ����
		bool							m_CftFollow;
	};
}

#endif