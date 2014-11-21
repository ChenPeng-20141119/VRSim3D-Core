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
		///zqgao 用世界坐标添加
		DynamicModel(const osg::Vec3f &pos,const std::string &filePath);
		///zqgao 用经纬度坐标添加
		DynamicModel(osgEarth::MapNode* globeNode,const float &lat,const float &lon,const float &ele,const std::string &filePath);
		~DynamicModel(void);
	protected:
		///zqgao 加载模型
		osg::Node* LoadModel(const std::string &filePath);
		///zqgao 初始化
		void init();
		///zqgao 遍历时进行调用
		virtual void traverse(osg::NodeVisitor& nv);
	public:
		///zqgao 设置坐标
		void setPosition(const osg::Vec3f &pos);
		void setPosition(const float &lat,const float &lon,const float &ele,bool parallel = true);
		///zqgao 获取坐标
		osg::Vec3f getPosition(){return m_Position;}
		///设置模型文件路径
		void setFilePath(const std::string &filePath);
		///zqgao 获取模型文件路径
		std::string getFilePath(){return m_FilePath;}
		///zqgao 添加信号
		void addCommEffect(osg::Vec3f start,osg::Vec3f end,float speed,float length,osg::Vec4f color,const std::string &movePoint = "NoMove");
		///zqgao 设置缩放
		void setScale(const osg::Vec3f &scale);
		///zqgao 设置旋转
		void setRotation(const osg::Quat &quat);
		///zqgao 开启信号跟随
		void OpenCftFollow();
		osg::ref_ptr<osg::MatrixTransform> m_dyGroup;
	private:
		///zqgao 球节点
		osg::ref_ptr<osgEarth::MapNode> m_GlobeNode;
		///zqgao 世界坐标
		osg::Vec3f						m_Position;
		///zqgao 模型坐标
		std::string						m_FilePath;
		///zqgao 模拟信号集
		std::map<CEFTATT*,CCommEffect*>  m_CommEffectMap;
		///zqgao 信号结束点
		osg::Vec3						m_CommEnd;
		///zqgao 信号是否跟随
		bool							m_CftFollow;
	};
}

#endif