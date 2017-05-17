#include "CommunicationCenter.h"
#include "CommonDefine.h"
#include <iostream>
#include <future>
#include <thread>
#include <condition_variable>
#include "opencv.hpp"

using namespace cv;

std::condition_variable g_cv_left;
std::condition_variable g_cv_right;
std::condition_variable g_cv_wait;

Mat obstacle_left;
Mat obstacle_right;

// 左右摄像头获取图像时的时间点
auto epoch_time = std::chrono::time_point<std::chrono::steady_clock>();
auto left_time = epoch_time;
auto right_time = epoch_time;

std::mutex mut_io;
std::mutex g_mtx_left;
std::mutex g_mtx_right;
std::mutex mut_wait;

int exposure_left = 0;
int exposure_right = 0;
int left_cam = 0;
int right_cam = 1;

double t_last = 0;
int callback(CONTROLMSG msg, void* User)
{
	//std::lock_guard<std::mutex> lk(mut);	
	std::cout << msg.value << std::endl;
	return 1;
}

DWORD WINAPI fun1()
{
	VideoCapture cap1(left_cam);
	if (!cap1.isOpened())
	{
		std::lock_guard<std::mutex> lk(mut_io);
		std::cout << "cap1(0) is not opened" << std::endl;
		return -1;
	}
	else
	{
		std::lock_guard<std::mutex> lk(mut_io);
		std::cout << "cap1(0) is opened " << std::endl;
	}

	cap1.set(CAP_PROP_FRAME_HEIGHT, D_HEIGHT);
	cap1.set(CAP_PROP_FRAME_WIDTH, D_WIDTH);

	if (!exposure_left)
		cap1.set(CV_CAP_PROP_AUTO_EXPOSURE, 1);
	else
		cap1.set(CV_CAP_PROP_EXPOSURE, exposure_left);

	cap1.set(CV_CAP_PROP_FPS, 15);

	std::unique_lock <std::mutex> lck(g_mtx_left);
	while (1)
	{
		g_cv_left.wait(lck);

		do {
			cap1 >> obstacle_left;

		} while (obstacle_left.empty());
		left_time = std::chrono::steady_clock::now();

		g_cv_wait.notify_one();
	}
	return 0;
}

DWORD WINAPI fun2()
{
	VideoCapture cap2(right_cam);

	if (!cap2.isOpened())
	{
		std::lock_guard<std::mutex> lk(mut_io);
		std::cout << "cap2(1) is not opened" << std::endl;
		return -1;
	}
	else
	{
		std::lock_guard<std::mutex> lk(mut_io);
		std::cout << "cap2(1) is opened " << std::endl;
	}

	cap2.set(CAP_PROP_FRAME_HEIGHT, D_HEIGHT);
	cap2.set(CAP_PROP_FRAME_WIDTH, D_WIDTH);

	if (!exposure_right)
		cap2.set(CV_CAP_PROP_AUTO_EXPOSURE, 1);
	else
		cap2.set(CV_CAP_PROP_EXPOSURE, exposure_right);

	cap2.set(CV_CAP_PROP_FPS, 15);

	std::unique_lock <std::mutex> lck(g_mtx_right);
	while (1)
	{
		g_cv_right.wait(lck);

		do
		{
			cap2 >> obstacle_right;

		} while (obstacle_right.empty());
		right_time = std::chrono::steady_clock::now();

		g_cv_wait.notify_one();
	}
	return 0;
}

//#define NEED_VISO
#ifdef NEED_VISO
#include <viso_stereo.h>
#endif

