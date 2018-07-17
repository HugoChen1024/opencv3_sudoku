#include "..\include\include.h"

AreaRDiv::AreaRDiv()
{

}

AreaRDiv::~AreaRDiv()
{

}

// �ָ�����
bool AreaRDiv::divide(const cv::Mat &img, const std::vector<double> &scales, std::vector<Line> &lines)const
{

    return true;
}




void AreaRDiv::drawLines(const cv::Mat &img, const std::vector<Line> &lines, cv::Mat &imgOut)const


{
    cv::Mat img_rgb = img.clone();
    if (img.channels() == 1)
        cv::cvtColor( img, img_rgb, CV_GRAY2BGR );

    //����accֵ���л���ͬ��ɫ���� С�ڵ���1 ����/   2 ����     ���� ����
    for (auto i : lines)
    {
        if (i.acc <= 1)
            cv::line( img_rgb, cv::Point( i.l, 0 ), cv::Point( i.l, img_rgb.rows - 1 ), cv::Scalar( 0, 255, 0 ) );
        else if (i.acc == 2)
            cv::line( img_rgb, cv::Point( i.l, 0 ), cv::Point( i.l, img_rgb.rows - 1 ), cv::Scalar( 255, 0, 0 ) );
        else
            cv::line( img_rgb, cv::Point( i.l, 0 ), cv::Point( i.l, img_rgb.rows - 1 ), cv::Scalar( 0, 0, 255 ) );

    }
    imgOut = img_rgb;
}



//ͼ��Ԥ����
bool AreaRDiv::preProc(const cv::Mat &srcImg, cv::Mat &dstImg) 
{
    Mat grayImg;
    if (srcImg.channels() != 1)
    {
        cvtColor(srcImg,grayImg,CV_BGR2GRAY);
    }

    //Canny(grayImg,dstImg,50,100,3);
    GaussianBlur(grayImg, grayImg, Size(5, 5), 1, 0);
    //���Ų�ֵͼ��
    pyrDown(grayImg, grayImg);
    pyrUp(grayImg, grayImg);



    Mat cannyImg;
    Canny(grayImg, cannyImg, 10, 80, 3); //ʵ�������������

   // cv::dilate(cannyImg, cannyImg, cv::Mat());
   // cv::erode(cannyImg, cannyImg, cv::Mat());



    dstImg = cannyImg.clone();

   // waitKey(0);
    return true;
}



void AreaRDiv::detectLines(const cv::Mat &cannyImg, std::vector<Line> &lines)const
{

     //���ڵõ���Ե��ͼ���ÿһ�н��м����� ��һ�еİ�ɫ����Ŀ����300ʱ����Ϊ��⵽ֱ��
    for (auto i = 0; i < cannyImg.cols; ++i)
    {
        int c = 0;
        for (auto j = 0; j < cannyImg.rows; ++j)
            c = (cannyImg.at<unsigned char>(j, i) == 255) ? c + 1 : c;

        //d���ڴ���300���׵���н��� ����line��
        if (c > 300)
            lines.push_back({ i,c / double(cannyImg.rows),true });
    }

}

//����⵽�ı�Ե�߽����˲�
bool AreaRDiv::filterLines(const std::vector<Line> &lines_in, std::vector<Line> &lines_out, const std::vector<double> &scales)const
{
    //���������֮����С��5 ������϶̵�����
    std::vector<Line> lines = lines_in;
    for (int i = 0; i < lines.size() - 1; )
    {
        if (fabs(lines[i].l - lines[i + 1].l) < 5)
        {
            if (lines[i].acc >= lines[i + 1].acc)
                lines.erase(lines.cbegin() + i + 1);
            else
                lines.erase(lines.cbegin() + i);
        }
        else
            ++i;
    }

    //����Ե�߼����˲���lines��
    //for (auto  it = lines.cbegin(); it < lines.cend(); it++)
        //lines_out.push_back(*it);
    //��ʱֻ���������߻�����
    lines_out.push_back(*lines.cbegin());

    for (auto i = 0;i < scales.size();i++)
    {
        lines_out.push_back({ lines_out[0].l + (int)scales[i],
            1,
            false });
    }


    return true;
}


