/****************************************************************************
**
** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "imageviewer.h"
//#include "comparator.h"

/*#include "matrice.h"
#include "classprob/Probability.h"
#include "classprob/BinomialProbability.h"
*/

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
	/*imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);*/
	
	//QHBoxLayout *layout = new QHBoxLayout;
	//painter = new QPainter;
	//layout->addStretch(painter);
	//imagePainter->setBackground(QPalette::Base);
	
	/*widMain = new QWidget;
	widMain->resize(1300, 1300);//   setGeometry(0,0,width()/2, height()/2);
	scrollArea = new QScrollArea(this);
	//scrollArea->setGeometry(0,0,width()/2, height()/2);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(widMain);
	//scrollArea->setWidget(imagePainter);
	//setCentralWidget(scrollArea);
	
	//QHBoxLayout *layout = new QHBoxLayout;
	//setFixedSize(width(), height());
	setBackgroundRole(QPalette::Base);
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	*/
	//setLayout(layout);
	
	//widMain->show();
	//connect(this, SIGNAL(leftClick()),this, SLOT(open()));
	//comp->im=NULL;
	//comp->keypoints=NULL;
	
	//siftObj1=siftObj2=NULL;
	
	dispInfo=false;
}
ImageViewer::~ImageViewer()
{
	//if(siftObj1!=NULL)delete siftObj1;
	//if(siftObj2!=NULL)delete siftObj2;
}


void ImageViewer::open()
{
	fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		QString fileNamePgm, name;
		name=fileName;
		
		name.replace(name.findRev('.'),9999,"");
		name.replace(0, name.findRev('/')+1, "");
		
		char cmd[256];
		
		sprintf(cmd, "cp -n %s jpg 2> /dev/null ", fileName.ascii());
		fileName.replace(0, fileName.findRev('/')+1, "jpg/");
		system(cmd);
		/*sprintf(cmd, "make pgm/%s.pgm", name.ascii());
		system(cmd);*/
		//sprintf(cmd, "make key/%s.key", name.ascii());
		//system(cmd);
		
		fileNamePgm=name;
		fileNamePgm.append(".pgm");
		fileNamePgm.prepend("pgm/");
		
		//siftObj1= new SiftObject(fileNamePgm.ascii());
		siftObj1.Reset();
		siftObj1.Load(fileNamePgm.ascii());
		image1.load(fileName);
		
		if(!siftObj2.IsEmpty())
			lastComparison = siftObj1.Compare1(siftObj2);
		
		update();
	}
}

void ImageViewer::compare()
{
	QString fileName2 = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
	if (!fileName2.isEmpty()) {
		QString fileName2Pgm, name2;
		name2=fileName2;
		
		name2.replace(name2.findRev('.'),9999,"");
		name2.replace(0, name2.findRev('/')+1, "");
		
		char cmd[256];
		
		sprintf(cmd, "cp -n %s jpg 2> /dev/null ", fileName2.ascii());
		fileName2.replace(0, fileName2.findRev('/')+1, "jpg/");
		system(cmd);
		/*sprintf(cmd, "make pgm/%s.pgm", name.ascii());
		system(cmd);*/
		//sprintf(cmd, "make key/%s.key", name.ascii());
		//system(cmd);
		
		fileName2Pgm=name2;
		fileName2Pgm.append(".pgm");
		fileName2Pgm.prepend("pgm/");

		siftObj2.Reset();
		siftObj2.Load(fileName2Pgm.ascii());
		image2.load(fileName2);
		
		if(!siftObj1.IsEmpty())
			lastComparison = siftObj1.Compare1(siftObj2);
		
		update();
	}
}

void ImageViewer::paintEvent(QPaintEvent *)
{
//	printf("paintEvent\n");
	QPainter painter(this);
	painter.setBackground(Qt::white);
	
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.drawRect(QRect(0, 0, width()-1, height()-1));
	
	if(!fileName.isNull()){
		//QImage image(fileName);
		//image.load(fileName);
		//QRect target(0,0,MIN(width(),image.width()),MIN(height(), image.height()));
		//resize(image.width(), image.height());
		int width1=image1.width();
		int height1=image1.height();
		
		/* If display matches, make painter bigger*/
		if(!siftObj2.IsEmpty() && dispMatch){
			width1=MAX(image1.width(), image2.width());
			height1+=image2.height();
		}
		
		resize(MAX(width1,640), MAX(height1, 480));
		
		QRect target(0, 0, image1.width(), image1.height());
		//setGeometry(0,0,image.size().width(),image.size().height());
		//printf("size %d %d ->  %d %d\n",width(),height(),image.size().width(),image.size().height());
		//painter.
		if(dispImage) painter.drawImage(target,image1);
		
		if(dispImage && dispMatch)
		{
			target=QRect(0, image1.height(), image2.width(), image2.height());
			painter.drawImage(target,image2);
		}
		if(dispKP) displayKeypoints(painter);
		if(dispMatch) displayMatches(painter);
		if(dispInfo) displayInfo(painter);
	}
}

