# shell 常用技能

本文是博主从业 Linux 一年的经验笔记，适用于有一定 Linux 基础的小白。

## 本文结构

*	[简介](#overview)
*	[Shell](#shell)
*	[Shell Script](#shell_script)

<h2 id="overview">简介</h2>

Linux 是大多数互联网行业中被广泛应用的一种操作系统，有关它的历史你可以随意百度、谷歌或查阅相关书籍了解，这里推荐两本书--《鸟哥的 Linux 私房菜》和《Linux 命令行与 shell 脚本编程大全》，它们能解决 80% Linux 相关的问题。

众所周知，Linux 分为 shell 和 shell script 两大部分，其中 shell 用于终端命令行，而 shell script 则用于编写脚本，本文也将分为这两部分。

<h2 id="shell">Shell</h2>

### 常用命令

```bash
$ whoami                                # 我是谁
$ who                                   # 环境上有哪些人
$ w                                     # 环境上那些人在干啥
$ PS1="\u@\h \W $ "                     # 设置个好看的终端命令提示符
$ date                                  # 查看当前系统时间
$ hwclock                               # 查看当前硬件时间
$ write [user] [tty]                    # 向位于 tty 的 user 发送消息，Ctrl+D 结束
$ uname -a                              # 查看系统版本信息 
$ uname -m                              # 查看系统硬件环境，x86,arm等
$ uname -r                              # 查看内核版本号
$ cat /etc/*release*                    # 查看系统版本类型，Ubuntu,Redhat,Suse等
$ lspci                                 # 列出所有 pci 设备
$ lsblk                                 # 列出所有磁盘信息
$ df -h                                 # 查看系统磁盘空间占用率
$ df [file]                             # 查看文件所属磁盘的空间占用率
$ du -h -d 1 [dir]                      # 查看目录第一层的空间占用大小
$ fdisk -l /dev/[disk]                  # 查看 disk 磁盘扇区大小等，当不指定 disk 时，打印所有磁盘信息
$ blockdev --getss /dev/[disk]          # 读取 disk 的罗技扇区大小，还有其他的命令读取不同的属性
$ free -h                               # 查看系统内存使用情况
$ cat /proc/meminfo                     # 查看系统内存使用情况，memfree+buffers+cached是当前能用的最大内存
$ file [file]                           # 查看文件类型
$ stat file                             # 查看文件相关信息，大小、权限、修改时间等
$ mkdir -p [dir]                        # 创建文件夹，当父目录不存在时创建之，目标目录存在时不报错
$ cp -a [srcdir] [dstdir]               # 拷贝目录，并保有原目录下所有文件的所有相关信息
$ cp -p [srcfile] [dstfile]             # 拷贝文件，并保有源文件的所有相关信息
$ cp -s [srcfile] [dstfile]             # 用软连接代替拷贝，当需要对某目录下的所有文件设置软连接时比较方便
$ ln -s [srcfile] [dstfile]             # 设置软连接
$ readlink -f [file]                    # 获取 file 软连接的位置
$ scp [[user@]host1:][file1] ./         # 服务器间传递文件
$ cksum [file]                          # 检查文件的 CRC,用于检查文件传输是否出错
$ sha256sum [file]                      # 检查文件的 SHA256 信息，用于检查文件传输是否出错
$ cmp [file1] [file2]                   # 按字节对比两个文件
$ diff -y [file1] [file2]               # 按行对比两个文件，并并排输出
$ chmod 750 [file]                      # 设置文件权限
$ chown [user]:[group] [file]           # 设置文件归属用户、用户组
$ chown --reference=[srcfile] [dstfile] # 设置 dstfile 的归属与 srcfile 一样
$ chattr +i [file]                      # 修改文件属性，使得文件不可修改
$ lsattr [file]                         # 查看文件属性
$ crontab -l                            # 查看系统周期任务列表,readhat 文件位于 /var/spool/cron/user
$ crontab -e                            # 修改系统周期任务，suse 文件位于 /var/spool/cron/tabs/user
$ tar -cvzf [123.tar.gz] [path]         # 打包文件
$ tar -xvzf [123.tar.gz] [path]         # 解压文件到 path
$ pidof [program_name]                  # 查询当前正在运行的 program_name 的 pid，对 java 程序无效
$ ps -ef | grep [program_name]          # 查询当前 program_name 的进程快照,如 PID,启动时间,运行时间等
$ ps -eo cmd,lstart,etime | grep [cmd]  # 查询 cmd 执行的确切开始时间、到现在的执行时间长度
$ time [cmd]                            # 查看 cmd 执行的时间
$ shutdown -r now                       # 立即重启
$ last reboot                           # 查看系统启动日志，何时启动、何时关闭
$ alias                                 # 查看别名列表，使用 \cmd 表示不使用别名
$ head/tail -n [num] [file]             # 显示 file 前/后 num 行的数据
$ tailf [file] | grep [pattern]         # 实时显示 file 最后 10 行的数据，并过滤出符合条件的行
$ ldd [target_file]                     # 查看一个目标文件所需要的动态库列表，查找顺序：rpath,LD_LIB_PATH,ldconfig
$ nm [target_file]                      # 查看一个目标文件的符号表，查询一些变量、函数是否存在
$ readelf [target_file]                 # 显示一个目标文件的完整结构，查询该目标文件需要的内存大小等
$ pmap -X [pid]                         # 查看进程 pid 的内存占用情况
```

### 网络命令

```bash
$ ifconfig                                      # 查看网络配置，网卡名、IP、网络掩码等
$ ethtool [eth0]                                # 查看网卡参数，网速等
$ ip rule                                       # 路由规则表
$ ip addr show                                  # 显示所有网络地址，现在倾向于使用其代替 ifconfig
$ ip route show                                 # 显示主路由表信息
$ ip neigh show                                 # 显示邻居表
$ route -n                                      # 显示路由表
$ ping [host]                                   # 发送 TCMP 请求到 host
$ arping [host]                                 # 发送 ARP 请求到 host
$ telnet [host] [port]                          # 建立 telnet 连接，一般用于测试目标主机端口是否畅通
$ ssh -p [port] [user@host]                     # 建立 ssh 连接，可以用于测试目标主机端口是否畅通
$ traceroute -s [srctp] [dstip] -p [port] -n -F # 跟踪网络包的路由线路
$ tracert [host]                                # windows 上的 traceroute 命令
$ tcpdump -nli [eth0]                           # 抓 eth0 的网络数据包
$ netstat -anp | grep [port]                    # 查看占用端口的应用程序
$ lsof -i:[port]                                # 查看占用端口的应用程序
$ ifdown/ifup [eth0]                            # 关闭/打开网卡
$ service network restart                       # 重启网络
```

#### 防火墙

Redhat/Centos：

```bash
$ service iptables off        # 7.0前关闭防火墙
$ chkconfig iptables off      # 7.0前开机禁止启动防火墙
$ systemctl stop firewalld    # 7.0后关闭防火墙
$ systemctl disable firewalld # 7.0后开机禁止启动防火墙
```

SUSE：

```bash
$ /etc/init.d/SuSEfirewall2_setup stop
$ /etc/init.d/SuSEfirewall2_init stop  # 关闭防火墙
$ chkconfig SuSEfirewall2_setup off
$ chkconfig SuSEfirewall2_init off     # 开机禁止启动防火墙
$ SuSEfirewall2 status                 # 查看防火墙状态
$ SuSEfirewall2 stop                   # 停止防火墙
```

当防火墙开启时，iptables 规则有可能不起作用，将自定义的链添加到 Firewall 创建的 IN_public 链上可解决该问题，`-A IN_public -j $chain`。

当一个网卡上出现多个 ip 时，使用 `-A $chain -i ${interface}+`，表示只要与 interface 匹配的都生效，直接使用带冒号的网卡名可能不会生效。

### 数据操作

```bash
$ grep -i [string]                            # 忽略字符大小写，grep 与 ^$ 合用超好用
$ grep -w [word]                              # 全匹配，word 由数字、字符、下划线组成
$ grep -n [pattern]                           # 输出对应项在源文件中的行数
$ grep -v [pattern]                           # 显示不包含匹配文本的所有行
$ grep -r [pattern] [dir]                     # 查找整个目录
$ grep -q [pattern]                           # 不显示任何信息，一般与 $? 在脚本里合用
$ grep -E [pattern]                           # 使用扩展的正则表达式，会慢一点
$ grep -e [pattern1] -e [pattern2] ...        # 显示符合 -e 指定的所有样式的行
$ zgrep -a [pattern] [file]                   # 搜索打包文件，不用解压，方便
$ sed -i [script] [file]                      # 编辑后直接输出到文件
$ sed 's/[pattern]/[dststr]/g' [file]         # 将 file 中所有符合 pattern 的字符串替换为 dststr
$ sed '/[pattern]/d' [file]                   # 将 file 中所有符合 pattern 的字符串删除
$ cut -d [delimiter] -f [num] [file]          # 以 delimiter 为分隔符，按行分割 file，输出第 num 个字段
$ awk -F [delimiter] '{print $[num]}' [file]  # 以 delimiter 为分隔符，按行分割 file，输出第 num 个字段
$ awk -v [val]=[value] '{print val+1}' [file] # -v 指定变量及其值，awk 可用于简单的数学运算、数值比较
$ echo "scale=2;1/2" | bc                     # 使用 bc 进行数学运算、数值比较，scale 用于指定精度，比较正确时返回 1
$ find / -type f|xargs -d [delim] grep [str]  # 全局搜索字符串
$ tr -cd [pattern]                            # 删除不匹配的所有其他字符
$ tr [set1] [set2]                            # 将 set1 中的字符集转换为 set2 中的字符集,经常用于字符转换,如等号转空格
$ dd if=[file1] of=[file2] conv=ucase         # 将 file1 中的字符转换为大写并输出到 file2
$ dd if=/dev/zero of=[file] bs=1M count=1000  # 生成一个 1000M 的文件
$ sort -b                                     # 忽略每行前面开始出的空格字符，以行为单位进行排序
$ uniq -c                                     # 删除文本文件中重复出现的行，并显示该行重复出现的次数，常与 sort 合用
$ wc -l                                       # 显示行数
```

### VIM

```bash
:set nu       # 显示行数
:set hlsearch # 高亮显示匹配字段
:set paste    # 设置粘贴模式，每行不会添加一个 tab 
:wq!          # 强制保存并退出
:q!           # 不保存退出
dd            # 删除光标所在行
x             # 删除光标所在字符
a             # 在光标后插入
/word         # 搜索 word
/\cword       # 忽略大小写
/\<word\>     # 精确匹配 word
```

<h2 id="shell_script">Shell Script</h2>

Shell 脚本可以参照[菜鸟教程](http://www.runoob.com/linux/linux-shell.html)进行简单的学习，这里介绍一些实用的技巧和注意事项：

* shell 变量以数字、字符、下划线组成，且数字不能打头，也不能与其它关键字冲突；
* 在函数最开始时使用 local 定义所有变量，对算数变量赋值时使用 let 关键字；
* 传递参数的方式要记牢：`$#,$*,$@,shift,getopts`；
* 一些特殊的变量：`$$,$!,$?`；
* 常用的 test 命令要记牢：-z,-n,-e,-f,-d,-eq,-ne,-gt,-ge,-lt,-le;
* 字符串比较时，如果含有变量，在字符串前加上 x，如 `[ x"oracle" == x"$val" ]`；
* 单条件语句可以使用 `&&` 或 `||` 写作一行，如 `[ $? -ne 0 ] && return 0` 或 `[ $? -eq 0 ] || return 0`；
* 获取命令输出时。尽量不要使用反引号方式，`$()`会更好；
* 当使用 for 循环变量时、使用 cp 拷贝时，不能加引号；
* 当编写定时任务脚本时，使用 flock 文件锁防止脚本重复执行；
* 使用 `date +%s` 可以获取从 1970 年 1 月 1 日 00:00:00 到目前经历的秒数，用于计时；
* 重定向输入输出：`>>/dev/null 2>&1` 等；
* 从标准输入读取单行数据：`read -p [tips] -t [timeout] -n [length] value`；
* 获取数组长度：`${#array[@]}`或`${#array[*]}`；
* 获取字符串长度：`${#string}`；
* 字符串替换：`${[string]//[substring]/[replacement]}`；
* 知道 `dirname`,`basename`；
* 当需要导入其它脚本时，使用 source 而非 \.；
* 使用 `BASH_SOURCE` 获取脚本所在路径：`basepath=$(dirname $BASH_SOURCE)`；
* [spawn + expect 实现脚本自动登录](https://www.cnblogs.com/lzrabbit/p/4298794.html)；
* 提供一段简单的并发操作脚本，该脚本写入管道的数据可能变得乱序，可以修改为只写入一个字符：

```bash
#!/bin/bash

Njob=15 # 任务总数
Nproc=5 # 最大并发数

# 创建管道文件并删除，由于打开了该文件，删除后能继续使用直至退出
mkfifo ./fifo.$$ && exec 777<>./fifo.$$ && rm -f ./fifo.$$

for((i=0;i<$Nproc;i++));do
  echo "init time add $i" >&777
done

for((i=0;i<$Njob;i++));do
{
  read -u 777 # 从 fifo 中读取一行，没有数据时会堵塞
  echo "progress $i is sleeping for 3 seconds zzz..."
  sleep 3
  echo "realtime add $(($i+$Nproc))" >&777
}&
done

wait
echo -e "time consuming: $SECONDS seconds"
```