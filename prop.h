#ifndef PROP_H
#define PROP_H


#include "object.h"
#include <QRandomGenerator>

class Prop: public Object{
private:

protected:
    bool isCollected;
    class Being* mOwner;
    enum Status{

    };

public:
    virtual void* getPtr() override{return (Prop*)this;}

    virtual void paint(QPainter* ) override{}

    virtual void setOwner(class Being* owner){mOwner = owner;}
    virtual void collected(class Being* being);
    Prop(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    ~Prop();
};

#endif // PROP_H
