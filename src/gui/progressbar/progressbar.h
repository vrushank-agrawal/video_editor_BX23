//
// Created by Minh Tung Nguyen on 29/11/2021.
//

#ifndef VIDEO_EDITOR_BX23_PROGRESSBAR_H
#define VIDEO_EDITOR_BX23_PROGRESSBAR_H

#include <QSlider>
#include <QMouseEvent>
#include "time.h"

class ProgressBar: public QSlider
{
Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();

signals:
    void frameChanged(int position);

private:
    std::time_t lastUpdateTime = 0;
    bool pressed = false;
    virtual void updateWhenPress();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif //VIDEO_EDITOR_BX23_PROGRESSBAR_H
