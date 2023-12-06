#include "getBar.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <string.h>
#include <vector>

void getBar::remove(cv::Mat &input, int num)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat src;
    input.copyTo(src);
    findContours(src, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    input.setTo(0);
    for (int i = 0; i < contours.size(); i++)
    {
        int areasValue = cv::contourArea(contours[i], false);
        if (areasValue >= num)
        {
            cv::Scalar color = cv::Scalar(255, 255, 255);
            drawContours(input, contours, i, color, cv::FILLED, 8, hierarchy, 0, cv::Point(0, 0));
        }
    }
}

void getBar::bar(cv::Mat input, cv::Mat &output)
{
    // 二值化处理
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    cv::threshold(output, output, 150, 255, cv::THRESH_BINARY);
    remove(output, 20);
    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(3, 3));
    // 膨胀腐蚀
    for (int i = 0; i < 3; i++)
    {
        cv::dilate(output, output, element);
        cv::erode(output, output, element);
    }
    remove(output, 20);
}

void getBar::lightbar(cv::Mat input, cv::Mat &output, std::vector<std::vector<Line>> &cen)
{
    // 定位光条位置
    // 根据templ获得在frame中的位置
    // 记录最好的counter的序号
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(input, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::RotatedRect> contour;
    for (int i = 0; i < contours.size(); i++)
    {
        // 先计算最小外接矩形
        cv::RotatedRect r = cv::minAreaRect(contours[i]);
        cv::Point2f pts[4];
        std::vector<cv::Point2f> pts2;
        // 记录四个角点
        r.points(pts);
        // 将点依次放到后面
        for (int i = 0; i < 4; i++)
            pts2.push_back(pts[i]);
        if (r.angle > 15 && r.angle < 75)
            continue;
        double hei = r.size.height, wid = r.size.width;
        if (r.angle > 45)
            cv::swap(hei, wid);
        if (hei < wid * 2)
            continue;
        if (cv::contourArea(contours[i]) / cv::contourArea(pts2) < 0.5)
            continue;
        contour.push_back(r);
    }
    // 画出矩形框
    for (int i = 0; i < contour.size(); i++)
    {
        // 一对灯条角度十分相近
        for (int j = i + 1; j < contour.size(); j++)
        {
            if (abs(contour[i].angle - contour[j].angle) < 10.0 || abs(contour[i].angle - contour[j].angle) > 85.0)
            {
                Line line1(contour[i]);
                Line line2(contour[j]);
                if (abs(line1.length - line2.length) < 0.3 * std::max(line1.length, line2.length) && line1.length > 50.0 && abs(line1.up.x - line2.up.x) < 250)
                {
                    cv::Point2f center((line1.up.x + line1.down.x + line2.up.x + line2.down.x)/4 ,(line1.up.y + line1.down.y + line2.up.y + line2.down.y)/4);
                    cv::line(output, line1.up, line2.up, cv::Scalar(0, 0, 255), 4);
                    cv::line(output, line1.down, line2.down, cv::Scalar(0, 0, 255), 4);
                    cv::line(output, line1.down, line1.up, cv::Scalar(0, 0, 255), 4);
                    cv::line(output, line2.down, line2.up, cv::Scalar(0, 0, 255), 4);
                    cv::circle(output, center, 5, cv::Scalar(0, 0, 255), 5);
                    std::vector<Line> temp;
                    temp.push_back(line1);
                    temp.push_back(line2);
                    cen.push_back(temp);
                }
            }
        }
    }
}