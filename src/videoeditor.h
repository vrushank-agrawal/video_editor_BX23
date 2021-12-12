//
// Created by nhatv on 11 Nov 2021.
//

#ifndef VIDEO_EDITOR_BX23_VIDEOEDITOR_H
#define VIDEO_EDITOR_BX23_VIDEOEDITOR_H

#include <QMainWindow>
#include <QListView>
#include <audiomanager.h>
#include <imagethumbnail.h>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoEditor; }
QT_END_NAMESPACE

class VideoEditor : public QMainWindow {
Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor() override;

signals:
    void imageChanged();
    void positionChanged(int position);
    void timeIndicatorChanged(double timeInSec);

public slots:
    void importMedia();
    void importImage();
    void importImages();
    void importAudio();
    void importAudios();
    void blurImage();
    void updatePosition(int position);
    void updateTimeIndicator(double time);
    void appendImageToThumbnail(QListWidgetItem*);

private:
    QSet<QString> imageFileTypes;
    QSet<QString> audioFileTypes;
    QString imageFileTypesFilter;
    QString audioFileTypesFilter;
    int position = 0, fps = 30;
    double timeInSec;
    Ui::VideoEditor *ui;
    std::vector<QPixmap> images;
    int imageIndex = -1; // index of image need to displayed in images
    void updateVideo(const cv::VideoCapture &video);
    void setupVideoPlayer();
    void setupMenus();
    void setupWidgets();
    QString importFile(const QString& caption, const QString& startingDirectory, const QString& filter);
    QStringList importFiles(const QString& caption, const QString& startingDirectory, const QString& filter);
    void importImage(const QString& dir);
    void importAudio(const QString& dir);
    ThumbnailManager *thumbnailManager;
    AudioManager *audioManager;
};


#endif //VIDEO_EDITOR_BX23_VIDEOEDITOR_H
