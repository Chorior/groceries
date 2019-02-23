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
$ free -h                               # 查看系统内存使用情况
$ cat /proc/meminfo                     # 查看系统内存使用情况，memfree+buffers+cached是当前能用的最大内存
$ file [file]                           # 查看文件类型
$ stat file                             # 查看文件相关信息，大小、权限、修改时间等
$ mkdir -p [dir]                        # 创建文件夹，当父目录不存在时创建之，目标目录存在时不报错
$ cp -a [srcdir] [dstdir]               # 拷贝目录，并保有原目录下所有文件的所有相关信息
$ cp -p [srcfile] [dstfile]             # 拷贝文件，并保有源文件的所有相关信息
$ cp -s [srcfile] [dstfile]             # 用软连接代替拷贝，当需要对某目录下的所有文件设置软连接时比较方便
$ ln -s [srcfile] [dstfile]             # 设置软连接
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
$ crontab -l                            # 查看系统周期任务列表
$ crontab -e                            # 修改系统周期任务
$ tar -cvzf [123.tar.gz] [path]         # 打包文件
$ tar -xvzf [123.tar.gz] [path]         # 解压文件到 path
$ pidof [program_name]                  # 查询当前正在运行的 program_name 的 pid，对 java 程序无效
$ ps -ef | grep [program_name]          # 查询当前 program_name 的进程快照,如 PID,启动时间,运行时间等
$ time [cmd]                            # 查看 cmd 执行的时间
$ shutdown -r now                       # 立即重启
$ last reboot                           # 查看系统启动日志，何时启动、何时关闭
$ alias                                 # 查看别名列表，使用 \cmd 表示不使用别名
$ iostat                                # 查看系统 IO 情况
```

### 网络命令

ifconfig,ip,route,netstat,lsof,service network restart,firewall,ping,arping,traceroute,lsof,tcpdump,ethtool

### 数据操作

grep,sed,cut,awk,xargs,sort,uniq,wc,source,BASH_SOURCE,tr,dd,bc,test,tail,head,tailf,zgrep -a,子字符串

### VIM

:set paste,:set nu,:set hlsearch,messages,sysmonitor,syslog

<h2 id="shell_script">Shell Script</h2>

function,local,let,>,read,并发,date, +%s,flock,数组，数值、字符串比较，文件、目录存在，$1,&&,||,dirname,basename,pwd,/dev/null