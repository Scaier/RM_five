#ifndef GETBAR_HPP
#define GETBAR_HPP
#include <cmath>
#include <iostream>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class Line
{
public:
    cv::Point2d up, down;
    double angle;
    double length;  // 长边长度
    Line(cv::RotatedRect rect)
    {
        cv::Point2f pts[4];
        rect.points(pts);
        angle = rect.angle;
        // 判断长边和短边
        if ((std::pow(pts[0].x - pts[1].x, 2) + std::pow(pts[0].y - pts[1].y, 2)) > (std::pow(pts[1].y - pts[2].y, 2) + std::pow(pts[1].x - pts[2].x, 2)))
        {
            up.x = pts[0].x / 2 + pts[3].x / 2;
            up.y = pts[0].y / 2 + pts[3].y / 2;
            down.x = pts[1].x / 2 + pts[2].x / 2;
            down.y = pts[1].y / 2 + pts[2].y / 2;
            length = std::sqrt(std::pow(pts[0].x - pts[1].x, 2) + std::pow(pts[0].y - pts[1].y, 2));
        }
        else
        {
            up.x = pts[0].x / 2 + pts[1].x / 2;
            up.y = pts[0].y / 2 + pts[1].y / 2;
            down.x = pts[3].x / 2 + pts[2].x / 2;
            down.y = pts[3].y / 2 + pts[2].y / 2;
            length = std::sqrt(std::pow(pts[1].y - pts[2].y, 2) + std::pow(pts[1].x - pts[2].x, 2));
        }
        if (up.y < down.y){
            swap(up, down);
        }
    }

private:
};

class getBar
{
private:
    void remove(cv::Mat &input, int num);
public:
    void bar(cv::Mat input, cv::Mat &output);
    void lightbar(cv::Mat input, cv::Mat &output, std::vector<std::vector<Line>> &cen);
};
#endif