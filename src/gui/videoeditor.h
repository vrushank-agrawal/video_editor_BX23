//
// Created by nhatv on 11 Nov 2021.
//

#ifndef VIDEO_EDITOR_BX23_VIDEOEDITOR_H
#define VIDEO_EDITOR_BX23_VIDEOEDITOR_H

#include <QMainWindow>
#include <QListView>
#include "imagethumbnail/imagethumbnail.h"


QT_BEGIN_NAMESPACE
namespace Ui { class VideoEditor; }
QT_END_NAMESPACE

class VideoEditor : public QMainWindow {
Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    void loadImage(const QString &path);
    ~VideoEditor() override;

public slots:
    void importImage();
    void setDisplayImage();

private:
    Ui::VideoEditor *ui;
    std::vector<QPixmap> images;
    int imageIndex = -1; // index of image need to displayed in images
    void setupMenus();
    void setupWidgets();
    void setupImageListWidget();
};


#endif //VIDEO_EDITOR_BX23_VIDEOEDITOR_H
