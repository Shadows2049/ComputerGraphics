#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include<QDesktopWidget>
#include <iostream>
#include <fstream>
#include "PixelWidget.h"
#include <string>
#include <iomanip>
#include <stdlib.h>
using namespace std;


void PixelWidget::DefinePixelValues(float x, float y, int r, int g, int b){ //add pixels here; write methods like this for the assignments
  SetPixel(x,y,RGBVal(r,g,b));
}

void PixelWidget::DrawLine(float a, float b, float c, float d, RGBVal rgb_1, RGBVal rgb_2){
    float x0 = a, y0 = b, x1 = c, y1 = d;
    float x2 = 0, y2 = 0;
    int r2, g2, b2 = 0;
    float dr, dg, db = 0;
    float r3, g3, b3 = 0;
    r3 = rgb_1._red;
    g3 = rgb_1._green;
    b3 = rgb_1._blue;


    r2 = rgb_2._red - rgb_1._red;
    g2 = rgb_2._green - rgb_1._green;
    b2 = rgb_2._blue - rgb_1._blue;


    double l = floor(sqrt(pow(abs(int(x0-x1)),2)+pow(abs(int(y0-y1)),2)));
    double s = 1/(l*4);

    dr = r2 * s;
    dg = g2 * s;
    db = b2 * s;

   for(double t = 0; t<=1; t=t+s){

        x2 = (1-t)*x0 + t*x1;
        y2 = (1-t)*y0 + t*y1;

            r3 += dr;
            g3 += dg;
            b3 += db;
            DefinePixelValues(x2,y2,r3,g3,b3);

    }

}

void PixelWidget::Barycentric(float x0, float y0, float x1, float y1, float x2, float y2, RGBVal rgb){
    float w1,w2,w3,px,py;
    for(px=0; px<=69; px++){
        for(py=0; py<=69; py++){
            w1 = ((y0 - y1)*px+(x1-x0)*py+x0*y1-x1*y0)/((y0-y1)*x2+(x1-x0)*y2+x0*y1-x1*y0);
            w2 = ((y0-y2)*px+(x2-x0)*py+x0*y2-x2*y0)/((y0-y2)*x1+(x2-x0)*y1+x0*y2-x2*y0);
            w3 = 1-w1-w2;
            if(w1>=0&&w1<=1&&w2>=0&&w2<=1&&w3>=0&&w3<=1){
                DefinePixelValues(px,py,rgb._red, rgb._green, rgb._blue);
           }
        }
    }

}

int PixelWidget::hpt(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
    if (x1 == x2){
        float check = (x3 - x1) * (x4 - x2);
        if(check >= 0){
            return 1;
        }
        return 0;
    }

    float k,b = 0;
    float r1, r2 = 0;
    k = (y1 - y2) / (x1 - x2);
    b = y2 - k * x2;
    r1 = y3 - k * x3 - b;
    r2 = y4 - k * x4 - b;
    if(r1 * r2 >= 0){
        return 1;
    }
    return 0;

}

bool PixelWidget::IsInside(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
    int r1, r2, r3 = 0;
    r1 = hpt(x1, y1, x2, y2, x3, y3, x4, y4);
    r2 = hpt(x2, y2, x3, y3, x1, y1, x4, y4);
    r3 = hpt(x3, y3, x1, y1, x2, y2, x4, y4);
    int final_result = r1 + r2 + r3;
    if(final_result == 3){
        return true;
    }
    return false;
}

