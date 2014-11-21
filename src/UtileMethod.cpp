#include "UtileMethod.h"
#include <osg/Camera>
#include <Windows.h>
#include <osg/BlendColor>
#include <osg/BlendFunc>
#include <osg/MatrixTransform>

using namespace VRSim3D;

UtileMethod::UtileMethod(void)
{
}

UtileMethod::~UtileMethod(void)
{
}

std::wstring UtileMethod::StrToWStr( std::string pszStr)
{
	int nSize = MultiByteToWideChar(CP_ACP,0,pszStr.c_str(),pszStr.length(),0,0);
	if (nSize <= 0) return NULL;
	WCHAR* pwszDst = new WCHAR[nSize+1];
	if (NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP,0,pszStr.c_str(),pszStr.length(),pwszDst,nSize);
	pwszDst[nSize]=0;
	if(pwszDst[0] == 0xFEFF)
		for (int i=0;i<nSize;i++)
			pwszDst[i]=pwszDst[i+1];
	std::wstring wcharString(pwszDst);
	delete pwszDst;

	return wcharString;
}

std::string UtileMethod::WStrToStr( std::wstring pszWstr )
{
	int nSize = WideCharToMultiByte(CP_ACP,0,pszWstr.c_str(),pszWstr.length(),0,0,NULL,false);
	if (nSize <= 0) return NULL;
	CHAR* pszDst = new CHAR[nSize+1];
	if (NULL == pszDst) return NULL;
	WideCharToMultiByte(CP_ACP,0,pszWstr.c_str(),pszWstr.length(),pszDst,nSize,NULL,false);
	pszDst[nSize]=0;
	if(pszDst[0] == 0xFEFF)
		for (int i=0;i<nSize;i++)
			pszDst[i]=pszDst[i+1];
	std::string charString(pszDst);
	delete pszDst;

	return charString;
}

void UtileMethod::findChild( osg::Group* parent,std::string childName,osg::Node** findNode )
{
	osg::ref_ptr<osg::Node> node = NULL;
	if(!parent)
		return ;
	for (unsigned int i = 0;i<parent->getNumChildren();++i)
	{
		node = parent->getChild(i);
		if(childName == node->getName())
		{
			(*findNode) =  node.get();
			return findChild(dynamic_cast<osg::Group*>(node.get()),childName,findNode);
		}else
		{
			osg::ref_ptr<osg::Group> group = dynamic_cast<osg::Group*>(node.get());
			if (group)
			{
				findChild(group.get(),childName,findNode);
			}
		}
	}
}

std::string UtileMethod::IntToStr( int num )
{
	char buffer[128];	  
	sprintf(buffer,"%i",num);
	std::string strnum(buffer);

	//delete buffer;
	return strnum;
}

void UtileMethod::HudCamera(osg::Camera*hUDCamera, float x, float y)
{
	///dbs 首先建立包含投影距阵和模型距阵的相机，然后在此相机基础上绘制屏幕显示的内容
	//osg::ref_ptr<osg::Camera> hUDCamera = new osg::Camera;

	///dbs 首先设置投影距阵
	hUDCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, x, 0, y));

	///dbs 然后设置参考坐标系和视图矩阵 
	hUDCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hUDCamera->setViewMatrix(osg::Matrix::identity());

	///dbs 设置清除深度缓存
	hUDCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	///dbs 设置绘图的顺序，将此相机对应的图象绘制在主相机之后
	hUDCamera->setRenderOrder(osg::Camera::POST_RENDER, 12);

	///dbs 设置该相机没有焦点，不接受事件
	hUDCamera->setAllowEventFocus(false);

	///dbs 设置节点的掩码
	hUDCamera->setNodeMask(0x00000003);

	///dbs 关闭光照和深度测试
	osg::ref_ptr<osg::StateSet> stateset = hUDCamera->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	//return hUDCamera.get();
}

std::string UtileMethod::GBKToUTF8( const std::string &strGBK )
{
	std::string strOutUTF8 = "";
	int n = MultiByteToWideChar(CP_ACP,0,strGBK.c_str(),-1,NULL,0);
	WCHAR * str1 = new WCHAR[n*2];
	MultiByteToWideChar(CP_ACP,0,strGBK.c_str(),-1,str1,n);
	n = WideCharToMultiByte(CP_UTF8,0,str1,-1,NULL,0,NULL,NULL);
	std::wstring wstr(str1);
	//strOutUTF8 = WStrToStr(wstr);
	char* str2 = new char[n];
	WideCharToMultiByte(CP_UTF8,0,str1,-1,str2,n,NULL,NULL);
	strOutUTF8 = str2;
	
	delete[] str1;
	delete[] str2;
	str1 = NULL;
	str2 = NULL;

	return strOutUTF8;
}

