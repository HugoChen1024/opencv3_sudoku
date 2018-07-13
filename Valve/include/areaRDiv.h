#pragma once



#ifndef AREARDIV_H
    #define AREARDIV_H

class AreaRDiv
{
public:
    AreaRDiv();
    ~AreaRDiv();

    class Line
    {
        public:
            int l;     // location ��ʾ�ߵ�λ����Ϣ
            double acc; //����ֵ�� ��ʾ��ǰ�еı�Ե�������������ı�ֵ
            bool aut;   //���������Զ��Ļ��ߵı�־λ
    };

    class Scar
    {
        //Point center;  //ȱ�����ĵ�
        double area; //ȱ�����
        double width;
        double height;
        double acc = height / width;
    };



    bool divide(const cv::Mat &img, const std::vector<double> &scales, std::vector<Line> &lines)const;
    void drawLines(const cv::Mat &img, const std::vector<Line> &lines, cv::Mat &imgOut)const;

//private:
    bool preProc(const cv::Mat &srcImg, cv::Mat &dstImg);

    void detectLines(const cv::Mat &, std::vector<Line> &)const;

    bool filterLines(const std::vector<Line> &, std::vector<Line> &, const std::vector<double> &)const;



    void drawScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars, cv::Mat &img_out);

    void detectScars(const cv::Mat &img, std::vector<std::vector<cv::Point>> &scars, const std::vector<AreaRDiv::Line> &lines)const;


    void filterScars(const cv::Mat &, const std::vector<std::vector<cv::Point>> &, std::vector<std::vector<cv::Point>> &, const std::vector<AreaRDiv::Line> &)const;


};







#endif // !AREARDIV_H

