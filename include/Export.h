/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#ifndef VRSim3D_EXPORT_
#define VRSim3D_EXPORT_ 1

#include<osg/Config>
/////////////////////////////////////////
///dbs ���������ͺͱ�������
#define NM_ALL_TRUE           0xFFFFFFFF
#define NM_ALL_FALSE          0x00000000
///dbs �������Ʋ���
#define NM_NORMAL_RENDER      0x00000003
///dbs �����ཻ����
#define NM_YES_REC_INTER      0xF0000001
///dbs ������������ӽڵ�
#define NM_GUI_MANAGER        0x0F000002
///dbs ֧���������ѯ����
#define NM_YES_QUARYABLE      0x00F00010


///dbs ���ε�ģʽ
enum TRAVEL_MODE {TM_FLY=0, TM_WALK};
///dbs �ƶ��ķ���
enum MOVE_DIRECTION {MD_NONE=0, MD_FORWARD, MD_BACK, MD_LEFT, MD_RIGHT, MD_UP, MD_DOWN};
///dbs ��ע���ֵ���ɫ
enum TEXT_COLOR {TC_RED=0, TC_GREEN, TC_BLUE, TC_YELLOW, TC_BLACK, TC_WHITE}; 
///dbs ��ѩ��Ч������
enum PRECIPITATION_TYPE {PT_SNOW=0, PT_RAIN}; 

///dbs mingzi
#define   NAME_SELECTABLE   "Selectable"

#define      INVALID_HEIGHT     -99999

///zj ��ʶ��Ӱ���ն���
const int ReceivesShadowTraversalMask = 0x1;
///zj ��ʶ��ӰͶӰ����
const int CastsShadowTrasversalMask = 0x2;

////////////////////
///dbs ��������
#define   OPERATION_ANAY_TWO_POINT          101
#define   OPERATION_ANAY_CIRCLE_AREA        102
#define   OPERATION_ANAY_MEASURE_DISTANCE   103
#define   OPERATION_ANAY_MEASURE_AREA       104 

#if defined(_MSC_VER) && defined(OSG_DISABLE_MSVC_WARNINGS)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4305 )
    #pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( OSG_LIBRARY_STATIC )
    #    define VRSim3D_EXPORT
    #  elif defined( VRSim3D_LIBRARY )
    #    define VRSim3D_EXPORT   __declspec(dllexport)
    #  else
    #    define VRSim3D_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define VRSim3D_EXPORT
#endif  


/**

\namespace VRSim3D

\zqgao  VRSim3D �ǻ���OpenSceneGraph-3.0.1��osgEarth2.2���з�װ����չ����Ҫ��Ӧ�ÿ����ṩ���ο����ӿ�

*/

#endif
