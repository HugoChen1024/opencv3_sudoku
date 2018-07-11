#include "..\include\include.h"



//ͨ��canny�����Ե֮��ָ�ͼ��
bool AreaDivider::divide(const cv::Mat &img, const std::vector<double> &scales, std::vector<Line> &lines)const
{
	std::vector<Line> lines_;
	int canny_param = 100;

	detectLines(img, lines_, canny_param);
	if (!filterLines(lines_, lines, scales))
	{
		detectLines(img, lines_, canny_param - 50);
		if (!filterLines(lines_, lines, scales))
			return false;
	}

	return true;
}


//�ָ������򻭳���ɫ���ߣ��Ƿָ���������ɫ����
void AreaDivider::drawLines(const cv::Mat &img, const std::vector<Line> &lines, cv::Mat &imgOut)const
{
	cv::Mat img_rgb = img.clone();
	if (img.channels() == 1)
		cv::cvtColor(img, img_rgb, CV_GRAY2BGR);

	for (auto i : lines)
		if (i.aut)
			cv::line(img_rgb, cv::Point(i.l, 0), cv::Point(i.l, img_rgb.rows - 1), cv::Scalar(0, 255, 0));
		else
			cv::line(img_rgb, cv::Point(i.l, 0), cv::Point(i.l, img_rgb.rows - 1), cv::Scalar(255, 0, 0));

	imgOut = img_rgb;
}

//��ͼ�����һ������֮�����Ե �ٽ����ߵĲ���
void AreaDivider::detectLines(const cv::Mat &img, std::vector<Line> &lines,double canny_param)const
{
	cv::Mat img_gray = img.clone();
	if (img.channels() == 3)
		cv::cvtColor(img, img_gray, CV_BGR2GRAY);

	cv::pyrDown(img_gray, img_gray);
	cv::pyrUp(img_gray, img_gray);

	cv::Canny(img_gray, img_gray, 50, canny_param, 3);
	
	//���ڵõ���Ե��ͼ���ÿһ�н��м����� ��һ�еı�Ե��������300ʱ����Ϊ��⵽ֱ��
	for (auto i = 0; i < img_gray.cols; ++i)
	{
		int c = 0;
		for (auto j = 0; j < img_gray.rows; ++j)
			c = img_gray.at<unsigned char>(j, i) == 255 ? c + 1 : c;

		if (c > 300)
			lines.push_back({ i,c / double(img_gray.rows),true });
	}
}

//����ȡ�����߽��й��ˣ�
bool AreaDivider::filterLines(const std::vector<Line> &lines_in, std::vector<Line> &lines_out, const std::vector<double> &scales)const
{
	//9���߶�Ӧƽ���������ߣ�С����һ���ߣ����в�6����
	if (lines_in.size() < 9)
		return false;

	
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

		
	double width = (lines.cend() - 1)->l - lines.cbegin()->l;   //���ſ��
	auto it = lines.cbegin();

	if (lines.size() < 9)
		return false;

	//scales{ 0.0063,0.0777,0.9045,0.9171 };
	//��Ӧƽ����ڶ����ߣ����ߣ����в��·��ߣ���һ�����в�
	
	lines_out.push_back(*it++);		//��һ����
	lines_out.push_back(*it++);		//�ڶ�����
	lines_out.push_back({ lines[0].l + int(scales[1] * width),1,false });   //�������������  ȷ����������

	//�����3���߸���10pix��ȷ��   ��������
	for (; it != lines.cend(); ++it)
		if (fabs(it->l - lines[0].l - scales[2] * width) < 10)
		{
			lines_out.push_back(*it);
			++it;
			break;
		}

	//����Ѿ����������򱨴� ���������Ƴ��ĵ��������Լ����вۿ�� ȷ����������
	for (; it != lines.cend(); ++it)
	{
		if (it == lines.cend() - 1)
			return false;
		if (fabs(it->l - (lines_out.cend() - 1)->l - (scales[3] - scales[2]) * width) < 4)
		{
			lines_out.push_back(*it);
			++it;
			break;
		}
	}

	//����Ѿ�������򱨴� ȷ����������
	if (it == lines.cend())
		return false;
	lines_out.push_back(*it);
	++it;

    //���ݵ��������Լ������ߵļ��ȷ����������
	for (; it != lines.cend(); ++it)
	{
		if (it == lines.cend() - 1)
			return false;
		if (fabs(it->l - (lines_out.cend() - 1)->l - ((lines_out.cend() - 2)->l - (lines_out.cend() - 3)->l)) < 4)
		{
			lines_out.push_back(*it);
			++it;
			break;
		}
	}

    //ȷ���ڰ�����
	if (it == lines.cend())
		return false;
	lines_out.push_back(*it);
	++it;

    //���ݵڰ������Լ������ߵļ��ȷ���ھ�����
	for (; it != lines.cend(); ++it)
	{
		if (it == lines.cend() - 1)
			return false;
		if (fabs(it->l - (lines_out.cend() - 1)->l - ((lines_out.cend() - 2)->l - (lines_out.cend() - 3)->l)) < 4)
		{
			lines_out.push_back(*it);
			++it;
			break;
		}
	}

    //ȷ�����һ����
	if (it == lines.cend() - 1 && lines_out.size() == 9)
		lines_out.push_back(*it);
	else
		return false;
    
    //ѭ���ж��Ƿ�����ȵ��� ���򱨴�
	for (auto i = lines_out.cbegin(); i != lines_out.cend(); ++i)
		for (auto j = i + 1; j != lines_out.cend(); ++j)
			if (i->l == j->l)
				return false;

	return true;
}