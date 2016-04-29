#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <vector>
#include <qDebug>
#include <QImage>
#include <QRgb>

#define MYWHITE  0xffffffff
#define MYBLACK  0xff000000

#define RIGHT_NOTE 0
#define UP_NOTE 1
#define LEFT_NOTE 2
#define DOWN_NOTE 3

typedef int NoteType;

void GetNodesFromBinaryImage(const QImage& src, std::vector<NoteType>& notes);

void GetNodeNumFromBinaryImage(const QImage& src, int &num);

void MarkNodeBeginEndFromBinaryImage(QImage& src);

bool IsBlack(int color);

void MorphologicalEdgeDetectionByWTT(QImage& src);

QImage useCertainFilterInGray(const QImage& src, int theFilter[], int filterSize);

QImage QImageMinus(const QImage& src1, const QImage& src2);

void Mark(QImage& src, int ymin, int ymax, int xmin, int xmax);

int ostu_tres(const QImage& src);

void QImageToBinary(QImage& src,int thred);

QImage Clip(const QImage& src, int ymin, int ymax, int xmin, int xmax);
#endif // IMAGEPROCESSING_H
