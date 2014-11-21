#include "SatelliteGroupManager.h"

using namespace VRSim3D;

SatelliteGroupManager::SatelliteGroupManager()
{
	m_group = new osg::Group;
	m_lines = new osg::Group;
}

SatelliteGroupManager::~SatelliteGroupManager()
{
	m_groupManager.clear();
	m_linesManager.clear();
}

bool SatelliteGroupManager::addNode( std::string name,std::string satellitePath,
	std::string satelliteName,bool useOld,std::vector<int> cutPositions )
{
	SatelliteAnimation *satelliteAnimation = new SatelliteAnimation(name,satellitePath,satelliteName,useOld,cutPositions);
	if(satelliteAnimation == NULL)
		return false;
	osg::Node *node = satelliteAnimation->buildSatelliteNode();
	if(node == NULL)
		return false;
	osg::Node *lineNode = satelliteAnimation->getLineNode();
	if(lineNode == NULL)
		return false;
	m_satelliteAnimations.insert(std::pair<std::string,SatelliteAnimation*>(name,satelliteAnimation));
	std::map<std::string,osg::Node*>::iterator it = m_groupManager.find(name);
	if(it != m_groupManager.end())
		return true;
	m_groupManager.insert(std::pair<std::string,osg::Node*>(name,node));

	it = m_linesManager.find(name);
	if(it != m_linesManager.end())
		return true;
	m_linesManager.insert(std::pair<std::string,osg::Node*>(name,lineNode));
	m_group->addChild(node);
	m_lines->addChild(lineNode);
	return true;
}

osg::Node * SatelliteGroupManager::getNode( std::string name )
{
	std::map<std::string,osg::Node*>::iterator it = m_groupManager.find(name);
	if(it != m_groupManager.end())
		return it->second;
	else
		return NULL;
}

osg::Node * SatelliteGroupManager::getLineNode( std::string name )
{
	std::map<std::string,osg::Node*>::iterator it = m_linesManager.find(name);
	if(it != m_linesManager.end())
		return it->second;
	else
		return NULL;
}

osg::Group * SatelliteGroupManager::getGroup()
{
	m_group->addChild(m_lines);
	m_group->getOrCreateStateSet()->setMode(GL_LIGHTING,0);
	m_group->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,1);
	return m_group.get();
}

std::map<std::string,SatelliteAnimation*> VRSim3D::SatelliteGroupManager::GetSatelliteAnimations()
{
	return m_satelliteAnimations;
}
