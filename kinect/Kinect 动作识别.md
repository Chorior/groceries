# Kinect for Windows 动作识别

本文为博主使用 Kinect 和 C++ 学习动作识别时的笔记。

## 本文结构

*	[Kinect for Windows 安装](#install)
*	[Hello Kinect](#hello_kinect)
*	[显示彩色图](#show_color)
*	[显示深度图](#show_depth)
*	[显示骨架图](#show_skeleton)
*	[融合骨架图与色彩图](#combine_skeleton_with_color_image)
*	[识别左右挥动手腕](#swing_wrist_left_and_right)
*	[识别上下挥动手腕](#swing_wrist_up_and_down)
*	[总结](#summary)

<h2 id="install">Kinect for Windows 安装</h2>

Google 搜索 Kinect for Windows SDK 下载与 Kinect 版本相对应的 SDK 和 Toolkit，我的是 Kinect for xbox v1 1414，所以使用的是1.8版本。

**拔掉 Kinect** 并安装好环境之后，你会看到三个新的环境变量：

```yml
FTSDK_DIR: C:\Program Files\Microsoft SDKs\Kinect\Developer Toolkit v1.8.0\
KINECT_TOOLKIT_DIR: C:\Program Files\Microsoft SDKs\Kinect\Developer Toolkit v1.8.0\
KINECTSDK10_DIR: C:\Program Files\Microsoft SDKs\Kinect\v1.8\
```

将 Kinect 插入 USB，其驱动程序会自动安装，之后你会在设备管理器中看到 Kinect for Windows，其下有四个节点：

```text
Kinect for Windows Audio Array Control
Kinect for Windows Camera
Kinect for Windows Device
Kinect for Windows Security Control
```

<h2 id="hello_kinect">Hello Kinect</h2>

作为我们第一个 Kinect 程序，它应该足够简单，所以我决定访问并修改 Kinect 相对于重力的倾斜角。

根据[SDK1.8 官方文档](https://msdn.microsoft.com/en-us/library/nuisensor.inuisensor.aspx)，我们知道 Kinect 的倾斜角范围为 [-27°,27°]，所有需要注意的就只有这一点而已。

打开 Visual Studio 并创建一个空项目，分别添加 `$(KINECTSDK10_DIR)\inc` 和 `$(KINECTSDK10_DIR)\lib\amd64` 到包含目录和库目录(32位为x86)，然后添加附加依赖项 `Kinect10.lib` 配置好 Kinect 环境。

通过 Toolkit 的 C++ sample，博主抽离出如下代码用于访问并修改 Kinect 的倾斜角：

```c++
#include <iostream>

#include <Windows.h> // must add before NuiApi.h
#include <NuiApi.h>  // Kincet SDK

int main()
{
	// 检测kinect传感器数量，包含未连接的
	int iSensorCount = 0;
	if (S_OK != NuiGetSensorCount(&iSensorCount)) {
		std::cerr << "There is no Kinect Sensor on the machine!" << std::endl;
		return EXIT_FAILURE;
	}

	// 查找可用的传感器
	INuiSensor *pNuiSensor(nullptr);
	for (int i = 0; i < iSensorCount; ++i) {

		// 创建指定索引的kinect传感器实例
		INuiSensor *pNuiSensor_tmp; // 指向创建的传感器实例
		if (S_OK != NuiCreateSensorByIndex(i, &pNuiSensor_tmp)) {
			continue;
		}

		// 检测kincet是否可用
		if (S_OK == pNuiSensor_tmp->NuiStatus()) {
			pNuiSensor = pNuiSensor_tmp;
			break;
		}

		// 不用的传感器实例需要释放
		pNuiSensor_tmp->Release();
	}

	if (!pNuiSensor) {
		std::cerr << "There is no valid Kincet Sensor to use!" << std::endl;
		return EXIT_FAILURE;
	}

	// 初始化Kinect
	if (S_OK != pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR)) {
		std::cerr << "Initialize the Kinect Sensor failed!" << std::endl;
		return EXIT_FAILURE;
	}

	while (1) {

		// 获取当前Kinect倾斜角
		LONG currAngleDegrees(0L);
		if (S_OK != pNuiSensor->NuiCameraElevationGetAngle(&currAngleDegrees)) {
			std::cerr << "Can't get the angle of Kinect!" << std::endl;
			return EXIT_FAILURE;
		}
		std::cout << "current angle: " << currAngleDegrees << "\n";

		// 修改并设置Kinect倾斜角
		LONG nextAngleDegrees(0L);
		std::cout << "tips:\n"
			<< "\tw: tilt up\n"
			<< "\ts: set to zero\n"
			<< "\tx: tilt down\n"
			<< "\tq: quit\n";

		char c = 'q';
		std::cin >> c;

		switch (c) {
		case 'w':
		{
			nextAngleDegrees = currAngleDegrees + 5L;
			if (nextAngleDegrees > 27L) {
				nextAngleDegrees = 27L;
			}
			break;
		}
		case 's':
		{
			nextAngleDegrees = 0L;
			break;
		}
		case 'x':
		{
			nextAngleDegrees = currAngleDegrees - 5L;
			if (nextAngleDegrees < -27L) {
				nextAngleDegrees = -27L;
			}
			break;
		}
		default:
			break;
		}

		if (c == 'q') {
			break; // quit
		}

		// 设置倾斜角
		pNuiSensor->NuiCameraElevationSetAngle(nextAngleDegrees);
	}

	// 释放传感器
	pNuiSensor->Release();
}
```

<h2 id="show_color">显示彩色图</h2>

由于博主经常接触到的是 opencv，所以再添加 opencv 目录和库到工程中。为了便于快速开发和调试，采用函数式编程。

获取彩色图相对于获取倾斜角来说，只不过需要打开并获取图像流而已，比较难的是 [HANDLE 的使用](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686915(v=vs.85).aspx)以及如何将 Kinect 图像数据转换到 opencv。

下面的代码将不断显示从 Kinect 获取到的彩色图像直到按下 Esc：

```c++
#include <ctime>
#include <string>
#include <iostream>

#include <Windows.h> // must add before NuiApi.h
#include <NuiApi.h>  // Kincet SDK
#include <opencv2/opencv.hpp> // opencv

INuiSensor* valid_kinect_sensor(); // 查找可用的Kinect传感器

cv::Mat get_color(const NUI_IMAGE_FRAME& imageFrame); // 转换kinect图像数据到opencv

void print_image_info(const NUI_IMAGE_FRAME& imageFrame); // 打印图像信息

int main()
{
	INuiSensor* pNuiSensor = valid_kinect_sensor();
	if (!pNuiSensor) {
		std::cerr << "There is no valid Kincet Sensor to use!" << std::endl;
		return EXIT_FAILURE;
	}

	// 初始化Kinect以获取彩色数据
	if (S_OK != pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR)) {
		std::cerr << "Initialize the Kinect Sensor failed!" << std::endl;
		return EXIT_FAILURE;
	}

	while (1) {

		// 图像获取与通知句柄
		HANDLE pColorStreamHandle(INVALID_HANDLE_VALUE); // 用于获取色彩图像流
		HANDLE hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // 下一帧图像通知句柄

		// 打开图像流
		if (S_OK != pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			hNextColorFrameEvent,
			&pColorStreamHandle)) {
			std::cerr << "Open an image stream failed!" << std::endl;
			return EXIT_FAILURE;
		}

		// 下一帧图像准备好了
		if (WAIT_OBJECT_0 == WaitForSingleObject(hNextColorFrameEvent, INFINITE)){

			NUI_IMAGE_FRAME imageFrame; // 图像数据
			if (S_OK == pNuiSensor->NuiImageStreamGetNextFrame(pColorStreamHandle, 0, &imageFrame)) {

				cv::Mat color = get_color(imageFrame);

				if (!color.empty()) {
					cv::imshow("color", color);
					int c = cv::waitKey(2);
					if (c == 27) {
						break;
					}
				}
			}
			// 释放当前图像和句柄
			pNuiSensor->NuiImageStreamReleaseFrame(pColorStreamHandle, &imageFrame);
			if (hNextColorFrameEvent != INVALID_HANDLE_VALUE) {
				CloseHandle(hNextColorFrameEvent);
			}
		}
	}

	// 释放传感器
	pNuiSensor->Release();
}

INuiSensor* valid_kinect_sensor()
{
	// 检测kinect传感器数量，包含未连接的
	int iSensorCount = 0;
	if (S_OK != NuiGetSensorCount(&iSensorCount)) {
		std::cerr << "There is no Kinect Sensor on the machine!" << std::endl;
		return nullptr;
	}

	// 查找可用的传感器	
	for (int i = 0; i < iSensorCount; ++i) {

		// 创建指定索引的kinect传感器实例
		INuiSensor *pNuiSensor_tmp; // 指向创建的传感器实例
		if (S_OK != NuiCreateSensorByIndex(i, &pNuiSensor_tmp)) {
			continue;
		}

		// 检测kincet是否可用
		if (S_OK == pNuiSensor_tmp->NuiStatus()) {
			return pNuiSensor_tmp;
		}

		// 不用的传感器实例需要释放
		pNuiSensor_tmp->Release();
	}

	return nullptr;
}

cv::Mat get_color(const NUI_IMAGE_FRAME& imageFrame)
{
	// 获取图像大小
	DWORD colorHeight, colorWidth;
	NuiImageResolutionToSize(imageFrame.eResolution, colorWidth, colorHeight);

	// 创建结果图
	cv::Mat result(colorHeight, colorWidth, CV_8UC4);

	// 打印并获取色彩图像信息
	print_image_info(imageFrame);
	INuiFrameTexture *pTexture = imageFrame.pFrameTexture;

	// 读取图像数据时需要锁住
	NUI_LOCKED_RECT LockedRect;
	if (S_OK == pTexture->LockRect(0, &LockedRect, NULL, 0)) {

		// 数据是否有效
		if (LockedRect.Pitch == 0) {
			return cv::Mat();
		}

		// 拷贝数据到cv::Mat
		BYTE* data = static_cast<BYTE *>(LockedRect.pBits);
		for (UINT y = 0; y < colorHeight; ++y)
		{
			cv::Vec4b* pColorRow = result.ptr<cv::Vec4b>(y);
			for (UINT x = 0; x < colorWidth; ++x)
			{
				pColorRow[x] = cv::Vec4b(data[y * LockedRect.Pitch + x * 4 + 0],
					data[y * LockedRect.Pitch + x * 4 + 1],
					data[y * LockedRect.Pitch + x * 4 + 2],
					data[y * LockedRect.Pitch + x * 4 + 3]);
			}
		}

		// 解锁
		pTexture->UnlockRect(0);
	}

	return result;
}

void print_image_info(const NUI_IMAGE_FRAME& imageFrame)
{
	// 时间戳
	std::cout << imageFrame.liTimeStamp.QuadPart << " ";

	// 数量
	std::cout << "frame " << imageFrame.dwFrameNumber << " ";

	// 类型
	switch (imageFrame.eImageType) {
	case NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX:
		std::cout << "DEPTH_AND_PLAYER_INDEX ";
		break;
	case NUI_IMAGE_TYPE_COLOR:
		std::cout << "COLOR ";
		break;
	case NUI_IMAGE_TYPE_COLOR_YUV:
		std::cout << "COLOR_YUV ";
		break;
	case NUI_IMAGE_TYPE_COLOR_RAW_YUV:
		std::cout << "COLOR_RAW_YUV ";
		break;
	case NUI_IMAGE_TYPE_DEPTH:
		std::cout << "DEPTH ";
		break;
	case NUI_IMAGE_TYPE_COLOR_INFRARED:
		std::cout << "COLOR_INFRARED ";
		break;
	case NUI_IMAGE_TYPE_COLOR_RAW_BAYER:
		std::cout << "COLOR_RAW_BAYER ";
		break;
	default:
		break;
	}

	// 大小
	switch (imageFrame.eResolution) {
	case NUI_IMAGE_RESOLUTION_80x60:
		std::cout << "80x60 ";
		break;
	case NUI_IMAGE_RESOLUTION_320x240:
		std::cout << "320x240 ";
		break;
	case NUI_IMAGE_RESOLUTION_640x480:
		std::cout << "640x480 ";
		break;
	case NUI_IMAGE_RESOLUTION_1280x960:
		std::cout << "1280x960 ";
		break;
	default:
		break;
	}

	// FPS
	static clock_t clock_pre = clock();
	clock_t clock_curr = clock();

	if (clock_curr != clock_pre) {
		unsigned ufps = CLOCKS_PER_SEC / (clock_curr - clock_pre);
		clock_pre = clock_curr;

		std::cout << "fps " << ufps << std::endl;
	}
	else {
		std::cout << std::endl;
	}
}
```

<h2 id="show_depth">显示深度图</h2>

获取深度图与获取彩色图类似，只需要修改相应参数：

```c++
pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH);
pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			hNextColorFrameEvent,
			&pColorStreamHandle);
```

并根据[SDK1.8 官方文档](https://msdn.microsoft.com/en-us/library/nuiimagecamera.nui_image_frame.aspx/#remarks)，注意深度图数据类型与色彩图像数据类型的不同，参照 Toolkit sample 构建出 Kinect 深度图数据到 opencv 的转换函数：

```c++
cv::Mat get_depth(const NUI_IMAGE_FRAME& imageFrame)
{
	// 获取图像大小
	DWORD depthHeight, depthWidth;
	NuiImageResolutionToSize(imageFrame.eResolution, depthWidth, depthHeight);

	// 创建结果图
	cv::Mat result(depthHeight, depthWidth, CV_16U);

	// 打印并获取色彩图像信息
	print_image_info(imageFrame);
	INuiFrameTexture *pTexture = imageFrame.pFrameTexture;

	// 读取图像数据时需要锁住
	NUI_LOCKED_RECT LockedRect;
	if (S_OK == pTexture->LockRect(0, &LockedRect, NULL, 0)) {

		// 数据是否有效
		if (LockedRect.Pitch == 0) {
			return cv::Mat();
		}

		// 拷贝数据到cv::Mat
		USHORT* data = reinterpret_cast<USHORT *>(LockedRect.pBits);
		for (UINT y = 0; y < depthHeight; ++y)
		{
			// Get row pointer for depth Mat
			USHORT* pDepthRow =result.ptr<USHORT>(y);

			for (UINT x = 0; x < depthWidth; ++x)
			{
				pDepthRow[x] = data[y * depthWidth + x];
			}
		}

		// 解锁
		pTexture->UnlockRect(0);
	}

	return result;
}
```

最后在主循环中替换掉原来的 `get_color` 即可：

```c++
NUI_IMAGE_FRAME imageFrame; // 图像数据
if (S_OK == pNuiSensor->NuiImageStreamGetNextFrame(pColorStreamHandle, 0, &imageFrame)) {

	cv::Mat depth = get_depth(imageFrame);

	if (!depth.empty()) {
		cv::imshow("depth", depth);
		int c = cv::waitKey(2);
		if (c == 27) {
			break;
		}
	}
}
```

<h2 id="show_skeleton">显示骨架图</h2>

你只需要参照 Toolkit sample 和 SDK 文档一步步理解并抽取相应代码就能获得骨架图了。

下面的代码由于使能了坐姿骨架，所以只有上半身的骨架数据：

```c++
#include <ctime>
#include <array>
#include <string>
#include <iostream>

#include <Windows.h> // must add before NuiApi.h
#include <NuiApi.h>  // Kincet SDK
#include <opencv2/opencv.hpp> // opencv

INuiSensor* valid_kinect_sensor(); // 查找可用的Kinect传感器

cv::Mat get_skeleton(const NUI_SKELETON_DATA& skel); // 转换kinect骨架数据到opencv

void draw_bone(cv::Mat& img, const std::array<cv::Point, NUI_SKELETON_POSITION_COUNT>& points,
	const NUI_SKELETON_DATA& skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1); // 绘制骨架

void print_skeleton_info(const NUI_SKELETON_FRAME& skeletonFrame); // 打印骨架信息

int main()
{
	INuiSensor* pNuiSensor = valid_kinect_sensor();
	if (!pNuiSensor) {
		std::cerr << "There is no valid Kincet Sensor to use!" << std::endl;
		return EXIT_FAILURE;
	}

	// 初始化Kinect以获取骨架数据
	if (S_OK != pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON)) {
		std::cerr << "Initialize the Kinect Sensor failed!" << std::endl;
		return EXIT_FAILURE;
	}

	while (1) {

		// 骨架数据通知句柄
		HANDLE hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

		// 使能坐姿骨架跟踪模式
		if (S_OK != pNuiSensor->NuiSkeletonTrackingEnable(hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT)) {
			std::cerr << "Enable skeleton tracking failed!" << std::endl;
			return EXIT_FAILURE;
		}

		// 骨架数据准备好了
		if (WAIT_OBJECT_0 == WaitForSingleObject(hNextSkeletonEvent, INFINITE)) {

			NUI_SKELETON_FRAME skeletonFrame = { 0 }; // 骨架数据
			if (S_OK == pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)) {

				// 使用默认参数进行滤波
				pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

				// 打印骨架数据信息
				print_skeleton_info(skeletonFrame);

				// 一帧最多识别NUI_SKELETON_COUNT个骨架
				for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {

					// 转换NUI_SKELETON_DATA到opencv
					cv::Mat skeleton = get_skeleton(skeletonFrame.SkeletonData[i]);

					if (!skeleton.empty()) {
						cv::imshow("skeleton", skeleton);
					}
				}

				// 退出指令
				int c = cv::waitKey(2);
				if (c == 27) {
					break;
				}
			}

			// 释放句柄
			if (hNextSkeletonEvent != INVALID_HANDLE_VALUE) {
				CloseHandle(hNextSkeletonEvent);
			}
		}
	}

	// 释放传感器
	pNuiSensor->Release();
}

INuiSensor* valid_kinect_sensor()
{
	// 检测kinect传感器数量，包含未连接的
	int iSensorCount = 0;
	if (S_OK != NuiGetSensorCount(&iSensorCount)) {
		std::cerr << "There is no Kinect Sensor on the machine!" << std::endl;
		return nullptr;
	}

	// 查找可用的传感器	
	for (int i = 0; i < iSensorCount; ++i) {

		// 创建指定索引的kinect传感器实例
		INuiSensor *pNuiSensor_tmp; // 指向创建的传感器实例
		if (S_OK != NuiCreateSensorByIndex(i, &pNuiSensor_tmp)) {
			continue;
		}

		// 检测kincet是否可用
		if (S_OK == pNuiSensor_tmp->NuiStatus()) {
			return pNuiSensor_tmp;
		}

		// 不用的传感器实例需要释放
		pNuiSensor_tmp->Release();
	}

	return nullptr;
}

void print_skeleton_info(const NUI_SKELETON_FRAME& skeletonFrame)
{
	// 时间戳
	std::cout << skeletonFrame.liTimeStamp.QuadPart << " ";

	// 数量
	std::cout << "frame " << skeletonFrame.dwFrameNumber << " ";

	// 类型
	std::cout << "SKELETON ";

	// FPS
	static clock_t clock_pre = clock();
	clock_t clock_curr = clock();

	if (clock_curr != clock_pre) {
		unsigned ufps = CLOCKS_PER_SEC / (clock_curr - clock_pre);
		clock_pre = clock_curr;

		std::cout << "fps " << ufps << std::endl;
	}
	else {
		std::cout << std::endl;
	}
}

cv::Mat get_skeleton(const NUI_SKELETON_DATA& skel)
{
	// 用于绘制骨架
	cv::Mat skeleton = cv::Mat::zeros(240, 320, CV_8UC3);

	// 用于保存骨架关节点在深度图的位置
	std::array<cv::Point, NUI_SKELETON_POSITION_COUNT> joints;

	NUI_SKELETON_TRACKING_STATE trackingState = skel.eTrackingState;

	// 所有骨节都被跟踪了
	if (NUI_SKELETON_TRACKED == trackingState) {

		// 每个Vector4包含该骨架的一个骨节位置信息
		for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
		{
			// 获取骨架关节点位于深度图的位置和深度值
			LONG x, y;
			USHORT depth;

			// 转换所用的深度图大小为320x240
			NuiTransformSkeletonToDepthImage(skel.SkeletonPositions[j], &x, &y, &depth);

			joints[j] = cv::Point(x, y);
		}
	}
	// 只有骨架的中点被跟踪了
	else if (NUI_SKELETON_POSITION_ONLY == trackingState) {
		// 将骨架中心转换到深度图
		LONG x, y;
		USHORT depth;
		NuiTransformSkeletonToDepthImage(skel.Position, &x, &y, &depth);

		// 以骨架中点为圆心绘制一个白色的圆
		cv::circle(skeleton, cv::Point(x, y), 10, cv::Scalar(255, 255, 255), 4, cv::LINE_AA);

		return skeleton;
	}
	// 该骨架未被跟踪
	else {
		return cv::Mat();
	}

	// http://dl2.iteye.com/upload/attachment/0089/8447/32dde3be-db41-35b3-837d-f07e2a36bd78.png
	// 躯干
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

	// 左手
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

	// 右手
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	// 左腿
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

	// 右腿
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
	draw_bone(skeleton, joints, skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

	return skeleton;
}

void draw_bone(cv::Mat& img, const std::array<cv::Point, NUI_SKELETON_POSITION_COUNT>& points,
	const NUI_SKELETON_DATA& skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1)
{
	// 指示相应的骨节状态：未跟踪、正在跟踪、根据以前数据推断而出
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[joint0];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[joint1];

	// 有一个骨节未被跟踪
	if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED)
	{
		return;
	}

	// 两个骨节都是推断而出
	if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED)
	{
		return;
	}

	// 两个骨节都是正在跟踪状态，则以绿色线画出
	if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED)
	{
		cv::line(img, points[joint0], points[joint1], cv::Scalar(0, 255, 0), 4, cv::LINE_AA);
	}
	// 一个骨节正在跟踪、一个骨节推断而出，则以红色线画出
	else
	{
		cv::line(img, points[joint0], points[joint1], cv::Scalar(0, 0, 255), 4, cv::LINE_AA);
	}
}
```

<h2 id="combine_skeleton_with_color_image">融合骨架图与色彩图</h2>

光有骨架图不知道是不是对齐的，所以将骨架绘制到色彩图上，那样就很直观了。

既然骨架数据是 320x240 的，那么获取色彩图之后，把它缩小成这么大不就好了。

把上面的代码合并一下，稍微改改就行：

```c++
#include <ctime>
#include <array>
#include <string>
#include <iostream>

#include <Windows.h> // must add before NuiApi.h
#include <NuiApi.h>  // Kincet SDK
#include <opencv2/opencv.hpp> // opencv

INuiSensor* valid_kinect_sensor(); // 查找可用的Kinect传感器

cv::Mat get_color(const NUI_IMAGE_FRAME& imageFrame); // 转换kinect色彩图像数据到opencv

cv::Mat draw_skeleton(cv::Mat color, const NUI_SKELETON_DATA& skel); // 将骨架绘制到color上

void draw_bone(cv::Mat& img, const std::array<cv::Point, NUI_SKELETON_POSITION_COUNT>& points,
	const NUI_SKELETON_DATA& skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1); // 绘制骨架

void print_image_info(const NUI_IMAGE_FRAME& imageFrame); // 打印图像信息

void print_skeleton_info(const NUI_SKELETON_FRAME& skeletonFrame); // 打印骨架信息

int main()
{
	INuiSensor* pNuiSensor = valid_kinect_sensor();
	if (!pNuiSensor) {
		std::cerr << "There is no valid Kincet Sensor to use!" << std::endl;
		return EXIT_FAILURE;
	}

	// 初始化Kinect以获取骨架数据
	DWORD dwFlags = NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON;
	if (S_OK != pNuiSensor->NuiInitialize(dwFlags)) {
		std::cerr << "Initialize the Kinect Sensor failed!" << std::endl;
		return EXIT_FAILURE;
	}

	while (1) {

		// 图像获取与通知句柄
		HANDLE pColorStreamHandle(INVALID_HANDLE_VALUE); // 用于获取色彩图像流
		HANDLE hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // 下一帧图像通知句柄

		// 打开图像流
		if (S_OK != pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			hNextColorFrameEvent,
			&pColorStreamHandle)) {
			std::cerr << "Open an image stream failed!" << std::endl;
			return EXIT_FAILURE;
		}

		// 色彩图准备好了
		cv::Mat color;
		if (WAIT_OBJECT_0 == WaitForSingleObject(hNextColorFrameEvent, INFINITE)) {

			NUI_IMAGE_FRAME imageFrame = { 0 }; // 图像数据
			if (S_OK == pNuiSensor->NuiImageStreamGetNextFrame(pColorStreamHandle, 0, &imageFrame)) {
				// 得到色彩图
				color = get_color(imageFrame);

				if (!color.empty()) {
					// 缩小为320x240
					cv::resize(color, color, cv::Size(320, 240));
					cv::imshow("color", color);
				}

				// 释放当前图像
				pNuiSensor->NuiImageStreamReleaseFrame(pColorStreamHandle, &imageFrame);
			}
		}

		// 释放句柄			
		if (hNextColorFrameEvent != INVALID_HANDLE_VALUE) {
			CloseHandle(hNextColorFrameEvent);
		}

		// 骨架数据通知句柄
		HANDLE hNextSkeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		// 使能坐姿骨架跟踪模式
		if (S_OK != pNuiSensor->NuiSkeletonTrackingEnable(hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT)) {
			std::cerr << "Enable skeleton tracking failed!" << std::endl;
			return EXIT_FAILURE;
		}

		// 骨架数据准备好了
		if (WAIT_OBJECT_0 == WaitForSingleObject(hNextSkeletonEvent, INFINITE)) {			

			NUI_SKELETON_FRAME skeletonFrame = { 0 }; // 骨架数据
			if (S_OK == pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)) {

				// 使用默认参数进行滤波
				pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

				// 打印骨架数据信息
				print_skeleton_info(skeletonFrame);
				
				// 一帧最多识别NUI_SKELETON_COUNT个骨架
				for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {

					// 转换NUI_SKELETON_DATA到opencv
					cv::Mat skeleton = draw_skeleton(color, skeletonFrame.SkeletonData[i]);
					
					// 显示图像
					if (!skeleton.empty()) {
						cv::imshow("skel_color", skeleton);
					}
				}
			}			
		}

		// 退出指令
		int c = cv::waitKey(2);
		if (c == 27) {
			break;
		}

		// 释放句柄						
		if (hNextSkeletonEvent != INVALID_HANDLE_VALUE) {
			CloseHandle(hNextSkeletonEvent);
		}
	}

	// 释放传感器
	pNuiSensor->Release();
}

cv::Mat draw_skeleton(cv::Mat color, const NUI_SKELETON_DATA& skel)
{
	// 若色彩图为空，则只绘制骨架图
	if (color.empty()) {
		color = cv::Mat::zeros(240, 320, CV_8UC3);
	}

	// 用于保存骨架关节点在色彩图的位置
	std::array<cv::Point, NUI_SKELETON_POSITION_COUNT> joints;

	NUI_SKELETON_TRACKING_STATE trackingState = skel.eTrackingState;

	// 所有骨节都被跟踪了
	if (NUI_SKELETON_TRACKED == trackingState) {		

		// 每个Vector4包含该骨架的一个骨节位置信息
		for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
		{
			// 获取骨架关节点位于深度图的位置和深度值
			LONG x, y;
			USHORT depth;

			// 转换所用的深度图大小为320x240
			NuiTransformSkeletonToDepthImage(skel.SkeletonPositions[j], &x, &y, &depth);

			joints[j] = cv::Point(x, y);
		}
	}
	// 只有骨架的中点被跟踪了
	else if (NUI_SKELETON_POSITION_ONLY == trackingState) {
		// 将骨架中心转换到深度图
		LONG x, y;
		USHORT depth;
		NuiTransformSkeletonToDepthImage(skel.Position, &x, &y, &depth);

		// 以骨架中点为圆心绘制一个白色的圆
		cv::circle(color, cv::Point(x, y), color.cols / 32, cv::Scalar(255, 255, 255), 4, cv::LINE_AA);

		return color;
	}
	// 该骨架未被跟踪
	else {
		return cv::Mat();
	}

	// 根据NUI_SKELETON_POSITION_INDEX指示的索引绘制骨架
	// 躯干
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

	// 左手
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

	// 右手
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	// 左腿
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

	// 右腿
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
	draw_bone(color, joints, skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

	return color;
}
```

<h2 id="swing_wrist_left_and_right">识别左右挥动手腕</h2>

有了上面的骨架图，就可以开始简单的动作识别了。

博主的第一个任务是识别手左右挥动这个超级简单的动作。首先，手左右挥动最明显的特征是什么？不管你怎么挥，你的**手腕总是要左右移动的**吧。

由于一帧可能识别多个骨架，博主只取第一个有效的骨架进行识别：

```c++
// 只取第一个骨架进行识别
for (int i = 0; i < NUI_SKELETON_COUNT; ++i) {

	cv::Mat skeleton = get_skeleton(skeletonFrame.SkeletonData[i]);

	if (!skeleton.empty()) {
		cv::imshow("skeleton", skeleton);
		break;
	}
}
```

博主试着在 Kinect 前面左右挥动右手，然后使用如下代码来保存得到的骨架图：

```c++
static int count;
std::string skel_name = "skeleton_" + std::to_string(++count) + ".png";

cv::cvtColor(skeleton, skeleton, cv::COLOR_BGRA2BGR);
cv::imwrite(skel_name, skeleton);
```

那么如何来衡量左右挥动的程度呢？为了使问题更加容易解决，博主假定右手挥动的姿势是手腕绕着手肘挥动，这样就能以手腕节点和手肘节点的线段、到以手肘节点和右肩节点的线段的角度来衡量左右挥动的程度了。

下面是计算右腕肘肩角度的函数：

```c++
float calculate_wrist_curvature(const NUI_SKELETON_DATA& skel)
{	
	// 检查骨架的状态
	NUI_SKELETON_TRACKING_STATE trackingState = skel.eTrackingState;
	if (NUI_SKELETON_TRACKED != trackingState) {
		return 0.0f;
	}

	// 右腕、肘、肩位置信息
	Vector4 wrist = skel.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	Vector4 elbow = skel.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT];
	Vector4 shoulder = skel.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT];

	// 为Vector4创建一个减法
	auto sub_func = [](const Vector4& p1, const Vector4& p2)->Vector4
	{
		Vector4 ret;
		ret.x = p1.x - p2.x;
		ret.y = p1.y - p2.y;
		ret.z = p1.z - p2.z;
		ret.w = p1.w - p2.w;

		return ret;
	};

	// 肘腕、肘肩向量
	Vector4 vec_ew = sub_func(wrist, elbow);
	Vector4 vec_es = sub_func(shoulder, elbow);	

	// 计算腕肘肩角度
	float cos_theta = (vec_ew.x*vec_es.x + vec_ew.y*vec_es.y + vec_ew.z*vec_es.z) /
		std::sqrt((vec_ew.x*vec_ew.x + vec_ew.y*vec_ew.y + vec_ew.z*vec_ew.z)*(vec_es.x*vec_es.x + vec_es.y*vec_es.y + vec_es.z*vec_es.z));

	float theta = 0.0f;
	if (cos_theta >= -1.0f && cos_theta <= 1.0f) {
		theta = std::acos(cos_theta) * 57.325f;
	}
	return theta;
}
```

博主在保存图像的时候，将这个函数的结果保存在图片名里面，这样就既有图像信息、又有角度信息了：

```c++
// 测试
float theta = calculate_wrist_curvature(skeletonFrame.SkeletonData[i]);

static int count;
std::string skel_name = "skeleton_" + std::to_string(++count) + "_" + std::to_string(theta) + ".png";

cv::cvtColor(skeleton, skeleton, cv::COLOR_BGRA2BGR);
cv::imwrite(skel_name, skeleton);
```

博主按照从慢到快的速度挥动右手臂，根据角度的递增递减区间截取最值得到如下三组数据：

序号 | 数据1 | 序号 | 数据2 | 序号 | 数据3
---- | ---- | ---- | ---- | ---- | ----
29 | 151.90 | 7 | 43.49 | 16 | 159.30
57 | 70.40 | 27 | 156.08 | 21 | 47.62
90 | 155.17 | 47 | 44.35 | 26 | 144.48
123 | 66.78 | 65 | 157.99 | 30 | 56.54
160 | 155.30 | 84 | 54.14 | 33 | 145.33
191 | 65.60 | 100 | 158.62 | 37 | 56.13
226 | 145.81 | 118 | 54.89 | 41 | 137.45

通过这三组数据我们知道：

*	随着挥动速度的增大，角度变化的周期不断减小，但又不是恒定不变，从慢到快依次是 33、20、4；
*	不管挥动的速度如何，角度的变化一定是U字型，不管是倒U还是正U。

我们就**根据角度的变化呈U字型这一特征来判别手腕左右挥动这一动作**吧。既然是左右挥动，那么至少两个U才行，介于主函数现在过于臃肿，动作识别又是跟 Kinect 不相关的另一个主题，所以博主新建了一个头文件 `action_recognition.h` 并稍微修改了一下计算腕肘肩角度的函数：

```c++
#pragma once
#include <cmath>

#include <NuiApi.h>  // Kincet SDK

#define ACTION_UNRECOGNIZED 0x00000000 // 未识别动作
#define SWING_WRIST_LEFT    0x00000001 // 左右挥动左手腕
#define SWING_WRIST_RIGHT   0x00000002 // 左右挥动右手腕

// 为Vector4创建一个减法
Vector4 operator-(const Vector4& p1, const Vector4& p2)
{
	Vector4 ret;
	ret.x = p1.x - p2.x;
	ret.y = p1.y - p2.y;
	ret.z = p1.z - p2.z;
	ret.w = p1.w - p2.w;

	return ret;
}

// 计算腕肘肩角度
float calculate_wrist_curvature(const NUI_SKELETON_DATA& skel, bool isLeft)
{
	// 检查骨架跟踪状态
	NUI_SKELETON_TRACKING_STATE trackingState = skel.eTrackingState;
	if (NUI_SKELETON_TRACKED != trackingState) {
		return 0.0f;
	}

	// 腕、肘、肩索引
	NUI_SKELETON_POSITION_INDEX index_wrist, index_elbow, index_shoulder;
	if (isLeft) {
		index_wrist = NUI_SKELETON_POSITION_WRIST_LEFT;
		index_elbow = NUI_SKELETON_POSITION_ELBOW_LEFT;
		index_shoulder = NUI_SKELETON_POSITION_SHOULDER_LEFT;		
	}
	else {
		index_wrist = NUI_SKELETON_POSITION_WRIST_RIGHT;
		index_elbow = NUI_SKELETON_POSITION_ELBOW_RIGHT;
		index_shoulder = NUI_SKELETON_POSITION_SHOULDER_RIGHT;		
	}

	// 都被追踪才计算角度
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[index_wrist];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[index_elbow];
	NUI_SKELETON_POSITION_TRACKING_STATE joint2State = skel.eSkeletonPositionTrackingState[index_shoulder];

	if (joint0State != NUI_SKELETON_POSITION_TRACKED ||
		joint1State != NUI_SKELETON_POSITION_TRACKED ||
		joint2State != NUI_SKELETON_POSITION_TRACKED) {
		return 0.0f;
	}

	// 腕、肘、肩位置
	Vector4 wrist = skel.SkeletonPositions[index_wrist];
	Vector4 elbow = skel.SkeletonPositions[index_elbow];
	Vector4 shoulder = skel.SkeletonPositions[index_shoulder];

	// 肘腕、肘肩向量
	Vector4 vec_ew = wrist - elbow;
	Vector4 vec_es = shoulder - elbow;

	// 计算腕肘肩角度
	float cos_theta = (vec_ew.x*vec_es.x + vec_ew.y*vec_es.y + vec_ew.z*vec_es.z) /
		std::sqrt((vec_ew.x*vec_ew.x + vec_ew.y*vec_ew.y + vec_ew.z*vec_ew.z)*(vec_es.x*vec_es.x + vec_es.y*vec_es.y + vec_es.z*vec_es.z));

	float theta = 0.0f;
	if (cos_theta >= -1.0f && cos_theta <= 1.0f) {
		theta = std::acos(cos_theta) * 57.325f;
	}
	return theta;
}

// 是否挥动手腕
bool is_swing_wrist(UCHAR& isSwing, float& theta_pre, float theta_curr)
{
	bool result = FALSE;
	float diff_theta = theta_curr - theta_pre;

	// 初始状态或没有挥动
	if (theta_pre == 0.0f || std::abs(diff_theta) <= 5.0f) {
		isSwing = 0x00;
	}
	// 第一次挥动
	else if (0x00 == isSwing) {
		if (diff_theta < 0.0f) {
			isSwing = 0x2F;
		}
		else {
			isSwing = 0x20;
		}
	}
	else {
		UCHAR hiPart = isSwing & 0xF0; // 高四位
		UCHAR loPart = isSwing & 0x0F; // 低四位

		// 与上次挥动方向相反
		if ((loPart == 0x00 && diff_theta < 0.0f) ||
			(loPart == 0x0F && diff_theta > 0.0f))
		{
			// 第四位置位或清零
			loPart ^= 0x0F;
			
			switch (hiPart) {			
			case 0x20:
				isSwing = 0x60 + loPart;
				break;
			case 0x60:
				isSwing = 0xE0 + loPart;
				break;
			case 0xE0:
				isSwing = 0xE0 + loPart;
				break;
			default:
				isSwing = 0x00;
			}			
		}

		// 满足双U条件
		if (hiPart == 0xE0) {
			result = TRUE;
		}
	}

	theta_pre = theta_curr;
	return result;
}

// 挥动手腕类型
LONG get_swing_wrist_type(const NUI_SKELETON_DATA& skel)
{
	// 高四位每一位表示当前角度变化趋势是否与上次变化趋势相反，第一位不算
	// 低四位0000表示上一次角度逐渐变大，1111表示上一次角度逐渐减小
	static UCHAR isSwingLeft = 0x00;
	static UCHAR isSwingRigt = 0x00;

	static float theta_left_pre = 0.0f; // 用于保存上一次的左腕肘肩角度
	static float theta_rigt_pre = 0.0f; // 用于保存上一次的右腕肘肩角度

	static const unsigned INTERVAL = 4; // 间隔次数，因为挥动左右边界速度会降低，导致角度变化不明显
	static int count = INTERVAL - 1;    // 控制间隔

	static LONG actionType = ACTION_UNRECOGNIZED; // 间隔期间保存上一次的动作类型

	if (++count != INTERVAL) {
		return actionType;
	}
	else {
		count = -1;
	}

	float theta_left_curr = calculate_wrist_curvature(skel, TRUE);
	float theta_rigt_curr = calculate_wrist_curvature(skel, FALSE);

	actionType = ACTION_UNRECOGNIZED;
	if (is_swing_wrist(isSwingLeft, theta_left_pre, theta_left_curr)) {
		actionType |= SWING_WRIST_LEFT;
	}

	if (is_swing_wrist(isSwingRigt, theta_rigt_pre, theta_rigt_curr)) {
		actionType |= SWING_WRIST_RIGHT;
	}

	return actionType;
}
```

通过调用上面的 `get_swing_wrist_type` 函数，我们就能在主循环中得到挥动手腕的类型了：

```c++
if (!skeleton.empty()) {						

	// 识别动作类型
	LONG actionType = get_swing_wrist_type(skeletonFrame.SkeletonData[i]);

	// 在图像上绘制动作类型						
	if (actionType & SWING_WRIST_LEFT) {
		cv::String str_action = "swing left hand";
		cv::putText(skeleton, str_action, cv::Point(10, 15),
			cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
	}
	if (actionType & SWING_WRIST_RIGHT) {
		cv::String str_action = "swing right hand";
		cv::putText(skeleton, str_action, cv::Point(10, 30),
			cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
	}
	if (actionType == ACTION_UNRECOGNIZED) {
		cv::String str_action = "unrecognized action";
		cv::putText(skeleton, str_action, cv::Point(10, 45),
			cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
	}

	// 显示图像
	cv::imshow("skeleton", skeleton);
	break;
}
```

**`get_swing_wrist_type` 必须在调用 `get_skeleton` 或 `draw_skeleton` 函数之后才能调用**，查看那两个函数，发现它们只不过检查了骨架的跟踪状态，仅此而已，所以你也可以像下面这般调用该函数：

```c++
if (NUI_SKELETON_TRACKED == skeletonFrame.SkeletonData[i].eTrackingState) {
	LONG actionType = get_swing_wrist_type(skeletonFrame.SkeletonData[i]);
				
	// ...

	break;
}
```

<h2 id="swing_wrist_up_and_down">识别上下挥动手腕</h2>

有了上面识别左右挥动手腕的经验，要想识别上下挥动手腕，只需想办法衡量上下挥动的程度，然后满足挥动两个来回就算识别成功了。

博主不断的上下挥动手腕，发现**手腕离摄像头的距离会经历从大到小和从小到大两个过程**，猜想手腕上下挥动的特征为：**手腕位置的Z值会呈现出U字型变化**。

因为距离不太好度量，所以将距离转化为手肘到手腕组成的射线与Z轴正方向的角度。

那么有必要为计算角度单独写一个函数了：

```c++
// 计算两条向量的夹角
inline float calculate_the_angle(const Vector4& v1, const Vector4& v2)
{
	float theta = 0.0f;

	float dist = std::sqrt((v1.x*v1.x + v1.y*v1.y + v1.z*v1.z)*(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z));
	if (dist == 0.0f) {
		return theta;
	}

	float cos_theta = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / dist;
	if (cos_theta >= -1.0f && cos_theta <= 1.0f) {
		theta = std::acos(cos_theta) * 57.325f;
	}

	return theta;
}
```

下面是计算肘腕与Z轴正方向的角度的函数：

```c++
float wrist_angle_with_Zaxis(const NUI_SKELETON_DATA& skel, bool isLeft)
{
	// 检查骨架跟踪状态
	NUI_SKELETON_TRACKING_STATE trackingState = skel.eTrackingState;
	if (NUI_SKELETON_TRACKED != trackingState) {
		return 0.0f;
	}

	// 腕、肘索引
	NUI_SKELETON_POSITION_INDEX index_wrist, index_elbow;
	if (isLeft) {
		index_wrist = NUI_SKELETON_POSITION_WRIST_LEFT;
		index_elbow = NUI_SKELETON_POSITION_ELBOW_LEFT;		
	}
	else {
		index_wrist = NUI_SKELETON_POSITION_WRIST_RIGHT;
		index_elbow = NUI_SKELETON_POSITION_ELBOW_RIGHT;		
	}

	// 都被追踪才计算角度
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[index_wrist];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[index_elbow];

	if (joint0State != NUI_SKELETON_POSITION_TRACKED ||
		joint1State != NUI_SKELETON_POSITION_TRACKED) {
		return 0.0f;
	}

	// 腕、肘位置
	Vector4 wrist = skel.SkeletonPositions[index_wrist];
	Vector4 elbow = skel.SkeletonPositions[index_elbow];

	// 肘腕向量
	Vector4 vec_ew = wrist - elbow;

	// Z轴正向向量
	Vector4 vec_pos_Z{ 0, 0, 1, 0 };

	// 计算肘腕与Z轴正向的角度
	float theta = calculate_the_angle(vec_ew, vec_pos_Z);
	return theta;
}
```

有了这个函数，我们就能像识别左右挥动手腕那样进行识别了(博主为了区分，稍微改了一下宏和变量的名字)：

```c++
#define ACTION_UNRECOGNIZED                  0x00000000 // 未识别动作
#define SWING_WRIST_LEFT_AND_RIGHT_LEFT      0x00000001 // 左右挥动左手腕
#define SWING_WRIST_LEFT_AND_RIGHT_RIGHT     0x00000002 // 左右挥动右手腕
#define SWING_WRIST_UP_AND_DOWN_LEFT         0x00000004 // 上下挥动左手腕
#define SWING_WRIST_UP_AND_DOWN_RIGHT        0x00000008 // 上下挥动右手腕

// ...

// 挥动手腕类型
LONG get_swing_wrist_type(const NUI_SKELETON_DATA& skel)
{
	// 高四位每一位表示当前角度变化趋势是否与上次变化趋势相反，第一位不算
	// 低四位0000表示角度逐渐变大，1111表示角度逐渐减小
	static UCHAR isSwingLR_Left = 0x00;
	static UCHAR isSwingLR_Rigt = 0x00;

	static UCHAR isSwingUD_Left = 0x00;
	static UCHAR isSwingUD_Rigt = 0x00;

	static float theta_LR_left_pre = 0.0f; // 用于保存上一次的左腕肘肩角度
	static float theta_LR_rigt_pre = 0.0f; // 用于保存上一次的右腕肘肩角度

	static float theta_UD_left_pre = 0.0f; // 用于保存上一次的左肘腕与Z轴正向的角度
	static float theta_UD_rigt_pre = 0.0f; // 用于保存上一次的右肘腕与Z轴正向的角度

	static const unsigned INTERVAL = 4; // 间隔次数，因为挥动左右边界速度会降低
	static int count = INTERVAL - 1;    // 控制间隔

	static LONG actionType = ACTION_UNRECOGNIZED; // 间隔期间保存上一次的动作类型

	if (++count != INTERVAL) {
		return actionType;
	}
	else {
		count = -1;
	}

	float theta_LR_left_curr = calculate_wrist_curvature(skel, TRUE);
	float theta_LR_rigt_curr = calculate_wrist_curvature(skel, FALSE);

	float theta_UD_left_curr = wrist_angle_with_Zaxis(skel, TRUE);
	float theta_UD_rigt_curr = wrist_angle_with_Zaxis(skel, FALSE);

	actionType = ACTION_UNRECOGNIZED;
	if (is_swing_wrist(isSwingLR_Left, theta_LR_left_pre, theta_LR_left_curr)) {
		actionType |= SWING_WRIST_LEFT_AND_RIGHT_LEFT;
	}
	else if (is_swing_wrist(isSwingUD_Left, theta_UD_left_pre, theta_UD_left_curr)) {
		actionType |= SWING_WRIST_UP_AND_DOWN_LEFT;
	}
	
	if (is_swing_wrist(isSwingLR_Rigt, theta_LR_rigt_pre, theta_LR_rigt_curr)) {
		actionType |= SWING_WRIST_LEFT_AND_RIGHT_RIGHT;
	}
	else if (is_swing_wrist(isSwingUD_Rigt, theta_UD_rigt_pre, theta_UD_rigt_curr)) {
		actionType |= SWING_WRIST_UP_AND_DOWN_RIGHT;
	}

	return actionType;
}
```

这样主函数只需要添加对新类型的输出就行了：

```c++
LONG actionType = get_swing_wrist_type(skeletonFrame.SkeletonData[i]);

// 在图像上绘制动作类型
if (actionType & SWING_WRIST_LEFT_AND_RIGHT_LEFT) {
	cv::String str_action = "swing left hand left and right";
	cv::putText(skeleton, str_action, cv::Point(10, 15),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
}
if (actionType & SWING_WRIST_LEFT_AND_RIGHT_RIGHT) {
	cv::String str_action = "swing right hand left and right";
	cv::putText(skeleton, str_action, cv::Point(10, 30),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
}
if (actionType & SWING_WRIST_UP_AND_DOWN_LEFT) {
	cv::String str_action = "swing left hand up and down";
	cv::putText(skeleton, str_action, cv::Point(10, 45),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
}
if (actionType & SWING_WRIST_UP_AND_DOWN_RIGHT) {
	cv::String str_action = "swing right hand up and down";
	cv::putText(skeleton, str_action, cv::Point(10, 60),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
}
if (actionType == ACTION_UNRECOGNIZED) {
	cv::String str_action = "unrecognized action";
	cv::putText(skeleton, str_action, cv::Point(10, 75),
		cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
}
```

<h2 id="summary">总结</h2>

你应该能够自行优化并封装相关代码。
