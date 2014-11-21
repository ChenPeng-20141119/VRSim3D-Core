#ifndef SATELLITEGROUPMANAGER_H
#define SATELLITEGROUPMANAGER_H

#include "Export.h"
#include <string>
#include <map>
#include <osg/Node>
#include <osg/Group>
#include "SatelliteAnimation.h"

namespace VRSim3D
{
	class VRSim3D_EXPORT SatelliteGroupManager
	{
	public:
		SatelliteGroupManager();
		~SatelliteGroupManager();
		//name : ģ�͵�����
		//satellitePath�����ǵ�����·���ļ�
		//satelliteName : ���ǵ�ģ���ļ�·��
		bool addNode(std::string name,std::string satellitePath,
			std::string satelliteName,bool useOld,std::vector<int> cutPositions);
		osg::Node *getNode(std::string name);
		osg::Node *getLineNode(std::string name);
		osg::Group * getGroup();
		std::map<std::string,SatelliteAnimation*> GetSatelliteAnimations();
	private:
		osg::ref_ptr<osg::Group> m_group;
		osg::ref_ptr<osg::Group> m_lines;
		std::map<std::string,osg::Node*> m_groupManager;
		std::map<std::string,osg::Node*> m_linesManager;
		std::map<std::string,SatelliteAnimation*> m_satelliteAnimations;
	};
}

#endif