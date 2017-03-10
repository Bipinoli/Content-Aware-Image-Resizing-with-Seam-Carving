#ifndef SEAMCARVER_H
#define SEAMCARVER_H

#include <opencv2/opencv.hpp>
#include <cassert>

using namespace cv;

class SeamCarver {
    Mat_<Vec3b> image;
    Mat_<Vec3b> duplicate;
    Mat_<Vec3b> horizontalMark;
    Mat_<Vec3b> verticalMark;
    Mat energy;

    //Compute the full energy matrix by scanning the whole image
    void computeFullEnergy();

    //Compute the energy function only for the pixels affected by seam-removal
    void computeEnergyAfterSeamRemoval(vector<uint> seam);

public:
    SeamCarver(Mat_<Vec3b> im) {
        image = im;
        im.copyTo(horizontalMark);
        duplicate = image;
        energy = Mat(image.rows, image.cols, CV_32S, Scalar(195075));
        computeFullEnergy();
    }

    virtual ~SeamCarver() {
        duplicate.release();
        energy.release();
    }

    //Show the image
    void showImage() {
        imshow("Image", image);
    }

    //Show the energy matrix as an image
    void showEnergy() {
        imshow("Energy", energy);
    }

    //Return the image
    Mat_<Vec3b> getImage() {
        return image;
    }

    //Return the images that are used for marking
    Mat_<Vec3b> getVerticalMark() {
        return verticalMark;
    }

    //Return the value of the energy at a particular pixel
    unsigned int getEnergy(unsigned int row, unsigned int col) {
        return energy.at<uint32_t>(row, col);
    }

    //Find the optimal seams
    vector<uint> findVerticalSeam();
    vector<uint> findHorizontalSeam();

    //Remove a given seam and update the image
    void removeVerticalSeam(vector<uint> seam);
    void removeHorizontalSeam(vector<uint> seam);

    //Mark and show the seams
    void markVerticalSeam(Vector<uint> seam);
    void markHorizontalSeam(Vector<uint> seam);
};

#endif // SEAMCARVER_H
