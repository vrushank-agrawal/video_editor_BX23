//
// Created by Minh Tung Nguyen on 18/11/2021.
//

#include <QStyle>
#include <iostream>
#include "videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent) :
        QWidget(parent)
{
}

VideoPlayer::~VideoPlayer()
{
    delete timer;
}

void VideoPlayer::updatePicture(){
    if (!isPlaying) return;

    cv::Mat frame;
    if (video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
        {
            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);
            label->setOriginalPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
            label->updatePixmap();
            label->update();
            emit updateSlider(video.get(cv::CAP_PROP_POS_FRAMES));
        }
    }
}

void VideoPlayer::setChild(VideoWindow *label,
                           QToolButton *playButton)
{
    this->label = label;

    this->playButton = playButton;
    this->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    this->playButton->setToolTip(tr("Play"));
}

void VideoPlayer::updateVideo(const cv::VideoCapture &video) {
    this->video = video;

    if (timer != nullptr) delete timer;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePicture()));
    double fps = this->video.get(cv::CAP_PROP_FPS);
    timer->start(int(1000 / fps));
}

void VideoPlayer::play(){
    if (isPlaying) {
        this->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        this->playButton->setToolTip(tr("Play"));
    }
    else {
        this->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        this->playButton->setToolTip(tr("Play"));
    }
    isPlaying = !isPlaying;
}

void VideoPlayer::sliderMoved(int position){
    video.set(cv::CAP_PROP_POS_FRAMES, position);
}

void VideoPlayer::forward(){
    int currentFrame = video.get(cv::CAP_PROP_POS_FRAMES);
    int fps = video.get(cv::CAP_PROP_FPS);
    video.set(cv::CAP_PROP_POS_FRAMES, currentFrame + fps * 5);
}

void VideoPlayer::backward(){
    int currentFrame = video.get(cv::CAP_PROP_POS_FRAMES);
    int fps = video.get(cv::CAP_PROP_FPS);
    video.set(cv::CAP_PROP_POS_FRAMES, currentFrame - fps * 5);
}