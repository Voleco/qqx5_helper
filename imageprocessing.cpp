

#include "imageprocessing.h"

bool IsBlack(int color)
{
    int res = color&0x00FFFFFF;
    if(res==0)
        return true;
    else
        return false;
}

void MorphologicalEdgeDetectionByWTT(QImage& src)
{
    //F - F。K
    int theFilter1[9] = {0,0,0,
                         0,0,0,
                         0,0,0};
    int theFilter2[9] = {1,1,1,
                         1,1,1,
                         1,1,1};
    //erosion, dilation

    QImage copy = QImage(src);
    src = useCertainFilterInGray(src,theFilter2,9);
    src = useCertainFilterInGray(src,theFilter1,9);

    src = QImageMinus(src,copy);
}

QImage QImageMinus(const QImage& src1, const QImage& src2)
{
    QImage dst = QImage(src1);
    int endx,endy;
    QRgb color;
    int r,g,b,alpha;
    if(src1.width() > src2.width())
        endx = src2.width();
    else
        endx = src1.width();
    if(src1.height() > src2.height())
        endy = src2.height();
    else
        endy = src1.height();

    for(int j=0;j<endy;j++)
        for(int i=0;i<endx;i++)
        {
            r = qRed(src1.pixel(i,j)) - qRed(src2.pixel(i,j));
            g = qGreen(src1.pixel(i,j)) - qGreen(src2.pixel(i,j));
            b = qBlue(src1.pixel(i,j)) - qBlue(src2.pixel(i,j));
            alpha = qAlpha(src1.pixel(i,j)) - qAlpha(src2.pixel(i,j));
            if(r<0)
                r=0;
            if(r>255)
                r=255;
            if(g<0)
                g=0;
            if(g>255)
                g=255;
            if(b<0)
                b=0;
            if(b>255)
                b=255;
            if(alpha<0)
                alpha=0;
            if(alpha>255)
                alpha=255;

            color = (alpha<<24) + (r<<16) + (g<<8) +b;
                dst.setPixel(QPoint(i,j),color);
        }

    return dst;
}


QImage useCertainFilterInGray(const QImage& src, int theFilter[], int filterSize)
{
    QImage dst =QImage(src);

    QRgb* nearArea;
    int kernel;//SE中心的标志,0代表寻找最小值，即膨胀,1代表寻找最大值,即腐蚀
    QRgb temp;

    //qDebug()<<kernelColor;
    //qDebug()<<src.pixel(src.width()-1,src.height()-1);
    switch(filterSize)
    {
    case 9:
        {
        nearArea = new QRgb[9];
        kernel = theFilter[4];
        for (int j = 1; j < src.height() - 1; j++)
        {
            for (int i = 1; i <src.width() - 1; i++)
            {
                nearArea[0] = src.pixel(i-1,j-1);
                nearArea[1] = src.pixel(i,j-1);
                nearArea[2] = src.pixel(i+1,j-1);
                nearArea[3] = src.pixel(i-1,j);
                nearArea[4] = src.pixel(i,j);
                nearArea[5] = src.pixel(i+1,j);
                nearArea[6] = src.pixel(i-1,j+1);
                nearArea[7] = src.pixel(i,j+1);
                nearArea[8] = src.pixel(i+1,j+1);

                if(kernel==0)//要寻找最小值，所以先让temp是最大值
                {
                    temp = MYWHITE;
                    for(int k=0;k<9;k++)
                    {
                        if((theFilter[k]==kernel)&&(qGray(temp)>qGray(nearArea[k])))
                        {
                            temp = nearArea[k];
                        }
                    }
                }
                else
                {
                    temp = MYBLACK;
                    for(int k=0;k<9;k++)
                    {
                        if((theFilter[k]==kernel)&&(qGray(temp)<qGray(nearArea[k])))
                        {
                            temp = nearArea[k];
                        }
                    }
                }
                dst.setPixel(QPoint(i,j),temp);
            }
        }
    }
    break;
    case 25:
        {
        nearArea = new QRgb[25];
        kernel = theFilter[12];
        for (int j = 2; j < src.height() - 2; j++)
        {
            for (int i = 2; i <src.width() - 2; i++)
            {
                nearArea[0] = src.pixel(i-2,j-2);
                nearArea[1] = src.pixel(i-1,j-2);
                nearArea[2] = src.pixel(i,j-2);
                nearArea[3] = src.pixel(i+1,j-2);
                nearArea[4] = src.pixel(i+2,j-2);
                nearArea[5] = src.pixel(i-2,j-1);
                nearArea[6] = src.pixel(i-1,j-1);
                nearArea[7] = src.pixel(i,j-1);
                nearArea[8] = src.pixel(i+1,j-1);
                nearArea[9] = src.pixel(i+2,j-1);
                nearArea[10] = src.pixel(i-2,j);
                nearArea[11] = src.pixel(i-1,j);
                nearArea[12] = src.pixel(i,j);
                nearArea[13] = src.pixel(i+1,j);
                nearArea[14] = src.pixel(i+2,j);
                nearArea[15] = src.pixel(i-2,j+1);
                nearArea[16] = src.pixel(i-1,j+1);
                nearArea[17] = src.pixel(i,j+1);
                nearArea[18] = src.pixel(i+1,j+1);
                nearArea[19] = src.pixel(i+2,j+1);
                nearArea[20] = src.pixel(i-2,j+2);
                nearArea[21] = src.pixel(i-1,j+2);
                nearArea[22] = src.pixel(i,j+2);
                nearArea[23] = src.pixel(i+1,j+2);
                nearArea[24] = src.pixel(i+2,j+2);
                if(kernel==0)//要寻找最小值，所以先让temp是最大值
                {
                    temp = MYWHITE;
                    for(int k=0;k<25;k++)
                    {
                        if((theFilter[k]==kernel)&&(qGray(temp)>qGray(nearArea[k])))
                        {
                            temp = nearArea[k];
                        }
                    }
                }
                else
                {
                    temp = MYBLACK;
                    for(int k=0;k<25;k++)
                    {
                        if((theFilter[k]==kernel)&&(qGray(temp)<qGray(nearArea[k])))
                        {
                            temp = nearArea[k];
                        }
                    }
                }
                dst.setPixel(QPoint(i,j),temp);
            }
        }
    }
    break;
    default:
        break;
    }
    return dst;
}


