#include "camera.hpp"
#include "getBar.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap;
    cap.open("../blue.avi");

    // 获取帧数和尺寸
    double fps = cap.get(cv::CAP_PROP_FPS);
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cout << fps << endl;
    cout << width << endl;
    cout << height << endl;

    // 创建一个窗口用于显示视频
    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);

    // 循环读取视频并显示
    cv::Mat frame;
    while (1)
    {
        cap >> frame;

        if (frame.empty())
        {
            break;
        }

        getBar templ;

        cv::Mat output;
        std::vector<std::vector<Line>> center;
        templ.bar(frame, output);
        templ.lightbar(output, frame, center);

        std::vector<double> martix;
        std::vector<double> coeffs;
        martix.push_back(708.6656);
        martix.push_back(611.5554);
        martix.push_back(688.0411);
        martix.push_back(565.2781);

        coeffs.push_back(-0.0849);
        coeffs.push_back(0.04752);
        coeffs.push_back(-0.0025);
        coeffs.push_back(0.00501);
        coeffs.push_back(-0.0305);

        Camera camera(martix, coeffs);

        for (int i = 0; i < center.size(); i++)
        {
            std::vector<cv::Point2d> screen_point;
            screen_point.push_back(center[i][0].up);
            screen_point.push_back(center[i][1].up);
            screen_point.push_back(center[i][0].down);
            screen_point.push_back(center[i][1].down);
            std::vector<cv::Point3d> world_point;
            world_point.push_back(cv::Point3d(-68.5, 30.0, 0.5));
            world_point.push_back(cv::Point3d(68.5, 30.0, 0.5));
            world_point.push_back(cv::Point3d(-68.5, -30.0, -0.5));
            world_point.push_back(cv::Point3d(68.5, -30.0, -0.5));
            cv::Mat rotate_vector, translation_vector;
            cv::solvePnP(world_point, screen_point, camera.camera_matrix, camera.dist_coeffs, rotate_vector, translation_vector);
            cv::Mat_<double> rotMat(3, 3);
            Rodrigues(rotate_vector, rotMat);
            cv::Mat vec = (cv::Mat_<double>(3, 1) << 0, 0, 0);
            cv::Mat location;
            location = rotMat * vec + translation_vector;
            double d = location.at<double>(2, 0);
            cout << d << endl;
        }

        cv::imshow("Video", frame);

        if (cv::waitKey(1) == 'q')
        {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}

// #include <iostream>
// #include <opencv2/opencv.hpp>
// #include <vector>

// int main()
// {
//     // 棋盘格的内角大小（以度为单位）
//     double squareSize = 90;

//     // 棋盘格的行数和列数
//     int patternWidth = 7;
//     int patternHeight = 7;

//     // 实际物体的大小（以毫米为单位）
//     double objectWidth = 21;
//     double objectHeight = 21;

//     // 存储棋盘格角点的三维坐标
//     std::vector<cv::Point3f> objectPoints;
//     std::vector<cv::Point2f> imagePoints;

//     // 生成棋盘格角点
//     for (int i = 0; i < patternHeight; ++i)
//     {
//         for (int j = 0; j < patternWidth; ++j)
//         {
//             float x = (j - patternWidth / 2.0f) * squareSize;
//             float y = (i - patternHeight / 2.0f) * squareSize;
//             objectPoints.push_back(cv::Point3f(x, y, 0));
//         }
//     }

//     // 读取图像并检测角点
//     cv::Mat image = cv::imread("../image.bmp");
//     cv::Mat grayImage;
//     cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
//     std::vector<cv::Point2f> corners;
//     cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1);
//     cv::findChessboardCorners(grayImage, cv::Size(patternWidth, patternHeight), corners);

//     std::cout << corners.size() << std::endl;
//     // 如果检测到角点，添加它们到imagePoints向量中
//     if (corners.size() == patternWidth * patternHeight)
//     {
//         imagePoints.assign(corners.begin(), corners.end());
//     }
//     else
//     {
//         std::cout << "未检测到足够的角点" << std::endl;
//         return -1;
//     }

//     // 计算相机参数
//     cv::Mat cameraMatrix, distCoeffs;
//     std::vector<cv::Mat> rvecs, tvecs;
//     cv::calibrateCamera(objectPoints, imagePoints, grayImage.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

//     // 输出相机参数
//     std::cout << "相机矩阵： " << std::endl << cameraMatrix << std::endl;
//     std::cout << "畸变系数： " << std::endl << distCoeffs << std::endl;

//     return 0;
// }
