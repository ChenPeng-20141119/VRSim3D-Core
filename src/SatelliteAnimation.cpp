#include "SatelliteAnimation.h"
#include <Windows.h>
#include "AnimationPathPlayBackCallback.h"
#include "osg/CullFace"
#include "osg/Depth"
using namespace VRSim3D;

class LineCallback : public osg::NodeCallback
{
public:
	LineCallback(AnimationPathPlayBackCallback *aniCallback,std::vector<int> cutPositions,osg::Vec3dArray *satellitePoints)
	{
		m_aniCallback = aniCallback;
		m_cutPositions = cutPositions;
		m_satellitePoints = satellitePoints;
	}

	void operator()( osg::Node* node, osg::NodeVisitor* nv )
	{
		if(m_cutPositions.size() < 1)
			return;
		osg::Geode *lineGeode = dynamic_cast<osg::Geode*>(node);
		if(node)
		{
			double aniTime = m_aniCallback->getAnimationTime();
			for(int i = 0 ; i < m_cutPositions.size() - 1; ++i)
			{
				if( fabs(aniTime - m_cutPositions.at(i)) < 20)
				{
					//geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,m_satellitePoints->size()));
					osg::Geometry *geom = dynamic_cast<osg::Geometry *>(lineGeode->getDrawable(0));
					osg::DrawArrays *drawArray = dynamic_cast<osg::DrawArrays *>(geom->getPrimitiveSet(0));
					drawArray->setFirst(m_cutPositions.at(i));
					drawArray->setCount(m_cutPositions.at(i + 1) - m_cutPositions.at(i) + 1);
					geom->dirtyDisplayList();
				}
			}
		}
	}
private:
	AnimationPathPlayBackCallback *m_aniCallback;
	std::vector<int> m_cutPositions;
	osg::Vec3dArray *m_satellitePoints;
};

SatelliteAnimation::SatelliteAnimation( std::string name,std::string satellitePath,
	std::string satelliteName,bool useOld,std::vector<int> cutPositions )
{
	m_name = name;
	m_satellitePath = satellitePath;
	m_satelliteName = satelliteName;
	m_useOld = useOld;
	m_cutPositions = cutPositions;
	m_posNum = 0;
}

SatelliteAnimation::~SatelliteAnimation()
{

}

void SatelliteAnimation::StringToWstring( std::wstring& szDst, std::string str )
{
	const char* schar = str.c_str();
	int len = MultiByteToWideChar(CP_ACP,0,schar,strlen(schar),NULL,0);
	wchar_t* m_wchar=new wchar_t[len+1];
	MultiByteToWideChar(CP_ACP,0,schar,strlen(schar),m_wchar,len);
	m_wchar[len]='\0';

	szDst = m_wchar;

	delete[] m_wchar;
}

osg::Node * SatelliteAnimation::buildSatelliteNode()
{
	osg::Node *satellite = osgDB::readNodeFile(m_satelliteName);
	osg::Vec3dArray *satellitePoints = NULL;
	if (m_useOld)
		satellitePoints = parseSatellitePath();
	else
		satellitePoints = newParseSatellitePath();

	//std::vector<int> cutPositions;
	//if(satellitePoints)
	//	cutPositions = DealCutVertexes(satellitePoints);
	m_lineGeode = new osg::Geode();

	osg::StateSet * ss = m_lineGeode->getOrCreateStateSet();
	//ss->setMode(GL_CULL_FACE, true);
	ss->setAttribute(new osg::CullFace(osg::CullFace::FRONT_AND_BACK), osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	ss->setMode(GL_LINE_SMOOTH, true);
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setWriteMask(false);
	ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
	ss->setMode(GL_NORMALIZE, true);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	ss->setRenderBinDetails(9999, "RenderBins");
	ss->setMode(GL_BLEND, true);
	ss->setMode(GL_DEPTH_TEST, true);

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	geom->setVertexArray(satellitePoints);
	//创建颜色数组
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	vc->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	geom->setColorArray(vc.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	if(m_useOld)
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,satellitePoints->size()));
	else
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,m_cutPositions.at(0)));
	m_lineGeode->addDrawable(geom.get());
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();
	animationPath = createAnimationPath(satellitePoints);

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	AnimationPathPlayBackCallback *aniCallback = new AnimationPathPlayBackCallback(animationPath,0.0f,1.0f);
	mt->setUpdateCallback(aniCallback);

	LineCallback *lineCallBack = new LineCallback(aniCallback,m_cutPositions,satellitePoints);
	m_lineGeode->addUpdateCallback(lineCallBack);

	//osg::AnimationPath::TimeControlPointMap::iterator it = animationPath->getTimeControlPointMap().find(animationPath->getFirstTime());
	//osg::Matrix m;
	//m.setTrans(it->second.getPosition());
	//mt->setMatrix(m);
	mt->addChild(satellite);
	mt->setName(m_name);
	mt->setNodeMask(NM_YES_QUARYABLE);

	//创建显示字
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode();
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setDataVariance(osg::Object::DYNAMIC);
	text->setFont("simhei.tff");
	std::wstring wstr;
	StringToWstring(wstr,m_name);
	text->setText(wstr.c_str());
	text->setCharacterSize(200000.0);
	text->setColor(osg::Vec4(0,0,1,1));
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setAlignment(osgText::TextBase::RIGHT_BOTTOM);
	textGeode->addDrawable(text);
	mt->addChild(textGeode);
	return mt;
}

