#ifndef BLOCK_H
#define BLOCK_H

#include<QPoint>
#include<qcolor.h>

class block
{
public:
	block();
	block(QPoint, QColor);
	QPoint  p;
	QColor c;
};

#endif // BLOCK_H
