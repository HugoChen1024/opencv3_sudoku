/********************************************************************
	�ļ�: commonHead.h
	����: ����ͷ�ļ���������
	��ע: 20180111 �����ļ�
***********************************************************************/
#pragma once
#pragma warning(disable:4996) //ȫ���ص��ļ���д��������s�Ĵ���

#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>



using namespace cv;

using namespace std;
//�궨����


//��������
int videoCap(void);
int imgChangeSave(void);
int creatAlphaMat(Mat &mat);
int creatSaveImg(void);








