//
// Created by nhatv on 11 Nov 2021.
//

#ifndef VIDEO_EDITOR_BX23_VIDEOEDITOR_H
#define VIDEO_EDITOR_BX23_VIDEOEDITOR_H

#include <QMainWindow>
#include <QListView>
#include <string>
#include <audiomanager.h>
#include <imagethumbnail.h>
#include <video.h>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoEditor; }
QT_END_NAMESPACE

class VideoEditor : public QMainWindow {
Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor() override;

signals:
    void positionChanged(int position);
    void currentTimeChanged(double timeInSec);
    void changeFrame(cv::Mat frame);

public slots:
    void resetImage();
    void updateBlurLevel();
    void imageSelected();
    void importMedia();
    void importImages();
    void importAudios();
    void blurImage();
    void rotateImageRight();
    void updatePosition(int newPosition);
    void updateCurrentTime(double time);
    void appendImageToThumbnail(QListWidgetItem*);
    void writeVideo();
    void addImageToResultVideo(img::Image *image, double startTime, double duration, vid::Animation animation);
    void deleteImageFromResultVideo(img::Image *image);
    void applyAnimation(img::Image *image, vid::Animation animation);

private:
    vid::Video *resultVideo;
    QSet<QString> imageFileTypes;
    QSet<QString> audioFileTypes;
    QString imageFileTypesFilter;
    QString audioFileTypesFilter;
    int fourcc;
    int position = 0, fps = 30, numberFrame = fps * 5 * 60;
    double timeInSec;
    Ui::VideoEditor *ui;
    void setupVideoPlayer();
    void setupMenus();
    void setupWidgets();
    void setupVideoClass();
    QStringList importFiles(const QString& caption, const QString& startingDirectory, const QString& filter);
    void importImage(const QString& dir);
    void importAudio(const QString& dir);
    ThumbnailManager *thumbnailManager;
    AudioManager *audioManager;
};


#endif //VIDEO_EDITOR_BX23_VIDEOEDITOR_H
