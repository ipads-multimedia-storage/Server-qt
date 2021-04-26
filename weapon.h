#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"
#include "Game.h"

class Weapon: public Object{
private:

protected:
    class Being* mOwner;
    class Label* mLabel;

public:
    virtual void* getPtr() override{return (Weapon*)this;}

    virtual void paint(QPainter* ) override{}
    virtual void update(int deltaTime) override;

    QString toString() override;

    Being* getOwner(){return mOwner;}
    virtual void setOwner(class Being* owner){mOwner = owner;}
    virtual void collected(class Being* being);
    virtual void attack(int m);
    Weapon(class Game* g, float x = 0, float y = 0, int w = 30, int h = 30);
    ~Weapon();
};

class Yasuo: public Weapon{
private:
    int qCoolDown, wCoolDown, eCoolDown;
    int qTimes = 0, eTime = 0;
public:
    virtual void update(int deltaTime) override;
    virtual void attack(int m) override;
    virtual void collected(class Being* being) override;

    Yasuo(class Game* g, float x = 0, float y = 0, int w = 30, int h = 30);
    ~Yasuo();
};

class Graves: public Weapon{
private:
    int qCoolDown, wCoolDown, eCoolDown;
    int qTime = 0;
public:
    virtual void update(int deltaTime) override;
    virtual void attack(int m) override;
    virtual void collected(class Being* being) override;

    Graves(class Game* g, float x = 0, float y = 0, int w = 30, int h = 30);
    ~Graves();
};

#endif // WEAPON_H
