/********************************************************************
�ļ�: main.cpp
����: ����������
��ע: 20180111 ��˨ �����ļ�
***********************************************************************/

#include "commonHead.h"

//����ͼƬ
#define PIC_LENA	"../picture/Lena.jpg"
#define SRC_WIN_NAME "����ͼ�任"
#define DST_WIN_NAME "���Ŀ�괰��"


/*======================================================================
����: main
����: ����������
����: ��
���: 0
��ע: ��
======================================================================*/



int main()
{
	//opencv ��ȡ����ͷ
	//videoCap();
	//ImageScanner barScanner;
	//barScanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	Mat srcImg = imread(PIC_LENA);
	//imshow(SRC_WIN_NAME,srcImg);

    Mat grayImg, edgeImg,maskImg;
   
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    const int LAPLACIAN_FILTER_SIZE =5;
    const int EDGES_THRESHOLD = 80;

    //����Ϊ1��
    Size size = srcImg.size();
    Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height /2;
    Mat smallImg = Mat(smallSize,CV_8UC3);
    resize(srcImg,smallImg,smallSize,0,0,INTER_LINEAR);

    //��������
    cvtColor(smallImg, grayImg, CV_BGR2GRAY);
    medianBlur(grayImg,grayImg,MEDIAN_BLUR_FILTER_SIZE);
    Laplacian(grayImg,edgeImg,CV_8U,LAPLACIAN_FILTER_SIZE);
    threshold(edgeImg,maskImg,EDGES_THRESHOLD,255,THRESH_BINARY_INV);

    //imshow(DST_WIN_NAME, edgeImg);



	waitKey(0);
    cvDestroyAllWindows();
	return 0;


}


