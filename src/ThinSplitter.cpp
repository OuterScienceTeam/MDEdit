#include "ThinSplitter.h"

#include <QSplitterHandle>
#include <QPainter>
#include <QPaintEvent>


class ThinHandle : public QSplitterHandle
{
public:
	ThinHandle(Qt::Orientation orientation, QSplitter* parent = 0)
		: QSplitterHandle(orientation, parent)
	{
		setMask(QRegion(contentsRect()));
		setAttribute(Qt::WA_MouseNoMask, true);
	}

protected:
	void paintEvent(QPaintEvent* event)
	{
		QPainter painter(this);
		painter.setBrush(Qt::black);
		painter.drawRect(event->rect());
		painter.end();
	}


	void resizeEvent(QResizeEvent* event)
	{
		setContentsMargins(1, 0, 1, 0);

		setMask(QRegion(contentsRect()));
		QSplitterHandle::resize(event->size());
		//QSplitterHandle::resizeEvent(event);
	}
};


ThinSplitter::ThinSplitter(QWidget *parent) :
	QSplitter(parent)
{
	setOrientation(Qt::Horizontal);
	setHandleWidth(1);
}


QSplitterHandle* ThinSplitter::createHandle()
{
	return new ThinHandle(orientation(), this);
}

