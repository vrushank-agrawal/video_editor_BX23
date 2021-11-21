//
// Created by nhatv on 11 Nov 2021.
//

#ifndef VIDEO_EDITOR_BX23_VIDEOEDITOR_H
#define VIDEO_EDITOR_BX23_VIDEOEDITOR_H

#include <QMainWindow>
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class VideoEditor; }
QT_END_NAMESPACE

class VideoEditor : public QMainWindow {
Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);

    ~VideoEditor() override;

private:
    Ui::VideoEditor *ui;
};


#endif //VIDEO_EDITOR_BX23_VIDEOEDITOR_H