//����scars
void AreaRDiv::drawScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars, cv::Mat &img_out)
{
    cv::Mat img_rgb = img.clone();
    if (img.channels() == 1)
        cv::cvtColor(img, img_rgb, CV_GRAY2BGR);

    //��������
    vector<Rect> bound(scars.size());
    for (int i = 0; i < scars.size(); i++)
    {
        //drawContours(img_rgb, scars, i, Scalar(0, 255, 255), 1, 8);

        bound[i] = boundingRect(Mat(scars[i]));
        rectangle(img_rgb, Point(bound[i].tl().x-5, bound[i].tl().y -5), Point(bound[i].br().x + 5, bound[i].br().y + 5), Scalar(0, 0, 255), 1, 8, 0);
        

    }


    //for (auto i = scars.cbegin(); i != scars.cend(); ++i)
    //    for (auto j = i->cbegin(); j != i->cend(); ++j)
    //        img_rgb.at<cv::Vec3b>(j->y, j->x) = { 0,0,255 };

    img_out = img_rgb.clone();


}

std::vector<std::vector<Point>> contours;
std::vector<Vec4i> hierarchy;


void AreaRDiv::detectScars(const cv::Mat &img, std::vector<std::vector<cv::Point>> &scars, const std::vector<AreaRDiv::Line> &lines)const
{
    //���ҵ������� �ɼ��������������������������������Ϣ
    findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

    /*
    Mat drawImg = Mat::zeros(img.size(), CV_8UC3);
    for (auto i = 0; i <contours.size(); i++)
    {

        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawContours(drawImg, contours, i, color, CV_FILLED,8, hierarchy, 0, Point());

        cout << contourArea(contours[i]) << "   " << arcLength(contours[i],false) << endl;

    }

    */

    // ������������������ͳ���
    for (auto i = contours.cbegin(); i != contours.cend(); ++i)
    {
        double contour_area = cv::contourArea(*i);
        double contour_length = cv::arcLength(*i, false);

        //�˺�����������10 ���߳�����10-1000��Χ����
        if (contour_area > 10 || (contour_length>=10 && contour_length<=500))
            scars.push_back(*i);

        //cout << contour_area << "    " << contour_length << endl;


    }

 
}


void AreaRDiv::filterScars(const cv::Mat &img, const std::vector<std::vector<cv::Point>> &scars_in, std::vector<std::vector<cv::Point>> &scars_out, const std::vector<AreaRDiv::Line> &lines)const
{


    //��ÿһ����Ե�����С���� ������С���εĳ�����б�ֵ�õ�
    //vector<Scar> scars_filter(scars_in.size());
    vector<RotatedRect> box(scars_in.size());
    for (auto i = 0;i <scars_in.size();i++)
    {
        box[i] = minAreaRect(Mat (scars_in[i]));

        //�ڱ�Ե��Ȳ�Ϊ0 �������
        //cout << box[i].size.width << "   "<< box[i].size.height <<  "   "<< (box[i].size.height + 0.001) / (box[i].size.width + 0.001)<< endl;
        if((box[i].size.height != 0) || (box[i].size.height != 0))
            if (((box[i].size.height+0.001) / (box[i].size.width+0.001)) <20)
                scars_out.push_back(scars_in[i]);


        //if ((box[i].center.x > lines[0].l + 30))
        //{
        //    continue;
        //}
        //else
        //{
        //    bool edgesC(scars_in[i].size());
        //    const unsigned char *data = img.ptr<unsigned char>(i);
        //    vector<cv::Point> edgeContours;

        //    for (auto j = scars_in[i].cbegin; j < scars_in[i].cend;j++)
        //    {
        //        if (j->x - lines[0].l > 3)
        //            edgesC[j] = true;
        //        else
        //            edgesC[j] = false;
        //    }

        //    for (auto j =0; j < scars_in[i].size();j++)
        //    {
        //        
        //    }

        //}



    }


    //�۲�����Ե����ȱ�� ��ͼ�������ߵĸ���30px ���ѡ����
    Mat imgROI = img(Range(0, img.rows), Range(lines[0].l - 30, lines[0].l + 30));



    //for (auto it = scars_in.cbegin();it < scars_in.cend();it++)
    //{

    //    

    //    for (auto j = it->cbegin();j < it->cend();j++)
    //    {

    //    }

    //}

    







}


