//
// Created by nhatv on 11 Nov 2021.
//

#ifndef VIDEO_EDITOR_BX23_VIDEOEDITOR_H
#define VIDEO_EDITOR_BX23_VIDEOEDITOR_H

#include <QMainWindow>
#include <QListView>
#include <string>
#include <audiomanager.h>
#include <imagemanager.h>
#include <video.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <audioplayer.h>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoEditor; }
QT_END_NAMESPACE

class VideoEditor : public QMainWindow {
Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor() override;



signals:
    void changeFrame(cv::Mat frame);
    void currentTimeChanged(double timeInSec);
    void imageChanged();
    void positionChanged(int position);



public slots:
    /*###################
    *     GENERIC
    ####################*/
    void updatePosition(int newPosition);
    void updateCurrentTime(double time);
    void writeVideo();
    void updateVideoLength(double length);
    void addImagesByAudio();

    /*###################
    *      IMPORT
    ####################*/
    void importAudios();
    void importImages();
    void importMedia();

    /*###################
    *       AUDIO
    ####################*/
    void appendAudioToThumbnail(QListWidgetItem *item);

    /*###################
    *       IMAGE
    ####################*/
    void addImageToResultVideo(img::Image *image, double startTime, double duration, vid::Animation animation);
    void appendImageToThumbnail(QListWidgetItem*);
    void applyAnimation(img::Image *image, vid::Animation animation);
    void applyBlur(ImageItem *imageItem);
    void blurImage();
    void brightImage();
    void deleteImageFromResultVideo(img::Image *image);
    void imageSelected();
    void resetImage();
    void rotateImageRight();
    void updateBlurLevel();
    void updateBrightLevel();

    /*###################
    *       MENU
    ####################*/
    void openAboutPage();
    void playFromBeginning();


private:
    /*###################
    *       SETUP
    ####################*/
    void setupAudio();
    void setupImage();
    void setupImageToolbar();
    void setupImports();
    void setupMenus();
    void setupTimeline();
    void setupVideoClass();
    void setupVideoPlayer();
    void setupWidgets();

    /*###################
    *      IMPORT
    ####################*/
    void importAudio(const QString& dir);
    QStringList importFiles(const QString& caption, const QString& startingDirectory, const QString& filter);
    void importImage(const QString& dir);

    QSet<QString> audioFileTypes, imageFileTypes, mediaFileTypes;
    QString audioFileTypesFilter = "", imageFileTypesFilter = "", mediaFileTypesFilter = "";

    /*###################
    *      IMAGE
    ####################*/
    void updateFrame();

    /*###################
    *     ATTRIBUTE
    ####################*/
    AudioManager *audioManager = nullptr;
    AudioPlayer *audioPlayer = nullptr;
    ImageManager *imageManager = nullptr;
    vid::Video *resultVideo = nullptr;
    QSlider *blurSlider = nullptr;
    QSlider *brightSlider = nullptr;

    int fourcc = -1;
    int position = 0, fps = 30, numberFrame = 0;
    int width = 1280, height = 720;
    double timeInSec = 0;
    Ui::VideoEditor *ui;
};


#endif //VIDEO_EDITOR_BX23_VIDEOEDITOR_H
