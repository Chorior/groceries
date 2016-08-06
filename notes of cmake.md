# Notes of CMake
---
>作者：彭侦  
>链接：<https://github.com/Chorior/groceries/blob/master/>

本文指在cmake的一些基本语法,用于快速上手

## 本文结构
  * [前言](#前言)
  * [cmake安装](#cmake安装)
  * [简单示例](#简单示例)
  * [进阶示例](#进阶示例)

<h2 id="前言">前言</h2>

* cmake编写的过程实际上是编程的过程,使用cmake语言和语法;
* 如果项目工程只有几个文件,建议直接编写makefile,或者使用gcc(CC)或g++;
* c、c++、java之外的语言,不要使用cmake;
* 有其它完备构建体系的不需要使用cmake;

<h2 id="cmake安装">cmake安装</h2>

* 源码安装
  * <http://www.cmake.org/HTML/Download.html>
  * 下载对应版本,自行安装;
* 命令行安装(ubuntu)
  * `sudo apt-get install cmake`;

<h2 id="简单示例">简单示例</h2>

* 工程结构
```
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 10:24 ./
drwxr-xr-x 9 pengzhen pengzhen 4096 Aug  6 10:13 ../
-rwxrwxr-x 1 pengzhen pengzhen   94 Aug  6 10:22 clear.sh*
-rw-rw-r-- 1 pengzhen pengzhen  191 Aug  6 10:24 CMakeLists.txt
-rw-rw-r-- 1 pengzhen pengzhen   85 Aug  6 10:14 main.cpp
```
* 工程内容
  * clear.sh
  ```bash
  #!/bin/bash
  rm -f CMakeCache.txt
  rm -rf CMakeFiles/
  rm -f cmake_install.cmake
  rm -f Makefile
  ```
  * CMakeLists.txt
  ```
  project(HELLO)
  set(src_list main.cpp)
  message(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
  message(STATUS "This is SOURCE dir " ${HELLO_SOURCE_DIR})
  add_executable(hello ${src_list})
  ```
  * main.cpp
  ```C++
  #include <iostream>
  int main()
  {
  	std::cout << "Hello CMake world!\n";
  	return 0;
  }
  ```
* 构建
```
~/Documents/cmake 11:00$ cmake .
-- The C compiler identification is GNU 5.4.0
-- The CXX compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- This is BINARY dir /home/pengzhen/Documents/cmake
-- This is SOURCE dir /home/pengzhen/Documents/cmake
-- Configuring done
-- Generating done
-- Build files have been written to: /home/pengzhen/Documents/cmake
```
* 构建结果
```
drwxrwxr-x 3 pengzhen pengzhen  4096 Aug  6 11:02 ./
drwxr-xr-x 9 pengzhen pengzhen  4096 Aug  6 10:13 ../
-rwxrwxr-x 1 pengzhen pengzhen    94 Aug  6 10:22 clear.sh*
-rw-rw-r-- 1 pengzhen pengzhen 11530 Aug  6 11:02 CMakeCache.txt
drwxrwxr-x 5 pengzhen pengzhen  4096 Aug  6 11:02 CMakeFiles/
-rw-rw-r-- 1 pengzhen pengzhen  1371 Aug  6 11:02 cmake_install.cmake
-rw-rw-r-- 1 pengzhen pengzhen   191 Aug  6 10:24 CMakeLists.txt
-rw-rw-r-- 1 pengzhen pengzhen    85 Aug  6 10:14 main.cpp
-rw-rw-r-- 1 pengzhen pengzhen  4722 Aug  6 11:02 Makefile
```
* 生成可执行文件
```
~/Documents/cmake 11:03$ make
Scanning dependencies of target hello
[ 50%] Building CXX object CMakeFiles/hello.dir/main.cpp.o
[100%] Linking CXX executable hello
[100%] Built target hello
```
* 执行程序
```txt
~/Documents/cmake 11:05$ ./hello
Hello CMake world!
```
* 清理工程
  * `make clean`;
  * cmake没有办法清理中间文件;
* 语法解析
  * CＭakeLists.txt是camke的构建定义文件,文件名是大小写相关的;
  * 如果工程存在多个目录,需要确保每个要管理的目录都存在一个CMakeLists.txt;
  * PROJECT指令
    * `PROJECT(projectname [CXX] [C] [Java])`;
    * 定义工程名称，并可指定工程支持的语言,默认支持所有语言;
    * 隐式定义了两个cmake变量
      * `<projectname>_BINARY_DIR`: 执行程序所在路径
      * `<projectname>_SOURCE_DIR`: 源路径
    * cmake预定义变量对应上面两个变量
      * PROJECT_BINARY_DIR      
      * PROJECT_SOURCE_DIR
    * 建议直接使用预定义变量
  * SET指令
    * `SET(VAR [VALUE] [CACHE TYPE DOSTRING [FORCE]])`;
    * 显示的定义变量；
  * MESSAGE指令
    * `MESSAGE([SEND_ERROR | STATUS | FINAL_ERROR] "message to dispaly"...)`;
    * 向终端输出用户定义的信息
    * 信息类型
      * SEND_ERROR: 产生错误;
      * STATUS: 输出前缀为--的信息;
      * FINAL_ERROR: 立即终止所有cmake过程
  * ADD_EXECUTABLE指令
    * `ADD_EXECUTABLE(programname src_list)`;
    * 生成可执行文件;
  * 变量使用`${}`方式取值,但IF控制语句直接使用变量名;
  * 指令(参数1 参数2 ...): 参数使用圆括号括起,参数间使用空格或分号分开;
  * 指令是大小写无关的,参数和变量是大小写相关的;推荐全部使用大写;
  * 文件名可以用双引号括起,如`SET(SRC_LIST "main.cpp")`,用以文件名中含有空格的情况;
* 内部编译和外部编译
  * 内部编译: 所有编译动作都发生在工程目录,如示例一般;
  * 外部编译: 所有编译动作全部发生在编译目录
    * 如在上面示例目录下新建目录build,进入build目录并执行cmake ..;make,那么会在build目录下生成所有编译文件和可执行文件hello;
  * cmake推荐使用外部编译;

<h2 id="进阶示例">进阶示例</h2>

* 工程结构
```
.:
总用量 20
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:00 ./
drwxr-xr-x 9 pengzhen pengzhen 4096 Aug  6 10:13 ../
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 20:03 build/
-rw-rw-r-- 1 pengzhen pengzhen   41 Aug  6 17:42 CMakeLists.txt
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 17:40 src/
./build:
总用量 12
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 20:03 ./
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:00 ../
-rwxrwxr-x 1 pengzhen pengzhen   94 Aug  6 10:22 clear.sh*
./src:
总用量 16
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 17:40 ./
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:00 ../
-rw-rw-r-- 1 pengzhen pengzhen   46 Aug  6 17:40 CMakeLists.txt
-rw-rw-r-- 1 pengzhen pengzhen   85 Aug  6 10:14 main.cpp
```
* 工程内容
  * CMakeList.txt
  ```
  PROJECT(HELLO)
  ADD_SUBDIRECTORY(src bin)
  ```
  * src/CMakeLists.txt
  ```
  PROJECT(HELLO)
  ADD_EXECUTABLE(hello main.cpp)
  ```
* 构建
  * `cd build/`;
  * `cmake ..`;
  * `make`;
* 构建结果
```
.:
总用量 20
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:00 ./
drwxr-xr-x 9 pengzhen pengzhen 4096 Aug  6 10:13 ../
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:07 build/
-rw-rw-r-- 1 pengzhen pengzhen   41 Aug  6 17:42 CMakeLists.txt
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 17:40 src/
./build:
总用量 44
drwxrwxr-x 4 pengzhen pengzhen  4096 Aug  6 20:07 ./
drwxrwxr-x 4 pengzhen pengzhen  4096 Aug  6 20:00 ../
drwxrwxr-x 3 pengzhen pengzhen  4096 Aug  6 20:07 bin/
-rwxrwxr-x 1 pengzhen pengzhen    94 Aug  6 10:22 clear.sh*
-rw-rw-r-- 1 pengzhen pengzhen 11886 Aug  6 20:07 CMakeCache.txt
drwxrwxr-x 4 pengzhen pengzhen  4096 Aug  6 20:07 CMakeFiles/
-rw-rw-r-- 1 pengzhen pengzhen  1548 Aug  6 20:07 cmake_install.cmake
-rw-rw-r-- 1 pengzhen pengzhen  4145 Aug  6 20:07 Makefile
./build/bin:
总用量 36
drwxrwxr-x 3 pengzhen pengzhen 4096 Aug  6 20:15 ./
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:07 ../
drwxrwxr-x 3 pengzhen pengzhen 4096 Aug  6 20:07 CMakeFiles/
-rw-rw-r-- 1 pengzhen pengzhen  989 Aug  6 20:07 cmake_install.cmake
-rwxrwxr-x 1 pengzhen pengzhen 9048 Aug  6 20:15 hello*
-rw-rw-r-- 1 pengzhen pengzhen 5186 Aug  6 20:07 Makefile
./src:
总用量 16
drwxrwxr-x 2 pengzhen pengzhen 4096 Aug  6 17:40 ./
drwxrwxr-x 4 pengzhen pengzhen 4096 Aug  6 20:00 ../
-rw-rw-r-- 1 pengzhen pengzhen   46 Aug  6 17:40 CMakeLists.txt
-rw-rw-r-- 1 pengzhen pengzhen   85 Aug  6 10:14 main.cpp
```
* 语法解析
  * ADD_SUBDIRECTORY指令
    * `ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])`;
    * 向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制存放的位置;
    * EXCLUDE_FROM_ALL参数的含义是将这个目录从编译过程中排除;
    * 上面的例子定义了将src子目录加入工程,并指定编译输出(包含编译中间结果)路径为bin目录;
    * 如果不进行bin目录的指定,那么编译结果(包括中间结果)都将存放在build/src目录;
  * 可以通过SET指令重新定义EXECUTABLE_OUTPUT_PATH和LIBRARY_OUTPUT_PATH变量来指定最终的目标二进制的位置;
  ```
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
  SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
  ```
  * 在哪里ADD_EXECUTABLE或ADD_LIBRARY,如果需要改变目标存放路径，就在哪里加入上述的定义,本例为src;
  * 安装(不是很懂)
    * CMAKE_INSTALL_PREFIX变量
      * 类似于configure脚本的–prefix: `cmake -DCMAKE_INSTALL_PREFIX=/usr`;
      * 默认定义是/usr/local;
    * INSTALL指令      
      * 用于定义安装规则,安装的内容可以包括目标二进制、动态库、静态库以及文件、目录、脚本等;
      * 目标文件的安装
       ```
       INSTALL(TARGETS targets...
         [[ARCHIVE|LIBRARY|RUNTIME]
           [DESTINATION <dir>]
           [PERMISSIONS permissions...]
           [CONFIGURATIONS
         [Debug|Release|...]]
           [COMPONENT <component>]
           [OPTIONAL]
           ] [...])
       ```
        * ARCHIVE特指静态库,LIBRARY特指动态库,RUNTIME特指可执行目标二进制;
        * DESTINATION定义了安装的路径,如果路径以/开头,那么指的是绝对路径,这时候CMAKE_INSTALL_PREFIX其实就无效了;
          * 如果你希望使用CMAKE_INSTALL_PREFIX来定义安装路径,就要写成相对路径,即不要以/开头,那么安装后的路径就是`${CMAKE_INSTALL_PREFIX}/<DESTINATION>`定义的路径;
          ```
          INSTALL(TARGETS myrun mylib mystaticlib
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION libstatic
          )
          ```
      * 普通文件的安装
       ```
       INSTALL(FILES files... DESTINATION <dir>
         [PERMISSIONS permissions...]
         [CONFIGURATIONS [Debug|Release|...]]
         [COMPONENT <component>]
         [RENAME <name>] [OPTIONAL])
       ```
        * 文件名是此指令所在路径下的相对路径;
        * 如果不定义权限PERMISSIONS,安装后的权限为644;
      * 非目标文件的可执行程序安装(比如脚本之类)
       ```
       INSTALL(PROGRAMS files... DESTINATION <dir>
         [PERMISSIONS permissions...]
         [CONFIGURATIONS [Debug|Release|...]]
         [COMPONENT <component>]
         [RENAME <name>] [OPTIONAL])
       ```
        * 文件名是此指令所在路径下的相对路径;
        * 如果不定义权限PERMISSIONS,安装后的权限为755;
      * 目录的安装
       ```
       INSTALL(DIRECTORY dirs... DESTINATION <dir>
         [FILE_PERMISSIONS permissions...]
         [DIRECTORY_PERMISSIONS permissions...]
         [USE_SOURCE_PERMISSIONS]
         [CONFIGURATIONS [Debug|Release|...]]
         [COMPONENT <component>]
         [[PATTERN <pattern> | REGEX <regex>]
         [EXCLUDE] [PERMISSIONS permissions...]] [...])
       ```
        * DIRECTORY后面连接的是所在source目录的相对路径,如果目录名(假设为abc)不以/结尾，那么这个目录将被安装为目标路径下的abc,如果目录名以/结尾,代表将这个目录中的内容安装到目标路径，但不包括这个目录本身;
        * PATTERN用于使用正则表达式进行过滤;
        * PERMISSIONS用于指定PATTERN过滤后的文件权限;
        ```
        INSTALL(DIRECTORY icons scripts/ DESTINATION share/myproj
          PATTERN "CVS" EXCLUDE
          PATTERN "scripts/*"
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ)
        ```
        * 上述指令的执行结果
          * 将icons目录安装到`<prefix>/share/myproj`,将scripts/中的内容安装到`<prefix>/share/myproj`;
          * 不包含目录名为CVS的目录;
          * 对于scripts/*文件指定权限为OWNER_EXECUTE、OWNER_WRITE、OWNER_READ、GROUP_EXECUTE、GROUP_READ(750);
      * 安装时CMAKE脚本的执行
        * `INSTALL([[SCRIPT <file>] [CODE <code>]] [...])`;
        * SCRIPT参数用于在安装时调用cmake脚本文件(也就是`<abc>.cmake`文件);
        * CODE参数用于执行CMAKE指令，必须以双引号括起来;
        ```
        INSTALL(CODE "MESSAGE(\"Sample install message.\")")
        ```
      
