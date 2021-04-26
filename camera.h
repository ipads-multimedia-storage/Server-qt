#ifndef CAMERA_H
#define CAMERA_H
#include "object.h"
#include "Master.h"
#include <QImage>


class Camera : public Object
{
private:
    QImage mImage;
    bool imageNotEmpty = false;
protected:

public:
    virtual void* getPtr() override{return (Camera*)this;}
    virtual void setFromBytes(QByteArray) override;

    void paint(class QPainter* p)override; //重写Object的显示函数
    void update(int deltaTime) override; //重写Object的更新函数

    Camera(Master* g = nullptr, int i = -1);
    virtual ~Camera();
};

#endif // CAMERA_H