osg::Vec3dArray* SatelliteAnimation::parseSatellitePath()
{
	osg::Vec3dArray *satellitePoints = new osg::Vec3dArray;
	std::string path = m_satellitePath;
	std::fstream ff(path.c_str(),std::ios::binary | std::ios::in);
	while(!ff.eof())
	{
		std::string date;
		int hour,minute,second,temp1,temp2,temp3,temp4,temp5;
		double worldX,worldY,worldZ,speedX,speedY,speedZ,acceleratedSpeedX,acceleratedSpeedY,acceleratedSpeedZ,yaw,pitch;
		ff >> date >> hour >> minute >> second >> temp1 >> temp2 >> temp3 >> temp4 >> temp5 >> worldX >> worldY >> 
			worldZ >> speedX >> speedY >> speedZ >> acceleratedSpeedX >> acceleratedSpeedY >> acceleratedSpeedZ >> yaw >> pitch;
		satellitePoints->push_back(osg::Vec3d(worldX,worldY,worldZ));
		SatellitePosInfo posInfo;
		posInfo.worldX = worldX;
		posInfo.worldY = worldY;
		posInfo.worldZ = worldZ;
		posInfo.speedX = speedX;
		posInfo.speedY = speedY;
		posInfo.speedZ = speedZ;
		posInfo.acceleratedSpeedX = acceleratedSpeedX;
		posInfo.acceleratedSpeedY = acceleratedSpeedY;
		posInfo.acceleratedSpeedZ = acceleratedSpeedZ;
		posInfo.yaw = yaw;
		posInfo.pitch = pitch;
		m_posInfo.insert(std::pair<int,SatellitePosInfo>(m_posNum,posInfo));
		m_posNum++;
	}
	return satellitePoints;
}

osg::Vec3dArray* SatelliteAnimation::newParseSatellitePath()
{
	osg::Vec3dArray *satellitePoints = new osg::Vec3dArray;
	std::string path = m_satellitePath;
	std::fstream ff(path.c_str(),std::ios::binary | std::ios::in);
	while(!ff.eof())
	{
		std::string name;
		double worldX,worldY,worldZ,acceleratedSpeedX,acceleratedSpeedY,acceleratedSpeedZ;
		ff >> name >> worldX >> worldY >> worldZ >> acceleratedSpeedX >> acceleratedSpeedY >> acceleratedSpeedZ;
		satellitePoints->push_back(osg::Vec3d(worldX,worldY,worldZ));
	}
	return satellitePoints;
}

osg::AnimationPath* SatelliteAnimation::createAnimationPath( osg::Vec3dArray *points )
{
	osg::AnimationPath *animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);
	osg::Vec3d center(0,0,0);
	double tempTime = points->size();
	double time = 0.0;
	double time_delta = tempTime/(double)(points->size());
	osg::Vec3d up,dir;
	double delta = osg::PI * 2.0 / (double)points->size();
	for (int i = 0; i < points->size();i++)
	{
		double angle = delta * double(i);
		osg::Quat quat(angle,up);
		osg::Vec3d point = points->at(i);
		up = point - center;
		up.normalize();
		osg::Vec3d side = up ^ osg::Vec3d(0,0,1);
		osg::Vec3d spoke = quat * (side * ((point - center).length()));
		spoke.normalize();
		osg::Quat makeUp;
		makeUp.makeRotate(osg::Vec3d(0,0,1),up);
		dir = up ^ spoke;

		osg::Quat rot;
		rot.makeRotate(makeUp * osg::Vec3d(0,-1,0),dir);
		animationPath->insert(time,osg::AnimationPath::ControlPoint(point,makeUp * rot,osg::Vec3f(20000,20000,20000)));
		time += time_delta;
	}

	return animationPath;
}

osg::Node * SatelliteAnimation::getLineNode()
{
	if(m_lineGeode)
		return m_lineGeode;
	return NULL;
}

//std::vector<int> SatelliteAnimation::DealCutVertexes( osg::Vec3dArray *vertexes )
//{
//	std::vector<int> cutPositions;
//	double firstX = vertexes->at(0).x();
//	double firstY = vertexes->at(0).y();
//	double firstZ = vertexes->at(0).z();
//	int count = vertexes->size();
//	double tempX = 0.0,tempY = 0.0,tempZ = 0.0;
//	for (int i = 1; i < count; ++i)
//	{
//		tempX = vertexes->at(i).x();
//		tempY = vertexes->at(i).y();
//		tempZ = vertexes->at(i).z();
//		if (fabs(tempX - firstX) < 0.1 &&
//			fabs(tempY - firstY) < 0.1 &&
//			fabs(tempZ - firstZ) < 0.1)
//		{
//			cutPositions.push_back(i);
//			firstX = vertexes->at(i + 1).x();
//			firstY = vertexes->at(i + 1).x();
//			firstZ = vertexes->at(i + 1).x();
//			i++;
//		}
//	}
//
//	return cutPositions;
//}

std::vector<int> SatelliteAnimation::DealCutVertexes( osg::Vec3dArray *vertexes )
{
	std::vector<int> cutPositions;
	osg::Vec3d tempvec,tempvec1;
	for (int i = 0; i < vertexes->size(); ++i)
	{
		tempvec = vertexes->at(i);
		for (int j = i+1; j < vertexes->size(); ++j)
		{
			tempvec1 = vertexes->at(j);
			if(( tempvec1 -  tempvec).length() < 0.000001)
			{
				cutPositions.push_back(j);
			}
		}
		if(cutPositions.size() > 1)
			break;
	}
	return cutPositions;
}

std::map<int,SatellitePosInfo> VRSim3D::SatelliteAnimation::GetSatellitePosInfo()
{
	return m_posInfo;
}
