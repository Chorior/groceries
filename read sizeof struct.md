# Add C++ plugins to qml
---
:art:
---
* 我们都知道struct可以容纳多个不同类型的成员变量,但是整个struct的sizeof到底是多大呢？
* 首先我们来认识一下sizeof
  * sizeof在编译时获取变量或类型的长度;
  * 所以那些在运行时长度会变的变量,sizeof得到的是其默认的长度,比如说
    * sizeof(std::string) == 4;
    * sizeof(std::vector<std::string>) == 12;
* 那么struct的长度是不是所有成员变量的总和呢,答案是不一定
  * struct会遵循一种地址对齐的规定
    * 结构体变量中成员相对起始地址的偏移量必须是成员大小的整数倍(0被认为是任何数的整数倍);
    * 结构体大小必须是所有成员大小的整数倍,也即所有成员大小的公倍数;
    * 如果是结构体嵌套的话(数组相当于多个成员)
      * 展开后的结构体的第一个成员的偏移量应当是被展开的结构体中最大的成员的整数倍;
      * 结构体大小必须是所有成员大小的整数倍,这里所有成员计算的是展开后的成员,而不是将嵌套的结构体当做一个整体;

* 一些常用的类型占用内存的大小
  * 32bit
    * char: 1;
    * short: 2;
    * int: 4;
    * long: 4;
    * float: 4;
    * double: 8;
    * pointer: 4;
  * 64bit
    * char: 1;
    * short: 2;
    * int: 4 or 8;
    * long: 8;
    * float: 4;
    * double: 8;
    * pointer: 8;

* 练习(64bit)

  ```C++
  struct stu1  
  {  
       int i;  
       char c;  
       int j;  
  };
  struct stu2  
  {  
        int k;  
        short t;  
  };
  struct stu3  
  {   
         char c1;   
         int i;  
         char c2;  
  };
  struct stu4  
  {  
         char c1;  
         char c2;  
         int i;  
  };
  struct stu5  
  {  
        short i;  
        struct   
        {  
             char c;  
             int j;  
        } ss;   
        int k;  
  };
  struct stu6  
  {  
        char i;  
        struct   
        {  
             char c;  
             int j;  
        } ss;   
        char a;  
        char b;  
        char d;  
        char e;  
        char f;  
  };
  struct stu7  
  {  
      float f;  
      char p;  
      int adf[3];  
  };
  struct stu8
  {
    int a;
    char b;
    short c;
    unsigned short d;
    double e;
    unsigned char f;
  };
  struct stu9
  {
    double e;
    int a;
    short c;
    unsigned short d;
    char b;
    unsigned char f;
  };
  ```

* 答案

  ```
  sizeof(stu1) = 12
  sizeof(stu2) = 8
  sizeof(stu3) = 12
  sizeof(stu4) = 8
  sizeof(stu5) = 16
  sizeof(stu6) = 20
  sizeof(stu7) = 20
  sizeof(stu8) = 32
  sizeof(stu9) = 24
  ```

* 32bit的计算方式貌似不同,比如说在32位机中,sizeof(stu8) == 24;
  * 猜想
    * 32bit最大宽度为4;
    * 64bit最大宽度为8;

* 建议
  * 将结构成员宽度自大往小排列能有效避免内存浪费,示例如stu8和stu9所示;
