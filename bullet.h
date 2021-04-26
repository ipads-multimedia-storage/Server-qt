#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include "Game.h"

class Bullet: public Object{
private:

protected:
    class Being* mOwner;
    int mDamage;

public:
    virtual void* getPtr() override{return (Bullet*)this;}

    virtual void paint(QPainter* ) override;
    virtual void update(int deltaTime) override; //重写Object的更新函数

    virtual void setOwner(class Being* owner){mOwner = owner;}
    Bullet(class Game* g, class Being* owner, float x = 0, float y = 0, double r = 0, float tv = 1000, int w = 10, int h = 10, int d = 10);
    ~Bullet();
};

class ZhanGangShan: public Bullet{
private:
    QVector<int> harmedList;
public:
    virtual void update(int deltaTime) override; //重写Object的更新函数

    ZhanGangShan(class Game* g, class Being* owner, float x = 0, float y = 0, double r = 0, float tv = 1000, int w = 10, int h = 200, int d = 35);
    ~ZhanGangShan();
};

class ZhanGangShan2: public Bullet{
private:
    QVector<int> harmedList;
public:
    virtual void update(int deltaTime) override; //重写Object的更新函数

    ZhanGangShan2(class Game* g, class Being* owner, float x = 0, float y = 0, double r = 0, float tv = 1000, int w = 220, int h = 220, int d = 35);
    ~ZhanGangShan2();
};

class Wind: public Bullet{
private:
    QVector<int> harmedList;
public:
    virtual void update(int deltaTime) override; //重写Object的更新函数

    Wind(class Game* g, class Being* owner = nullptr, float x = 0, float y = 0, double r = 0, float tv = 1000, int w = 120, int h = 120, int d = 35);
    ~Wind();
};

class TaQianZhan: public Bullet{
private:
    QVector<int> harmedList;
public:
    virtual void update(int deltaTime) override; //重写Object的更新函数

    TaQianZhan(class Game* g, class Being* owner, float x = 0, float y = 0, double r = 0, float tv = 1000, int w = 20, int h = 20, int d = 15);
    ~TaQianZhan();
};

class GravesBullet: public Bullet{
public:
    virtual void update(int deltaTime) override; //重写Object的更新函数

    GravesBullet(class Game* g, class Being* owner, float x = 0, float y = 0, double r = 0, float tv = 1400, int w = 20, int h = 20, int d = 8);
    ~GravesBullet();
};


#endif // BULLET_H
