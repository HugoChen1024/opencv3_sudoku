/********************************************************************
	�ļ�: main.cpp
	����: ����������
	��ע: 20180111 ��˨ �����ļ�

***********************************************************************/

#include "commonHead.h"

/*======================================================================
	����: main
	����: ����������
	����: ��
	���: 0
	��ע: ��

======================================================================*/
int main()
{
	//��������ͷ����
	VideoCapture capture(1);

	//�Ҷ�ͼ,�ڰ�ͼ,�߽�ͼ
	Mat grayFrame, binFrame, edgeFrame;

	//ѭ����ʾ
	while (1)
	{ 
			Mat srcFrame;
			capture >> srcFrame;

			//ת��Ϊ�Ҷ�ͼ
			cvtColor(srcFrame, grayFrame, CV_BGR2GRAY);
			//ת��Ϊ�ڰ�ͼ
			threshold(grayFrame, binFrame, 128, 255, CV_THRESH_OTSU);
			//ģ����
			blur(grayFrame, grayFrame, Size(3, 3));
			//ת��Ϊ�߽�ͼ
			Canny(grayFrame, edgeFrame, 0, 30, 3);
			
			imshow("srcFrame", srcFrame);
			imshow("grayFrame", grayFrame);
			imshow("binFrame", binFrame);
			imshow("edgeFrame", edgeFrame);

			waitKey(30);
	}
	capture.release();
	destroyAllWindows();
	return 0;
}