void ImageViewer::print()
{
	/*Q_ASSERT(imageLabel->pixmap());
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
       QPainter painter(&printer);
       QRect rect = painter.viewport();
       QSize size = imageLabel->pixmap()->size();
       size.scale(rect.size(), Qt::KeepAspectRatio);
       painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
       painter.setWindow(imageLabel->pixmap()->rect());
       painter.drawPixmap(0, 0, *imageLabel->pixmap());
}*/
}

void ImageViewer::zoomIn()
{
	//	scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
	//	scaleImage(0.8);
}

void ImageViewer::normalSize()
{
	/*imageLabel->adjustSize();
	scaleFactor = 1.0;*/
}

void ImageViewer::fitToWindow()
{
	/*	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow) {
       normalSize();
}
updateActions();*/
}


void ImageViewer::scaleImage(double factor)
{
	/*Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
	
	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);
	
	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);*/
}

/*void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
	+ ((factor - 1) * scrollBar->pageStep()/2)));
	}*/

QSize ImageViewer::minimumSizeHint() const
{
	return QSize(200, 200);
}

QSize ImageViewer::sizeHint() const
{
	return QSize(200, 200);
}

void ImageViewer::setDispImage(bool a){
	dispImage=a;
	update();
	//	emit dispImageSetted(a);
}

void ImageViewer::setDispKP(bool a){
	dispKP=a;
	update();
	//	emit dispImageSetted(a);
}

void ImageViewer::setDispInfo(bool a){
	dispInfo=a;
	update();
	//	emit dispImageSetted(a);
}

void ImageViewer::setDispMatch(bool a){
	dispMatch=a;
	update();
	//	emit dispImageSetted(a);
}

void ImageViewer::displayKeypoints(QPainter& painter)const
{
	//CvSize s;
	QPoint pt1, pt2;
	//CvScalar color;
	
	/*char keyFile[256];
	sprintf(keyFile, "key/%s.key"
	if(!fopen(""));*/
	
	if (siftObj1.keypoints==NULL){
		printf("ERROR : Keypoints NULL\n");
		exit(-1);
	}
	
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::white);
	
	/*Display keypoints*/
	if (!siftObj1.IsEmpty() /*&& siftObj1.keypoints!=NULL*/){
		for (Keypoint k=siftObj1.keypoints;k!=NULL;k=k->next) {
			//printf("kp1 %p %f,%f\n", k, k->col, k->row);
			pt1.setX(ROUND(k->col));
			pt1.setY(ROUND(k->row));
			painter.drawEllipse(pt1, 3, 3);
			
			// Display keypoints vectors
			pt2.setX(ROUND(k->col + k->scale * 1 * cos(k->ori)));
			pt2.setY(ROUND(k->row + k->scale * 1 * sin(k->ori)));
			painter.drawLine (pt1, pt2);
		}
	}
	//cout<<dispMatch<<" ADFADSF "<<siftObj2.IsEmpty()<<endl;
	
	if (dispMatch && !siftObj2.IsEmpty() /*&& siftObj2.keypoints!=NULL*/){
		/*Display keypoints 2*/
		for (Keypoint k=siftObj2.keypoints;k!=NULL;k=k->next) {
			//printf("kp2 %p %f,%f    %p\n", k, k->col, k->row, k->next);
			pt1.setX(ROUND(k->col));
			pt1.setY(ROUND(k->row) + image1.height());
			painter.drawEllipse(pt1, 3, 3);
			
			// Display keypoints vectors
			pt2.setX(ROUND(k->col + k->scale * 1 * cos(k->ori)));
			pt2.setY(ROUND(k->row + k->scale * 1 * sin(k->ori) + image1.height()));
			painter.drawLine (pt1, pt2);
		}
	}
}