void PixelWidget::bc(float x0, float y0, float x1, float y1, float x2, float y2){
    ofstream OutFile("result.csv");
    OutFile<<"w1"<<",";
    OutFile<<"w2"<<",";
    OutFile<<"w3"<<",";
    OutFile<<"result_barycentric"<<",";
    OutFile<<"result_half_plain_test"<<",";
    OutFile<<"same"<<",";
    OutFile<<'\n';

    int x, y =0;
    float w1, w2, w3 = 0;
   //float a, b, c, d, e, f, g, h = 0;
    bool rhpt, rbc, result = false;

    for(x = 0; x <= 69; x++){
        for(y = 0; y <= 69; y++ ){
            w1 = ((y0 - y1)*x+(x1-x0)*y+x0*y1-x1*y0)/((y0-y1)*x2+(x1-x0)*y2+x0*y1-x1*y0);
            w2 = ((y0-y2)*x+(x2-x0)*y+x0*y2-x2*y0)/((y0-y2)*x1+(x2-x0)*y1+x0*y2-x2*y0);
            w3 = 1-w1-w2;
            if(0<=w1 && w1 <=1 && 0<=w2 && w2<=1 && 0<=w3 && w3<=1){
                rbc = true;
            }
            else{
                rbc = false;
            }
            rhpt = IsInside(x0, y0, x1, y1, x2, y2, x, y);

            if (rbc == rhpt){
                    result = true;
            }
            else{
                result = false;
            }
            OutFile<<setiosflags(ios::fixed)<<setprecision(2)<<w1<<",";
            OutFile<<setiosflags(ios::fixed)<<setprecision(2)<<w2<<",";
            OutFile<<setiosflags(ios::fixed)<<setprecision(2)<<w3<<",";

            if(rbc == 1){
                OutFile<<"True"<<",";
            }
            else{
                OutFile<<"False"<<",";
            }
            if(rhpt == 1){
                OutFile<<"True"<<",";
            }
            else{
                OutFile<<"False"<<",";
            }
            if(result == 1){
                OutFile<<"True"<<",";
            }
            else{
                OutFile<<"False"<<",";
            }
            OutFile<<'\n';

        }
    }
    OutFile.close();



}

void PixelWidget::c_ppm(float x1,float y1,float x2,float y2,float x3,float y3,RGBVal rgb)
{
    ofstream OutFile("image.ppm");
    OutFile<<"P3"<<"\n"<<"70 70"<<"\n"<<"255"<<"\n";
    for(int x = 0;x<=69;x++){
        for(int y =0;y<=69;y++){

          if(IsInside(x1,y1,x2,y2,x3,y3,y,x) == 1)
          {
            OutFile<<rgb._red<<" "<<rgb._green<<" "<<rgb._blue<<"\n";
          }
          else{
              OutFile<<"0 0 0"<<"\n";
          }
        }
    }
    OutFile.close();
}




// -----------------Most code below can remain untouched -------------------------
// ------but look at where DefinePixelValues is called in paintEvent--------------

PixelWidget::PixelWidget(unsigned int n_vertical, unsigned int n_horizontal):
  _n_vertical   (n_vertical),
  _n_horizontal (n_horizontal),
  _vec_rects(0)
{
  // all pixels are initialized to black
     for (unsigned int i_col = 0; i_col < n_vertical; i_col++)
       _vec_rects.push_back(std::vector<RGBVal>(n_horizontal));
}



void PixelWidget::SetPixel(unsigned int i_column, unsigned int i_row, const RGBVal& rgb)
{

  // if the pixel exists, set it
  if (i_column < _n_vertical && i_row < _n_horizontal)
    _vec_rects[i_column][i_row] = rgb;
}


void PixelWidget::paintEvent( QPaintEvent * )
{

  QPainter p( this );
  // no antialiasing, want thin lines between the cell
  p.setRenderHint( QPainter::Antialiasing, false );

  // set window/viewport so that the size fits the screen, within reason
  p.setWindow(QRect(-1.,-1.,_n_vertical+2,_n_horizontal+2));
  int side = qMin(width(), height());  
  p.setViewport(0, 0, side, side);

  // black thin boundary around the cells
  QPen pen( Qt::black );
  pen.setWidth(0.);

  // here the pixel values defined by the user are set in the pixel array
    //DefinePixelValues(0.5,0.5,100,100,100);
  //DrawLine(14,64,40,24,RGBVal(0, 0, 255),RGBVal(0, 255, 0));
  Barycentric(5,65,60,65,30,5, RGBVal(0, 255, 150));
    //bc(5,65,60,65,30,5);
    //c_ppm(5,65,60,65,30,5, RGBVal(0, 255, 150));
  for (unsigned int i_column = 0 ; i_column < _n_vertical; i_column++)
    for(unsigned int i_row = 0; i_row < _n_horizontal; i_row++){
      QRect rect(i_column,i_row,1,1);
      QColor c = QColor(_vec_rects[i_column][i_row]._red, _vec_rects[i_column][i_row]._green, _vec_rects[i_column][i_row]._blue);
    
      // fill with color for the pixel
      p.fillRect(rect, QBrush(c));
      p.setPen(pen);
      p.drawRect(rect);
    }
}
