#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <cmath>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

class Camera
{
public:
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    Camera(std::vector<double> martix, std::vector<double> coeffs)
    {
        camera_matrix = (cv::Mat_<double>(3, 3) << martix[0], 0, martix[1],
                         0, martix[2], martix[3],
                         0, 0, 1);
        dist_coeffs = (cv::Mat_<double>(5, 1) << coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    }
};
#endif