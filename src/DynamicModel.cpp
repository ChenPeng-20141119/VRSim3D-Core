#include "DynamicModel.h"
#include "osgDB/ReadFile"
#include "osgDB/FileUtils"
using namespace VRSim3D;

DynamicModel::DynamicModel(void)
{
	init();
	setNumChildrenRequiringUpdateTraversal(1);
}

DynamicModel::DynamicModel( const osg::Vec3f &pos,const std::string &filePath )
{
	m_CftFollow = false;
	m_Position = pos;
	m_FilePath = filePath;
	m_dyGroup = new osg::MatrixTransform();
	m_dyGroup->addChild(LoadModel(filePath));
	setPosition(pos);
	setNumChildrenRequiringUpdateTraversal(1);
}

DynamicModel::DynamicModel( osgEarth::MapNode* globeNode,const float &lat,const float &lon,const float &ele,const std::string &filePath )
{
	m_CftFollow = false;
	m_GlobeNode = globeNode;
	m_FilePath = filePath;
	m_dyGroup = new osg::MatrixTransform();
	m_dyGroup->addChild(LoadModel(filePath));
	//m_dyGroup->setMatrix(osg::Matrix::scale(400,400,400));
	addChild(m_dyGroup.get());
	setPosition(lat,lon,ele);
	setNumChildrenRequiringUpdateTraversal(1);
}

DynamicModel::~DynamicModel(void)
{
}

void DynamicModel::init()
{
	m_CftFollow = false;
	m_FilePath = "";
	m_GlobeNode = NULL;
	m_Position = osg::Vec3f(0,0,0);
}

osg::Node* DynamicModel::LoadModel( const std::string &filePath )
{
	std::string path = osgDB::findDataFile(filePath);
	if (path.empty())
		return NULL;
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path);
	if(model)
		return model.release();
	return NULL;
}

void DynamicModel::setPosition( const osg::Vec3f &pos )
{
	if(m_Position != pos)
	{
		m_Position = pos;
		m_dyGroup->setMatrix(getMatrix()*osg::Matrix::translate(m_Position));
	}
}

void DynamicModel::setPosition( const float &lat,const float &lon,const float &ele,bool parallel /*= true*/ )
{
	if(lat < -90.0f || lat > 90.0f || lon <-180.0f || lon > 180.0f)
		return;
	if(!m_GlobeNode.valid())
		return;
	///zqgao 不与地球表面保持平行
	if(!parallel)
	{
		osg::Vec3d vec = osg::Vec3d(0,0,0);
		m_GlobeNode->getMapSRS()->getEllipsoid()->convertLatLongHeightToXYZ(osg::DegreesToRadians(lat),osg::DegreesToRadians(lon),ele,vec.x(),vec.y(),vec.z());
		setPosition(vec);
	}else
		///zqgao 与地球表面保持平行
	{
		osgEarth::Util::ObjectPlacer obp(m_GlobeNode.get());
		osg::Matrixd matrix;
		if ( obp.createPlacerMatrix(lat,lon,ele,matrix))
		{
			m_Position = matrix.getTrans();
			m_dyGroup->setMatrix(getMatrix()*matrix);
			//setMatrix(matrix);
		}
	}
}

void DynamicModel::setFilePath( const std::string &filePath )
{
	if(filePath != m_FilePath)
		m_FilePath = filePath;
}

void DynamicModel::addCommEffect(osg::Vec3f start,osg::Vec3f end ,float speed,float length,osg::Vec4f color,const std::string &movePoint)
{
	std::map<CEFTATT*,CCommEffect*>::iterator itor = m_CommEffectMap.begin();
	CEFTATT* cefatt = new CEFTATT();
	cefatt->movePoint = movePoint;
	cefatt->start = start;
	cefatt->end = end;
	osg::ref_ptr<CCommEffect> commEffect = new CCommEffect(start,500,end,7000);
	m_CommEffectMap.insert(std::make_pair(cefatt,commEffect.get()));
	commEffect->setSpeed(speed);
	commEffect->setLength(length);
	commEffect->setColor(color);
	addChild(commEffect.get());
}

void DynamicModel::traverse( osg::NodeVisitor& nv )
{
	osg::NodeVisitor::VisitorType vt = nv.getVisitorType();
	if (vt == osg::NodeVisitor::UPDATE_VISITOR)
	{
		if(m_CftFollow)
		{
			std::map<CEFTATT*,CCommEffect*>::iterator itor = m_CommEffectMap.begin();
			for (itor;itor != m_CommEffectMap.end();++itor)
			{
				osg::Vec3 movePoint = m_dyGroup->getMatrix().getTrans();
				if("start" == itor->first->movePoint)
				{
					itor->second->setXYZPosition(movePoint,itor->first->end);
				}else
					if ("end" == itor->first->movePoint)
					{
						itor->second->setXYZPosition(itor->first->start,movePoint);
					}
			}
		}
	}

	osg::Group::traverse(nv);
}

void DynamicModel::setScale( const osg::Vec3f &scale )
{
	if(m_dyGroup.valid())
		m_dyGroup->setMatrix(osg::Matrix::scale(scale)*m_dyGroup->getMatrix());
}

void DynamicModel::setRotation( const osg::Quat &quat )
{
	if(m_dyGroup.valid())
		m_dyGroup->setMatrix(osg::Matrix::rotate(quat)*m_dyGroup->getMatrix());
}

void DynamicModel::OpenCftFollow()
{
	m_CftFollow = true;
}