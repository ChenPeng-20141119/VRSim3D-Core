#include "NodeInfoDrawCallback.h"

VRSim3D::NodeInfoDrawCallback::NodeInfoDrawCallback( osgEarth::MapNode *globeNode,osg::Node* node )
{
	_globeNode = globeNode;
	_node = node;
}

void VRSim3D::NodeInfoDrawCallback::drawImplementation( osg::RenderInfo& renderInfo, const osg::Drawable* drawable ) const
{
	osgText::Text* text = (osgText::Text*)drawable;
	if(_node)
	{
		std::string name = _node->getName();
		std::wstring wname = UtileMethod::StrToWStr(name);

		osg::Vec3 center = _node->getBound().center();
		double lat,lon,el;
		_globeNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(center.x(),center.y(),center.z(),lat,lon,el);

		char* s = new char[256];
		sprintf(s,"星下点 纬度:%6.2f°  经度:%6.2f°",osg::RadiansToDegrees(lat), osg::RadiansToDegrees(lon));
		std::string str(s);
		std::wstring ws = UtileMethod::StrToWStr(str);
		delete s;

		std::wstring wstr = wname + L"\n" +ws;
		text->setText(wstr.c_str());
	}else
	{
		text->setText("");
	}
	///dbs 实现字体的重绘
	text->drawImplementation(renderInfo);
}

void VRSim3D::NodeInfoDrawCallback::setIfoNode( osg::Node* node )
{
	_node = node;
}