void Mark(QImage& src, int ymin, int ymax, int xmin, int xmax)
{
    for(int i=xmin;i<xmax;i++)
    {
        src.setPixel(i,ymin,0x00ff0000);
        src.setPixel(i,ymax,0x00ff0000);
    }
    for(int j=ymin;j<ymax;j++)
    {
        src.setPixel(xmin,j,0x00ff0000);
        src.setPixel(xmax,j,0x00ff0000);
    }
}


int ostu_tres(const QImage& src)
{
    int thr_value = 0;
    double maxB = -1;
    int w = 0;
    int all_u = 0; //avg_h为总灰度值
    double u = 0; //h为平均灰度值

    qDebug()<<"changedimage width"<<src.width()<<"height"<<src.height();
    int grayValue;
    int pixel_gray[256]={0};
    for(int j=0;j<src.height();j++)
    {
              for(int i=0;i<src.width();i++)
              {
                  grayValue = qGray(src.pixel(i,j));
                  pixel_gray[grayValue]++;
              }
    }

    for (int i = 0; i < 256; i++)
    {
        all_u += (double)pixel_gray[i] * (double)i;
        w += pixel_gray[i];
    }
    u = (double)all_u / (double)w;
    for (int i = 0; i < 256; i++)
    {
        double w0 = 0, u0 = 0, w1 = 0, u1 = 0, b;
        int j = 0;
        for (; j <= i; j++)
        {
            w0 += pixel_gray[j];
            u0 += pixel_gray[j] * j;
        }
        w1 = ((double)w - (double)w0) / (double)w;
        u1 = ((double)all_u - (double)u0) / ((double)w - (double)w0);
        u0 = (double)u0 / (double)w0;
        w0 = (double)w0 / w;
        b = (double)w0 * ((double)u0 - (double)u) * ((double)u0 - (double)u)
             + (double)w1 * ((double)u1 - (double)u) * ((double)u1 - (double)u);
        if (b > maxB)
        {
           maxB = b;
           thr_value = i;
        }
    }
//    Now the thresholdValue has been caculated!
//    qDebug()<<thr_value;

    return thr_value;
}


void QImageToBinary(QImage& src,int thred)
{
      qDebug()<<"imagetobin width"<<src.width()<<"height"<<src.height();
    int grayValue;//灰度值
    QRgb grayColor;//由灰度值计算得到的颜色值
    for(int i=0;i<src.width();i++)
        for(int j=0;j<src.height();j++)
        {
            grayValue = qGray(src.pixel(i,j));
            if(grayValue>thred)
                grayColor = MYWHITE;
            else
                grayColor = MYBLACK;
            src.setPixel(QPoint(i,j),grayColor);
        }
}


QImage Clip(const QImage& src, int ymin, int ymax, int xmin, int xmax)
{
    QImage dst = QImage(xmax-xmin,ymax-ymin,src.format());
    for(int j=0;j<ymax-ymin;j++)
        for(int i=0;i<xmax-xmin;i++)
            dst.setPixel(i,j,src.pixel(i+xmin,j+ymin));


    return dst;
}

