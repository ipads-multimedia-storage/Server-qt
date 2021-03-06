#ifndef ARM_H
#define ARM_H

#include "object.h"
#include "Master.h"


class Arm : public Object
{
private:
    float mRadius;  //能抓取的范围半径
    float mDelay;  //抓取所需要的时间
protected:

public:
    virtual void* getPtr() override{return (Arm*)this;}

    void paint(class QPainter* p)override; //重写Object的显示函数
    void update(int deltaTime) override; //重写Object的更新函数

    void setRadius(int r){mRadius = r;}

    Arm(Master* g = nullptr, int i = -1);
    virtual ~Arm();
};

#endif // ARM_H
