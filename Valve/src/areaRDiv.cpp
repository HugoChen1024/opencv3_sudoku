#include "..\include\include.h"

AreaRDiv::AreaRDiv()
{

}

AreaRDiv::~AreaRDiv()
{

}

// �ָ�����
bool AreaRDiv::divide(const cv::Mat &img, const std::vector<double> &scales, std::vector<Line> &lines)const
{

    return true;
}

std::vector<std::vector<Point>> contours;
std::vector<Vec4i> hierarchy;


void AreaRDiv::drawLines(const cv::Mat &img, const std::vector<Line> &lines, cv::Mat &imgOut)const
{
    cv::Mat img_rgb = img.clone();
    if (img.channels() == 1)
        cv::cvtColor(img, img_rgb, CV_GRAY2BGR);

    for (auto i : lines)
        if (i.aut)
            cv::line(img_rgb, cv::Point(i.l, 0), cv::Point(i.l, img_rgb.rows - 1), cv::Scalar(0, 255, 0));
        else
            cv::line(img_rgb, cv::Point(i.l, 0), cv::Point(i.l, img_rgb.rows - 1), cv::Scalar(255, 0, 0));

    //��������
    for (int i = 0; i < (int)contours.size(); i++)
    {
        drawContours(img_rgb, contours, i, Scalar(0, 0, 255), 1, 8);
    }


    imgOut = img_rgb;
}




//ͼ��Ԥ����
bool AreaRDiv::preProc(const cv::Mat &srcImg, cv::Mat &dstImg) 
{
    Mat grayImg;
    if (srcImg.channels() != 1)
    {
        cvtColor(srcImg,grayImg,CV_BGR2GRAY);
    }



    pyrDown(grayImg, grayImg);
    pyrUp(grayImg, grayImg);

    //Canny(grayImg,dstImg,50,100,3);
    GaussianBlur(grayImg, grayImg, Size(5, 5), 0.5, 0);

    Mat cannyImg;
    Canny(grayImg, cannyImg, 10, 80, 3); //ʵ�������������



    findContours(cannyImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


    dstImg = cannyImg.clone();

   // waitKey(0);

    return true;
}



void AreaRDiv::detectLines(const cv::Mat &cannyImg, std::vector<Line> &lines)const
{

     //���ڵõ���Ե��ͼ���ÿһ�н��м����� ��һ�еİ�ɫ����Ŀ����300ʱ����Ϊ��⵽ֱ��
    for (auto i = 0; i < cannyImg.cols; ++i)
    {
        int c = 0;
        for (auto j = 0; j < cannyImg.rows; ++j)
            c = (cannyImg.at<unsigned char>(j, i) == 255) ? c + 1 : c;

        //d���ڴ���300���׵���н��� ����line��
        if (c > 300)
            lines.push_back({ i,c / double(cannyImg.rows),true });
    }

}

#define AreaRWidth 350
bool AreaRDiv::filterLines(const std::vector<Line> &lines_in, std::vector<Line> &lines_out, const std::vector<double> &scales)const
{
    //���������֮����С��5 ������϶̵�����
    std::vector<Line> lines = lines_in;
    for (int i = 0; i < lines.size() - 1; )
    {
        if (fabs(lines[i].l - lines[i + 1].l) < 5)
        {
            if (lines[i].acc >= lines[i + 1].acc)
                lines.erase(lines.cbegin() + i + 1);
            else
                lines.erase(lines.cbegin() + i);
        }
        else
            ++i;
    }

    //����Ե�߼����˲���lines��
    for (auto  it = lines.cbegin(); it < lines.cend(); it++)
    {
        lines_out.push_back(*it);
    }

    for (auto i = 0;i < scales.size();i++)
    {
        lines_out.push_back({ lines_out[0].l + (int)scales[i],
            1,
            false });
    }


    return true;
}



void AreaRDiv::drawScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars)const
{


}

void AreaRDiv::detectScars(const cv::Mat &, std::vector<std::vector<cv::Point>> &, const std::vector<AreaDivider::Line> &)const
{
    
}


void AreaRDiv::filterScars(const cv::Mat &, const std::vector<std::vector<cv::Point>> &, std::vector<std::vector<cv::Point>> &, const std::vector<AreaDivider::Line> &)const
{

}

















