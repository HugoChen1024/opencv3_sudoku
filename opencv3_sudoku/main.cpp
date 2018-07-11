/********************************************************************
�ļ�: main.cpp
����: ����������
��ע: 20180111 ��˨ �����ļ�
***********************************************************************/

#include "commonHead.h"

//����ͼƬ
#define PIC	"../picture/001-2.bmp"
#define WINDOWS_NAME "�ര����ʾ"


/*======================================================================
����: main
����: ����������
����: ��
���: 0
��ע: ��
======================================================================*/


int main(void)
{
	//Mat srcImg = imread(PIC_LENA);
	//imshow(WINDOWS_NAME,srcImg);

    Mat srcImg = imread(PIC);
    Mat grayImg,dstImg;
    cv::cvtColor(srcImg,grayImg,CV_BGR2GRAY);

    pyrDown(grayImg,grayImg);
    pyrUp(grayImg,grayImg);

    cv::Canny(grayImg, grayImg, 50, 100, 3);

    cv::imshow(WINDOWS_NAME,grayImg);





	waitKey(0);
	return 0;
}



