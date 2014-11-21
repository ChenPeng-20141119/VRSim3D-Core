#ifndef COMPOSITE_SCENE_VIEW_BUILLDER_H
#define COMPOSITE_SCENE_VIEW_BUILLDER_H 1

#include "Export.h"
#include "sceneviewbuilder.h"

namespace VRSim3D
{

class VRSim3D CompositeSceneViewBuilder :
	public VRSim3D::SceneViewBuilder
{
public:
	CompositeSceneViewBuilder(void);
	~CompositeSceneViewBuilder(void);
};

}
#endif
