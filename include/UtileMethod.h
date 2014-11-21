//////////////////////////////////////////////////////////////////////////
///�ļ�������̬������
///�����ߣ���־��
///����ʱ�䣺2012��11��15��
///�����������ṩһЩ�����ұȽ϶�������
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
		// HUD���
		static void HudCamera(osg::Camera*hUDCamera, float x, float y);
		///zqgao ��ȡ�ٿ�����KeySwitchMatrixManipulatorλ��
		static int getMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);

		/*///zqgao ��KeySwitchMatrixManipulator�л�ȡ�ٿ���
		static int getMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);*/

		///zqgao KeySwitchMatrixManipulator���òٿ���
		static bool RemoveMPIndexFromSwichMP(osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name);

		///zqgao ת���ӽڵ�
		static void transferChilds(osg::Group *oldGroup,osg::Group *newGroup); 
		// ��γ��ת��������
		/*static osg::Vec3d */

		///zqgao �Ƿ�Ϊ����ģ��
		static bool isHighLight(osg::Node* node);
		///zqgao ģ�͸���
		static void highLight(osg::Node* node);
		///zqgao ȥ��ģ�͸���
		static bool wipeHighLight(osg::Node* node);
	};
}
#endif
