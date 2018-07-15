#include "..\include\include.h"

WaterShedProc::WaterShedProc()
{
}

WaterShedProc::~WaterShedProc()
{
}

//��ͼ��markers ת����Ϊ����
void WaterShedProc::setMarkers( const Mat & markerImage )
{
    markerImage.convertTo( markers, CV_32S );
}

//����Ҫ�����ͼƬ����ǰ���� 
void WaterShedProc::preProc(const Mat &srcImg, Mat &dstImg)
{
    Mat grayImg, edgeImg;
    
    if (srcImg.channels() != 1)
    {
        cvtColor( srcImg, grayImg, CV_BGR2GRAY );
    }

    GaussianBlur( grayImg, grayImg, Size( 5, 5 ), 0 );

    //��ֵ��֮��ȡ�� ����Ҫ��ȡ�Ĳ�����Ϊ��ɫ
    threshold( grayImg, edgeImg, 0, 255, CV_THRESH_OTSU );
    bitwise_not( edgeImg, dstImg );

}

//ִ��watershed ��������markers
void WaterShedProc::process( const Mat &srcImg ,Mat &markers )
{
    //ǰ����õ���Եͼ��
    Mat edgeImg;
    preProc( srcImg, edgeImg );

    //�������͵õ�ͼƬ������ȷ�����򣬴�����Ϊ����Ҫ�Ĳ���
    Mat sure_bg, sure_fr;
    Mat kernel = getStructuringElement( MORPH_RECT, Size( 3, 3 ) );

    dilate( edgeImg, sure_bg, kernel, Point( -1, -1 ), 3 );

    //����任���ɾ���ͼ�񣬶�ֵ���õ�������� ��������
    Mat distImg;
    distanceTransform( edgeImg, distImg, DIST_C, 3 );
    distImg.convertTo( distImg, CV_8U );
    
    //����߽糬��1�Ķ����ڲ�
    threshold( distImg, sure_fr, 1, 255, CV_THRESH_BINARY );

    Mat unknownImg;
    subtract( sure_bg, sure_fr, unknownImg );

    connectedComponents( sure_fr, markers );

    //��markers �б����Ĳ���ȫ������Ϊ1 ����Ӱ���ˮ
    markers = markers + 1;

    //��markers��unknown�Ĳ�����Ϊ0 ������һ����ˮ
    for (int i = 0;i < unknownImg.rows;i++)
    {
        unsigned char *data = unknownImg.ptr<unsigned char>( i );
        int *dataM = markers.ptr< int>( i );
        for (int j = 0;j < unknownImg.cols;j++)
        {
            if (data[j] == 255)
            {
                dataM[j] = 0;
            }
        }
    }

    watershed( srcImg, markers );


}

void WaterShedProc::drawMarkersLine( cv::Mat &dstImg, const cv::Mat &markers)
{
   
    //��marker�е����������ԭͼ��
    for (auto i = 0;i < markers.rows;i++)
    {

        for (auto j = 0;j < markers.cols;j++)
        {
            if (markers.at<int>( i, j ) == -1)
            {
                //dataM[j] =255;
                dstImg.at<cv::Vec3b>( i, j ) = Vec3b( 0, 0, 255 );
            }
        }
    }

}

Vec3b RandomColor( int value )//���������ɫ����
{
    value = value % 255;  //����0~255�������  
    RNG rng;
    int aa = rng.uniform( 0, value );
    int bb = rng.uniform( 0, value );
    int cc = rng.uniform( 0, value );
    return Vec3b( aa, bb, cc );
}

void WaterShedProc::drawMarkersCon( cv::Mat &dstImg, const cv::Mat &markers )
{
    //��ÿһ�����������ɫ���
    Mat PerspectiveImage = Mat::zeros( dstImg.size(), CV_8UC3 );
    for (int i = 0;i<markers.rows;i++)
    {
        for (int j = 0;j<markers.cols;j++)
        {
            int index = markers.at<int>( i, j );
            if (markers.at<int>( i, j ) == -1)
            {
                PerspectiveImage.at<Vec3b>( i, j ) = Vec3b( 255, 255, 255 );
            }
            else
            {
                PerspectiveImage.at<Vec3b>( i, j ) = RandomColor( index );
            }
        }
    }

    //�ָ�����ɫ�Ľ����ԭʼͼ���ں�
    addWeighted( dstImg, 0.4, PerspectiveImage, 0.6, 0, dstImg );

}







