//����ͼ�����򻮷�
void AreaRDiv::divideImg( Mat &srcImg, Mat &dstImg, vector<double> &borderScales, vector<AreaRDiv::Line> &borderlines)
{



    Mat grayImg, edgeImg, cannyImg;


    cvtColor( srcImg, grayImg, CV_BGR2GRAY );

    blur( grayImg, grayImg, Size( 7, 7 ) );


    //grayImg.convertTo( grayImg, CV_16S );

    //Sobel( grayImg, edgeImg, CV_16S, 1,0,5 );
    //Laplacian( grayImg, edgeImg, CV_32F, 5 );

    //ͳ�������ƽ���������Էֿ����� ������ǰ���������ͷβ ���ٴ󲿷�������
    vector<int> sumY{};
    //double *sumY = new double[grayImg.cols];
    for (int i = 0;i < grayImg.cols;++i)
    {
        double avg = 0;
        for (int j = 0;j < grayImg.rows;j++)
        {
            //cout << grayImg.at<uchar>( j, i )<<"  "<<i<< endl;
            avg += (double) grayImg.at<uchar>( j, i ) / grayImg.rows;
        }
        sumY.push_back( (int)avg );
        //sumY[i] = sum;
    }



    //���������ݶ�
    vector<int> gradX{};
    gradX.push_back( 0 );
    for (int i = 1;i < grayImg.cols;i++)
    {
        gradX.push_back( 10 * ( sumY[i] - sumY[i - 1] ) );

        //cout << gradX[i] << endl;
    }

    //�����õ�ͼ���ݶȵ�������ֵ���õ�������Ե��ֵ  ���ѡ 0-150 �Ҳ�ѡ1900-2048
    int max = 0, max_l = 0;
    for (int i = 0;i < 150;i++)
    {
        if (gradX[i] > max)
        {
            max = gradX[i];
            max_l = i;
        }
    }
    //cout << max << "  " << max_l << endl;


    //�ӵ�һ����ֵ��ʼ�ҵ�30px ����Сֵ �͹�
    int min1 = 100, min1_l = 0;
    for (int i = max_l;i < max + 300;i++)
    {
        if (gradX[i] < min1)
        {
            min1 = gradX[i];
            min1_l = i;
        }
    }
    //cout << min1 << "  " << min1_l << endl;


    //�����ҵ���һ����ֵ  E���ҵ͹�
    int min = 128, min_l = 0;
    for (int i = grayImg.cols - 1;i >grayImg.cols - 121;i--)
    {
        if (gradX[i] < min)
        {
            min = gradX[i];
            min_l = i;
        }
    }
    //cout << min << "  " << min_l << endl;

    

    max_l = max_l - 2;
    //�������� �ֶ�
    borderlines.push_back( { max_l, 1, false } );

    //��ڶ������� �ֶ�
    borderlines.push_back( { min1_l, 1, false } );

    int maxWidthpx = abs( min_l - max_l );

    //�����������
    for (int i = 2;i < borderScales.size() - 1;i++)
    {
        borderlines.push_back( { max_l + (int) ( borderScales[i] * maxWidthpx ), 1, false } );
    }
    //���Ҳ����
    borderlines.push_back( { min_l, 1, false } );

    AreaRDiv ard;

    //ard.drawLines( srcImg, borderlines, dstImg );

/*
    //����������ˮƽͶӰͼ��
    cv::Mat projImg( 255, grayImg.cols, CV_8U, cv::Scalar( 255 ) );

    for (int i = 0; i < grayImg.cols; ++i)
    {
    cv::line( projImg, cv::Point( i, 128 - gradX[i] ), cv::Point( i, 128 ), cv::Scalar::all( 0 ) );
    }

    Mat tmp, tmp2;
    bitwise_not( projImg, tmp );
    resize( tmp, tmp, grayImg.size() );
    addWeighted( tmp, 0.3, grayImg, 0.7, 0, dstImg );

    //imwrite( "Pic\\R\\Test_grad.png", tmp2 );


    

    //edgeImg.convertTo( edgeImg, CV_8U );
    //threshold( edgeImg, edgeImg, 100, 255, CV_THRESH_OTSU );


    //Sobel( grayImg, dstImg, CV_8U, 1, 0, 5 );
    //Canny( grayImg, cannyImg, 30, 60, 3 );

    vector<Mat> cellImg;
    cutImage( srcRImg,borderlines, cellImg );

    Mat AreaBImg, AreaD2Img, AreaEImg, AreaFImg, AreaGImg;
    AreaBImg = cellImg[0];
    AreaD2Img = cellImg[2];
    AreaEImg = cellImg[3];
    AreaFImg = cellImg[4];
    AreaGImg = cellImg[5];

    */
}

