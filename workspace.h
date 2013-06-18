//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +                                                                      +
// + This file is part of enVisu.                                         +
// +                                                                      +
// + Copyright 2013 O. Gloth, enGits GmbH                                 +
// +                                                                      +
// + enGrid is free software: you can redistribute it and/or modify       +
// + it under the terms of the GNU General Public License as published by +
// + the Free Software Foundation, either version 3 of the License, or    +
// + (at your option) any later version.                                  +
// +                                                                      +
// + enGrid is distributed in the hope that it will be useful,            +
// + but WITHOUT ANY WARRANTY; without even the implied warranty of       +
// + MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        +
// + GNU General Public License for more details.                         +
// +                                                                      +
// + You should have received a copy of the GNU General Public License    +
// + along with enVisu. If not, see <http://www.gnu.org/licenses/>.       +
// +                                                                      +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

#ifndef WORKSPACE_H
#define WORKSPACE_H

//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>

class WorkSpace;
class MainWindow;

#include <q3canvas.h>
#include <qlabel.h>
#include <qapplication.h>

#include <list>

#include "wsitem.h"
#include "wsconnection.h"

//#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace std;

class WorkSpace : public Q3CanvasView
{

  Q_OBJECT

  vtkRenderer* m_Renderer;
  MainWindow*  m_MainWindow;

  Q3Canvas               ws_canvas;
  QPixmap               *tile;
  list<WsItem*>          items;
  list<WsConnection*>    connections;
  WsItem                *drag_item;
  WsItem                *connect_item;
  WsItem                *del_item;
  double                 drag_dx;
  double                 drag_dy;
  double                 drag_x0;
  double                 drag_y0;
  Q3CanvasLine           *connect_line;
  Q3CanvasSprite         *bin_icon;
  Q3CanvasPixmapArray    *bin_pixmap;
  QString                image_path;

protected:

  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseDoubleClickEvent(QMouseEvent *e);
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);

  void clearStatusText();
  void setStatusText(QString text);

  bool checkTypes(QString t1, QString t2);

public:

  static const int xcorr = -2;
  static const int ycorr = -2;
  static const int iconw = 60;
  static const int iconh = 60;

  WorkSpace(QWidget *parent=0, const char *name=0, Qt::WFlags f=0);

  void setMainWindow (MainWindow *mw);
  void addItem       (WsItem *new_item);
  void addConnection (WsConnection *new_connection);
  void delItem       (WsItem *old_item);
  void delConnection (WsConnection *old_connection);

  static void error (QString txt);

  vtkRenderer* getRenderer() { return m_Renderer; }


  WsItem*          findItem      (int x, int y);
  list<WsItem*>*   getItems      () { return &items; }
  list<WsConnection*>* getConnections() { return &connections; }
  QString              imagePath     () { return image_path; }
  void   	             setView       (double x1, double y1, double z1, double x2, double y2, double z2);


public slots:

  virtual void render();
  virtual void zoomAll();
  virtual void load(QString file_name);
  virtual void save(QString file_name);
  virtual void showBin() { bin_icon->show(); }
  virtual void hideBin() { bin_icon->hide(); }

  void viewLeft();
  void viewRight();
  void viewTop();
  void viewBottom();
  void viewFront();
  void viewBack();

};	


#endif
