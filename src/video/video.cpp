//
// Created by korkot on 11/21/2021.
//
#include "video.h"

using namespace vid;
using namespace std;
using namespace img;


void Video::test() {
    cout << "OK" << endl;
}

Video::Video(int width, int height, int fps) {
    this->number_of_animations = 0;
    this->Clear();
    this->fps = fps;
    Mat blank_img_mat(500, 1000, CV_8UC3, Scalar(0, 0, 0));
    this->blank = Image(blank_img_mat);
    this->Resize(width, height);
}

Video::Video(Image **images, double *start_times, double *times_of_display, int size, int width, int height, int fps)
        : Video(width, height, fps) {
    for (int index = 0; index < size; index++) {
        this->Insert(images[index], start_times[index], times_of_display[index]);
        number_of_animations++;
    }
}

Video::~Video() {
    this->Clear();
}

void Video::addImage(Image *img, double start_time, double time_to_display) {
    int index = this->GetPlace(start_time);
    this->Insert(img, start_time, time_to_display, index);
}

void Video::deleteImage(Image *img) {
    int to_remove = this->GetIndex(img);
    if (to_remove != -1) {
        this->Remove(to_remove);
    }
}


void Video::insertImage(Image *image, double start_time, double time_to_display) {
    image->equalizeImgDim(this->width, this->height);
    this->image_pointers.push_back(image);
    ImageAnimator new_animator = ImageAnimator(image, start_time, time_to_display, this->fps);
    this->animators.push_back(new_animator);
    this->number_of_animations++;
}

void Video::insertImage(Image *image, double start_time, double time_to_display, int index) {
    image->equalizeImgDim(this->width, this->height);
    if (index > this->number_of_animations) {
        cout << "Index is out of reach";
    } else if (index == this->number_of_animations) {
        this->Insert(image, start_time, time_to_display);
    } else {
        this->image_pointers.insert(this->image_pointers.begin() + index, image);
        ImageAnimator new_animator = ImageAnimator(image, start_time, time_to_display, this->fps);
        this->animators.insert(this->animators.begin() + index, new_animator);
        this->number_of_animations++;
    }
}

void Video::removeImageAtIndex(int index) {
    if (index >= this->number_of_animations) {
        cout << "Index out of reach" << endl;
        return;
    }
    this->image_pointers.erase(this->image_pointers.begin() + index);
    this->animators.erase(this->animators.begin() + index);
    number_of_animations--;
}

void Video::clear() {
    this->image_pointers.clear();
    this->animators.clear();
    this->number_of_animations = 0;
    this->width = 0;
    this->height = 0;
}

Mat Video::getMatAtFrame(int frame_number) {
    for (int i = 0; i < this->number_of_animations; i++) {
        int start = (int) this->animators[i].start_time * fps;
        int end = (int) (this->animators[i].start_time + this->animators[i].time) * fps;
        if (start <= frame_number && frame_number <= end) {
            return this->animators[i].GetMatAt(frame_number - start);
        }
        if (i < number_of_animations - 1) {
            int next_start = (int) this->animators[i + 1].start_time * fps;
            if (end < frame_number && frame_number < next_start) {
                return this->blank.getModifiedImg();
            }
        } else {
            return this->blank.getModifiedImg();
        }
    }
    return this->blank.getModifiedImg();
}

Mat Video::getMatByTime(double time) {
    int frame_number = time * fps;
    return this->GetMat(frame_number);
}

void Video::resize(int width, int height) {
    this->width = width;
    this->height = height;
    for (int index = 0; index < this->number_of_animations; index++) {
        this->image_pointers[index]->resizeImg(width, height);
    }
    //this->blank.equalizeImgDim(width, height);
}

int Video::animationNumber() {
    return this->number_of_animations;
}

void Video::displayCurrentVideo() {
    double cur_time = 0;
    for (int i = 0; i < number_of_animations; i++) {
        double blank_time_dur = animators[i].time - cur_time;
        if (blank_time_dur > 0) {
            this->ShowBlank(blank_time_dur);
        }
        animators[i].Display();
    }
    destroyAllWindows();
}

void Video::writeVideo(string output_name) {
    VideoWriter video_writer(output_name, VideoWriter::fourcc('m', 'p', '4', 'v'),
                             this->fps, Size(this->width, this->height));

    double cur_time = 0;
    for (int i = 0; i < this->number_of_animations; i++) {
        double blank_time_dur = animators[i].start_time - cur_time;
        if (blank_time_dur > 0) {
            this->AddBlank(video_writer, blank_time_dur);
        }
        this->animators[i].Write(video_writer);
        cur_time = this->animators[i].start_time + this->animators[i].time;
    }
    video_writer.release();
    destroyAllWindows();
}

//Supposed to be a binary search
int Video::getPlace(double time) {
    int index = 0;
    for (int i = 0; i < this->number_of_animations; i++) {
        if (time < this->animators[i].time) {
            break;
        }
        index++;
    }
    return index;
}

int Video::getIndex(Image *img) {
    for (int i = 0; i < number_of_animations; i++) {
        if (this->image_pointers[i] == img) {
            return i;
        }
    }
    cout << "Video class doesn't contain this object" << endl;
    return -1;
}

Image *Video::getImgAtIndex(int index) {
    if (0 > index || index >= this->number_of_animations) {
        cout << "Index out of range" << endl;
        return NULL;
    }
    return this->image_pointers[index];
}

void Video::showBlank(double time) {
    int i = 0;
    int num_fames = time * fps;
    while (i < num_fames) {
        imshow("Frame", this->blank.getModifiedImg());
        char c = (char) waitKey(1);
        if (c == 27)
            break;
        i++;
    }
}

