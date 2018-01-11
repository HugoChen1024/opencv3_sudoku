/********************************************************************
	�ļ�: main.cpp
	����: ����������
	��ע: 20180111 ��˨ �����ļ�

***********************************************************************/

#include "commonHead.h"

//���õ�ͼ���ĸ�ͼ
#define PIC_LENA	"../picture/Lena.jpg"
#define PIC_AIRPLANE "../picture/Airplane.jpg"
#define PIC_FRUIT	"../picture/Fruits.jpg"
#define PIC_BABOON	"../picture/Baboon.jpg"


#define WINDOWS_NAME "���������Ի��"

const int g_nMaxAlphaValue = 100;
int g_nAlphaValueSlide;
double g_dAlphaValue;
double g_dBetaValue;

Mat g_dstImg;
Mat g_srcImg1;
Mat g_srcImg2;

void on_Trackbar(int, void *)
{
	g_dAlphaValue = (double)g_nAlphaValueSlide / g_nMaxAlphaValue;
	g_dBetaValue = 1 - g_dAlphaValue;

	addWeighted(g_srcImg1, g_dAlphaValue, g_srcImg2, g_dBetaValue, 0.0, g_dstImg);
	imshow(WINDOWS_NAME, g_dstImg);
}
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
	//ͼ���ֵ��������
	//imgChangeSave();

	//Mat srcImg = imread("../picture/Lena.jpg",CV_LOAD_IMAGE_ANYCOLOR);
	//imshow("ԭʼͼ��",srcImg);

	g_srcImg1 = imread(PIC_LENA);
	g_srcImg2 = imread(PIC_AIRPLANE);

	g_nAlphaValueSlide = 30;

	namedWindow(WINDOWS_NAME, 1);

	char TrackbarName[50];
	sprintf(TrackbarName, "͸��ֵ: %d", g_nMaxAlphaValue);

	createTrackbar(TrackbarName, WINDOWS_NAME, &g_nAlphaValueSlide, g_nMaxAlphaValue, on_Trackbar);

	on_Trackbar(g_nAlphaValueSlide, 0);
	imwrite("pic/04_lena_airplane.png", g_dstImg);







	waitKey(0);












	return 0;
}
