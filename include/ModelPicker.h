//////////////////////////////////////////////////////////////////////////
///�ļ���:ģ�ͼ�ѡ��
///�����ߣ���־��
///����ʱ�䣺2014��4��25��
///����������ģ�ͼ�ѡ���࣬ʵ��ģ�ͼ�ѡ�����ؼ�ѡ�¼�
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
		///zqgao ��ѡ�¼�
		Signal<std::string> m_PickEvent;
		///zqgao �������¼�
		Signal<int>m_PickOverEvent;
	private:
		///zqgao �ڵ��ʾ����ǰ��ѡ���ڵ�ı�ʾ
		int m_mask;
	};
};

#endif