void Video::addBlank(VideoWriter video_writer, double time) {
    int i = 0;
    int num_fames = time * this->fps;
    while (i < num_fames) {
        video_writer.write(this->blank.getModifiedImg());
        i++;
    }
}

void Video::applyAnimation(Image *img, Animation animation_type) {
    int index = this->GetIndex(img);
    this->animators[index].SetAnimation(animation_type);
}


//*****************************************ImageAnimator struct***********************************************

Video::ImageAnimator::ImageAnimator(Image *image, double start_time, double display_time, int fps) {
    this->image = image;
    this->start_time = start_time;
    this->time = display_time;
    this->fps = fps;
    this->animation_type = Normal;
    this->InitFunctions();
}


Video::ImageAnimator::ImageAnimator(Image *image, double start_time, double display_time, int fps,
                                    animation animation_type)
        : ImageAnimator(image, start_time, display_time, fps) {
    this->animation_type = animation_type;
}


Video::ImageAnimator::~ImageAnimator() {
    //To modify
}

Video::ImageAnimator::ImageAnimator() {

}

void Video::ImageAnimator::initFunctions() {
    this->anim_functions[0] = &ImageAnimator::NormalDisplay;
    this->anim_functions[1] = &ImageAnimator::RotateAnimation;
}

void Video::ImageAnimator::setAnimation(Animation animation_type) {
    this->animation_type = animation_type;
}

void Video::ImageAnimator::display() {
    int i = 0;
    int num_fames = this->time * fps;
    while (i < num_fames) {
        Mat disp = (this->*anim_functions[animation_type])(i);
        imshow("Frame", disp);
        char c = (char) waitKey(1);
        if (c == 27)
            break;
        i++;
    }
}

void Video::ImageAnimator::write(VideoWriter video_writer) {
    int i = 0;
    int num_fames = this->time * this->fps;
    while (i < num_fames) {
        Mat disp = this->GetMatAt(i);
        video_writer.write(disp);
        i++;
    }
}

Mat Video::ImageAnimator::getMatAt(int frame_number) {
    return (this->*anim_functions[animation_type])(frame_number);
}


Mat Video::ImageAnimator::normalDisplay(int frame_number) {
    return this->image->getModifiedImg();
}


Mat Video::ImageAnimator::rotateAnimation(int frame_number) {
    int angle = 5 * frame_number;
    double frame_angle = angle;
    Mat modified_image = this->image->getModifiedImg();
    Image new_image = Image(modified_image);
    new_image.rotateImg(frame_angle);
    return new_image.getModifiedImg();
}


//************************************************************************************************

//Not tested
void Video::ImageAnimator::zoomAnimation(int frame_number) {
    int ratio = 0.2;
    int num_frame = fps * time;
    double change_per_frame = 1 + ratio;
    int img_h = this->image->getMat().size().height;
    int img_w = this->image->getMat().size().width;
    for (int i = 1; i <= num_frame; i++) {
        img_h *= change_per_frame;
        img_w *= change_per_frame;
        this->image->resizeImg(img_w, img_h);
        imshow("Frame", this->image->getModifiedImg());
        char c = (char) waitKey(1);
        if (c == 27)
            break;
        change_per_frame += ratio;
    }
}


/*
//Not tested
void Video::ImageAnimator::CropAnimation() {
    int num_frame = fps * time;
    int h = this->image->getMat().rows;
    int w = this->image->getMat().cols;
    int window_h = h / 2;

    for (int i = 1; i <= num_frame; i++) {
        while (10 * i + window_h < h and 10 * i + window_h < w) {
            Mat new_img = this->image->getMat().clone();
            cv::Rect myROI(10 * i, 10 * i, window_h, window_h);
            Mat cropped = new_img(myROI);
            imshow("Frame", cropped);
            char c = (char) waitKey(1);
            if (c == 27)
                break;
        }

    }
}
    
    

//Needs testing... 
//The collage animation using stitching / collage functions from image class 
void Video::ImageAnimator::CollageAnimation() { 
    
        Image image_mat = img::Image(this->img); // transforming matrix from video class into image so that we can use image functions 
        
        imshow( "Frame", image_mat);
        char c = (char)waitKey(time/4);
        if( c == 27 )
            break;

        int initial_width = image_mat.getMat().size().width;
        int initial_height = image_mat.getMat().size().height; 
        
        vector<Image> imageArr2 = {image_mat, image_mat}; 
        Collage collage2 = Collage(imageArr2);
        collage2.twoStitch();
        Image collage_img2 = Image(collage2.getModifiedImage());
        collage_img2.resizeImg(int initial_width, int initial_height);// resize the image after collage using intial height and width

        imshow( "Frame", collage_img2);
        char c = (char)waitKey(time/4);
        if( c == 27 )
            break;
        
        vector<Image> imageArr3 = {image_mat, image_mat, image_mat}; 
        Collage collage3 = Collage(imageArr3);
        collage3.threeStitch();
        Image collage_img3 = Image(collage3.getModifiedImage());
        collage_img3.resizeImg(int initial_width, int initial_height);
        imshow( "Frame", collage_img3);
        char c = (char)waitKey(time/4);
        if( c == 27 )
            break;

        vector<Image> imageArr4 = {image_mat, image_mat, image_mat, image_mat}; 
        Collage collage4 = Collage(imageArr4);
        collage4.fourStitch();
        Image collage_img4 = Image(collage4.getModifiedImage());
        collage_img4.resizeImg(int initial_width, int initial_height);
        imshow( "Frame", collage_img4);
        char c = (char)waitKey(time/4);
        if( c == 27 )
            break;
    }
}
*/
