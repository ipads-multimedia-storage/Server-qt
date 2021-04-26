#include "Master.h"
#include "object.h"
#include "widgets.h"
#include <iostream>
#include <QDateTime>

Master::Master()     //直接调用初始化函数
    :QThread(0){
}

Master::~Master(){
}

void Master::run(){
    timer = new QElapsedTimer();
    timer->start();
    qDebug() << "[Process] START GAME";

    while(isRunning){
        int deltaTime = timer->elapsed();

        //默认每秒50帧
        while(deltaTime < 20){
            deltaTime = timer->elapsed();
        }
        FPS = 1000 / deltaTime;
        timer->restart();

        //简简单单的循环，响应输入->计算更新->输出
        ProcessInput();
        Update(deltaTime);
        GenerateOutput();
    }
}

void Master::Initialize(){       //目前暂时直接调出一个游戏组件，以后应该显示主界面，而游戏组件是主界面的一部分
    isRunning = true;
    Paused = false;
}


void Master::ProcessInput(){
    //这个函数从Server中获取待处理请求，逐个响应
    Request* request = mServer->getRequest();
    while(request){
        //qDebug() << "Parse Request: " << request->event_time <<request->type << request->id;
        if (request->type == "NEW"){
            QJsonDocument jsonDoc = QJsonDocument::fromJson(request->content.data());
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.value("type") == "Camera"){
                Camera* newCamera = new Camera(this, request->id);
                newCamera->setXY(jsonObj.value("x").toDouble(), jsonObj.value("y").toDouble());
                newCamera->setWH(jsonObj.value("w").toInt(), jsonObj.value("h").toInt());
                newCamera->setRot(jsonObj.value("rot").toDouble());
                addObject(newCamera);
            }
            if (jsonObj.value("type") == "Arm"){
                Arm* newArm = new Arm(this, request->id);
                newArm->setXY(jsonObj.value("x").toDouble(), jsonObj.value("y").toDouble());
                newArm->setRadius(jsonObj.value("r").toDouble());
                newArm->setRot(jsonObj.value("rot").toDouble());
                addObject(newArm);
            }
            if (jsonObj.value("type") == "Belt"){
                Belt* newBelt = new Belt(this, request->id);
                newBelt->setXY(jsonObj.value("x").toDouble(), jsonObj.value("y").toDouble());
                newBelt->setWH(jsonObj.value("w").toInt(), jsonObj.value("h").toInt());
                newBelt->setRot(jsonObj.value("rot").toDouble());
                newBelt->setSpeed(jsonObj.value("s").toDouble());
                addObject(newBelt);
            }
        }
        if (request->type == "POST"){
            mObjMap[request->id]->setFromBytes(request->content);
        }
        if (request->type == "STAT"){
            switch (request->content.toInt()) {
            case 0:
                mObjMap[request->id]->setStatus(Object::IDLE);
            case 1:
                mObjMap[request->id]->setStatus(Object::BUSY);
            case 2:
                mObjMap[request->id]->setStatus(Object::OFFLINE);
            }
        }
        mObjMap[request->id]->setEventTime(request->event_time);
        request = mServer->getRequest();
    }
}

void Master::Update(int deltaTime){
    //这个函数遍历更新所有对象，并处理对象增删
    if (Paused) deltaTime = 0; //若暂停则让时间静止

    //遍历更新所有对象，同时检查对象是否待删除
    for(auto i : mObjects){
        i->update(deltaTime);
        if (i->getClearStatus())
            mDeadObjects.push_back(i);
    }

    //将待加入的对象按显示优先级加入对象表，同时加入Widget中等待显示
    for(auto i : mPendingObjects){
        int j;
        for(j = 0; j < mObjects.length(); j++)
            if(mObjects[j]->getPaintOrder() < i->getPaintOrder()){
                break;
        }
        mObjects.insert(j, i);
        if (i->getType() == "Block"){
            mBlocks.push_back(static_cast<Block*>(i));
        }

        qDebug() << "[Game] Add Object:" << i->getType();
        gameWidget->addObject(i);
    }
    mPendingObjects.clear();

    //将死亡的待删除的对象从对象表中移除
    for(auto i : mDeadObjects){

        mObjects.removeOne(i);
        qDebug() << "[Game] Remove Object:" << i->getType();
        gameWidget->removeObject(i);

        delete i;
    }
    mDeadObjects.clear();

}

void Master::GenerateOutput(){       //调用游戏组件的update，注意这个update默认包含了paintEvent
    //在Widget中显示目前的对象
    gameWidget->paint();

    //待完成：调用mServer->sendTo(msg, id)发送待发送的指令到客户端
    //mServer->sendTo("{\"test\":\"hello!\"}", 0);
}

void Master::Pause(){
    Paused = true;
    timer->restart();
    qDebug() << "[Process] Pause";
}

void Master::unPause(){
    Paused = false;
    timer->restart();
    qDebug() << "[Process] Continue";
}


//这些函数处理鼠标键盘事件（暂时无用）

void Master::keyPress(QString key){
    mPressedKeys.push_back(key);
    if (key == "p"){
        if (Paused)
            unPause();
        else
            Pause();
    }
    if (key == "a"){
        addBlock(new Block(this, 500, 850, 20, 20));
    }
    if (key == "d" && mSelect){
        removeObject(mSelect);
    }
    if (key == "s" && mSelect){
        mSelect->mStatus = Object::IDLE;
    }
}

void Master::keyRelease(QString key){
    mPressedKeys.removeOne(key);
}

void Master::mouseMove(int x, int y){
    mouseX = x; mouseY = y;
}

void Master::mousePress(int x, int y){
    mouseX = x; mouseY = y;
    //设置选中的对象
    if (mSelect){
        mSelect->setSelected(false);
        mSelect = nullptr;
    }
    for (auto i : mObjects){
        //if (x <= i->getX()+i->getW()/2 && x >= i->getX()-i->getW()/2 && y <= i->getY()+i->getH()/2 && y >= i->getY()-i->getH()/2){
        if (i->contains(x, y)){
            mSelect = i;
            i->setSelected(true);
            break;
        }
    }
}

void Master::mouseRelease(int x, int y){
    mouseX = x; mouseY = y;
}

void Master::addObject(class Object *obj){
    //添加对象，不能直接添加到对象列表，而是加入缓冲区，是为了避免遍历列表过程中改变列表
    mPendingObjects.push_back(obj);
    if (obj->id >= 0)
        mObjMap[obj->id] = obj;
}

void Master::removeObject(class Object *obj){
    //移除对象，除了从总对象列表中移除，还要从一些特殊列表中移除
    obj->setClearStatus(true);
    if (mBlocks.contains(static_cast<Block*>(obj)))
        mBlocks.removeOne(static_cast<Block*>(obj));
    if (mArms.contains(static_cast<Arm*>(obj))){
        mArms.removeOne(static_cast<Arm*>(obj));
    }
    for (auto i :mObjMap.keys()){
        if (mObjMap[i] == static_cast<Arm*>(obj))
            mObjMap.remove(i);
    }
}

void Master::addBlock(class Block *block){
    addObject(block);
}

void Master::addArm(class Arm *arm){
    mArms.push_back(arm);
    addObject(arm);
}

void Master::removeEverything(){
    for(auto i: mObjects){
        removeObject(i);
    }
}