int GetNoteNumAndBeginFromBinaryImage(const QImage& src,std::vector<int>& begins)
{
    begins.resize(0);
    std::vector<int> counts;
    int count = 0;
    for(int i=0;i<src.width();i++)
    {
        count = 0;
        for(int j=0;j<src.height();j++)
            if(IsBlack(src.pixel(i,j)))
                count++;
        counts.push_back(count);
    }
    int num = 0;
    int num_error = 0;
    int consecutive_zero = 0;
    int consecutive_nonzero = 0;
    int begin =0;
    for(unsigned int i=0;i<counts.size();i++)
    {
        if(counts[i]>0)
        {
            if(consecutive_nonzero>0)
                consecutive_nonzero++;
            else
            {
                consecutive_zero = 0;
                consecutive_nonzero = 1;
                begin = i-1;
            }
        }
        else
        {
            if(consecutive_nonzero>0)
            {
                if(consecutive_nonzero>22 && consecutive_nonzero<28)
                {
                    num++;
                    begins.push_back(begin);
                }
                else
                    num_error++;
                consecutive_zero = 1;
                consecutive_nonzero = 0;

            }
            else
            {
                consecutive_zero++;
            }

        }
    }
    if(num>9||num<1||num_error>5)
        num = -1;
    return num;
}


void MarkNodeBeginEndFromBinaryImage(QImage& src, std::vector<QImage> &notes)
{
    notes.resize(0);
    std::vector<int> counts;
    int count = 0;
    for(int i=0;i<src.width();i++)
    {
        count = 0;
        for(int j=0;j<src.height();j++)
            if(IsBlack(src.pixel(i,j)))
                count++;
        counts.push_back(count);
        //qDebug()<<count;
    }
    int num = 0;
    int num_error = 0;
    int consecutive_zero = 0;
    int consecutive_nonzero = 0;
    int begin =0;
    int end = 0;
    for(unsigned int i=0;i<counts.size();i++)
    {
        if(counts[i]>0)
        {
            if(consecutive_nonzero>0)
                consecutive_nonzero++;
            else
            {
                consecutive_zero = 0;
                consecutive_nonzero = 1;
                begin = i-1;
            }
        }
        else
        {
            if(consecutive_nonzero>0)
            {
                //qDebug()<<"conse_non_zero"<<consecutive_nonzero;
                if(consecutive_nonzero>20 && consecutive_nonzero<30)
                {
                    num++;
                    end = i;

                    QImage note(end-begin,src.height(),src.format());
                    for(int y=0;y<src.height();y++)
                        for(int x=0;x<end-begin;x++)
                            note.setPixel(x,y,src.pixel(x+begin,y));
                    notes.push_back(note);
                    for(int j=0;j<src.height();j++)
                    {
                        src.setPixel(begin,j,0x00ff0000);
                        src.setPixel(end,j,0x00ff0000);
                    }
                }
                else
                    num_error++;
                consecutive_zero = 1;
                consecutive_nonzero = 0;

            }
            else
            {
                consecutive_zero++;
            }

        }
    }
    if(num>9||num<1||num_error>5)
        num = -1;
}


NoteType GuessNote(const QImage& src, int begin, const std::vector<QImage>& stdnotes)
{
    std::vector<int> scores(stdnotes.size(),0);
    for(unsigned int i=0;i<stdnotes.size();i++)
    {
        //for each candidate note, we compute 3 scores and take the max one
        int max = 0;
        int current = 0;
        for(int offset=-1;offset<2;offset++)
        {
            current = 0;
            for(int y=0;y<stdnotes[i].height();y++)
            {
                for(int x=0;x<stdnotes[i].width();x++)
                {
                    if(src.pixel(begin+offset+x,y)==stdnotes[i].pixel(x,y))
                        current++;
                }
            }
            if(current>max)
                max=current;
        }
        scores[i]=max;
    }
    int maxScore=0;
    int index=0;
    for(unsigned int i=0;i<scores.size();i++)
    {
        if(scores[i]>maxScore)
        {
            maxScore = scores[i];
            index = i;
        }
    }
    return index;
}

void GetNodeSequenceFromBinaryImage(const QImage& src, const std::vector<QImage>& stdnotes, std::vector<NoteType>& notesequence)
{
    //I assume src is 325x40 for now
    notesequence.resize(0);
    std::vector<int> begins;
    int num = GetNoteNumAndBeginFromBinaryImage(src,begins);
    if(num==-1)
        return;

    //in this case, it is probably the case that the sequence we recieve is incomplete
    if((325-begins[begins.size()-1])-(begins[0]-0)>50)
        return;
    NoteType current;
    for(int i=0;i<num;i++)
    {
        current = GuessNote(src, begins[i],stdnotes);
        notesequence.push_back(current);
    }
}
