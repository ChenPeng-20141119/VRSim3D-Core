//////////////////////////////////////////////////////////////////////////
///文件名:模型拣选器
///创建者：高志清
///创建时间：2014年4月25日
///内容描述：模型拣选器类，实现模型拣选，返回拣选事件
//////////////////////////////////////////////////////////////////////////
#ifndef MODELPICKER_H
#define MODELPICKER_H 1

#include "osgGA/GUIEventHandler"

#include "Signal.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT ModelPicker :
		public osgGA::GUIEventHandler
	{
	public:
		ModelPicker();
		~ModelPicker(void);
		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
		int getNowMask(){return m_mask;}
	public:
		///zqgao 拣选事件
		Signal<std::string> m_PickEvent;
		///zqgao 点击完成事件
		Signal<int>m_PickOverEvent;
	private:
		///zqgao 节点标示，当前拣选到节点的标示
		int m_mask;
	};
};

#endif