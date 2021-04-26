#include "being.h"


void Being::paint(class QPainter *p){
    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(Qt::black));
        p->drawRect(int(xPos - 25), int(yPos - 25), 50, 50);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

void Being::update(int deltaTime){
    Object::update(deltaTime);
    if (curHealth <= 0){
        mGame->removeObject(this);
    }
    mHealthBar->setValue(float(curHealth)/maxHealth);
    mHealthBar->setXY(xPos, yPos - 50);
    mHealthBar->setColor(mColor);
    if (mWeapon){
        mWeapon->setXY(xPos, yPos);
        mWeapon->setRot(rot);
    }
}

void Being::damage(int dam){
    curHealth -= dam;
    mGame->addObject(new DyingLabel(mGame, 600, xPos + rand() % 50 -100, yPos + rand() % 50 -100, 30, 30, 0, -400, QString::number(dam)));
}

Being::Being(Game* g, float x, float y, int w, int h)
    :Object(g){
    curHealth = maxHealth = 100;
    lifeNum = 1;
    xPos = x; yPos = y;
    width = w; height = h;
    mType = "Being";
    mInfo = "Basic";
    mWeapon = nullptr;
    mHealthBar = new Bar(mGame, 1, xPos, yPos-50, 120, 16, mColor);
    mGame->addObject(mHealthBar);
}

void Being::addProp(class Prop *prop){
    mProps.push_back(prop);
}

void Being::useProp(class Prop *prop){
    mProps.removeOne(prop);
}

void Being::attack(int m){
    if (mWeapon){
        mWeapon->attack(m);
    }else if(mTime > 500){
        mTime = 1;
        Bullet* bullet = new Bullet(mGame, this, xPos, yPos, rot, 1000, 10, 10, rand() % 15 + 7);
        mGame->addObject(bullet);
        bullet->setColor(mColor);
    }
}

void Being::dropWeapon(){
    if (mWeapon){
        mWeapon->setOwner(nullptr);
        mWeapon = nullptr;
    }
}

void Being::searchForWeapon(){
    for (auto i : *mGame->getWeapons()){
        if (!i->getOwner() && i->getX() + i->getW()/2 > xPos &&i->getX() - i->getW()/2 < xPos &&i->getY() + i->getH()/2 > yPos &&i->getY() - i->getH()/2 < yPos){
            dropWeapon();
            i->collected(this);
            break;
        }
    }
}

Being::~Being(){
    mGame->removeObject(mHealthBar);
    if(mWeapon)dropWeapon();
}


void Player::paint(class QPainter *p){
    if (isVisible){      //显示一个小黑块，上面有个笑脸
        p->save();
        p->translate(xPos, yPos);
        p->rotate(rot*180/acos(-1));
        p->setPen(Qt::NoPen);
        p->setBrush(mColor);
        p->drawRect(int( - 25), int( - 25), 50, 50);   //以pos为中心绘制方块
        p->setBrush(QBrush(Qt::white));
        p->drawRect(int( - 15) , int( - 15), 10, 10);
        p->drawRect(int( + 5) , int( - 15), 10, 10);
        p->drawRect(int( - 15) , int( + 10), 30, 8);
        p->drawRect(int( - 15), int( + 2), 8, 10);
        p->drawRect(int( + 7) , int( + 2), 8, 10);
        p->restore();
    }
}

