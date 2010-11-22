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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#endif

#include "defs.h"
#include "match.h"
#include "sift_object.h"

#include <QMainWindow>
#include <QPrinter>
#include <QPainter>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class ImageViewer : public QWidget
{
	Q_OBJECT
	
	public:
		ImageViewer(QWidget *parent = 0);
		~ImageViewer();
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		
	public slots:
		void open();
		void compare();
		void print();
		void zoomIn();
		void zoomOut();
		void normalSize();
		void fitToWindow();

		void setDispImage(bool a);
		void setDispKP(bool a);
		void setDispInfo(bool a);
		void setDispMatch(bool a);
		
	//	void setFileName2(QString qstr);
//	signals:
//		void dispImageSetted(bool a);
		
	protected:
		
		void scaleImage(double factor);
		//void adjustScrollBar(QScrollBar *scrollBar, double factor);
		
		//void loadKeypoints();
		//void loadKeypoints2();
		void displayKeypoints(QPainter& painter)const;
		void displayMatches(QPainter& painter)const;
		void displayInfo(QPainter& painter)const;

		void paintEvent(QPaintEvent *event);
		
		//QLabel *imageLabel;
		//QPainter *imagePainter;
		double scaleFactor;
		
		bool dispImage;
		bool dispKP;
		bool dispInfo;
		bool dispMatch;
		
		QPrinter printer;
		//QPainter painter;
		
		//IplImage* im;
		QImage image1;
		QImage image2;
		//Keypoint keypoints;
		//QString name;
		QString fileName;
		//int nbFeat;
		SiftObject siftObj1;
		SiftComparison lastComparison;
		SiftObject siftObj2;

		/*IplImage* im2;
		QImage image2;
		Keypoint keypoints2;
		QString name2;
		QString fileName2;
		int nbFeat2;*/
};

#endif
