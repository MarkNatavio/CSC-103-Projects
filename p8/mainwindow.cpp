#include <QtGui>

#include "mainwindow.h"
#include "mset.h"


MainWindow::MainWindow()
{
	msetArea = new mandelbrotArea;
	setCentralWidget(msetArea);

	createActions();
	createMenus();

	setWindowTitle(tr("mandelbrot"));
	resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	return;
}

void MainWindow::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void MainWindow::iterationChange()
{
	bool ok;
	int iterations = QInputDialog::getInteger(this, tr("mandelbrot"),
											tr("Change max iterations:"),
											msetArea->getIterations(),
											10, 1<<30, 30, &ok);
	if (ok)
		msetArea->setIterations(iterations);
}

void MainWindow::createActions()
{
	foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
		QString text = tr("%1...").arg(QString(format).toUpper());

		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
		saveAsActs.append(action);
	}

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	iterationAct = new QAction(tr("&Iteration control..."), this);
	iterationAct->setShortcut(tr("Ctrl+I"));
	connect(iterationAct, SIGNAL(triggered()), this, SLOT(iterationChange()));
	
	refreshAct = new QAction(tr("&Redraw image"), this);
	refreshAct->setShortcut(tr("Ctrl+R"));
	connect(refreshAct, SIGNAL(triggered()), msetArea, SLOT(refreshImage()));
}

void MainWindow::createMenus()
{
	saveAsMenu = new QMenu(tr("&Save As"), this);
	foreach (QAction *action, saveAsActs)
		saveAsMenu->addAction(action);

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(iterationAct);
	optionMenu->addAction(refreshAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
							   initialPath,
							   tr("%1 Files (*.%2);;All Files (*)")
							   .arg(QString(fileFormat.toUpper()))
							   .arg(QString(fileFormat)));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return msetArea->saveImage(fileName, fileFormat);
	}
}
