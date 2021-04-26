#include "prop.h"
#include "Game.h"


Prop::Prop(class Game* g, float x, float y, int w, int h)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h;
    mType = "Prop";
    mInfo = "Basic";
}

Prop::~Prop(){

}

void Prop::collected(class Being *being){
    setOwner(being);
    being->addProp(this);
}
