// EE569 Homework Assignment #2
// Problem 1 (b) Canny Edge Detector
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Feb 16, 2020

// set OpenCV_DIR in CMakeLists.txt given different environment
// find_package(OpenCV REQUIRED)
// include_directories(${OpenCV_INCLUDE_DIRS})
// target_link_libraries(Canny_Edge_Detector ${OpenCV_LIBS})

// From OpenCV Canny Edge Detector Tutorial Code

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

Mat input, grayscale;
Mat background, detected_edges;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int Thr_ratio = 3; // ratio of lower:upper threshold
const int kernel_size = 3;
const char *window_name = "Edge Map";

void CannyThreshold(int, void*)
{
    /* // Reduce noise with a kernel 3x3
    blur(input, detected_edges, Size(3,3));*/

    // Canny detector
    Canny(grayscale, detected_edges, lowThreshold, lowThreshold*Thr_ratio, kernel_size);

    // completely white background image
    background = Scalar::all(255);

    // Use detected_edges as mask, put edges on a black background,
    grayscale.copyTo(background, detected_edges);
    imshow(window_name, background);
    imwrite("Edge_map.jpg", background);
}

int main(int argc, char *argv[])

{
    // Load an image
    input = imread( argv[1] );

    if( !input.data )
    { return -1; }

    // Create a matrix of the same type and size as input
    background.create(input.size(), input.type());

    /// Convert the image to grayscale
    cvtColor(input, grayscale, CV_BGR2GRAY);

    // Create a window
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);

    // Create a Trackbar for user to enter threshold
    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    // Show the image
    CannyThreshold(0, 0);

    // Wait until user exit program by pressing a key
    waitKey(0);

    return 0;
}