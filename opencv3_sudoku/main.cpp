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


#define WINDOWS_NAME "��������ȡ"

Rect gRect;
bool gDrawingBox = false;
RNG gRNG(12345);
void drawRectangle(cv::Mat& img, cv::Rect box);
void mouseHandle(int event, int x, int y, int flags, void* param);



void mouseHandle(int event, int x, int y, int flags, void* param)
{
	Mat& Img = *(cv::Mat*)param;
	switch (event)
	{
		//����ƶ���¼����仯
		case EVENT_MOUSEMOVE:
		{
			if (gDrawingBox)
			{
				gRect.width = x - gRect.x;
				gRect.height = y - gRect.y;
			}
		}	break;

		//������� ��¼��ʼ��
		case EVENT_LBUTTONDOWN:
		{
			gDrawingBox = true;
			gRect = Rect(x, y, 0, 0);
		} break;

		//����ɿ� ��ͼ
		case EVENT_LBUTTONUP:
		{
			gDrawingBox = false;
			if (gRect.width < 0)
			{
				gRect.x += gRect.width;
				gRect.width *= -1;
			}
			if (gRect.height < 0)
			{
				gRect.y += gRect.height;
				gRect.height *= -1;
			}

			drawRectangle(Img, gRect);

		} break;

		default:
			break;
	}
}

void drawRectangle(cv::Mat& img, cv::Rect box)
{
	rectangle(img, box.tl(), box.br(), Scalar(gRNG.uniform(0, 255), gRNG.uniform(0, 255), gRNG.uniform(0, 255)));
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

	gRect = Rect(-1, -1, 0, 0);
	Mat srcImg(600, 800, CV_8UC3), tmpImg;
	srcImg.copyTo(tmpImg);
	srcImg = Scalar::all(0);

	namedWindow(WINDOWS_NAME);
	setMouseCallback(WINDOWS_NAME, mouseHandle, (void*)&srcImg);

	while (1)
	{
		srcImg.copyTo(tmpImg);
		if (gDrawingBox)
			drawRectangle(tmpImg, gRect);
		imshow(WINDOWS_NAME, tmpImg);
		if (waitKey(10) == 27) break;
	}

	imwrite("pic/05_mouseBox.png", tmpImg);
	return 0;


}

