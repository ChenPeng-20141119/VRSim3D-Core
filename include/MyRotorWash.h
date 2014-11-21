//////////////////////////////////////////////////////////////////////////
///�ļ���������ϴ����
///�����ߣ���־��
///����ʱ�䣺2014��8��5��
///��������������ϴ����ˮ���Ӱ��ģ�⣨������Triton::RotorWashHandler��
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

	///zqgao ����������
	void setmaxVelocity(const double &velocity );
	///zqgao ���÷��ٱ���
	void setwindScale(const double &scale);

protected:

};
}
#endif
