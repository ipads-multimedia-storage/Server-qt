#include "camera.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "asmOpenCV.h"

using namespace cv;

Camera::Camera(Master* g, int i)
    :Object(g, i)
{
    mColor = QColor(100, 150, 200);
    paintOrder = 20;
    mType = "Camera";
}

Camera::~Camera(){

}

void Camera::paint(class QPainter* p){
    Object::paint(p);
    if (isVisible){
        p->save();
        p->translate(xPos, yPos);
        p->rotate(rot*180/acos(-1));
        p->setBrush(Qt::NoBrush);
        if (isSelected){
            p->drawImage(QRect(int( - width/2), int( - height/2), width, height), mImage);
        }
        p->drawRect(int( - width/2), int( - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
}
void Camera::update(int deltaTime)
{
    Object::update(deltaTime);
    //此处完成：图像处理 and 添加物体(Block)并设定位置速度参数
    Scalar rgba_min(0, 151, 100);
    Scalar rgba_max(255, 255, 255);
    double MIN_BLOB_AREA = 5000;
    double MAX_BLOB_AREA = 100000;

    if (imageNotEmpty) {
        int camWidth = mImage.width();
        int camHeight = mImage.height();

        Mat cvtProcessed, colorSelected, frame = ASM::QImageToCvMat(mImage);
        imwrite("wrong.png", frame);
        cvtColor(frame, cvtProcessed, COLOR_BGR2Lab);
        inRange(cvtProcessed, rgba_min, rgba_max, colorSelected);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        findContours(colorSelected, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
        int i = 1;
        for (auto it  = contours.begin(); it != contours.end(); ++it) {
            double area = contourArea(*it);
            if (area < MAX_BLOB_AREA && area > MIN_BLOB_AREA) {
                cv::RotatedRect rotateRect = cv::minAreaRect(contours[0]); //轮廓最小外接矩形
                float camX = rotateRect.center.x, camY = rotateRect.center.y;
                float worldx = width * camX / camWidth - width/2, worldy = height * camY / camHeight - height/2;
                float worldX = xPos +worldy, worldY = yPos - worldx;
                float angle = rotateRect.angle;
                qDebug() << '[' << i++ << "] " << worldX << ',' << worldY << angle;
                if (mMaster->getBlocks()->empty()) {
                    mMaster->addObject(new Block(mMaster, worldX, worldY, 20, 20, angle, QColor(255, 0, 0)));
                    return;
                }
                for (auto item : *mMaster->getBlocks()) {
                    if ((worldX - item->getX())*(worldX - item->getX()) + (worldY - item->getY())*(worldY - item->getY()) > 2500) {
                        mMaster->addObject(new Block(mMaster, worldX, worldY, 20, 20, angle, QColor(255, 0, 0)));
                        return;
                    } else {
                        item->setXY(worldX, worldY);
                        item->setRot(angle);
                    }
                }
            }
        }
    }
}


void Camera::setFromBytes(QByteArray array){
    //qDebug() << "Set Camera Image: " << array.length() << width << height;
    mImage = QImage::fromData(array);
    imageNotEmpty = true;
}
