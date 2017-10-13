#include "draw_path.h"

int main(int argc, char *argv[])
{
	assert(argc == 2);
	cv::Mat map = cv::imread(argv[1]);
	cv::cvtColor(map, map, CV_BGR2GRAY);

	draw_path dp(map);
	dp.show_map();
}
