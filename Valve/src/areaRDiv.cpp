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

    //Canny(grayImg,dstImg,50,100,3);
    GaussianBlur(grayImg, grayImg, Size(5, 5), 1, 0);
    //���Ų�ֵͼ��
    pyrDown(grayImg, grayImg);
    pyrUp(grayImg, grayImg);



    Mat cannyImg;
    Canny(grayImg, cannyImg, 10, 80, 3); //ʵ�������������

   // cv::dilate(cannyImg, cannyImg, cv::Mat());
   // cv::erode(cannyImg, cannyImg, cv::Mat());



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

//����⵽�ı�Ե�߽����˲�
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
    //for (auto  it = lines.cbegin(); it < lines.cend(); it++)
        //lines_out.push_back(*it);
    //��ʱֻ���������߻�����
    lines_out.push_back(*lines.cbegin());

    for (auto i = 0;i < scales.size();i++)
    {
        lines_out.push_back({ lines_out[0].l + (int)scales[i],
            1,
            false });
    }


    return true;
}


//����scars
void AreaRDiv::drawScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars, cv::Mat &img_out)
{
    cv::Mat img_rgb = img.clone();
    if (img.channels() == 1)
        cv::cvtColor(img, img_rgb, CV_GRAY2BGR);

    //��������
    vector<Rect> bound(scars.size());
    for (int i = 0; i < scars.size(); i++)
    {
        //drawContours(img_rgb, scars, i, Scalar(0, 255, 255), 1, 8);

        bound[i] = boundingRect(Mat(scars[i]));
        rectangle(img_rgb, Point(bound[i].tl().x-5, bound[i].tl().y -5), Point(bound[i].br().x + 5, bound[i].br().y + 5), Scalar(0, 0, 255), 1, 8, 0);
        

    }


    //for (auto i = scars.cbegin(); i != scars.cend(); ++i)
    //    for (auto j = i->cbegin(); j != i->cend(); ++j)
    //        img_rgb.at<cv::Vec3b>(j->y, j->x) = { 0,0,255 };

    img_out = img_rgb.clone();


}

std::vector<std::vector<Point>> contours;
std::vector<Vec4i> hierarchy;


void AreaRDiv::detectScars(const cv::Mat &img, std::vector<std::vector<cv::Point>> &scars, const std::vector<AreaRDiv::Line> &lines)const
{
    //���ҵ������� �ɼ��������������������������������Ϣ
    findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

    /*
    Mat drawImg = Mat::zeros(img.size(), CV_8UC3);
    for (auto i = 0; i <contours.size(); i++)
    {

        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawContours(drawImg, contours, i, color, CV_FILLED,8, hierarchy, 0, Point());

        cout << contourArea(contours[i]) << "   " << arcLength(contours[i],false) << endl;

    }

    */

    // ������������������ͳ���
    for (auto i = contours.cbegin(); i != contours.cend(); ++i)
    {
        double contour_area = cv::contourArea(*i);
        double contour_length = cv::arcLength(*i, false);

        //�˺�����������10 ���߳�����10-1000��Χ����
        if (contour_area > 10 || (contour_length>=10 && contour_length<=500))
            scars.push_back(*i);

        //cout << contour_area << "    " << contour_length << endl;


    }

 
}


void AreaRDiv::filterScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars_in, std::vector<std::vector<cv::Point>> &scars_out, const std::vector<AreaRDiv::Line> &lines)const
{


    //��ÿһ����Ե�����С���� ������С���εĳ�����б�ֵ�õ�
    //vector<Scar> scars_filter(scars_in.size());
    vector<RotatedRect> box(scars_in.size());
    for (auto i = 0;i <scars_in.size();i++)
    {
        box[i] = minAreaRect(Mat (scars_in[i]));

        //�ڱ�Ե��Ȳ�Ϊ0 �������
        //cout << box[i].size.width << "   "<< box[i].size.height <<  "   "<< (box[i].size.height + 0.001) / (box[i].size.width + 0.001)<< endl;
        if((box[i].size.height != 0) || (box[i].size.height != 0))
            if (((box[i].size.height+0.001) / (box[i].size.width+0.001)) <20)
                scars_out.push_back(scars_in[i]);


        //if ((box[i].center.x > lines[0].l + 30))
        //{
        //    continue;
        //}
        //else
        //{
        //    bool edgesC(scars_in[i].size());
        //    const unsigned char *data = img.ptr<unsigned char>(i);
        //    vector<cv::Point> edgeContours;

        //    for (auto j = scars_in[i].cbegin; j < scars_in[i].cend;j++)
        //    {
        //        if (j->x - lines[0].l > 3)
        //            edgesC[j] = true;
        //        else
        //            edgesC[j] = false;
        //    }

        //    for (auto j =0; j < scars_in[i].size();j++)
        //    {
        //        
        //    }

        //}



    }


    //�۲�����Ե����ȱ�� ��ͼ�������ߵĸ���30px ���ѡ����
    Mat imgROI = img(Range(0, img.rows), Range(lines[0].l - 30, lines[0].l + 30));



    for (auto it = scars_in.cbegin();it < scars_in.cend();it++)
    {

        

        for (auto j = it->cbegin();j < it->cend();j++)
        {

        }

    }

    







}


