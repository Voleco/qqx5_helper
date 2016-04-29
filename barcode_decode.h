#ifndef BARCODE_DECODE_H
#define BARCODE_DECODE_H

#include<QImage>
#include<QtCore>
#include<QFile>
#include<QTextStream>

#define FILE_NAME  "data.txt"

const int MAX_PIECES = 255;
/*
  left_odd
  0 0001101
  1 0011001
  2 0010011
  3 0111101
  4 0100011
  5 0110001
  6 0101111
  7 0111011
  8 0110111
  9 0001011

  */

const int LEFT_HAND_ODD_0[] = {3,2,1,1};
const int LEFT_HAND_ODD_1[] = {2,2,2,1};
const int LEFT_HAND_ODD_2[] = {2,1,2,2};
const int LEFT_HAND_ODD_3[] = {1,4,1,1};
const int LEFT_HAND_ODD_4[] = {1,1,3,2};
const int LEFT_HAND_ODD_5[] = {1,2,3,1};
const int LEFT_HAND_ODD_6[] = {1,1,1,4};
const int LEFT_HAND_ODD_7[] = {1,3,1,2};
const int LEFT_HAND_ODD_8[] = {1,2,1,3};
const int LEFT_HAND_ODD_9[] = {3,1,1,2};

const int LEFT_HAND_ODD_ALL[10][4] = {{3,2,1,1},
                                   {2,2,2,1},
                                   {2,1,2,2},
                                   {1,4,1,1},
                                   {1,1,3,2},
                                   {1,2,3,1},
                                   {1,1,1,4},
                                   {1,3,1,2},
                                   {1,2,1,3},
                                   {3,1,1,2}};
/*
  left_even
  0 0100111
  1 0110011
  2 0011011
  3 0100001
  4 0011101
  5 0111001
  6 0000101
  7 0010001
  8 0001001
  9 0010111

  */

const int LEFT_HAND_EVEN_0[] = {1,1,2,3};
const int LEFT_HAND_EVEN_1[] = {1,2,2,2};
const int LEFT_HAND_EVEN_2[] = {2,2,1,2};
const int LEFT_HAND_EVEN_3[] = {1,1,4,1};
const int LEFT_HAND_EVEN_4[] = {2,3,1,1};
const int LEFT_HAND_EVEN_5[] = {1,3,2,1};
const int LEFT_HAND_EVEN_6[] = {4,1,1,1};
const int LEFT_HAND_EVEN_7[] = {2,1,3,1};
const int LEFT_HAND_EVEN_8[] = {3,1,2,1};
const int LEFT_HAND_EVEN_9[] = {2,1,1,3};

const int LEFT_HAND_EVEN_ALL[10][4] = {{1,1,2,3},
                                       {1,2,2,2},
                                       {2,2,1,2},
                                       {1,1,4,1},
                                       {2,3,1,1},
                                       {1,3,2,1},
                                       {4,1,1,1},
                                       {2,1,3,1},
                                       {3,1,2,1},
                                       {2,1,1,3}};

const int LEFT_HAND_ALL[20][4] = {{1,1,2,3},
                                  {1,2,2,2},
                                  {2,2,1,2},
                                  {1,1,4,1},
                                  {2,3,1,1},
                                  {1,3,2,1},
                                  {4,1,1,1},
                                  {2,1,3,1},
                                  {3,1,2,1},
                                  {2,1,1,3},
                                  {3,2,1,1},
                                  {2,2,2,1},
                                  {2,1,2,2},
                                  {1,4,1,1},
                                  {1,1,3,2},
                                  {1,2,3,1},
                                  {1,1,1,4},
                                  {1,3,1,2},
                                  {1,2,1,3},
                                  {3,1,1,2}};

/*
  right
  0 1110010
  1 1100110
  2 1101100
  3 1000010
  4 1011100
  5 1001110
  6 1010000
  7 1000100
  8 1001000
  9 1110100

  */

const int RIGHT_HAND_0[] = {3,2,1,1};
const int RIGHT_HAND_1[] = {2,2,2,1};
const int RIGHT_HAND_2[] = {2,1,2,2};
const int RIGHT_HAND_3[] = {1,4,1,1};
const int RIGHT_HAND_4[] = {1,1,3,2};
const int RIGHT_HAND_5[] = {1,2,3,1};
const int RIGHT_HAND_6[] = {1,1,1,4};
const int RIGHT_HAND_7[] = {1,3,1,2};
const int RIGHT_HAND_8[] = {1,2,1,3};
const int RIGHT_HAND_9[] = {3,1,1,2};

const int RIGHT_HAND_ALL[10][4] = {{3,2,1,1},
                                {2,2,2,1},
                                {2,1,2,2},
                                {1,4,1,1},
                                {1,1,3,2},
                                {1,2,3,1},
                                {1,1,1,4},
                                {1,3,1,2},
                                {1,2,1,3},
                                {3,1,1,2}};

typedef struct
{
    int pieces;//图的块数
    double *percent;//pieces大小的数组,存储每一块占整体的比例
    double *one_point_o;//pieces大小的数组,存储每一块中的一个点的o坐标值
    double *one_point_w;//pieces大小的数组,存储每一块中的一个点的w坐标值
}partition_info;

extern partition_info CODE_INFO[20];
//0-9存储 left_hand_even 的info，10-19存储 left_hand_odd的info,right_hand的info与left_hand_odd相同
//extern partition_info CODE_INFO[20];

//void readInfoFromFile();
//CODE_INFO[0].pieces = 44;
//CODE_INFO[0].percent = {};



int Min(int x,int y);
int max(int x,int y);

int findmin(int *A,int low,int high);
int findmin(std::vector<int> A, int low, int high);

//ostu 算法求阈值,针对一个数组，数组存储的为点的灰度值
int ostuForArray(int value[],int size);

void getMeanAndVariance(const QImage* src,int scanline_y,double o,double w,int* values,double &umax,double &umin,double &variancemax,double &variancemin);

// D(I(n),1)
double DPositiveOne(int in,const double& umax,const double& variancemax);

// D(I(n),-1)
double DNegtiveOne(int in,const double& umin,const double& variancemin);

// 用于左半边解码的D(I,M)
int DForLeftPart(double o,double w,int* values,const double &umax,const double &umin,const double &variancemax,const double &variancemin);

int DForRightPart(double o,double w,int* values,const double &umax,const double &umin,const double &variancemax,const double &variancemin);


#endif // BARCODE_DECODE_H
