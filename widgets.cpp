#include "widgets.h"

#include "Master.h"
#include "server.h"
#include "object.h"
#include <QKeyEvent>

GameWidget::GameWidget(QWidget *parent)  //游戏组件的初始化，目前还很粗糙
    : QScrollArea(parent)
{
    resize(1000, 1000);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameWidget::addObject(class Object *obj){  //添加对象
    qDebug() << "[GameWidget] Add Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    int i;
    for(i = 0; i < mObjects.length(); i++)
        if(mObjects[i]->getPaintOrder() > obj->getPaintOrder()){
            break;
    }
    mObjects.insert(i, obj);
}

void GameWidget::removeObject(class Object *obj){
    qDebug() << "[GameWidget] Remove Object:" << obj->getType() << " At: "<< obj->getX() << "," << obj->getY();
    mObjects.removeOne(obj);
}

GameWidget::~GameWidget()
{
}

void GameWidget::Initialize(){
    setStyleSheet("QScrollArea{border:0px solid #707070}");

    mMap = new Map(this, this);

    setWidget(mMap);
}

void GameWidget::paint(){ //遍历所有对象，逐个调用对象的paint方法，直接传送QPainter
    mMap->update();
}

void GameWidget::keyPressEvent(QKeyEvent *k){
    if (k->text().length()>0)
        emit(keyPressSignal(k->text()));
    else{
        if (k->key() == Qt::Key_Right)
            emit(keyPressSignal("Right"));
        if (k->key() == Qt::Key_Left)
            emit(keyPressSignal("Left"));
        if (k->key() == Qt::Key_Up)
            emit(keyPressSignal("Up"));
        if (k->key() == Qt::Key_Down)
            emit(keyPressSignal("Down"));
    }
    QWidget::keyPressEvent(k);
}

void GameWidget::keyReleaseEvent(QKeyEvent *k){
    if (k->text().length()>0)
        emit(keyReleaseSignal(k->text()));
    else{
        if (k->key() == Qt::Key_Up)
            emit(keyReleaseSignal("Up"));
        if (k->key() == Qt::Key_Down)
            emit(keyReleaseSignal("Down"));
        if (k->key() == Qt::Key_Left)
            emit(keyReleaseSignal("Left"));
        if (k->key() == Qt::Key_Right)
            emit(keyReleaseSignal("Right"));
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *k){
    emit(mouseMoveSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mouseMoveEvent(k);
}

void GameWidget::mousePressEvent(QMouseEvent* k){
    emit(mousePressSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mousePressEvent(k);
}

void GameWidget::mouseReleaseEvent(QMouseEvent* k){
    emit(mouseReleaseSignal(k->x() + horizontalScrollBar()->value(), k->y() + verticalScrollBar()->value()));
    QScrollArea::mouseReleaseEvent(k);
}


void Map::paintEvent(QPaintEvent* ){
    QPainter p(this);
    p.drawText(1, 16, "Time Now:"+QString::number(qlonglong(mMaster->getCurrentTime())));
    for(auto i : *mGameWidget->getObjects())
        i->paint(&p);
}

Map::Map(QWidget* parent, GameWidget* gameWidget)
    :QWidget(parent){
    mGameWidget = gameWidget;
    mMaster = mGameWidget->getGame();

    resize(1000, 1500);

    //mMaster->addBlock(new Block(mMaster, 500, 850, 20, 20));
    //mMaster->addObject(new Belt(mMaster, 5));

    setMouseTracking(true);
}

Map::~Map(){

}

MainWindow::MainWindow(QWidget *parent)  //没用
    : QMainWindow(parent)
{
    resize(1350, 1000);
    //setWindowFlag(Qt::FramelessWindowHint);

    QString font;

    QFile fontFile("BulletproofBB.ttf");
    if(!fontFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open font file error";
    }

    int loadedFontID = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty())
    {
        font = loadedFontFamilies.at(0);
    }
    fontFile.close();

    qDebug() << font;

    mMaster = new Master();
    mServer = new Server();
    qDebug() << "[Build] Game Core Built";

    mGameWidget = new GameWidget(this);
    mGameWidget->hide();
    mGameWidget->setGame(mMaster);
    mMaster->setGameWidget(mGameWidget);
    mMaster->setServer(mServer);

    qDebug() << "[Build] GameWidget Built";

    if (mGameWidget){
        connect(mGameWidget, &GameWidget::keyPressSignal, mMaster, &Master::keyPress);
        connect(mGameWidget, &GameWidget::keyReleaseSignal, mMaster, &Master::keyRelease);
        connect(mGameWidget, &GameWidget::mouseMoveSignal, mMaster, &Master::mouseMove);
        connect(mGameWidget, &GameWidget::mousePressSignal, mMaster, &Master::mousePress);
        connect(mGameWidget, &GameWidget::mouseReleaseSignal, mMaster, &Master::mouseRelease);
   }else qDebug() << "[Warning] Game: Fail to Connect with GameWidget";

    setWindowTitle("Server - MainWindow");
    show();

    mWelWidget = new WelcomeWidget(this);

    mSetWidget = new SettingsWidget(this);

}

void MainWindow::StartGame(){
    resize(1000, 1000);
    qDebug() << "[Build] Start Game";
    mWelWidget->hide();
    mGameWidget->show();
    setCentralWidget(mGameWidget);
    mGameWidget->Initialize();
    mMaster->Initialize();
    qDebug() << "[Build] Game Initialized Succeessfully";

    mGameWidget->setFocus();
    mGameWidget->setMouseTracking(true);
    setMouseTracking(true);
    mMaster->start();
    isStarted = true;
}

void MainWindow::saveAndExit(){
    mMaster->removeEverything();
    mWelWidget->show();
    qDebug() << "[System] Game Saved Succeessfully";

    isStarted = false;
}

void MainWindow::ChangePause(){
    if (mGameWidget->isHidden()){
        mGameWidget->show();
        mGameWidget->setFocus();
        mMaster->unPause();
    }
    else{
        mGameWidget->hide();
        mMaster->Pause();
    }
}


void MainWindow::WelToSet(){
    mWelWidget->hide();
    mSetWidget->show();
}

void MainWindow::SetToWel(){
    mSetWidget->hide();
    mWelWidget->show();

    mSetWidget->save();
}

MainWindow::~MainWindow()
{
}



WelcomeWidget::WelcomeWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    playButton = new QPushButton("Start Server", this);
    settingButton = new QPushButton("Settings", this);
    exitButton = new QPushButton("X", this);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(230);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

//    label = new QLabel("HAPPY", this);
//    label2 = new QLabel("519021910798 蒋哲", this);

//    label->move(215, 180);
//    label->resize(970, 300);
//    label2->move(1000, 920);
//    label2->resize(300, 50);
//    label->setFont(font);
//    font.setPixelSize(30);
//    label2->setFont(font);

    playButton->move(325, 500);
    playButton->resize(700, 150);
    connect(playButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::StartGame);

    settingButton->move(325, 700);
    settingButton->resize(700, 150);
    connect(settingButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::WelToSet);

    exitButton->move(1190, 0);
    exitButton->resize(160, 120);
    connect(exitButton, &QPushButton::clicked, parent, &QWidget::close);

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );

    show();
}

WelcomeWidget::~WelcomeWidget(){

}


SettingsWidget::SettingsWidget(QWidget* parent)
    :QWidget(parent){
    resize(1350, 1000);

    QFont font;
    font.setFamily("Bulletproof BB");
    font.setPixelSize(140);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);

    label = new QLabel("Settings",this);
    label->move(320, 50);
    label->resize(1000, 200);
    label->setFont(font);

    backButton = new QPushButton("<<<", this);
    backButton->move(0, 0);
    backButton->resize(160, 120);
    connect(backButton, &QPushButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::SetToWel);

    mAddress = new QLineEdit(this);
    mAddress->move(320, 400);
    mAddress->resize(600, 60);
    mAddress->setText("122.230.103.223:50080");

    setStyleSheet(""
                  "QPushButton:hover{"
                  "background:#FFFFFF;"
                  " border:10px solid #000000;"
                  " padding: 0px; "
                  " color: #000000; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  "QPushButton{"
                  "background:#000000;"
                  " border:10px solid #AAAAAA;"
                  " padding: 0px; "
                  " color: #FFFFFF; "
                  " font-family: 'Bulletproof BB';"
                  " font-size: 80px;"
                  "}"
                  );
    hide();
}

void SettingsWidget::save(){
    ADDR = mAddress->text();
}

SettingsWidget::~SettingsWidget(){

}


