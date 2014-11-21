#ifndef MOUSE_COORDS_HANDLER_H
#define MOUSE_COORDS_HANDLER_H 1

#include "Export.h"
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/LatLongFormatter>
#include <osgDB/ConvertUTF>

using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
namespace VRSim3D
{

///dbs 实现鼠标当前位置
class VRSim3D_EXPORT MouseCoordsHandler : public osgGA::GUIEventHandler
{
public:
	MouseCoordsHandler(LabelControl* label, const osgEarth::MapNode* mapNode)
		: _label(label), _mapNode(mapNode)
	{

	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::View> view = static_cast<osgViewer::View*>(aa.asView());
		///dbs case 1: 鼠标移动时，实时显示当前经纬度坐标
		if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG)
		{
			osgUtil::LineSegmentIntersector::Intersections results;
			if (view->computeIntersections(ea.getX(), ea.getY(), results, NM_YES_REC_INTER))
			{
				///dbs 选择鼠标拾取队列中的第一个交点
				osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
				osg::Vec3d worldPoint = first.getWorldIntersectPoint();

				///dbs 将当前所在点的空间坐标转化为经纬度坐标
				GeoPoint mapPoint;
				mapPoint.fromWorld(_mapNode->getMapSRS(), worldPoint);

				std::stringstream ss;
				///dbs 采用小数形式或者度分表形式
				LatLongFormatter::AngularFormat fFormat = LatLongFormatter::FORMAT_DECIMAL_DEGREES; //LatLongFormatter::FORMAT_DEGREES_MINUTES_SECONDS; ///
				LatLongFormatter f(fFormat);
				///dbs 输出显示的文字提示信息
				ss << "纬度: " << f.format(Angular(mapPoint.vec3d().y(), Units::DEGREES), 10) << "°"
				   << "经度: " << f.format(Angular(mapPoint.vec3d().x(), Units::DEGREES), 11) << "°"
				   << "高程: " << mapPoint.vec3d().z() << "m";
				_label->setText(osgDB::convertStringFromCurrentCodePageToUTF8(ss.str()));
			}
			else
			{
				///dbs 清空文字显示的内容
				_label->setText("");
			}
		}
		return false;
	}

private:
	mutable osg::ref_ptr<LabelControl> _label;
	const	MapNode*      _mapNode;
};
}
#endif
