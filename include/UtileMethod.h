//////////////////////////////////////////////////////////////////////////
///文件名：静态方法类
///创建者：高志清
///创建时间：2012年11月15日
///内容描述：提供一些常用且比较独立函数
//////////////////////////////////////////////////////////////////////////

#ifndef UTILEMETHOD_H
#define UTILEMETHOD_H 1

#include "Export.h"

#include "osg/Group"
#include "osgGA/KeySwitchMatrixManipulator"

#include <string>

namespace VRSim3D {

	class VRSim3D_EXPORT UtileMethod
	{
	public:
		UtileMethod(void);
		~UtileMethod(void);
	public:
		// zqgao string to wstring
		static std::wstring StrToWStr(std::string pszStr);
		// zqgao wstring to string
		static std::string WStrToStr(std::wstring pszWstr);
		static std::string GBKToUTF8(const std::string &strGBK);
		static std::string UTF8ToGBK(const std::string &strUTF8);
		// zqgao find Node
		static void findChild(osg::Group* parent,std::string childName,osg::Node** findNode);
		//int to string
		static std::string IntToStr(int num);
		// HUD相机
		static void HudCamera(osg::Camera*hUDCamera, float x, float y);
		///zqgao 获取操控器在KeySwitchMatrixManipulator位置
		static int getMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);

		/*///zqgao 在KeySwitchMatrixManipulator中获取操控器
		static int getMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);*/

		///zqgao KeySwitchMatrixManipulator内置操控器
		static bool RemoveMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);

		///zqgao 转移子节点
		static void transferChilds(osg::Group *oldGroup,osg::Group *newGroup); 
		// 经纬度转世界坐标
		/*static osg::Vec3d */

		///zqgao 是否为高亮模型
		static bool isHighLight(osg::Node* node);
		///zqgao 模型高亮
		static void highLight(osg::Node* node);
		///zqgao 去除模型高亮
		static bool wipeHighLight(osg::Node* node);
	};
}
#endif
