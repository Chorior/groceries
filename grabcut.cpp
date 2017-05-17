//#include "CommunicationCenter.h"
//#include "CommonDefine.h"
#include <iostream>
#include <future>
#include <thread>
#include <condition_variable>
#include "opencv2/opencv.hpp"

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

int fun1()
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

	/*cap1.set(CAP_PROP_FRAME_HEIGHT, D_HEIGHT);
	cap1.set(CAP_PROP_FRAME_WIDTH, D_WIDTH);*/

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

int fun2()
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

	/*cap2.set(CAP_PROP_FRAME_HEIGHT, D_HEIGHT);
	cap2.set(CAP_PROP_FRAME_WIDTH, D_WIDTH);*/

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

int main()
{
	unsigned int total_count = 0;
	unsigned int drop_count = 0;

	const int sleep_time_min = 10;
	const int sleep_time_max = 200;
	int sleep_time = sleep_time_min;

	std::thread t1(fun1);
	std::thread t2(fun2);

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
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

		imshow("left", obstacle_left);
		imshow("right", obstacle_right);

		obstacle_left.release();
		obstacle_right.release();
		right_time = left_time = epoch_time;
	}

	t1.join();
	t2.join();
	return 0;
}
