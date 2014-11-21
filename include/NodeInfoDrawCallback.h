#ifndef NODEINFODRAWCALLBACK_H
#define NODEINFODRAWCALLBACK_H

#include "Export.h"
#include "osg/Drawable"
#include "osgEarth/MapNode"
#include "osgText/Text"
#include "UtileMethod.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT NodeInfoDrawCallback : public virtual osg::Drawable::DrawCallback
	{
	public:
		NodeInfoDrawCallback(osgEarth::MapNode *globeNode,osg::Node* node);

		/** do customized draw code.*/
		virtual void drawImplementation(osg::RenderInfo& renderInfo, const osg::Drawable* drawable) const;

		void setIfoNode(osg::Node* node);
	private:
		osg::ref_ptr<osgEarth::MapNode>    _globeNode;
		osg::ref_ptr<osg::Node>					_node;	
	};
}

#endif