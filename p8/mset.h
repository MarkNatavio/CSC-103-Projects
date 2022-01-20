#pragma once
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "complex.h"


class mandelbrotArea : public QWidget
{
	Q_OBJECT

public:
	mandelbrotArea(QWidget *parent = 0);

	void render(); // draw current portion of mandelbrot set.
	//unsigned long maxIter; // turned this into a global
	bool saveImage(const QString &fileName, const char *fileFormat);

	unsigned long getIterations();
	void setIterations(unsigned long newMax);

public slots:
	void refreshImage();

protected:
	// override some of the QWidget event handlers:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	void drawLineTo(const QPoint &endPoint);
	void resizeImage(QImage *image, const QSize &newSize);

	QImage image; 		// stores an array of pixels to draw.
	complex llCoord; 	// the point representing the lower left of our
						// little window of the complex plane.
	double windowWidth; // how wide is our window?  Note: the units here
						// are not pixels, but units in the complex plane.
	unsigned long maxIterations;
};
