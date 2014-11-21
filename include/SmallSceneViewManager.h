#ifndef SMALL_SCENE_VIEW_MANAGER_H
#define SMALL_SCENE_VIEW_MANAGER_H 1

#include "Export.h"
#include "sceneviewmanager.h"

namespace VRSim3D
{

class SmallSceneViewManager :
	public VRSim3D::SceneViewManager
{
public:
	SmallSceneViewManager(void);
	~SmallSceneViewManager(void);
};

}
#endif
