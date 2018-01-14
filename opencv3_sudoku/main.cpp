/********************************************************************
�ļ�: main.cpp
����: ����������
��ע: 20180111 ��˨ �����ļ�
***********************************************************************/

#include "commonHead.h"

//����ͼƬ
#define PIC_LENA	"../picture/Lena.jpg"

#define BAR1		"../picture/qrcode/barcode1.png"
#define BAR2		"../picture/qrcode/barcode2.png"

#define WINDOWS_NAME "������ʶ��"


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
	ImageScanner barScanner;
	barScanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	Mat srcImg = imread(BAR1);
	imshow(WINDOWS_NAME,srcImg);
	Mat grayImg;
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);
	int width = grayImg.cols;
	int height = grayImg.rows;
	uchar* raw = (uchar*)grayImg.data;
	Image zbarImg(width, height, "Y800", raw, width*height);
	barScanner.scan(zbarImg);
	Image::SymbolIterator symbol = zbarImg.symbol_begin();
	if (zbarImg.symbol_begin() == zbarImg.symbol_end())
	{
		cout << "��ѯ����ʧ��,����ͼƬ!" << endl;
	}
	for (; symbol != zbarImg.symbol_end(); ++symbol);
	{
		cout << "����:" << endl << symbol->get_type_name() << endl << endl;
		cout << "����:" << endl << symbol->get_data() << endl << endl;
	}

	zbarImg.set_data(NULL, 0);



	
	
	//imwrite("pic/05_mouseBox.png", tmpImg);

	waitKey(0);
	return 0;


}