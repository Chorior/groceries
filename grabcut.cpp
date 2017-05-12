#include <opencv2\opencv.hpp>
#include <chrono>

using namespace cv;


int main(int argc, char* argv[])
{
	Mat srcImg = imread(argv[1], 1);
	imshow("test", srcImg);

	//Mat resultX,resultY,result;
	//
	//cv::Sobel(srcImg, resultX, CV_32F, 1, 0, 3);
	//cv::Sobel(srcImg, resultY, CV_32F, 0, 1, 3);
	//cv::Sobel(srcImg, result, CV_32F, 1, 1, 3);
	//
	//imshow("resultX",resultX);
	//imshow("resultY",resultY);
	//imshow("result",result);

	auto time_start = std::chrono::high_resolution_clock::now();

	//cv::Rect rect=cv::Rect(5,10,srcImg.cols-20,srcImg.rows-20);
	cv::Rect rect = cv::Rect(15, 25, srcImg.cols - 30, srcImg.rows - 50);
	//cv::Rect rect=cv::Rect(3,10,59,167);
	//cv::Rect rect=cv::Rect(30,30,srcImg.cols-32,srcImg.rows-32);
	Mat res;
	//srcImg.copyTo(res);
	//rectangle( res, Point( rect.x, rect.y ), Point(rect.x + rect.width, rect.y + rect.height ), Scalar(0,255,0), 2);
	//imshow("res",res);

	Mat mask = cv::Mat(cv::Size(srcImg.size()), CV_8UC1, Scalar::all(GC_BGD));
	(mask(rect)).setTo(Scalar(GC_PR_FGD));

	Mat bgdModel, fgdModel;
	//grabCut(srcImg, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK );
	//grabCut(srcImg, mask, rect, bgdModel, fgdModel, 1 );
	mygrabCut(srcImg, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
	//grabCut(srcImg, mask, rect, bgdModel, fgdModel, 1 );
	//imshow("mask",mask);

	Mat binMask = cv::Mat(cv::Size(mask.size()), CV_8UC1);
	binMask = mask & 1;
	srcImg.copyTo(res, binMask);

	auto time_stop = std::chrono::high_resolution_clock::now();
	std::cout << "grabcut took " << std::chrono::duration<double, std::milli>(time_stop - time_start).count() << " ms\n";

	imshow("grabCut", res);

	waitKey();
	return 0;
}
