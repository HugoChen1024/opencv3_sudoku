/********************************************************************
�ļ�: function.cpp
����: ʵ�ֻ�����������
��ע:	20180111  �����ļ�
20180111 videoCap����
**********************************************************************/

#include "commonHead.h"
/*
����: videoCap
����: ʵ������ͷץȡͼ�����򵥴���]
����: ��
���: 0
��ע: ��
*/
int videoCap(void)
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

/*
����: imgChangeSave
����: ʵ�ֶ�ȡͼƬ���򵥴���֮�󴢴�����.
����: ��
���: 0
��ע: ��
*/

int imgChangeSave(void)
{

	Mat srcImg = imread("../picture/Lena.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	imshow("ԭʼͼ��", srcImg);
	Mat grayImg, binImg;
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	threshold(grayImg, binImg, 128, 255, CV_THRESH_BINARY);
	imshow("�ڰ�ͼ��", binImg);
	imwrite("../picture/02_binImg.jpg", binImg);
	waitKey(0);

	return 0;
}

/*
����: imgChangeSave
����: ʵ�ֶ�ȡͼƬ���򵥴���֮�󴢴�����.
����: ��
���: 0
��ע: ��
*/
int creatAlphaMat(Mat &mat)
{
	for (int i = 0; i < mat.rows; ++i)
	{
		for (int j = 0; j < mat.cols; ++j)
		{
			Vec4b&rgba = mat.at<Vec4b>(i, j);
			rgba[0] = UCHAR_MAX;
			rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols)*UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows)*UCHAR_MAX);
			rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
		}
	}
	return 0;
}

int creatSaveImg(void)
{
	Mat mat(480, 640, CV_8UC4);
	creatAlphaMat(mat);

	vector<int>compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("pic/͸��Alphaֵͼ.png", mat, compression_params);
		imshow("���ɵ�pngͼƬ", mat);
		waitKey(0);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "ͼ��ת������:%s\n", ex.what());
		return 1;
	}
	return 0;
}