//�������򻮷�
void AreaRDiv::RdivideImg( Mat &srcImg, Mat &dstImg, vector<double> &RborderScales )
{

    //Mat srcImg = imread( RFace0 );
    //Mat dstImg;


    //imshow( "src", srcRImg );
    Mat grayImg, edgeImg, cannyImg;


    cvtColor( srcImg, grayImg, CV_BGR2GRAY );

    blur( grayImg, grayImg, Size( 5, 5 ) );


    //ͳ�������ƽ������
    vector<double> sumY{};
    for (int i = 0;i < grayImg.cols;++i)
    {
        double sum = 0;
        for (int j = 0;j < grayImg.rows;j++)
        {
            sum += (double) grayImg.at<uchar>( j, i ) / grayImg.rows;
        }
        sumY.push_back( sum );
    }

    //���������ݶ�
    vector<int> gradX{};
    gradX.push_back( 0 );
    for (int i = 1;i < grayImg.cols;i++)
    {
        gradX.push_back( (int) 10 * ( sumY[i] - sumY[i - 1] ) );
        //cout << gradX[i] << endl;
    }

    //�����õ�ͼ���ݶȵ�������ֵ���õ�������Ե��ֵ  ���ѡ 0-180 �Ҳ�ѡ����250
    int max = 0, max_l = 0;  //������Ե����ڶ�����Ե
    for (int i = 0;i < 180;i++)
    {
        if (gradX[i] > max)
        {
            max = gradX[i];
            max_l = i;
        }


    }
    //cout << max << "  " << max_l << endl;
    //�ӵ�һ����ֵ��ʼ�ҵ�100px ����Сֵ �͹�
    int min = 100, min_l = 0;
    for (int i = max_l;i < max + 100;i++)
    {
        if (gradX[i] < min)
        {
            min = gradX[i];
            min_l = i;
        }
    }
    //cout << min << "  " << min_l << endl;

    //�����ҵ���һ����ֵ  R �洦���ҵ���ֵ
    int max2 = 0, max2_l = 0;
    for (int i = grayImg.cols - 1;i >grayImg.cols - 251;i--)
    {
        //�ٴ��ҵ�һ����ֵ
        if (gradX[i] > max2)
        {
            max2 = gradX[i];
            max2_l = i;
        }
    }
    //cout << max2 << "  " << max2_l << endl;

    vector<AreaRDiv::Line> Rborderlines;

    //�������� �ֶ����ƶ�2px  �Ҳ�����1px
    max_l -= 0;
    max2_l += 0;

    Rborderlines.push_back( { max_l, 1, false } );
    Rborderlines.push_back( { min_l, 1, false } );

    int maxWidthpx = abs( max2_l - max_l );

    //�����������
    for (int i = 2;i < RborderScales.size() - 1;i++)
    {
        Rborderlines.push_back( { max_l + (int) ( RborderScales[i] * maxWidthpx ), 1, false } );
    }
    //���Ҳ����
    Rborderlines.push_back( { max2_l, 1, false } );

    //AreaRDiv ard;

    //ard.drawLines( srcImg, Rborderlines, dstImg );


    //����������ˮƽͶӰͼ��
    cv::Mat projImg( 255, grayImg.cols, CV_8U, cv::Scalar( 255 ) );

    for (int i = 0; i < grayImg.cols; ++i)
    {
        cv::line( projImg, cv::Point( i, 128 - gradX[i] ), cv::Point( i, 128 ), cv::Scalar::all( 0 ) );
    }

    Mat tmp, tmp2;
    bitwise_not( projImg, tmp );
    resize( tmp, tmp, grayImg.size() );
    addWeighted( tmp, 0.3, grayImg, 0.7, 0, dstImg );
    //imwrite( "Pic\\R\\Test_grad.png", tmp2 );



    //edgeImg.convertTo( edgeImg, CV_8U );
    //threshold( edgeImg, edgeImg, 100, 255, CV_THRESH_OTSU );


    //Sobel( grayImg, dstImg, CV_8U, 1, 0, 5 );
    //Canny( grayImg, cannyImg, 30, 60, 3 );

    AreaRDiv ard;
    ard.drawLines( srcImg, Rborderlines, dstImg );

    //vector<Mat> cellImg;
    //cutImage( srcImg,Rborderlines, cellImg );

    //Mat AreaCImg, AreaD1Img, AreaD0Img, AreaD2Img;
    //AreaCImg = cellImg[0];
    //AreaD1Img = cellImg[1];
    //AreaD0Img = cellImg[2];
    //AreaD2Img = cellImg[3];

}









