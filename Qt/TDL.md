#   基础篇

*   [Qt 概述](#overview)
*	[Hello World]()
*	[字符串]()
	*	[QByteArray、QBitArray]()
	*	[QChar、QLatin1Char、QCharRef]()
	*	[QLatin1String、QString、QStringRef]() QLocal QStringBuilder QStringAlgorithms QStringMatcher
	*	[字符编码]()
*	[命令行处理]()
	*	[QCommandLineOption]()
	*	[QCommandLineParser]()
*	[时间、日期]()
	*	[QDate](#qdate)
	*	[QTime](#qtime)
	*	[QDateTime](#qdatetime)
	*	[QTimer]()
*	[通用IO](http://doc.qt.io/qt-5/topics-data-storage.html)
	*	[QIODevice]()
	*	[QBuffer]()
	*	[QDataStream]()
	*	[QTextStream]()
	*	[QDebug]()
	*	[QUrl]() QUrlQuery
*	[文件IO]()
	*	[QFileDevice]()
	*	[QFile]()
	*	[QDir]()	
	*	[QFileInfo]()
	*	[QFileSelector]()
	*	[QLockFile]()
	*	[QResource]()
	*	[QSaveFile]()
	*	[QSettings]()
*	[顺序容器]()
	*	[QList]() QStringList QByteArrayList
	*	[QLinkedList]()
	*	[QVector]()
	*	[QStack]()
	*	[QQueue]()
*	[关联容器]()
	*	[QSet]()
	*	[QMap]() QPair
	*	[QMultiMap]()
	*	[QHash]()
	*	[QMultiHash]()
*	[泛型算法]()
*	[智能指针]() QScopedPointer QSharedPointer
*	[SQL、XML、JSON](http://doc.qt.io/qt-5/topics-data-storage.html)
*	[GUI基础]()
	*	[QPoint]() 
	*	[QSize]()
	*	[QLine]()
	*	[QRect]()
	*	[QPolygon]()

# GUI 篇

# 线程篇

*	[进程间通信](http://doc.qt.io/qt-5/ipc.html)
*	[多线程](http://doc.qt.io/qt-5/topics-core.html#threading)

窗口部件
{
几何布局 图

模态窗口与非模态窗口 modal modeless

自动信号连接 手动信号连接 QMetaObject::connectSlotsByName(this); connect

Dialog: QColorDialog QFileDialog QFontDialog QInputDialog QMessageBox QProcessDialog QErrorMessage QWizard QPageSetupDialog QPrintDialog QPrintPreviewDialog

QFrame: QLabel(setBuddy()) QLCDNumber QStackedWidget QToolBox 

QAbstractButton: QPushButton QCheckBox QRadioButton QGroupBox QButtonGroup

QLineEdit: setFocus() setValidator() QRegExp QCompleter 

QAbstractSpinBox: QDateTimeEdit QSpinBox QDoubleSpinBox 

QAbstractSlider: QScrollBar QSlider QDial
}

布局管理
{
QLayout: QBoxLayout QGirdLayout QFromLayout QStackedLayout

大小策略: QSizePolicy 伸缩因子

QSplitter

setTabOrder
}

QMainWindow

