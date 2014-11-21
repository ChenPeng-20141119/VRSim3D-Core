#ifndef SATELLITEANIMATION_H
#define SATELLITEANIMATION_H

#include "Export.h"
#include <string>
#include <fstream>
#include <osg/Node>
#include <osg/AnimationPath>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgText/Text>

namespace VRSim3D
{
	struct SatellitePosInfo
	{
		double worldX;
		double worldY;
		double worldZ;
		double speedX;
		double speedY;
		double speedZ;
		double acceleratedSpeedX;
		double acceleratedSpeedY;
		double acceleratedSpeedZ;
		double yaw;
		double pitch;
	};
	class VRSim3D_EXPORT SatelliteAnimation
	{
	public:
		SatelliteAnimation(std::string name,std::string satellitePath,
			std::string satelliteName,bool useOld,std::vector<int> cutPositions);
		~SatelliteAnimation();
		osg::Node* buildSatelliteNode();
		osg::Node *getLineNode();
		std::map<int,SatellitePosInfo> GetSatellitePosInfo();
	private:
		osg::Vec3dArray* parseSatellitePath();
		osg::Vec3dArray* newParseSatellitePath();
		osg::AnimationPath* createAnimationPath(osg::Vec3dArray *points);
		void StringToWstring( std::wstring& szDst, std::string str );
		std::vector<int> DealCutVertexes(osg::Vec3dArray *vertexes);
	private:
		osg::ref_ptr<osg::Geode> m_lineGeode;
		std::string m_name;
		std::string m_satellitePath;
		std::string m_satelliteName;
		bool m_useOld;
		std::vector<int> m_cutPositions;
		//记录卫星当前位置的信息
		std::map<int,SatellitePosInfo> m_posInfo;
		int m_posNum;
	};
}

#endif