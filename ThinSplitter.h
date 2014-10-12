#ifndef THINSPLITTER_H
#define THINSPLITTER_H

#include <QSplitter>

class ThinSplitter : public QSplitter
{
public:
	ThinSplitter(QWidget *parent);

protected:
	virtual QSplitterHandle *createHandle();
};

#endif // THINSPLITTER_H