//#define NEED_RECTIFY
#define SENDING
//#define NEED_DISP
#ifdef NEED_DISP
#include "elas.h"
static void F_Gray2Color(CvMat* gray_mat, CvMat* color_mat)
{
	if (color_mat)
		cvZero(color_mat);

	int stype = CV_MAT_TYPE(gray_mat->type), dtype = CV_MAT_TYPE(color_mat->type);
	int rows = gray_mat->rows, cols = gray_mat->cols;

	// 登僅補秘議子業夕才補竃議留科弼夕頁倦寄弌・揖、鯉塀頁倦憲栽勣箔
	if (CV_ARE_SIZES_EQ(gray_mat, color_mat) && stype == CV_8UC1 && dtype == CV_8UC3)
	{
		CvMat* red = cvCreateMat(gray_mat->rows, gray_mat->cols, CV_8U);
		CvMat* green = cvCreateMat(gray_mat->rows, gray_mat->cols, CV_8U);
		CvMat* blue = cvCreateMat(gray_mat->rows, gray_mat->cols, CV_8U);
		CvMat* mask = cvCreateMat(gray_mat->rows, gray_mat->cols, CV_8U);

		// 柴麻光科弼宥祇議・殆峙
		cvSubRS(gray_mat, cvScalar(255), blue);         // blue(I) = 255 - gray(I)
		cvCopy(gray_mat, red);                     // red(I) = gray(I)
		cvCopy(gray_mat, green);                          // green(I) = gray(I),if gray(I) < 128
		cvCmpS(green, 128, mask, CV_CMP_GE);     // green(I) = 255 - gray(I), if gray(I) >= 128
		cvSubRS(green, cvScalar(255), green, mask);
		cvConvertScale(green, green, 2.0, 0.0);

		// 栽撹留科弼夕
		cvMerge(blue, green, red, NULL, color_mat);

		cvReleaseMat(&red);
		cvReleaseMat(&green);
		cvReleaseMat(&blue);
		cvReleaseMat(&mask);
	}
}
#endif
int main()
{

#ifdef NEED_RECTIFY
#define XML_MAP	"map_03011.xml"
	Mat mlx, mly, mrx, mry;
	FileStorage fs(XML_MAP, FileStorage::READ);
	if (!fs.isOpened())
	{
		return -1;
	}

	fs["maplx"] >> mlx;
	fs["maply"] >> mly;
	fs["maprx"] >> mrx;
	fs["mapry"] >> mry;

	fs.release();
#endif
#ifdef NEED_VISO
#define XML_MATRIX "MATRIX_Q_03011.xml"
	cv::Mat p1, p2;
	cv::FileStorage fs_cam(XML_MATRIX, cv::FileStorage::READ);
	fs_cam["p1"] >> p1;
	fs_cam["p2"] >> p2;
	fs_cam.release();
	double f = p1.at<double>(0, 0);
	double cu = p1.at<double>(0, 2);
	double cv = p1.at<double>(1, 2);
	double b = -p2.at<double>(0, 3) / p2.at<double>(0, 0) / 1000;
	fs_cam.release();

	VisualOdometryStereo::parameters param;

	// calibration parameters for sequence 2010_03_09_drive_0019 
	param.calib.f = f; // focal length in pixels
	param.calib.cu = cu; // principal point (u-coordinate) in pixels
	param.calib.cv = cv; // principal point (v-coordinate) in pixels
	param.base = b; // baseline in meters
					//wuying
	param.match.half_resolution = 0;
	param.match.multi_stage = 1;
	param.bucket.max_features = 1;
	//param.match.nms_n=1;
	// init visual odometry
	VisualOdometryStereo viso(param);
	int32_t width = D_WIDTH;
	int32_t height = D_HEIGHT;
	int32_t dims[] = { width,height,width };

#endif
#ifdef NEED_DISP

	Elas::parameters param_elas(Elas::ROBOTICS);
	param_elas.postprocess_only_left = true;
	param_elas.subsampling = 0;
	param_elas.filter_adaptive_mean = 1;
	param_elas.filter_median = 0;
	param_elas.ipol_gap_width = 5000;
	param_elas.add_corners = 1;
	param_elas.disp_max = 256;
	param_elas.disp_min = 0;
	param_elas.useFilter = 1;

	//param_elas.candidate_stepsize = 3;
	//param_elas.incon_min_support = 2;
	Elas elas(param_elas);
#ifndef NEED_VISO
	int32_t width = D_WIDTH;
	int32_t height = D_HEIGHT;
	int32_t dims[] = { width,height,width };
#endif
	// convert input images to uint8_t buffer
	uint8_t* left_img_data = (uint8_t*)malloc(width*height * sizeof(uint8_t));
	uint8_t* right_img_data = (uint8_t*)malloc(width*height * sizeof(uint8_t));
	float* D1 = (float*)malloc(D_WIDTH*D_HEIGHT * sizeof(float));
	float* D2 = (float*)malloc(D_WIDTH*D_HEIGHT * sizeof(float));


#endif
	unsigned int total_count = 0;
	unsigned int drop_count = 0;

	const DWORD sleep_time_min = 10;
	const DWORD sleep_time_max = 200;
	DWORD sleep_time = sleep_time_min;

	string ip_addr;
	//ip_addr="127.0.0.1";
	{
		int ip;
		FileStorage fs("cap.xml", FileStorage::READ);
		if (!fs.isOpened())
		{
			//std::lock_guard<std::mutex> lk(mut);
			std::cout << "can't find xmlfile" << std::endl;
			return 0;
		}

		fs["exposure_left"] >> exposure_left;
		fs["exposure_right"] >> exposure_right;
		fs["left_cam"] >> left_cam;
		fs["right_cam"] >> right_cam;
		fs["ip"] >> ip;
		fs.release();

		if (1 == ip)
			cin >> ip_addr;
		else if (0 == ip)
			ip_addr = "127.0.0.1";
		else
		{
			stringstream ss;
			ss << "192.168.43." << ip;
			ip_addr = ss.str();
		}
	}

	std::thread t1(fun1);
	std::thread t2(fun2);
#ifdef SENDING
	CommunicationCenter c(ip_addr, 4001);
	c.Initialize(callback, NULL);
#endif
	Sleep(5000);
	int picnumber = 0;
	while (1)
	{
		Sleep(sleep_time);
		g_cv_left.notify_one();
		g_cv_right.notify_one();

		std::unique_lock <std::mutex> lck(mut_wait);
		g_cv_wait.wait(lck,
			[] {return (left_time != epoch_time) &&
			(right_time != epoch_time);
		}
		);

		auto timediff = fabs(std::chrono::duration<double, std::milli>(left_time - right_time).count());
		std::cout << "timediff: " << timediff << std::endl;

		++total_count;
		if (timediff > 5)
		{

			std::cout << "\t\t\t\t\t\t\t\tdrop frames" << std::endl;

			if (drop_count == 0)
			{
				drop_count = total_count;
			}
			else
			{
				if (3 > (total_count - drop_count))
				{
					sleep_time = (sleep_time < sleep_time_max) ? (sleep_time + 2) : sleep_time_min;
					std::cout << "\t\t\tsleep_time = " << sleep_time << std::endl;
				}
				drop_count = 0;
				total_count = 0;
			}

			obstacle_left.release();
			obstacle_right.release();
			right_time = left_time = epoch_time;

			continue;
		}

		double t = getTickCount() / getTickFrequency() * 1000;
#ifdef SENDING
		printf("diff:%.2f\n", t - t_last);
		t_last = t;
		c.sendMsg(obstacle_left.data, obstacle_right.data, &t);
#else
		imshow("left", obstacle_left);
		imshow("right", obstacle_right);
#endif

#ifdef NEED_RECTIFY
		Mat left_grey, right_grey;
		cvtColor(obstacle_left, left_grey, CV_BGR2GRAY);
		cvtColor(obstacle_right, right_grey, CV_BGR2GRAY);

		remap(left_grey, left_grey, mlx, mly, INTER_LINEAR);
		remap(right_grey, right_grey, mrx, mry, INTER_LINEAR);

#endif

#ifdef NEED_DISP

		int32_t k = 0;
		for (int32_t v = 0; v<height; v++) {
			for (int32_t u = 0; u<width; u++) {
				left_img_data[k] = left_grey.at<uchar>(v, u);
				right_img_data[k] = right_grey.at<uchar>(v, u);
				k++;
			}
		}



		// compute visual odometry


		elas.process(left_img_data, right_img_data, D1, D2, dims);

		Mat disp(height, width, CV_32FC1);
		Mat disp_withoutFilter(height, width, CV_8UC1);
		disp.data = (uchar*)D1;
		double minval, maxval;
		cv::minMaxLoc(disp, &minval, &maxval);
		if ((int)maxval != 0)
		{
			//disp.convertTo(disp8, CV_8U, 1);
			//disp.convertTo(disp_withoutFilter, CV_8U, 1);
			disp.convertTo(disp_withoutFilter, CV_8U, 255 / maxval);
			imshow("disp", disp_withoutFilter);

			Mat color = Mat(disp.rows, disp.cols, CV_8UC3);
			CvMat* greydisp = &CvMat(disp_withoutFilter);
			F_Gray2Color(greydisp, &CvMat(color));


			for (int j = 0; j < color.rows; j++)
			{
				for (int i = 0; i < color.cols; i++)
				{
					int d;

					d = (int)disp.at<float>(j, i);

					if (d < 0)
					{
						Vec3b black;
						black[0] = black[1] = black[2] = 0;
						color.at<Vec3b>(j, i) = black;
					}
				}
			}
			imshow("disp.color", color);
		}
		else
		{
			printf("elas failed...\n");

			obstacle_left.release();
			obstacle_right.release();
			right_time = left_time = epoch_time;
			continue;
		}



#endif

#ifdef NEED_VISO
		bool replace = true;
		if (picnumber == 0) replace = false;
		if (viso.process(left_img_data, right_img_data, dims, replace))
		{
			double num_matches = viso.getNumberOfMatches();
			double num_inliers = viso.getNumberOfInliers();
			cout << ", Matches: " << num_matches;
			cout << ", Inliers: " << 100.0*num_inliers / num_matches << " %" << endl;
		}

#endif

		char c = waitKey(30);
		if (c == 's')
		{
			stringstream ss;
			//////ss<<"rectify\\left_"<<picnumber<<".bmp";
			imwrite(ss.str(), obstacle_left);
			ss.str("");
			ss << "rectify\\right_" << picnumber << ".bmp";
			imwrite(ss.str(), obstacle_right);
			ss.str("");
#ifdef NEED_DISP
			ss << "disp\\disp_" << picnumber << ".bmp";
			imwrite(ss.str(), disp_withoutFilter);
#endif

#ifdef NEED_VISO
			viso.process(left_img_data, right_img_data, dims);
#endif

			picnumber++;
		}

		obstacle_left.release();
		obstacle_right.release();
		right_time = left_time = epoch_time;
	}

	t1.join();
	t2.join();
	return 0;
}
