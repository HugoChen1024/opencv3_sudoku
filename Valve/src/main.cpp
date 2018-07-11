/*
    �ļ���main.cpp
    ���ܣ����Էָ���Լ�ʶ����
    
*/


#include "..\include\include.h"



#define TESTCYLINDER    0   //���Ը���ͼ��
    #define CFace1  "Pic\\CylinderFace1.bmp"


#define TESTBFACE       0   //���Դ����ͼ��
    #define BigFace1 "Pic\\BigFace1.bmp"
    #define BigFace2 "Pic\\BigFace2.bmp"

#define TESTRFACE       1   //R�� ���� ׶�����
    #define RFace1  "Pic\\RFace1.bmp"   
    #define RFace2  "Pic\\RFace2.bmp"  
    #define RFace3  "Pic\\RFace3.bmp"  
    const char *RFaceWindowsName = "����ͼ����ʾ����";

    using namespace cv;

int  main(void)
{
#if TESTCYLINDER
    Mat srcImg = imread(BigFace1);
    //���������ʾ
    namedWindow("src");
    //imshow("src", srcImg);
    std::vector<double> scales{0.0063,0.0777,0.9045,0.9017};
    std::vector<AreaDivider::Line> lines;

    AreaDivider ad;
    ad.divide(srcImg,scales,lines);

    Mat dividerImg;
    ad.drawLines(srcImg,lines,dividerImg);
    //imshow("dst",dividerImg);
    cv::waitKey(0);
#endif

#if TESTBFACE
    Mat bigFaceImg = imread(BigFace1);
    
    Mat grayImg,edgeImg;
    
    if(bigFaceImg.channels() == 3)
    {
        cvtColor(bigFaceImg, grayImg, CV_BGR2GRAY);
    }

#endif

#if TESTRFACE
    Mat srcRImg = imread(RFace3);

    Mat cannyImg,dstImg;

    AreaRDiv ard;
    ard.preProc(srcRImg, cannyImg);

    std::vector<AreaRDiv::Line> lines, linesFilter;
    ard.detectLines(cannyImg, lines);

    //����ͼ���ݶ����������ߣ��ֱ�Ϊ��׶����ߣ�������࣬�����Ҳ࣬�ܿ��
    std::vector<double> scales_l={ 58.0,171.0,260.0,400.0};

    std::vector<double> scales{ 0.0063,0.0777,0.9045,0.9017 };

    ard.filterLines(lines, linesFilter, scales_l);

    ard.drawLines(srcRImg,linesFilter, dstImg);








    namedWindow(RFaceWindowsName, CV_WINDOW_AUTOSIZE);
    imshow(RFaceWindowsName, dstImg);

    waitKey(0);


#endif



    return 0;
}



