#include "barcode_decode.h"

partition_info CODE_INFO[20];

int Min(int x, int y)
{
    if(x<y)
        return x;
    else
        return y;
}

int max(int x, int y)
{
    if(x>y)
        return x;

    else
        return y;
}


int findmin(int *A, int low, int high)
{
    int temp = A[low];
    for(int i=low;i<high;i++)
    {
        if(A[low]<temp)
            temp = A[low];
    }
    return temp;
}
int findmin(std::vector<int> A, int low, int high)
{
    int temp = A[low];
    for(int i=low;i<high;i++)
    {
        if(A[low]<temp)
            temp = A[low];
    }
    return temp;
}


int ostuForArray(int value[],int size)
{
    int thr_value = 0;
    double maxB = -1;
    int w = 0;
    int all_u = 0; //avg_h为总灰度值
    double u = 0; //h为平均灰度值

    int grayValue;
    int pixel_gray[256]={0};
    for(int i=0;i<size;i++)
    {
        grayValue = value[i];
        pixel_gray[grayValue]++;
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
    //qDebug()<<thr_value;

    return thr_value;
}

void getMeanAndVariance(const QImage *src, int scanline_y, double o, double w,int* values, double &umax, double &umin, double &variancemax, double &variancemin)
{
    //qDebug()<<"ok 1 begin";
    //预防越界
    //int start = max(0,o-w);
    //int end = min(src->width(),o+8*w);
    int start = o-w;
    int end = o+8*w;

    //values = new int[end-start];
    int *sortedValues = new int[end-start];
    //获取范围内的点的灰度值
    for(int i=0;i<end-start;i++)
    {
        values[i] = qGray(src->pixel(start+i,scanline_y));
        sortedValues[i] = values[i];
    }
    //对灰度值按照从小到大排序
    for(int i=0;i<end-start;i++)
    {
        int temp = sortedValues[i];
        for(int j=i;j<end-start;j++)
        {
            if(sortedValues[j]<temp)
            {
                sortedValues[i] = sortedValues[j];
                sortedValues[j] = temp;
                temp = sortedValues[i];
            }
        }
    }
    //采用阈值算法得到的small half
    int threshold = ostuForArray(values,end-start);
    int smallHalfSize = 0;
    for(int i=1;i<=end-start;i++)
    {
        if(i==end-start)
        {
            smallHalfSize = i;
            break;
        }
        else if(sortedValues[i-1]<threshold&&sortedValues[i]>=threshold)
        {
            smallHalfSize = i;
            break;
        }
    }

    int bigHalfSize = (end-start)-smallHalfSize;
    //qDebug()<<smallHalfSize<<bigHalfSize;
    int smallHalfSum = 0;
    int bigHalfSum = 0;
    //求平均值
    for(int i=0;i<smallHalfSize;i++)
    {
        smallHalfSum = smallHalfSum + sortedValues[i];
    }
    umin = 1.0*smallHalfSum/smallHalfSize;
    for(int i=0;i<bigHalfSize;i++)
    {
        bigHalfSum = bigHalfSum + sortedValues[i+smallHalfSize];
    }
    umax = 1.0*bigHalfSum/bigHalfSize;

    //求方差
    variancemin =0;
    variancemax =0;

    /*
    for(int i=0;i<smallHalfSize;i++)
    {
      variancemin = variancemin + (sortedValues[i]-umin)*(sortedValues[i]-umin);
    }
    for(int i=0;i<bigHalfSize;i++)
    {
        variancemax = variancemax + (sortedValues[i]-umax)*(sortedValues[i]-umax);
    }
    variancemin = variancemin/smallHalfSize;
    variancemax = variancemax/bigHalfSize;
    */
    for(int i=0;i<end-start;i++)
    {
        variancemin = variancemin + (sortedValues[i]-threshold)*(sortedValues[i]-threshold);
    }
    variancemax = variancemin;



    //qDebug()<<"ok 1 end"<<endl;;
}

double DPositiveOne(int in, const double &umax, const double &variancemax)
{
    //qDebug()<<"umax"<<umax<<endl;
    double x = 1.0*Min(in-umax,0);
    //qDebug()<<"variancemax= "<<variancemax<<endl;
    return x*x*0.5/variancemax;
}

double DNegtiveOne(int in, const double &umin, const double &variancemin)
{
    double x = 1.0*max(in-umin,0);
    return x*x*0.5/variancemin;
}

int DForLeftPart(double o, double w,int* values,const double &umax,const double &umin,const double &variancemax,const double &variancemin)
{
    //qDebug()<<"ok 2";
    double A[6];
    for(int i=0;i<6;i++)
        A[i]=0;

    int seperator[7];
    int start = o-w;
    int end = o+8*w;

    //values[]数组存储的是o-w 到 o+8*w的点的灰度值
    //从o-w 到 o+8*w范围内所有点的灰度值I(n)的指数部分D(1)和D(-1)的集合
    double *dPOne = new double[end -start];
    double *dNOne = new double[end -start];

    for(int i=0;i<end-start;i++)
    {
        dPOne[i] = DPositiveOne(values[i],umax,variancemax);
        //qDebug()<<"ok 2.0.3"<<endl;
        dNOne[i] = DNegtiveOne(values[i],umin,variancemin);
        //qDebug()<<"the "<<i<<"th dPone& dNone"<<dPOne[i]<<dNOne[i]<<endl;
    }


    //qDebug()<<"ok 2.1"<<endl;
    //20种可能的左边部分编码的p的指数部分的集合
    //dset0-9为left_hand_even编码,dset10-19为left_hand_odd
    double dset[20];
    for(int i=0;i<20;i++)
        dset[i] = 0;

    seperator[0] = -1;
    seperator[1] = 0;
    seperator[6] = 8;

    char result[7];
    double DPO = 0;
    double DNO = 0;

    for(int i=0;i<7;i++)
    {
        DPO= 0;
        DNO = 0;
        for(int j=o+i*w;j<o+(i+1)*w;j++)
        {
            DPO +=dPOne[j-start];
            DNO +=dNOne[j-start];
        }
        qDebug()<<"DPO"<<DPO<<"DNO"<<DNO;
        if(DPO<DNO)
            result[i] = '1';
        else
            result[i] = '0';
    }
    qDebug()<<result;

    //A[0] = A[5] =0;

    //qDebug()<<"ok 2.2"<<endl;
    for(int k=0;k<20;k++)
    {
        for(int i=1;i<6;i++)
        {
            seperator[i]=0;
            for(int j=0;j<i-1;j++)
                seperator[i] = seperator[i]+ LEFT_HAND_ALL[k][j];
            //qDebug()<<"seperator"<<i<<" = "<<seperator[i]<<"\t";
        }
        //以下计算A[1] - A[4],分别代表条形码的一个字符的4个信息

        for(int l=0;l<CODE_INFO[k].pieces;l++)
        {
            //o+do, w+dw
            double oPdo = o+CODE_INFO[k].one_point_o[l];
            double wPdw = w+CODE_INFO[k].one_point_w[l];
//            double oPdo = o;
//            double wPdw = w;
            for(int i=1;i<5;i++)
            {
                A[i]=0;
                if(i%2==0)//i=2,4,期望匹配-1
                {
                    for(int j=oPdo+seperator[i]*wPdw;j<oPdo+seperator[i+1]*wPdw;j++)
                    {
                        A[i] = A[i] + dNOne[j-start];
                    }
                }
                if(i%2==1)//i=1,3,期望匹配+1
                {
                    for(int j=oPdo+seperator[i]*wPdw;j<oPdo+seperator[i+1]*wPdw;j++)
                    {
                        A[i] = A[i] + dPOne[j-start];
                    }
                }
                //qDebug()<<"A"<<i<<" = "<<A[i]<<"\t";
                //qDebug()<<"A is sum of"<<o+seperator[i]*w-start<<"to"<<o+seperator[i+1]*w-start<<"\t";
                // dset[k] = sigma(A[i],i=1-4)
                A[i] = A[i]*CODE_INFO[k].percent[l];
                dset[k] = dset[k] + A[i];
            }
        }

    }

    //qDebug()<<"ok 2.4"<<endl;
    //minD记录最小的d值,minIndex记录这个d值在dset的位置
    int minIndex=0;
    double minD=dset[0];
    for(int i=1;i<20;i++)
    {
        if(dset[i]<minD)
        {
            minIndex = i;
            minD = dset[i];
        }
    }

//    for(int i=0;i<20;i++)
//    {
//        qDebug()<<"the "<<i<<"th dset"<<dset[i]<<endl;;
//    }
    //qDebug()<<"ok2end"<<endl;;
    return minIndex;
}

int DForRightPart(double o, double w,int* values,const double &umax,const double &umin,const double &variancemax,const double &variancemin)
{
    //qDebug()<<"ok 2";
    double A[6];
    for(int i=0;i<6;i++)
        A[i]=0;

    int seperator[7];
    int start = o-w;
    int end = o+8*w;

    //values[]数组存储的是o-w 到 o+8*w的点的灰度值
    //从o-w 到 o+8*w范围内所有点的灰度值I(n)的指数部分D(1)和D(-1)的集合
    double *dPOne = new double[end -start];
    double *dNOne = new double[end -start];

    for(int i=0;i<end-start;i++)
    {
        dPOne[i] = DPositiveOne(values[i],umax,variancemax);
        //qDebug()<<"ok 2.0.3"<<endl;
        dNOne[i] = DNegtiveOne(values[i],umin,variancemin);
        //qDebug()<<"the "<<i<<"th dPone& dNone"<<dPOne[i]<<dNOne[i]<<endl;
    }


    //qDebug()<<"ok 2.1"<<endl;
    //20种可能的左边部分编码的p的指数部分的集合
    //dset0-9为left_hand_even编码,dset10-19为left_hand_odd
    double dset[10];
    for(int i=0;i<10;i++)
        dset[i] = 0;

    seperator[0] = -1;
    seperator[1] = 0;
    seperator[6] = 8;

    char result[7];
    double DPO = 0;
    double DNO = 0;

    for(int i=0;i<7;i++)
    {
        DPO= 0;
        DNO = 0;
        for(int j=o+i*w;j<o+(i+1)*w;j++)
        {
            DPO +=dPOne[j-start];
            DNO +=dNOne[j-start];
        }
        if(DPO<DNO)
            result[i] = '1';
        else
            result[i] = '0';
    }
    qDebug()<<result;

    //A[0] = A[5] =0;

    //qDebug()<<"ok 2.2"<<endl;
    for(int k=0;k<10;k++)
    {
        for(int i=1;i<6;i++)
        {
            seperator[i]=0;
            for(int j=0;j<i-1;j++)
                seperator[i] = seperator[i]+ LEFT_HAND_ALL[k+10][j];
            //qDebug()<<"seperator"<<i<<" = "<<seperator[i]<<"\t";
        }
        //以下计算A[1] - A[4],分别代表条形码的一个字符的4个信息

        for(int l=0;l<CODE_INFO[k+10].pieces;l++)
        {
            //o+do, w+dw
            double oPdo = o+CODE_INFO[k+10].one_point_o[l];
            double wPdw = w+CODE_INFO[k+10].one_point_w[l];
//            double oPdo = o;
//            double wPdw = w;
            for(int i=1;i<5;i++)
            {
                A[i]=0;
                if(i%2==0)//i=2,4,期望匹配+1
                {
                    for(int j=oPdo+seperator[i]*wPdw;j<oPdo+seperator[i+1]*wPdw;j++)
                    {
                        A[i] = A[i] + dPOne[j-start];
                    }
                }
                if(i%2==1)//i=1,3,期望匹配-1
                {
                    for(int j=oPdo+seperator[i]*wPdw;j<oPdo+seperator[i+1]*wPdw;j++)
                    {
                        A[i] = A[i] + dNOne[j-start];
                    }
                }
                //qDebug()<<"A"<<i<<" = "<<A[i]<<"\t";
                //qDebug()<<"A is sum of"<<o+seperator[i]*w-start<<"to"<<o+seperator[i+1]*w-start<<"\t";
                // dset[k] = sigma(A[i],i=1-4)
                A[i] = A[i]*CODE_INFO[k+10].percent[l];
                dset[k] = dset[k] + A[i];
            }
        }

    }

    //qDebug()<<"ok 2.4"<<endl;
    //minD记录最小的d值,minIndex记录这个d值在dset的位置
    int minIndex=0;
    double minD=dset[0];
    for(int i=1;i<10;i++)
    {
        if(dset[i]<minD)
        {
            minIndex = i;
            minD = dset[i];
        }
    }

//    for(int i=0;i<20;i++)
//    {
//        qDebug()<<"the "<<i<<"th dset"<<dset[i]<<endl;;
//    }
    //qDebug()<<"ok2end"<<endl;;
    return minIndex;
}
