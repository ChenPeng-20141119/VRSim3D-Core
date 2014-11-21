//////////////////////////////////////////////////////////////////////////
///文件名：控制条类
///创建者：高志清
///创建时间：2014年4月20日
///内容描述：提供屏幕控制条，可用于进度条，可控制拖动条等
//////////////////////////////////////////////////////////////////////////
#ifndef CONTROLBAR_H
#define CONTROLBAR_H 1

#include "osgViewer/Viewer"
#include "osgEarthUtil/Controls"
#include <osgEarth/StringUtils>
#include <AnimationPathPlayBackCallback.h>

#include "Signal.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT ControlBar :
		public osg::Referenced
	{
	public:
		ControlBar(osgViewer::Viewer* view);
		~ControlBar(void);

		struct SliderHandler : public osgEarth::Util::Controls::ControlEventHandler
		{
			SliderHandler(ControlBar* contrBar,osgEarth::Util::Controls::LabelControl* lable) : m_lable(lable) ,m_contrBar(contrBar) { }

			osgEarth::Util::Controls::LabelControl* m_lable;
			osg::ref_ptr<ControlBar> m_contrBar;
			void onValueChanged( osgEarth::Util::Controls::Control* control, float value )
			{
				char buffer[128];	  
				sprintf(buffer,"%6.3f",value);
				std::string strnum(buffer);

				m_lable->setText(strnum);

				///zqgao 发送事件
				if(m_contrBar->getIsDragger())
					m_contrBar->m_DraggerEvent(value);
				//delete buffer;
			}
		};
		///zqgao 添加LabelControl用于显示提示信息
		void addControlInfor(const std::string &infor);

		///zqgao 添加滑块用于显示当前的进度信息
		void addSlider(const float &min,const float &max);

		///zqgao 设置滑块显隐
		void showControl(const bool &isShow);
		///zqgao 设置滑块名称
		void setControlName(const std::string &name);
		///zqgao 设置滑块数值范围
		void setSliderRange(const float &min,const float &max,const float &now);

		void setSliderValue(const float &now);

		void setIsDragger(bool dragger){m_isDragger = dragger;}
		bool getIsDragger(){return m_isDragger;}
	public:
		///zqgao 拖动滑块事件
		Signal<float> m_DraggerEvent;
	private:
		///zqgao 控制条容器
		osg::ref_ptr<osgEarth::Util::Controls::VBox>     m_Control;
		///zqgao 控制条名称
		osg::ref_ptr<osgEarth::Util::Controls::LabelControl> m_tile;
		///zqgao 滑块容器
		osg::ref_ptr<osgEarth::Util::Controls::HSliderControl> m_slider;
		///是否是由于拖动导致数值更改
		bool  m_isDragger;
	};

	class VRSim3D_EXPORT ConBarCallback : public osg::NodeCallback
	{
	public:
		ConBarCallback(osg::Node *nowPickNode,ControlBar *conBar)
		{
			m_NowPickNode = nowPickNode;
			m_conBar = conBar;
			m_isUpdate = true;
		}

		void setNowPickNode(osg::Node *nowPickNode)
		{
			m_NowPickNode = nowPickNode;
		}

		void setUpdate(const bool &isupdate)
		{
			if(isupdate != m_isUpdate)
				m_isUpdate = isupdate;
		}

		virtual void operator()(osg::Node *node,osg::NodeVisitor *nv)
		{
			if(m_isUpdate && m_NowPickNode != NULL && m_conBar != NULL &&
				nv->getVisitorType()==osg::NodeVisitor::UPDATE_VISITOR &&
				nv->getFrameStamp())
			{
				osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(m_NowPickNode.get());
				if(mt == NULL) return;
				osg::NodeCallback *callback = mt->getUpdateCallback();
				if(callback == NULL) return;
				AnimationPathPlayBackCallback *pbCallback = 
					dynamic_cast<AnimationPathPlayBackCallback *>(callback);
				if(pbCallback == NULL) return;
				double nowTime = pbCallback->getAnimationTime();
				double maxTime = pbCallback->getMaxTime();
				double value = nowTime;
				if(nowTime > maxTime)
				{
					int nowInt = floor(nowTime);
					double nowDouble = nowTime - nowInt;

					int maxInt = floor(maxTime);
					double maxDouble = maxTime - maxInt;
					value = nowInt % maxInt + nowDouble - maxDouble;
				}
				m_conBar->setSliderValue(value);
			}
			NodeCallback::traverse(node,nv);
		}
	private:
		osg::ref_ptr<osg::Node> m_NowPickNode;
		ControlBar *m_conBar;
		bool		m_isUpdate;
	};
}
#endif
