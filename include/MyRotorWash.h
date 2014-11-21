//////////////////////////////////////////////////////////////////////////
///文件名：旋翼洗流类
///创建者：高志清
///创建时间：2014年8月5日
///内容描述：旋翼洗流对水面的影像模拟（整合自Triton::RotorWashHandler）
//////////////////////////////////////////////////////////////////////////
#ifndef  MY_ROTOR_WASH_H
#define MY_ROTOR_WASH_H 1

#include "Export.h"

#include<Triton.h>

namespace VRSim3D
{
class VRSim3D_EXPORT MyRotorWash : public virtual Triton::RotorWash
{
public:
    MyRotorWash(Triton::Ocean *pOcean, double pRotorDiameter, bool pSprayEffects = false, bool pUseDecals = false);
	virtual ~MyRotorWash();

	///zqgao 设置最大风速
	void setmaxVelocity(const double &velocity );
	///zqgao 设置风速比例
	void setwindScale(const double &scale);

protected:

};
}
#endif
