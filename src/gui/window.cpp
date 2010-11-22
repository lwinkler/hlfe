#include <QtGui>

#include <QWidget>
#include <QCheckBox>

#include "imageviewer.h"
#include "window.h"

Window::Window()
{
	/*aliasedLabel = createLabel(tr("Aliased"));
	antialiasedLabel = createLabel(tr("Antialiased"));
	intLabel = createLabel(tr("Int"));
	floatLabel = createLabel(tr("Float"));*/
	
	//QGridLayout *layout = new QGridLayout;
	//QHBoxLayout *layout = new QHBoxLayout;
	/*layout()->addWidget(aliasedLabel);
	layout()->addWidget(antialiasedLabel);
	layout()->addWidget(intLabel);
	layout()->addWidget(floatLabel);*/
	
	QTimer *timer = new QTimer(this);
	
	//QPushButton *quit = new QPushButton(tr("&Quit"));
	
	QWidget* mainWidget = new QWidget;
	QGridLayout* mainLayout = new QGridLayout;
	
	setWindowState(Qt::WindowMaximized);
	
	for (int i = 0; i < nbViewer; ++i) {
		scroll[i] = new QScrollArea;
		imageViewer[i] = new ImageViewer;
		scroll[i]->setWidget(imageViewer[i]);
		//imageViewer[i]->setGeometry(i * width()/2, 0, (i + 1) * width()/2, height()/2);
		//scroll[i]->setBaseSize(width()/2, width()/2);
		scroll[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		imageViewer[i]->resize(1000, 1000);
		
		//imageViewer[i]->setBaseSize(100, 100);
		//scroll[i]->setBaseSize(200, 400);
		//imageViewer[i]->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
		mainLayout->addWidget(scroll[i], 1, i);
		
		//imageViewer[i]->setAntialiased(j != 0);
		//imageViewer[i]->setFloatBased(i != 0);
		
		QCheckBox* cbDispImage=new QCheckBox(tr("Display image"));
		connect(cbDispImage, SIGNAL(toggled(bool)),imageViewer[i], SLOT(setDispImage(bool)));
		cbDispImage->setChecked(true);
		//mainLayout->addWidget(cbDispImage, 2, i);
		//connect(imageViewer[i], SIGNAL(dispImageSetted(bool)),cbDispImage, SLOT(setChecked(bool)));
		
		QCheckBox* cbDispKP=new QCheckBox(tr("Display key points"));
		connect(cbDispKP, SIGNAL(toggled(bool)),imageViewer[i], SLOT(setDispKP(bool)));
		cbDispKP->setChecked(true);

		QCheckBox* cbDispMatch=new QCheckBox(tr("Display matches"));
		connect(cbDispMatch, SIGNAL(toggled(bool)),imageViewer[i], SLOT(setDispMatch(bool)));
		cbDispMatch->setChecked(true);

		QCheckBox* cbDispInfo=new QCheckBox(tr("Display informations"));
		connect(cbDispInfo, SIGNAL(toggled(bool)),imageViewer[i], SLOT(setDispInfo(bool)));
		cbDispInfo->setChecked(false);
		
		QPushButton *btnLoad = new QPushButton(tr("&Load image"));
		connect(btnLoad, SIGNAL(clicked()),imageViewer[i], SLOT(open()));

		QComboBox *comCompare = new QComboBox; //(tr("&Compare image"));
		comCompare->insertItem( QString("Test 1") );
		connect(comCompare, SIGNAL(valueChanged(QString)),imageViewer[i], SLOT(setFileName2()));
		
		QPushButton *btnCompare = new QPushButton(tr("&Compare image"));
		connect(btnCompare, SIGNAL(clicked()),imageViewer[i], SLOT(compare()));
		
		QGroupBox *groupBox = new QGroupBox(tr("Display options"));
		groupBox->setFlat(true);
		QGridLayout *vbox = new QGridLayout;
		vbox->addWidget(cbDispImage, 0, 0);
		vbox->addWidget(cbDispKP, 1, 0);
		vbox->addWidget(cbDispMatch, 2, 0);
		vbox->addWidget(cbDispInfo, 3, 0);
		vbox->addWidget(btnLoad, 0, 1);
		vbox->addWidget(comCompare, 1, 1);
		vbox->addWidget(btnCompare, 2, 1);
		//vbox->addStretch(1);
		groupBox->setLayout(vbox);
		
		mainLayout->addWidget(groupBox, 2, i);
		//connect(timer, SIGNAL(timeout()),imageViewer[i], SLOT(update()));
		//QPushButton *quit = new QPushButton(tr("&Quit"));
		//mainLayout->addWidget(quit, 1, i);
	}
	mainWidget->setLayout(mainLayout);

	setCentralWidget(mainWidget);
	//layout()->addWidget(quit);
	//	layout->addWidget(quit);//, 0 , 1 );
	//timer->start(100);
	createActions();
	createMenus();

	setWindowTitle(tr("High level feature recognition"));
}

QLabel *Window::createLabel(const QString &text)
{
	QLabel *label = new QLabel(text);
	label->setAlignment(Qt::AlignCenter);
	label->setMargin(2);
	label->setFrameStyle(QFrame::Box | QFrame::Sunken);
	return label;
}

void Window::about()
{
	QMessageBox::about(this, tr("About Image Viewer"),
				    tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
				    "and QScrollArea to display an image. QLabel is typically used "
				    "for displaying a text, but it can also display an image. "
				    "QScrollArea provides a scrolling view around another widget. "
				    "If the child widget exceeds the size of the frame, QScrollArea "
				    "automatically provides scroll bars. </p><p>The example "
				    "demonstrates how QLabel's ability to scale its contents "
				    "(QLabel::scaledContents), and QScrollArea's ability to "
				    "automatically resize its contents "
				    "(QScrollArea::widgetResizable), can be used to implement "
				    "zooming and scaling features. </p><p>In addition the example "
				    "shows how to use QPainter to print an image.</p>"));
}

void Window::createActions()
{
	openAct1 = new QAction(tr("&Open1..."), this);
	openAct1->setShortcut(tr("Ctrl+O"));
	connect(openAct1, SIGNAL(triggered()), imageViewer[0], SLOT(open()));
	
	openAct2 = new QAction(tr("O&pen2..."), this);
	openAct2->setShortcut(tr("Ctrl+P"));
	connect(openAct2, SIGNAL(triggered()), imageViewer[1], SLOT(open()));
	
	cleanAct = new QAction(tr("Clean &temp files"), this);
	cleanAct->setShortcut(tr("Ctrl+T"));
	connect(cleanAct, SIGNAL(triggered()), this, SLOT(cleanTmp()));
	
	printAct = new QAction(tr("&Print..."), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setEnabled(false);
	connect(printAct, SIGNAL(triggered()), imageViewer[0], SLOT(print()));
	
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	
	zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), imageViewer[0], SLOT(zoomIn()));
	
	zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), imageViewer[0], SLOT(zoomOut()));
	
	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), imageViewer[0], SLOT(normalSize()));
	
	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	connect(fitToWindowAct, SIGNAL(triggered()), imageViewer[0], SLOT(fitToWindow()));
	
	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	
	aboutQtAct = new QAction(tr("About &Qt"), this);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Window::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct1);
	fileMenu->addAction(openAct2);
	fileMenu->addAction(printAct);

	fileMenu->addAction(cleanAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	
	
	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	viewMenu->addSeparator();
	viewMenu->addAction(fitToWindowAct);
	
	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
	
	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(helpMenu);
}

void Window::updateActions()
{
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void Window::cleanTmp()
{
	system("make clean");
}
