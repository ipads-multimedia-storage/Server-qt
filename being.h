#ifndef BEING_H
#define BEING_H


#include "object.h"
#include "Game.h"
#include <QDebug>

class Being: public Object{

protected:
    int curHealth, maxHealth;
    int lifeNum;
    QVector<class Prop*> mProps;

    Bar* mHealthBar;
    class Weapon* mWeapon;
public:
    bool isMovabel = true;
    virtual void* getPtr() override{return (Being*)this;}

    int getMaxHealth(){return maxHealth;}
    int getCurHealth(){return curHealth;}

    void paint(class QPainter*) override;
    void update(int deltaTime) override;

    void damage(int dam);
    void addLife(int x){lifeNum += x;}

    void addProp(class Prop* prop);
    void setWeapon(Weapon* w){mWeapon = w;}
    void searchForWeapon();
    void dropWeapon();
    virtual void useProp(class Prop* prop);
    virtual void attack(int m);

    Being(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    virtual ~Being();
};

class Player: public Being{
private:
protected:
    int score = 0;
public:
    virtual void* getPtr() override{return (Player*)this;}

    void paint(class QPainter*) override;
    void update(int deltaTime) override;
    void processInput(QVector<QString>*) override;

    Player(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    virtual ~Player() override;
};
#endif // BEING_H
