#ifndef WINDOW_H
#define WINDOW_H

#include <QScrollArea>
#include <QMainWindow>

class QLabel;

class ImageViewer;

class Window : public QMainWindow
{
	Q_OBJECT
	
	public:
		Window();
		
	private:
		void createActions();
		void createMenus();
		void updateActions();
		
		QLabel *createLabel(const QString &text);
		
		/*QLabel *aliasedLabel;
		QLabel *antialiasedLabel;
		QLabel *intLabel;
		QLabel *floatLabel;*/
		static const int nbViewer = 1;
		ImageViewer *imageViewer[nbViewer];
		QScrollArea *scroll[nbViewer];
		
		QAction *openAct1;
		QAction *openAct2;
		QAction *printAct;
		QAction *cleanAct;
		QAction *exitAct;
		QAction *zoomInAct;
		QAction *zoomOutAct;
		QAction *normalSizeAct;
		QAction *fitToWindowAct;
		QAction *aboutAct;
		QAction *aboutQtAct;
		
		QMenu *fileMenu;
		QMenu *viewMenu;
		QMenu *helpMenu;
		
	private slots:
		void cleanTmp();
		void about();

};

#endif
