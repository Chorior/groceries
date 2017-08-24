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

QT基础
{
	随机数

	QString: 隐式共享 数值转换 搜索 国际化(tr()) QT Linguist

	QByteArray 
	
	QVariant: canConvert

	QTime QDate QDateTime

	QPointer

	IO

	顺序容器: QList QLinkedList QVector QStack QQueue

	关联容器: QSet QMap QMultiMap QHash QMultiHash

	泛型算法 QtAlgorithms

	正则表达式

	XML
}

QT特性
{
	信号槽: QSignalMapper

	Q_PROPERTY: setProperty() property() 动态属性

	对象树: children() 

	元对象系统: metaObject() className() inherits() newInstance() qobject_cast--dynamic_cast
}

布局管理
{
QLayout: QBoxLayout QGirdLayout QFromLayout QStackedLayout

大小策略: QSizePolicy 伸缩因子

布局外观: QStyle setStyle QStyleFactory::keys() 

调色板: QPalette Palette,setPalette

样式表: 

QWhatsThis: createAction() setWhatsThis() Qt Assistant

QSplitter

setTabOrder setWindowState 

*	[部件容器](#widget_containers)
	*	[QGroupBox](#qgroupbox)
	*	[QScrollArea](#qscrollarea)
	*	[QToolBox](#qtoolbox)
	*	[QTabWidget](#qtabwidget)
	*	[QStackedWidget](#qstackedwidget)
	*	[QFrame](#qframe)
	*	[QMdiArea](#qmdiarea)
	*	[QDockWidget](#qdockwidget)

QListView、QTreeView、QTableView、QColumnView

QListWidget、QTreeWidget、QTableWidget
}

窗口部件
{
几何布局 图

模态窗口与非模态窗口 modal modeless

自动信号连接 手动信号连接 QMetaObject::connectSlotsByName(this); connect

Dialog: QColorDialog QFileDialog QFontDialog QInputDialog QMessageBox QProcessDialog QErrorMessage QWizard QPageSetupDialog QPrintDialog QPrintPreviewDialog

QFrame: QLabel(setBuddy()) QLCDNumber QStackedWidget QToolBox 

QAbstractButton: QPushButton QCheckBox QRadioButton QGroupBox QButtonGroup

QAbstractSpinBox: QDateTimeEdit QSpinBox QDoubleSpinBox 

QAbstractSlider: QScrollBar QSlider QDial
}

QMainWindow
{
	QMenuBar,QToolBar: QWidgetAction
	
	QMdiArea: QMdiSubWindow

	QDockWidget

	QStatusBar: QSizeGrip
}

事件管理
{
	何为事件

	处理事件的5种方法

	QMouseEvent: QCursor setMouseTracking

	QWheelEvent

	QKeyEvent: modifiers()  isAutoRepeat() 

	QTimerEvent: QTimer startTimer timerId stop singleShot

	拖放 drag and drop

	打印 QPrinter

	事件过滤器: installEventFilter installEventFilter

	发送事件: sendEvent(立即处理) postEvent(等待调度)
}

文本
{
	纯文本与富文本

	QLineEdit: setFocus() setValidator() QRegExp QCompleter 

	QTextEdit: QTextDocument QTextFrame QtextFrameFormat QTextBlock 

	QTextBrower:
}