//Ŀ¼�е�����ͼƬ(��ÿһ��Ŀ¼)
void listFiles(const char * dir, vector<string>& files)
{
    char dirNew[200];
    strcpy(dirNew, dir);
    strcat(dirNew, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
    intptr_t handle;
    _finddata_t findData;
    handle = _findfirst(dirNew, &findData);
    if (handle == -1)        // ����Ƿ�ɹ�
        return;
    do
    {
        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
            //cout << findData.name << "\t<dir>\n";
            // ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
            strcpy(dirNew, dir);
            strcat(dirNew, "\\");
            strcat(dirNew, findData.name);
            listFiles(dirNew, files);
        }
        else
            files.push_back(string(dir).append("\\").append(findData.name));
        //cout << findData.name << "\t" << findData.size << " bytes.\n";
    } while (_findnext(handle, &findData) == 0);
    _findclose(handle);    // �ر��������
}


void renameFiles( string &filespath, string &newname,string &newpath  )
{


    //��λ�����ļ���
    int pos = filespath.find_last_of( '\\' );
    int pos_ = filespath.find( '.' );
    string path( filespath.substr( 0, pos + 1 ) );   // ·��
    string name( filespath.substr( pos + 1, pos_- pos-1 ) ); //�ļ���
    string exte( filespath.substr( pos_+1, 3 ) ); //��չ��

    string suffix = "png";

    //�趨�ļ�����ʽ
    newname = newpath+"\\" +"res_" + name +"."+ suffix;

   // files_rename = 



    ////�ַ���·���ָ�
    //char drive[_MAX_DRIVE];
    //char dir[_MAX_DIR];
    //char fname[_MAX_FNAME];
    //char ext[_MAX_EXT];
    //_splitpath( filespath, drive, dir, fname, ext );


}





//��ͼ�񻮷ֳ�Ϊ�����Ԫ
void cutImage( cv::Mat &srcImg, std::vector<AreaRDiv::Line> &lines, std::vector<cv::Mat> &cellImg )
{
    int height = srcImg.rows;


    ////����ROI�ܻ��ڵ��������ʧЧ�޷�������һ��
    //for (int i = 0;i < lines.size()-1;i++)
    //{

    //    Mat roi_img(srcImg, Rect( lines[i].l - 5, 0, lines[i+1].l- lines[i].l + 10, height ) );

    //    cellImg.push_back( roi_img );
    //}

    Mat roi_img;

    for (int i = 0;i < lines.size()-3;i++)
    {

        Rect rect( lines[i].l - 5, 0, lines[i + 1].l - lines[i].l + 10, height );
        //�ڵ��ĸ����ǳ�����  
        //Ӧ����roi�����С�����޷�����
        srcImg( rect ).copyTo( roi_img );
        cellImg.push_back( roi_img );
        //imshow( "roi_img", roi_img );

    }

    

    // ǿ�ƽ������Ƴ��� ���֮ǰ��bug
    Mat roi_img4;
    Rect rect4( lines[4].l - 5, 0, lines[5].l - lines[4].l + 10, height );
    //�ڵ��ĸ����ǳ�����
    srcImg( rect4 ).copyTo( roi_img4 );
    cellImg.push_back( roi_img4 );

    Mat roi_img5;
    Rect rect5( lines[5].l - 5, 0, lines[6].l - lines[5].l + 10, height );
    //�ڵ��ĸ����ǳ�����
    srcImg( rect5 ).copyTo( roi_img5 );
    cellImg.push_back( roi_img5 );


}


