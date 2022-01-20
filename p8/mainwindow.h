#pragma once
#include <QList>
#include <QMainWindow>

class mandelbrotArea;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void save();
	void iterationChange();

private:
	void createActions();
	void createMenus();
	bool saveFile(const QByteArray &fileFormat);

	mandelbrotArea *msetArea;

	QMenu *saveAsMenu;
	QMenu *fileMenu;
	QMenu *optionMenu;

	QList<QAction *> saveAsActs;
	QAction *exitAct;
	QAction *iterationAct;
	QAction *refreshAct;
};
