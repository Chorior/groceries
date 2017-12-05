# Kinect for Windows 动作识别

本文为博主使用 Kinect 和 C++ 学习动作识别时的笔记。

## 本文结构

*	[Kinect for Windows 安装](#install)
*	[Hello Kinect](#hello_kinect)
*	[显示彩色图](#show_color)
*	[显示深度图](#show_depth)
*	[显示骨架图](#show_skeleton)

<h2 id="install">Kinect for Windows 安装</h2>

Google 搜索 Kinect for Windows SDK 下载与 Kinect 版本相对应的 SDK 和 Toolkit，我的是 Kinect xbox v1 1414，所以使用的是1.8版本。

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
	print_frame_info(imageFrame);
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

