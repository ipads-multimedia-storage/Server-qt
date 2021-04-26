#include "bullet.h"


Bullet::Bullet(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h; mOwner = owner; rot = r; tVel = tv;
    mType = "Bullet";
    mInfo = "Basic Bullet";
    mDamage = d;
}

Bullet::~Bullet(){

}


void Bullet::paint(class QPainter* p){
    Object::paint(p);
}

void Bullet::update(int deltaTime){     //用于检测处理碰撞、踩踏、摧毁等事件
    xVel = tVel * sin(rot) + nVel * cos(rot);
    yVel = nVel * sin(rot) - tVel * cos(rot);
    Object::update(deltaTime);

    for (auto i : *mGame->getBlocks()){
        if (i->mShape == i->Circle && (i->getX()-xPos)*(i->getX()-xPos)+(i->getY()-yPos)*(i->getY()-yPos) < 0.25*i->getW()*i->getW()){
            mGame->removeObject(this);
            break;
        }
        if (i->mShape == i->Squre && i->getX() + i->getW()/2 > xPos &&i->getX() - i->getW()/2 < xPos &&i->getY() + i->getH()/2 > yPos &&i->getY() - i->getH()/2 < yPos){
            mGame->removeObject(this);
            break;
        }
    }

    for (auto i: *mGame->getBeings()){
        if (i != mOwner && i->getX() + i->getW()/2 > xPos &&i->getX() - i->getW()/2 < xPos &&i->getY() + i->getH()/2 > yPos &&i->getY() - i->getH()/2 < yPos){
            i->damage(mDamage);
            mGame->removeObject(this);
            break;
        }
    }
}

ZhanGangShan::ZhanGangShan(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Bullet(g, owner, x, y, r, tv, w, h, d){
    tVel = 0;
    mType = "ZhanGangShan";
    mInfo = "Yasuo's Q-Ability";
    mColor = QColor(60, 80, 155);
}

ZhanGangShan::~ZhanGangShan(){

}

void ZhanGangShan::update(int deltaTime){
    Object::update(deltaTime);
    for (auto i: *mGame->getBeings()){
        if (harmedList.contains( i->id))continue;
        int x, y;
        for (int j = -4; j<= 4; j++){
            x = xPos + j * 25 * sin(rot); y = yPos - j * 25 * cos(rot);
            double dis = (i->getX() - x)*(i->getX() - x)+(i->getY() - y)*(i->getY() - y);
            if (i != mOwner && dis < 625){
                i->damage(mDamage);
                harmedList.append(i->id);
                break;
            }
        }
    }
    if (mTime > 255) mGame->removeObject(this);
}
ZhanGangShan2::ZhanGangShan2(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Bullet(g, owner, x, y, r, tv, w, h, d){
    tVel = 0;
    mType = "ZhanGangShan2";
    mInfo = "Yasuo's Q-Ability2";
    mColor = QColor(60, 80, 155);
}

ZhanGangShan2::~ZhanGangShan2(){

}

void ZhanGangShan2::update(int deltaTime){
    Object::update(deltaTime);
    for (auto i: *mGame->getBeings()){
        if (harmedList.contains( i->id))continue;
        double dis = (i->getX() - xPos)*(i->getX() - xPos)+(i->getY() - yPos)*(i->getY() - yPos);
        if (i != mOwner && dis < 12100){
            i->damage(mDamage);
            harmedList.append(i->id);
            break;
        }
    }
    if (mTime > 255) mGame->removeObject(this);
}

Wind::Wind(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Bullet(g, owner, x, y, r, tv, w, h, d){
    tVel = 550;
    xVel = tVel * sin(rot) + nVel * cos(rot);
    yVel = nVel * sin(rot) - tVel * cos(rot);
    mType = "Wind";
    mInfo = "Yasuo's Wind";
    mColor = QColor(150, 180, 255);
    rotVel = 6;
}

Wind::~Wind(){

}

void Wind::update(int deltaTime){
    Object::update(deltaTime);
    for (auto i: *mGame->getBeings()){
        if (harmedList.contains( i->id))continue;
        double dis = (i->getX() - xPos)*(i->getX() - xPos)+(i->getY() - yPos)*(i->getY() - yPos);
        if (i != mOwner && dis < 3600){
            i->damage(mDamage);
            harmedList.append(i->id);
            break;
        }
    }
    if (mTime > 1000) mGame->removeObject(this);
}


TaQianZhan::TaQianZhan(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Bullet(g, owner, x, y, r, tv, w, h, d){
    tVel = 750;
    xVel = tVel * sin(rot) + nVel * cos(rot);
    yVel = nVel * sin(rot) - tVel * cos(rot);
    mType = "TaQianZhan";
    mInfo = "Yasuo's E-Ability";
    mColor = QColor(60, 80, 155);
}

TaQianZhan::~TaQianZhan(){

}

void TaQianZhan::update(int deltaTime){
    Object::update(deltaTime);
    for (auto i: *mGame->getBeings()){
        if (harmedList.contains( i->id))continue;
        double dis = (i->getX() - xPos)*(i->getX() - xPos)+(i->getY() - yPos)*(i->getY() - yPos);
        if (i != mOwner && dis < 2000){
            i->damage(mDamage);
            harmedList.append(i->id);
            break;
        }
    }
    if (mTime > 420) mGame->removeObject(this);
}

GravesBullet::GravesBullet(class Game* g, class Being* owner, float x, float y, double r, float tv, int w, int h, int d)
    :Bullet(g, owner, x, y, r, tv, w, h, d){
    tVel = 1400;
    mType = "GravesBullet";
    mInfo = "Graves's Bullet";
    mColor = QColor(80, 80, 55);
}

GravesBullet::~GravesBullet(){

}

void GravesBullet::update(int deltaTime){
    if (mTime > 230){
        mGame->removeObject(this);
        return;
    }
    Bullet::update(deltaTime);
}
