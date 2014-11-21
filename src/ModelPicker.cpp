#include "ModelPicker.h"
#include "osgViewer/View"
#include "osg/MatrixTransform"

using namespace VRSim3D;

ModelPicker::ModelPicker()
{
	m_mask = 0;
}

ModelPicker::~ModelPicker(void)
{
}

bool ModelPicker::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osg::ref_ptr<osgViewer::View> view = static_cast<osgViewer::View*>(aa.asView());
	if (!view) return false;
	std::string name = "";
	osgUtil::LineSegmentIntersector::Intersections results;
	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH :
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{


				if (view->computeIntersections(ea.getX(), ea.getY(), results, NM_YES_QUARYABLE))
				{
					osgUtil::LineSegmentIntersector::Intersection intersection = *results.begin();
					osg::NodePath& nodePath = intersection.nodePath;
					osg::MatrixTransform* mt = NULL;
					for (size_t i = 0;i<nodePath.size();++i)
					{
						mt = dynamic_cast<osg::MatrixTransform*>(nodePath[i]);
						if(mt)
						{
							name = mt->getName();
							break;
						}
					}
					m_mask = NM_YES_QUARYABLE;
				}else if (view->computeIntersections(ea.getX(), ea.getY(), results,NM_GUI_MANAGER ))
				{
					m_mask = NM_GUI_MANAGER;
				}else if (view->computeIntersections(ea.getX(), ea.getY(), results, NM_YES_REC_INTER))
				{
					m_mask = NM_YES_REC_INTER;
				}
				m_PickEvent(name);

			}
			break;
		}
	case osgGA::GUIEventAdapter::RELEASE:
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if (view->computeIntersections(ea.getX(), ea.getY(), results,NM_GUI_MANAGER ))
				{
					m_mask = NM_GUI_MANAGER;
					m_PickOverEvent(NM_GUI_MANAGER);
				}
			}
			break;
		}
	default:
		break;
	}
	return false;
}