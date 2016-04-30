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

static std::ostream& operator <<(std::ostream & out, const NoteType &a)
{
    switch (a)
    {
    case RIGHT_NOTE:
        out << "Right";
        break;
    case UP_NOTE:
        out << "Up";
        break;
    case LEFT_NOTE:
        out << "Left";
        break;
    case DOWN_NOTE:
        out << "Down";
        break;
    default:
        break;
    }
    return out;
}

void GetNodeSequenceFromBinaryImage(const QImage& src, const std::vector<QImage>& stdnotes, std::vector<NoteType>& notesequence);

int GetNoteNumAndBeginFromBinaryImage(const QImage& src,std::vector<int>& begins);

void MarkNodeBeginEndFromBinaryImage(QImage& src, std::vector<QImage>& notes);

bool IsBlack(int color);

void MorphologicalEdgeDetectionByWTT(QImage& src);

QImage useCertainFilterInGray(const QImage& src, int theFilter[], int filterSize);

QImage QImageMinus(const QImage& src1, const QImage& src2);

void Mark(QImage& src, int ymin, int ymax, int xmin, int xmax);

int ostu_tres(const QImage& src);

void QImageToBinary(QImage& src,int thred);

QImage Clip(const QImage& src, int ymin, int ymax, int xmin, int xmax);

NoteType GuessNote(const QImage& src, int begin, const std::vector<QImage>& stdnotes);


#endif // IMAGEPROCESSING_H