std::string UtileMethod::UTF8ToGBK( const std::string &strUTF8 )
{
	int len = MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);
	unsigned short* wszGBK = new unsigned short(len +1);
	memset(wszGBK,0,len * 2+2);
	MultiByteToWideChar(CP_UTF8,0,(LPCTSTR)strUTF8.c_str(),-1,(LPWSTR)wszGBK,len);

	len = WideCharToMultiByte(CP_ACP,0,(LPWSTR)wszGBK,-1,NULL,0,NULL,NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK,0,len + 1);
	WideCharToMultiByte(CP_ACP,0,(LPWSTR)wszGBK,-1,szGBK,len,NULL,NULL);
	
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	szGBK = NULL;
	wszGBK = NULL;

	return strTemp;
}

int VRSim3D::UtileMethod::getMPIndexFromSwichMP( osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name )
{
	osgGA::KeySwitchMatrixManipulator::KeyManipMap keyManipMap = SwitchWP->getKeyManipMap();
	osgGA::KeySwitchMatrixManipulator::KeyManipMap::iterator itor = keyManipMap.begin();
	while(itor != keyManipMap.end())
	{
		if(itor->second.first == name)
			///zqgao 返回char，便于按键控制
			return itor->first - 48;
		itor++;
	}
	return 99999;
}

bool VRSim3D::UtileMethod::RemoveMPIndexFromSwichMP( osgGA::KeySwitchMatrixManipulator* SwitchWP,std::string name )
{
	osgGA::KeySwitchMatrixManipulator::KeyManipMap keyManipMap = SwitchWP->getKeyManipMap();
	osgGA::KeySwitchMatrixManipulator::KeyManipMap::iterator itor = keyManipMap.begin();
	while(itor != keyManipMap.end())
	{
		if(itor->second.first == name)
		{
			keyManipMap.erase(itor++);
		}else
		{
			itor++;
		}
	}
	return false;
}

void VRSim3D::UtileMethod::transferChilds( osg::Group *oldGroup,osg::Group *newGroup )
{
	if (oldGroup && newGroup)
	{
		for(unsigned i=0; i<oldGroup->getNumChildren(); ++i)
		{
			newGroup->addChild( oldGroup->getChild(i) );
		}
		oldGroup->removeChildren(0, oldGroup->getNumChildren());
	}
}

bool VRSim3D::UtileMethod::isHighLight( osg::Node* node )
{
	bool ishigh = false;
	osg::ref_ptr<osg::Node> parent = node->getParent(0);
	if(!node && !parent.valid())
		return ishigh;
	osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(node);
	if(mt.valid())
	{
		osg::StateSet* state = node->getOrCreateStateSet();
		ishigh = state->getMode(GL_BLEND)&&
			state->getAttribute(osg::StateAttribute::BLENDCOLOR)&&
			state->getAttribute(osg::StateAttribute::BLENDFUNC);
		return ishigh;
	}
}


void VRSim3D::UtileMethod::highLight( osg::Node* node )
{
	if(isHighLight(node))
		return;
	osg::StateSet* state = node->getOrCreateStateSet();
	osg::BlendColor* blendColor = new osg::BlendColor(osg::Vec4(106/255.0,106/255.0,255/255.0,1.0));
	osg::BlendFunc* blendFunc = new osg::BlendFunc;
	blendFunc->setDataVariance(osg::Object::STATIC);
	blendFunc->setSource(osg::BlendFunc::CONSTANT_COLOR);
	blendFunc->setDestination(osg::BlendFunc::CONSTANT_COLOR);
	state->setDataVariance(osg::Object::DYNAMIC);
	state->setAttribute(blendFunc,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	state->setAttribute(blendColor,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
}

bool VRSim3D::UtileMethod::wipeHighLight( osg::Node* node )
{
	if(!isHighLight(node))
		return true;
	osg::StateSet* state = node->getOrCreateStateSet();
	state->removeMode(GL_BLEND);
	state->removeAttribute(osg::StateAttribute::BLENDCOLOR);
	state->removeAttribute(osg::StateAttribute::BLENDFUNC);

	return true;
}