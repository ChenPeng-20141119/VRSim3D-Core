#include "ControlBar.h"

using namespace VRSim3D;

ControlBar::ControlBar(osgViewer::Viewer* view)
{
	if(!view)
		return;
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> canvas =  
		osgEarth::Util::Controls::ControlCanvas::get(view);
	m_isDragger = true;
	m_Control = new osgEarth::Util::Controls::VBox();
	m_Control->setPosition(10, 10);
	m_Control->setPadding(10);
	m_Control->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
	m_Control->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	m_Control->setNodeMask(NM_GUI_MANAGER);
	canvas->addControl(m_Control.get());

}

ControlBar::~ControlBar(void)
{
}

void ControlBar::addControlInfor( const std::string &infor )
{
	m_tile = new osgEarth::Util::Controls::LabelControl();
	//title->setFont(m_AllFont.get());
	m_tile->setFontSize(18);
	m_tile->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);    
	m_tile->setText(infor+":");
	m_tile->setForeColor(1, 0, 0, 0.8);
	m_tile->setNodeMask(NM_GUI_MANAGER);
	m_Control->addControl(m_tile.get());
}

void ControlBar::addSlider( const float &min,const float &max )
{
	osg::ref_ptr<osgEarth::Util::Controls::HBox> hbox  = new osgEarth::Util::Controls::HBox();
	hbox->setChildSpacing( 10 );

	///zqgao 添加滑块
	m_slider = new osgEarth::Util::Controls::HSliderControl(min, max,min);
	m_slider->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	m_slider->setBackColor(0, 0.9, 0.9, 0.8);
	m_slider->setHorizFill( true);
	m_slider->setHeight(20);
	m_slider->setWidth(300);
	m_slider->setNodeMask(NM_GUI_MANAGER);
	hbox->addControl(m_slider.get());

	///zqgao 添加信息条
	osg::ref_ptr<osgEarth::Util::Controls::LabelControl>label= new  osgEarth::Util::Controls::LabelControl();
	label->setFontSize(18);
	label->setVertAlign( osgEarth::Util::Controls::Control::ALIGN_CENTER );
	//label->setHorizAlign( osgEarth::Util::Controls::Control::ALIGN_LEFT);    
	label->setText("label");
	label->setForeColor(1, 0, 0, 0.8);
	label->setNodeMask(NM_GUI_MANAGER);
	hbox->addControl(label.get());

	m_slider->addEventHandler(new SliderHandler(this,label.get()));
	m_Control->addControl(hbox.get());
}

void ControlBar::setControlName( const std::string &name )
{
	m_tile->setText(name + ":");
}

void ControlBar::setSliderRange( const float &min,const float &max,const float &now )
{
	m_slider->setMin(min);
	m_slider->setMax(max);
	m_slider->setValue(now);
	m_isDragger = false;
}

void ControlBar::showControl( const bool &isShow )
{
	if(isShow != m_Control->visible())
		m_Control->setVisible(isShow);
}

void ControlBar::setSliderValue( const float &now )
{
	m_slider->setValue(now);
	m_isDragger = false;
}