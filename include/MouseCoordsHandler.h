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

///dbs ʵ����굱ǰλ��
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
		///dbs case 1: ����ƶ�ʱ��ʵʱ��ʾ��ǰ��γ������
		if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG)
		{
			osgUtil::LineSegmentIntersector::Intersections results;
			if (view->computeIntersections(ea.getX(), ea.getY(), results, NM_YES_REC_INTER))
			{
				///dbs ѡ�����ʰȡ�����еĵ�һ������
				osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
				osg::Vec3d worldPoint = first.getWorldIntersectPoint();

				///dbs ����ǰ���ڵ�Ŀռ�����ת��Ϊ��γ������
				GeoPoint mapPoint;
				mapPoint.fromWorld(_mapNode->getMapSRS(), worldPoint);

				std::stringstream ss;
				///dbs ����С����ʽ���߶ȷֱ���ʽ
				LatLongFormatter::AngularFormat fFormat = LatLongFormatter::FORMAT_DECIMAL_DEGREES; //LatLongFormatter::FORMAT_DEGREES_MINUTES_SECONDS; ///
				LatLongFormatter f(fFormat);
				///dbs �����ʾ��������ʾ��Ϣ
				ss << "γ��: " << f.format(Angular(mapPoint.vec3d().y(), Units::DEGREES), 10) << "��"
				   << "����: " << f.format(Angular(mapPoint.vec3d().x(), Units::DEGREES), 11) << "��"
				   << "�߳�: " << mapPoint.vec3d().z() << "m";
				_label->setText(osgDB::convertStringFromCurrentCodePageToUTF8(ss.str()));
			}
			else
			{
				///dbs ���������ʾ������
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
