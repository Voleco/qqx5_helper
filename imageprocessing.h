#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <vector>
#include <qDebug>
#include <QImage>
#include <QRgb>

#define MYWHITE  0xffffffff
#define MYBLACK  0xff000000

#define NOTE_RIGHT 0
#define NOTE_UP 1
#define NOTE_LEFT 2
#define NOTE_DOWN 3

typedef int NoteType;

static std::ostream& operator <<(std::ostream & out, const NoteType &a)
{
    switch (a)
    {
    case NOTE_RIGHT:
        out << "Right";
        break;
    case NOTE_UP:
        out << "Up";
        break;
    case NOTE_LEFT:
        out << "Left";
        break;
    case NOTE_DOWN:
        out << "Down";
        break;
    default:
        break;
    }
    return out;
}

//if the image is not a screen of notes, the function will return FALSE indicating that it fails to do the decoding
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
