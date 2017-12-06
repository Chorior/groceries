# Kinect for Windows 动作识别

本文为博主使用 Kinect 和 C++ 学习动作识别时的笔记。

## 本文结构

*	[Kinect for Windows 安装](#install)
*	[Hello Kinect](#hello_kinect)
*	[显示彩色图](#show_color)
*	[显示深度图](#show_depth)
*	[显示骨架图](#show_skeleton)
*	[融合骨架图与色彩图](#combine_skeleton_with_color_image)
*	[简单动作识别](#simple_action_recognition)

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

获取彩色图相对于获取倾斜角来说，只不过需要打开并获取图像流而已，比较难的是 [HANDLE 的使用](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686915(v=vs.85).aspx)以及如何将 Kinect 图像数据转换到 opencv 图像。

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

				// 每个NUI_SKELETON_DATA包含一个骨架数据
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
				
				// 每个NUI_SKELETON_DATA包含一个骨架数据
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

<h2 id="simple_action_recognition">简单动作识别</h2>

有了上面的骨架图，就可以开始简单的动作识别了。

博主的第一个任务是识别手左右摆动这个超级简单的动作。首先，手左右摆动最明显的特征是什么？不管你怎么摆，你的手腕总是要左右移动的吧。

博主试着在 Kinect 前面左右摆动右手，然后使用如下代码来保存得到的骨架图：

```c++
static int count;
std::string skel_name = "skeleton_" + std::to_string(++count) + ".png";

cv::cvtColor(skeleton, skeleton, cv::COLOR_BGRA2BGR);
cv::imwrite(skel_name, skeleton);
```

这些骨架图以六个为一组，连续的图手腕移动距离可能不大，那么取最边上的两张进行比较，应该会有比较大的变化。

那么如何来衡量左右摆动的程度呢？为了使问题更加容易解决，博主假定右手摆动的姿势是手腕绕着手肘摆动，这样就能以手腕节点和手肘节点的线段、到以手肘节点和右肩节点的线段的角度来衡量左右摆动的程度了。
