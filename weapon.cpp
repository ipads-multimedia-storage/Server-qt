#include "weapon.h"

Weapon::Weapon(class Game* g, float x, float y, int w, int h)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h;mOwner = nullptr;
    mType = "Weapon";
    mInfo = "Basic Weapon";
    mLabel = new Label(mGame, xPos, yPos, 50, 18, "Basic Weapon");
    mGame->addObject(mLabel);
}

Weapon::~Weapon(){
    if(mLabel) mGame->removeObject(mLabel);
}

void Weapon::collected(class Being *being){
    setOwner(being);
    being->setWeapon(this);
    mGame->addObject(new DyingLabel(mGame, 1000, xPos - 70, yPos-50, 100, 15, 0, -100, mInfo));
}

void Weapon::attack(int m){
    if (m == 1 && mTime > 400){
        Bullet* bullet1 = new Bullet(mGame, mOwner, mOwner->getX(), mOwner->getY(), mOwner->getRot() - 0.1, 1200, 10, 10, rand() % 15 + 7);
        Bullet* bullet2 = new Bullet(mGame, mOwner, mOwner->getX(), mOwner->getY(), mOwner->getRot(), 1200, 10, 10, rand() % 15 + 7);
        Bullet* bullet3 = new Bullet(mGame, mOwner, mOwner->getX(), mOwner->getY(), mOwner->getRot() + 0.1, 1200, 10, 10, rand() % 15 + 7);
        mGame->addObject(bullet1);
        mGame->addObject(bullet2);
        mGame->addObject(bullet3);
        bullet1->setColor(mColor);
        bullet2->setColor(mColor);
        bullet3->setColor(mColor);
        mTime = 1;
    }
}

void Weapon::update(int deltaTime){
    Object::update(deltaTime);
    if (mLabel) mLabel->setXY(xPos-40, yPos-80);
}

QString Weapon::toString(){
    QString result = "";
    result += mType + ":";
    result += QString::number(id) + ":";
    result += QString::number(xPos) + ":";
    result += QString::number(yPos) + ":";
    result += QString::number(tVel) + ":";
    result += QString::number(nVel) + ":";
    result += QString::number(width) + ":";
    result += QString::number(height) + ":";
    result += QString::number(rot) + ":";
    result += QString::number(rotVel) + ":";
    result += QString::number(mColor.red()) + ":";
    result += QString::number(mColor.green()) + ":";
    result += QString::number(mColor.blue()) + ":";
    result += QString::number(mColor.alpha()) + ":";
    if(mOwner)result += QString::number(mOwner->id) + ":";
    else result += "-1:";
    return result;
}

Yasuo::Yasuo(class Game* g, float x, float y, int w, int h)
    :Weapon(g, x, y, w, h){
    mType = "Yasuo";
    mInfo = "Yasuo's Wind Sword";
    mLabel->setText("Yasuo's Wind Sword");
    qCoolDown = wCoolDown = eCoolDown = 0;
}

Yasuo::~Yasuo(){

}

void Yasuo::collected(class Being *being){
    setOwner(being);
    being->setWeapon(this);
    mGame->addObject(new DyingLabel(mGame, 1000, xPos - 90, yPos-50, 100, 15, 0, -100, mInfo));
}


void Yasuo::update(int deltaTime){
    Object::update(deltaTime);
    if (mLabel) mLabel->setXY(xPos-50, yPos-80);
    if (qCoolDown > 0) qCoolDown -= deltaTime;
    if (wCoolDown > 0) wCoolDown -= deltaTime;
    if (eCoolDown > 0) eCoolDown -= deltaTime;
    if (eTime > 0){
        eTime -= deltaTime;
        if (eTime <= 0 && mOwner){
            mOwner->setVel(0, 0);
            mOwner->isMovabel = true;
        }
    }
}

void Yasuo::attack(int m){
    if (m == 1 && qCoolDown <= 0){
        qCoolDown = 2000;
        qTimes ++;
        if (eTime > 0){
            mGame->addObject(new ZhanGangShan2(mGame, mOwner, xPos, yPos, rot, 0));
        }else if (qTimes >= 3){
            mGame->addObject(new Wind(mGame, mOwner, xPos, yPos, rot));
            qTimes = 0;
        }
        else mGame->addObject(new ZhanGangShan(mGame, mOwner, xPos + 100*sin(rot), yPos - 100*cos(rot), rot, 0));
    }
    if (m == 2 && wCoolDown <= 0){
        wCoolDown = 10000;
        mGame->addBlock(new WindWall(mGame, xPos, yPos, 120, 30, rot));
    }
    if (m == 3 && eCoolDown <= 0){
        eCoolDown = 8000;
        eTime = 350;
        mGame->addObject(new TaQianZhan(mGame, mOwner, xPos, yPos, rot, 0));
        mOwner->isMovabel = false;
        mOwner->setVel(1000, 0);
    }
}

Graves::Graves(class Game* g, float x, float y, int w, int h)
    :Weapon(g, x, y, w, h){
    mType = "Graves";
    mInfo = "Graves's Gun";
    mLabel->setText("Graves's Gun");
    qCoolDown = wCoolDown = eCoolDown = 0;
}

Graves::~Graves(){

}

void Graves::collected(class Being *being){
    setOwner(being);
    being->setWeapon(this);
    mGame->addObject(new DyingLabel(mGame, 1000, xPos - 90, yPos-50, 100, 15, 0, -100, mInfo));
}


void Graves::update(int deltaTime){
    Object::update(deltaTime);
    if (mLabel) mLabel->setXY(xPos-50, yPos-80);
    if (qCoolDown > 0) qCoolDown -= deltaTime;
    if (wCoolDown > 0) wCoolDown -= deltaTime;
    if (eCoolDown > 0) eCoolDown -= deltaTime;
}

void Graves::attack(int m){
    if (m == 1 && qCoolDown <= 0){
        qTime ++;
        qCoolDown = 600;
        if (qTime > 1){
            qCoolDown = 3000;
            qTime = 0;
        }
        for (int i = -3; i <= 3; i++){
            GravesBullet* bullet = new GravesBullet(mGame, mOwner, xPos, yPos, rot + i * 0.1, 1400, 20, 20);
            mGame->addObject(bullet);
        }
    }
    if (m == 2 && wCoolDown <= 0){
        wCoolDown = 10000;
    }
    if (m == 3 && eCoolDown <= 0){
        eCoolDown = 8000;
        mOwner->update(500);
    }
}
