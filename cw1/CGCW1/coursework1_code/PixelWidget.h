#ifndef PIXELWIDGET_H
#define PIXELWIDGET_H

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDesktopWidget>
#include <iostream>
#include <vector>
#include "RGBVal.h"


class PixelWidget : public QWidget {
public:


  // set the number of pixels that the widget is meant to display
  PixelWidget
  (
   unsigned int n_horizontal, // the first integer determines the number of horizontal pixels
   unsigned int n_vertical    // the second integer determines the number of vertical pixels
   );

  // sets a pixel at the specified RGBVal value; ignores non-existing pixels without warning
  void SetPixel
  (
   unsigned int  i_x, // horizontal pixel coordinate
   unsigned int  i_y, // vertical pixel coordinate
   const RGBVal& c    // RBGVal object for RGB values
    );

  // Use the body of this function to experiment with rendering algorithms
  void DefinePixelValues(float x, float y, int r, int g, int b);
  void DrawLine(float x0, float y0, float x1, float y1,RGBVal rgb_1, RGBVal rgb_2);
  void Barycentric(float x0, float y0, float x1, float y1, float x2, float y2, RGBVal rgb);
  int hpt(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
  bool IsInside(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
  void bc(float x1,float y1,float x2,float y2,float x3,float y3);
  void c_ppm(float x1,float y1,float x2,float y2,float x3,float y3,RGBVal rgb);
protected:

  virtual void paintEvent(QPaintEvent*);


private:

  unsigned int _n_vertical;
  unsigned int _n_horizontal;

  std::vector<std::vector<RGBVal> > _vec_rects;
};




#endif // PIXELWIDGET_H