//��G��ͼ�񵥶�����
void GcutImage( cv::Mat &srcImg, std::vector<AreaRDiv::Line> &lines, std::vector<cv::Mat> &cellImg )
{
    int height = srcImg.rows;

    //����ROI�ܻ��ڵ��������ʧЧ�޷�������һ��
    for (int i = 0;i < lines.size()-1;i++)
    {

        Mat roi_img(srcImg, Rect( lines[i].l - 5, 0, lines[i+1].l- lines[i].l + 10, height ) );

        cellImg.push_back( roi_img );
    }

}



//------------------------------���������¸Ľ��桿----------------------------------------------
// ����������ͨ��  �ɼ����� 
// https://blog.csdn.net/qq_37059483/article/details/78018539 
// https://blog.csdn.net/icvpr/article/details/10259577
// �Զ�ֵͼ�������ͨ������,��1��ʼ���
void  Two_PassNew( const Mat &bwImg, Mat &labImg )
{
    assert( bwImg.type() == CV_8UC1 );
    labImg.create( bwImg.size(), CV_32SC1 );   //bwImg.convertTo( labImg, CV_32SC1 );
    labImg = Scalar( 0 );
    labImg.setTo( Scalar( 1 ), bwImg );
    assert( labImg.isContinuous() );
    const int Rows = bwImg.rows - 1, Cols = bwImg.cols - 1;
    int label = 1;
    vector<int> labelSet;
    labelSet.push_back( 0 );
    labelSet.push_back( 1 );
    //the first pass
    int *data_prev = (int*) labImg.data;   //0-th row : int* data_prev = labImg.ptr<int>(i-1);
    int *data_cur = (int*) ( labImg.data + labImg.step ); //1-st row : int* data_cur = labImg.ptr<int>(i);
    for (int i = 1; i < Rows; i++)
    {
        data_cur++;
        data_prev++;
        for (int j = 1; j<Cols; j++, data_cur++, data_prev++)
        {
            if (*data_cur != 1)
                continue;
            int left = *( data_cur - 1 );
            int up = *data_prev;
            int neighborLabels[2];
            int cnt = 0;
            if (left>1)
                neighborLabels[cnt++] = left;
            if (up > 1)
                neighborLabels[cnt++] = up;
            if (!cnt)
            {
                labelSet.push_back( ++label );
                labelSet[label] = label;
                *data_cur = label;
                continue;
            }
            int smallestLabel = neighborLabels[0];
            if (cnt == 2 && neighborLabels[1]<smallestLabel)
                smallestLabel = neighborLabels[1];
            *data_cur = smallestLabel;
            // ������С�ȼ۱�
            for (int k = 0; k<cnt; k++)
            {
                int tempLabel = neighborLabels[k];
                int& oldSmallestLabel = labelSet[tempLabel];  //�����&����ȡ��ַ����,�������÷���
                if (oldSmallestLabel > smallestLabel)
                {
                    labelSet[oldSmallestLabel] = smallestLabel;
                    oldSmallestLabel = smallestLabel;
                }
                else if (oldSmallestLabel<smallestLabel)
                    labelSet[smallestLabel] = oldSmallestLabel;
            }
        }
        data_cur++;
        data_prev++;
    }
    //���µȼ۶��б�,����С��Ÿ��ظ�����
    for (size_t i = 2; i < labelSet.size(); i++)
    {
        int curLabel = labelSet[i];
        int prelabel = labelSet[curLabel];
        while (prelabel != curLabel)
        {
            curLabel = prelabel;
            prelabel = labelSet[prelabel];
        }
        labelSet[i] = curLabel;
    }
    //second pass
    data_cur = (int*) labImg.data;
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < bwImg.cols - 1; j++, data_cur++)
            *data_cur = labelSet[*data_cur];
        data_cur++;
    }
}