void Player::update(int deltaTime){
    xVel = tVel * sin(rot) + nVel * cos(rot);
    yVel = nVel * sin(rot) - tVel * cos(rot);

    bool crashOnBlockTop = false, crashOnBlockBottom = false, crashOnBlockLeft = false, crashOnBlockRight = false;

    float crashPossTop, crashPossBottom, crashPossLeft, crashPossRight;
    for (auto i : *mGame->getBlocks()){
        if (i->getCrossable())continue;
        if (i->mShape == i->Squre){
            crashPossTop = crashPossBottom = crashPossLeft = crashPossRight = -1;
            if (i->getY() - i->getH()/2 - yPos - height/2 + 2*yVel * deltaTime / 1000>= 0 && yVel > 0)
                crashPossTop = (i->getY() - i->getH()/2 - yPos - height/2 + 2*yVel * deltaTime / 1000) / yVel;

            if (yPos - height/2 - i->getY() - i->getH()/2 - 2*yVel * deltaTime / 1000>= 0 && yVel < 0)
                crashPossBottom = - (yPos - height/2 - i->getY() - i->getH()/2 - 2*yVel * deltaTime / 1000) / yVel;

            if (i->getX() - i->getW()/2 - xPos - width/2 + 2*xVel * deltaTime / 1000>= 0 && xVel > 0)
                crashPossLeft = (i->getX() - i->getW()/2 - xPos - width/2 + 2*xVel * deltaTime / 1000) / xVel;

            if (xPos - width/2 - i->getX() - i->getW()/2 - 2*xVel * deltaTime / 1000>= 0 && xVel < 0)
                crashPossRight = - (xPos - width/2 - i->getX() - i->getW()/2 - 2*xVel * deltaTime / 1000) / xVel;

            if (qAbs(i->getY() - yPos)*2 < height + i->getH() && (i->getX() - xPos)*2 <= i->getW() + width && i->getX() - xPos != 0)
                if (crashPossLeft >= 0 && (crashPossLeft < crashPossTop || crashPossTop == -1) && (crashPossLeft < crashPossBottom || crashPossBottom == -1)) {
                    crashOnBlockLeft = true;
                    //qDebug() << "Crashed Left: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                    xPos = i->getX() - i->getW()/2 - width/2;
            }
            if (qAbs(i->getY() - yPos)*2 < height + i->getH() && (xPos - i->getX())*2 <= i->getW() + width && i->getX() - xPos != 0){
                if (crashPossRight >= 0 && (crashPossRight < crashPossTop || crashPossTop == -1) && (crashPossRight < crashPossBottom || crashPossBottom == -1)){
                    //qDebug() << "Crashed Right: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                    crashOnBlockRight = true;
                    xPos = i->getX() + i->getW()/2 + width/2;
            }}
            if (qAbs(i->getX() - xPos)*2 < width + i->getW() && (i->getY() - yPos)*2 <= i->getH() + height && i->getY() - yPos != 0)
                if (crashPossTop >= 0 && (crashPossTop < crashPossLeft || crashPossLeft == -1)&& (crashPossTop < crashPossRight ||  crashPossRight == -1)){
                    crashOnBlockTop = true;
                    //qDebug() << "Crashed Top: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                    yPos = i->getY() - i->getH()/2 - height/2;
            }
            if (qAbs(i->getX() - xPos)*2 < width + i->getW() && (yPos - i->getY())*2 <= i->getH() + height && i->getY() - yPos != 0)
                if (crashPossBottom >= 0 && (crashPossBottom < crashPossLeft || crashPossLeft == -1) && (crashPossBottom < crashPossRight || crashPossRight == -1)){
                    crashOnBlockBottom = true;
                    //qDebug() << "Crashed Bottom: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                    yPos = i->getY() + i->getH()/2 + height/2;
            }
        }
        if (i->mShape == i->Circle){
            float xDis = qAbs(i->getX()-xPos - xVel * deltaTime / 1000);
            float yDis = qAbs(i->getY()-yPos - yVel * deltaTime / 1000);
            if ((i->getW()+width)*(i->getW()+width) > 4*(xDis*xDis+yDis*yDis)){
                xVel = yVel = 0;
            }
        }
    }
    Being::update(deltaTime);
}

void Player::processInput(QVector<QString>* vec){
    if (vec->contains("w")){
        tVel = 400;
    }
    else if (vec->contains("s")){
        tVel = -400;
    }
    else {tVel = 0;}
    if (vec->contains("e")){
        nVel = 200;
    }
    else if (vec->contains("q")){
        nVel = -200;
    }
    else {nVel = 0;}
    if (vec->contains("a"))
        rotVel = -3;
    else if (vec->contains("d"))
        rotVel = 3;
    else rotVel = 0;

    if (vec->contains("j") && mTime > 500){
        int fire = rand() % 15 + 7;
        mGame->addObject(new Bullet(mGame, this, xPos, yPos, rot, 1000, 10, 10, fire));
        mTime = 1;
    }
}

Player::Player(Game* g, float x, float y, int w, int h)
    :Being(g, x, y, w, h){
    width = 50;
    height = 50;
    mType = "Player";
    mInfo = "Basic";
}

Player::~Player(){
}