void ImageViewer::displayMatches(QPainter& painter)const
{
	QPoint pt1, pt2;

	if (siftObj1.keypoints==NULL){
		printf("ERROR : Keypoints NULL\n");
		exit(-1);
	}
		
	if (dispMatch && lastComparison.tab_match!=NULL && !siftObj1.IsEmpty() ){
		// Display matches
		for (int i=0;i<lastComparison.nb_match;i++) {
			pt1.setX(ROUND(lastComparison.tab_match[i].x1));
			pt1.setY(ROUND(lastComparison.tab_match[i].y1));
			pt2.setX(ROUND(lastComparison.tab_match[i].x2));
			pt2.setY(ROUND(lastComparison.tab_match[i].y2 + siftObj1.im->height));
			
			painter.setBrush(Qt::white);
			if (lastComparison.tab_match[i].id==0)
				painter.setPen(Qt::red); //red for discarded matches
			else painter.setPen(Qt::green); //green
			
			painter.drawLine(pt1, pt2);
			painter.drawEllipse(pt1, 3, 3);
			painter.drawEllipse(pt2, 3, 3);
		}
	}
	
	#ifdef AAA
		
	//IplImage * im,* imcol;
	QSize s;
	//QPoint pt1, pt2;
	//CvScalar color;
	int i,j,im2null=0;
	Keypoint k1*=siftObj1->keypoints;
	Keypoint k2*=siftObj2->keypoints;
		/*Affine transform of the image border*/
		
		if (param.size_m()>0) {
			Matrice p1(2,1), p2(2,1), p3(2,1), p4(2,1), transl(2,1);
			transl.set_val(0,0,0);
			transl.set_val(1,0,im1->height);
			p1.set_val(0,0,0);
			p1.set_val(1,0,0);
			p2.set_val(0,0,im1->width);
			p2.set_val(1,0,0);
			p3.set_val(0,0,im1->width);
			p3.set_val(1,0,im1->height);
			p4.set_val(0,0,0);
			p4.set_val(1,0,im1->height);
			
			p1=Transform(p1,param)+transl;
			p2=Transform(p2,param)+transl;
			p3=Transform(p3,param)+transl;
			p4=Transform(p4,param)+transl;
			
			color=CV_RGB(0,128,255); //light blue
			pt1.x=ROUND(p1.get_val(0,0));
			pt1.y=ROUND(p1.get_val(1,0));
			pt2.x=ROUND(p2.get_val(0,0));
			pt2.y=ROUND(p2.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p2.get_val(0,0));
			pt1.y=ROUND(p2.get_val(1,0));
			pt2.x=ROUND(p3.get_val(0,0));
			pt2.y=ROUND(p3.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p3.get_val(0,0));
			pt1.y=ROUND(p3.get_val(1,0));
			pt2.x=ROUND(p4.get_val(0,0));
			pt2.y=ROUND(p4.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p4.get_val(0,0));
			pt1.y=ROUND(p4.get_val(1,0));
			pt2.x=ROUND(p1.get_val(0,0));
			pt2.y=ROUND(p1.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			
			/* Draw the border of the object */
			CvMemStorage *storage= cvCreateMemStorage (0); /* Memory used by openCV */
			int header_size = sizeof( CvContour );
			CvSeq *contours;
			
			IplImage* imthres = cvCreateImage(cvSize(im1->width,im1->height),IPL_DEPTH_8U, 1 );
			cvCopy( im1, imthres, 0 );
			
			/* First find the contour of a thresholded image*/
			
			cvThreshold(imthres, imthres, border_threshold, 255, CV_THRESH_BINARY );
			cvFindContours ( imthres, storage, &contours, header_size, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
			
			/* For each contour found*/
			
			while ( contours != NULL) {
				double area=fabs(cvContourArea(contours,CV_WHOLE_SEQ)); // compute area
				if ( area > 20) {
					for (int i=0;i<contours->total;i++) {
						
						/* Compute transform of contour*/
						
						CvPoint* cvpt=(CvPoint*)cvGetSeqElem( contours, i);
						p1.set_val(0,0,cvpt->x);
						p1.set_val(1,0,cvpt->y);
						p1= Transform(p1,param) + transl;
						cvpt->x=ROUND(p1.get_val(0,0));
						cvpt->y=ROUND(p1.get_val(1,0));
					}
					//                cvDrawContours( imcol, contours, CV_RGB(0,0,255),CV_RGB(255,0,0),0,2,8);
					cvDrawContours( imcol, contours, CV_RGB(0,0,255),CV_RGB(0,0,255),0,2,8);
				}
				contours = contours->h_next; // ?????
			}
			free( contours );
			cvReleaseMemStorage( &storage );
			
	}
	#endif
}

void ImageViewer::displayInfo(QPainter& painter)const
{
	QString qstr;
	char str[256];
	int lines=4;
	
	painter.setPen(Qt::white);
	painter.setBrush(Qt::darkBlue);

	qstr.append(tr(" Image size"));
	sprintf(str, " : %dx%d\n", image1.width(), image1.height());
	qstr.append(str);
	qstr.append(tr(" Number of features"));
	sprintf(str, " : %d\n", siftObj1.nb_feat);
	qstr.append(str);
	
	qstr.append(tr(" Features in model"));
	sprintf(str, " : %d\n", siftObj1.nb_feat_model);
	qstr.append(str);
	
	qstr.append(tr(" Total number of features"));
	sprintf(str, " : %d\n", siftObj1.nb_feat_tot);
	qstr.append(str);

	if(lastComparison.name.compare("")!=0)
	{
		qstr.append(tr(" Comparison"));
		sprintf(str, " : %s\n", lastComparison.name.c_str());
		qstr.append(str);
		
		qstr.append(tr("  - nb matches"));
		sprintf(str, " : %d\n", lastComparison.nb_match);
		qstr.append(str);
		
		qstr.append(tr("  - feature in projection"));
		sprintf(str, " : %d\n", lastComparison.feat_in_proj);
		qstr.append(str);
		
		qstr.append(tr("  - probability of presence"));
		sprintf(str, " : %f\n", lastComparison.prob_presence);
		qstr.append(str);
		
		lines+=4;
	}
	QRect target(0, 0, 270, lines*16);

	painter.drawRect(target);
	painter.drawText(target, qstr);
}
