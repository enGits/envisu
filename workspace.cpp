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

#include "workspace.h"
#include "inputdialog.h"
#include "items.h"
#include "mainwindow.h"

#include <iostream>
#include <qstatusbar.h>
#include <qcombobox.h>
#include <qmessagebox.h>

#include <QTextStream>

//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>

#include <vtkRenderWindow.h>
#include <vtkCamera.h>

WorkSpace::WorkSpace(QWidget *parent, const char *name, Qt::WFlags f) : Q3CanvasView(parent,name,f)
{
  image_path = ":/icons/icons/";
  tile = new QPixmap(image_path + "tile.png");
  ws_canvas.resize(2000,2000);
  ws_canvas.setBackgroundColor(QColor(200,200,200));
  ws_canvas.setBackgroundPixmap(*tile);
  setCanvas(&ws_canvas);
  drag_item    = NULL;
  connect_item = NULL;
  del_item     = NULL;
  setMouseTracking(true);
  viewport()->setMouseTracking(true);
  bin_pixmap = new Q3CanvasPixmapArray(imagePath() + "bin.png");
  bin_icon   = new Q3CanvasSprite(bin_pixmap,canvas());
  bin_icon->setX(30);
  bin_icon->setY(30);
  bin_icon->show();
  canvas()->update();
}

void WorkSpace::setMainWindow(MainWindow *mw)
{
  m_MainWindow = mw;
  m_Renderer = vtkRenderer::New();
  mw->getQVtkWidget()->GetRenderWindow()->AddRenderer(m_Renderer);
}

void WorkSpace::addItem(WsItem *new_item)
{
  items.push_back(new_item);
  new_item->getIcon()->setX(100);
  new_item->getIcon()->setY(30);
  new_item->getIcon()->setZ(0);
  new_item->getIcon()->show();
  canvas()->update();
}

void WorkSpace::addConnection(WsConnection *new_connection)
{
  connections.push_back(new_connection);
  canvas()->update();
}

void WorkSpace::delItem(WsItem *old_item)
{
  list<WsConnection*>::iterator i;
  bool finished;
  do {
    finished = true;
    for (i = connections.begin(); i != connections.end(); ++i) {
      if (((*i)->fromItem() == old_item) || ((*i)->toItem() == old_item)) {
        delete (*i);
        connections.erase(i);
        finished = false;
        break;
      }
    }
  } while (!finished);
  list<WsItem*>::iterator j;
  for (j = items.begin(); j != items.end(); ++j) {
    if ((*j) == old_item) {
      delete old_item;
      items.erase(j);
      break;
    };
  };
  canvas()->update();
}

void WorkSpace::delConnection(WsConnection *old_connection)
{
  list<WsConnection*>::iterator i;
  for (i = connections.begin(); i != connections.end(); ++i) {
    if ((*i) == old_connection) {
      connections.erase(i);
      delete (*i);
      break;
    }
  }
  canvas()->update();
}

void WorkSpace::mousePressEvent(QMouseEvent *e)
{
  WsItem *item = NULL;
  int x = e->x() + contentsX() + xcorr;
  int y = e->y() + contentsY() + ycorr;
  list<WsItem*>::iterator i;
  for (i = items.begin(); i != items.end(); ++i) {
    QRect r = (*i)->getIcon()->boundingRect();
    if (r.contains(x,y)) {
      item = *i;
      break;
    }
  }
  if (!item) return;
  if (e->button() == Qt::LeftButton) {
    drag_item = item;
    drag_item->getIcon()->setZ(10);
    drag_x0 = drag_item->getIcon()->x();
    drag_y0 = drag_item->getIcon()->y();
    drag_dx = drag_item->getIcon()->x() - double(e->x());//double(x - xcorr);
    drag_dy = drag_item->getIcon()->y() - double(e->y());//double(y - ycorr);
    //setStatusText(drag_item->name());
    canvas()->update();
  } else if (e->button() == Qt::MidButton) {
    if (item->hasOutput()) {
      connect_item = item;
      connect_line = new Q3CanvasLine(canvas());
      connect_line->setPoints(int(connect_item->getIcon()->x()) + iconw/2,
                              int(connect_item->getIcon()->y()) + iconh/2,
                              int(connect_item->getIcon()->x()) + iconw/2,
                              int(connect_item->getIcon()->y()) + iconh/2);
      connect_line->setPen(QPen(QColor(0,0,255),2,Qt::DotLine));
      connect_line->setZ(-20);
      connect_line->show();
      canvas()->update();
    };
  } else if (e->button() == Qt::RightButton) {
    del_item = item;
    connect_line = new Q3CanvasLine(canvas());
    connect_line->setPoints(int(del_item->getIcon()->x()) + iconw/2,
                            int(del_item->getIcon()->y()) + iconh/2,
                            int(del_item->getIcon()->x()) + iconw/2,
                            int(del_item->getIcon()->y()) + iconh/2);
    connect_line->setPen(QPen(QColor(255,0,0),3,Qt::SolidLine));
    connect_line->setZ(-20);
    connect_line->show();
    canvas()->update();
  }
}

void WorkSpace::mouseDoubleClickEvent(QMouseEvent *e)
{
  WsItem *item = NULL;
  int x = e->x() + contentsX() + xcorr;
  int y = e->y() + contentsY() + ycorr;
  list<WsItem*>::iterator i;
  for (i = items.begin(); i != items.end(); ++i) {
    QRect r = (*i)->getIcon()->boundingRect();
    if (r.contains(x,y)) {
      item = *i;
      break;
    }
  }
  if (!item) return;
  if (e->button() == Qt::LeftButton) {
    item->config();
  }
}

void WorkSpace::mouseMoveEvent(QMouseEvent *e)
{
  if (drag_item) {
    double nx = double(e->x())+drag_dx;
    double ny = double(e->y())+drag_dy;
    drag_item->getIcon()->move(nx,ny,0);
    list<WsConnection*>::iterator i;
    for (i = connections.begin(); i != connections.end(); ++i) {
      if      ((*i)->fromItem() == drag_item) (*i)->update();
      else if ((*i)->toItem()   == drag_item) (*i)->update();
    }
    canvas()->update();
  } else if (connect_item || del_item) {
    connect_line->setPoints(connect_line->startPoint().x(),
                            connect_line->startPoint().y(),
                            e->x() + contentsX() + xcorr,
                            e->y() + contentsY() + ycorr);
    canvas()->update();
  }
  {
    WsItem *item = NULL;
    int x = e->x() + contentsX() + xcorr;
    int y = e->y() + contentsY() + ycorr;
    list<WsItem*>::iterator i;
    for (i = items.begin(); i != items.end(); ++i) {
      QRect r = (*i)->getIcon()->boundingRect();
      if (r.contains(x,y)) {
        item = *i;
        break;
      }
    }
    if (item) setStatusText(item->name());
    else      clearStatusText();
  }
}

WsItem *WorkSpace::findItem(int x, int y)
{
  WsItem *item = NULL;
  x += contentsX() + xcorr;
  y += contentsY() + ycorr;
  list<WsItem*>::iterator i;
  for (i = items.begin(); i != items.end(); ++i) {
    QRect r = (*i)->getIcon()->boundingRect();
    if (r.contains(x,y)) {
      item = *i;
      break;
    }
  }
  return item;
}

void WorkSpace::error(QString txt)
{
  QMessageBox::warning (0, "warning", txt,
                        QMessageBox::Ok, QMessageBox::NoButton);
}

bool WorkSpace::checkTypes(QString t1, QString t2)
{
  if (t1 == t2) return true;
  if (t2 == "vtkPolyData/DataSet") {
    if (t1 == "vtkPolyData") return true;
    if (t1 == "vtkDataSet")  return true;
  }
  return false;
}

void WorkSpace::mouseReleaseEvent(QMouseEvent *e)
{
  if (drag_item) {
    drag_item->getIcon()->setZ(0);
    if (drag_item->getIcon()->collidesWith(bin_icon)) {
      delItem(drag_item);
    } else {
      list<WsItem*>::iterator i;
      bool valid = true;
      drag_item->getIcon()->setX(double(e->x()) + drag_dx);
      drag_item->getIcon()->setY(double(e->y()) + drag_dy);
      for (i = items.begin(); i != items.end(); ++i) {
        if ((*i) != drag_item) {
          if (drag_item->getIcon()->collidesWith((*i)->getIcon())) {
            valid = false;
            break;
          }
        }
      }
      if (!valid) {
        drag_item->getIcon()->setX(drag_x0);
        drag_item->getIcon()->setY(drag_y0);
      } else {
        double xx = drag_item->getIcon()->x()/10;
        double yy = drag_item->getIcon()->y()/10;
        drag_item->getIcon()->setX(round(xx)*10);
        drag_item->getIcon()->setY(round(yy)*10);
      }
      list<WsConnection*>::iterator j;
      for (j = connections.begin(); j != connections.end(); ++j) {
        if      ((*j)->fromItem() == drag_item) (*j)->update();
        else if ((*j)->toItem()   == drag_item) (*j)->update();
      }
    }
    drag_item = NULL;
  } else if (connect_item) {
    WsItem *item = findItem(e->x(),e->y());
    if (item) {
      bool found = false;
      list<WsConnection*>::iterator i;
      for (i = connections.begin(); i != connections.end(); ++i) {
        if (((*i)->fromItem() == connect_item) && ((*i)->toItem() == item)) {
          found = true;
          break;
        } else if (((*i)->fromItem() == item) && ((*i)->toItem() == connect_item)) {
          found = true;
          break;
        }
      }
      if (!found) {
        if (item->numInputs() == 0) {
          error("The item has no input slot");
        } else if (item->numInputs() == 1) {
          bool ok = true;
          for (i = connections.begin(); i != connections.end(); ++i) {
            if ((*i)->inputSlot() == 0) {
              if ((*i)->toItem() == item) {
                error("This slot is not free!");
                ok = false;
                break;
              }
            }
          }
          if (ok) {
            if (!checkTypes(connect_item->outputType(),item->inputType(0))) {
              error("The types do not match!");
              ok = false;
            }
            if (ok) new WsConnection(this,connect_item,item,0);
          }
        } else {
          InputDialog dlg(this);
          for (int j = 0; j < item->numInputs(); ++j) {
            QString pre = "+ ";
            for (i = connections.begin(); i != connections.end(); ++i) {
              if ((*i)->inputSlot() == j) {
                if ((*i)->toItem() == item) {
                  pre = "- ";
                  break;
                }
              }
            }
            dlg.addInputItem(pre + item->inputName(j) + "(" + item->inputType(j) + ")");
          }
          if (dlg.exec()) {
            bool ok = true;
            int islot = dlg.currentItem();
            for (i = connections.begin(); i != connections.end(); ++i) {
              if ((*i)->inputSlot() == islot) {
                if ((*i)->toItem() == item) {
                  error("This slot is not free!");
                  ok = false;
                  break;
                }
              }
            }
            if (ok) {
              if (!checkTypes(connect_item->outputType(),item->inputType(islot))) {
                error("The types do not match!");
                ok = false;
              }
              if (ok) new WsConnection(this,connect_item,item,islot);
            }
          }
        }
      } else {
        error("Only one connection is allowed between two items!");
      }
    }
    delete connect_line;
    connect_item = NULL;
  } else if (del_item) {
    list<WsConnection*>::iterator i;
    WsItem *item = findItem(e->x(),e->y());
    if (item) {
      bool deleted = false;
      for (i = connections.begin(); i != connections.end(); ++i) {
        if (((*i)->fromItem() == del_item) && ((*i)->toItem() == item)) {
          delConnection(*i);
          deleted = true;
          break;
        } else if (((*i)->fromItem() == item) && ((*i)->toItem() == del_item)) {
          delConnection(*i);
          deleted = true;
          break;
        }
      }
      if (!deleted) {
        error("There is no such connection!");
      }
    }
    delete connect_line;
    del_item = NULL;
  }
  canvas()->update();
}

void WorkSpace::clearStatusText()
{
  //m_MainWindow->clearStatusText();
}

void WorkSpace::setStatusText(QString text)
{
  //m_MainWindow->setStatusText(text);
}

void WorkSpace::render()
{
  m_MainWindow->renderVtk();
  list<WsItem*>::iterator i;
  for (i = items.begin(); i != items.end(); ++i) {
    (*i)->update();
  }
}

void WorkSpace::zoomAll()
{
  //m_Renderer->ResetCameraClippingRange();
  m_Renderer->ResetCamera();
  //m_Renderer->Print(cout);
  render();
}

void WorkSpace::save(QString file_name)
{
  QFile file(file_name);
  if (!file.open(QIODevice::WriteOnly)) {
    return;
  }
  QTextStream f(&file);

  map<WsItem*,int> save_items;
  f << items.size() << '\n';
  {
    list<WsItem*>::iterator i;
    int j = 0;
    for (i = items.begin(); i != items.end(); ++i) {
      save_items[*i] = j;
      if      (dynamic_cast<VtkDisplay*>(*i))    		      f << "VtkDisplay\n";
      else if (dynamic_cast<Cutter*>(*i))     		        f << "Cutter\n";
      else if (dynamic_cast<Plane*>(*i))      		        f << "Plane\n";
      else if (dynamic_cast<Cutter*>(*i))         	      f << "Cutter\n";
      else if (dynamic_cast<Cell2Node*>(*i))    	        f << "Cell2Node\n";
      else if (dynamic_cast<Geometry*>(*i))        	      f << "Geometry\n";
      else if (dynamic_cast<Acummulate*>(*i))         	  f << "Cummulate\n";
      else if (dynamic_cast<Tubes*>(*i))              	  f << "Tubes\n";
      else if (dynamic_cast<Sphere*>(*i))                 f << "Sphere\n";
      else if (dynamic_cast<Clipper*>(*i))         	      f << "Clipper\n";
      else if (dynamic_cast<AsciiPointsFile*>(*i))        f << "AsciiPointsFile\n";
      else if (dynamic_cast<PolyDataReader*>(*i))         f << "PolyDataReader\n";
      else if (dynamic_cast<UnstructuredGridReader*>(*i)) f << "UnstructuredGridReader\n";
      else if (dynamic_cast<MultiBlockReader*>(*i))       f << "MultiBlockReader\n";
      else if (dynamic_cast<Contour*>(*i))                f << "Contour\n";
      else if (dynamic_cast<Cylinder*>(*i))               f << "Cylinder\n";
      //else if (dynamic_cast<*>(*i))      f << "\n";
      else {
        QString txt = "unknown item to save (";
        txt += typeid(*(*i)).name();
        txt += ")";
        error(txt);
      }
      (*i)->save(f);
      ++j;
    }
  }
  f << connections.size() << '\n';
  {
    list<WsConnection*>::iterator i;
    int j = 0;
    for (i = connections.begin(); i != connections.end(); ++i) {
      f << save_items[(*i)->fromItem()] << ' ';
      f << save_items[(*i)->toItem()] << ' ';
      f << (*i)->inputSlot() << '\n';
    }
  }
}

void WorkSpace::load(QString file_name)
{
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }
  QTextStream f(&file);
  int N;
  f >> N;
  vector<WsItem*> load_items(N);
  for (int i = 0; i < N; ++i) {
    QString type;
    f >> type;
    if      (type == "VtkDisplay")    	       load_items[i] = new VtkDisplay(this);
    else if (type == "Cutter")     	           load_items[i] = new Cutter(this);
    else if (type == "Plane")      	           load_items[i] = new Plane(this);
    else if (type == "Cell2Node")  	           load_items[i] = new Cell2Node(this);
    else if (type == "Geometry")   	           load_items[i] = new Geometry(this);
    else if (type == "Acummulate")  	         load_items[i] = new Acummulate(this);
    else if (type == "Tubes")           	     load_items[i] = new Tubes(this);
    else if (type == "Sphere")     	           load_items[i] = new Sphere(this);
    else if (type == "Clipper")    	           load_items[i] = new Clipper(this);
    else if (type == "AsciiPointsFile")        load_items[i] = new AsciiPointsFile(this);
    else if (type == "PolyDataReader")         load_items[i] = new PolyDataReader(this);
    else if (type == "UnstructuredGridReader") load_items[i] = new UnstructuredGridReader(this);
    else if (type == "MultiBlockReader")       load_items[i] = new MultiBlockReader(this);
    else if (type == "Contour")                load_items[i] = new Contour(this);
    else if (type == "Cylinder")               load_items[i] = new Cylinder(this);
    //else if (type == "Mp")      load_items[i] = new Mp(this);
    else {
      QString txt = "unknown item to load (" + type + ")";
      error(txt);
      load_items[i] = NULL;
    }
    if (load_items[i]) load_items[i]->load(f);
  }
  f >> N;
  for (int i = 0; i < N; ++i) {
    int islot,from,to;
    f >> from >> to >> islot;
    if (load_items[from] && load_items[to]) {
      new WsConnection(this,load_items[from],load_items[to],islot);
    }
  }
}

void WorkSpace::setView(double x1, double y1, double z1,
                        double x2, double y2, double z2)
{
  vtkCamera *cam = getRenderer()->GetActiveCamera();
  cam->SetFocalPoint(0,0,0);
  cam->SetPosition(x1,y1,z1);
  cam->SetViewUp(x2,y2,z2);
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewLeft()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[0] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,1);
  } else {
    x[0] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,1,0);
  }
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewRight()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[0] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,1);
  } else {
    x[0] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,1,0);
  }
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewTop()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[2] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,1,0);
  } else {
    x[1] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,-1);
  }
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewBottom()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[2] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,-1,0);
  } else {
    x[1] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,-1);
  }
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewFront()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[1] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,1);
  } else {
    x[2] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,1,0);
  }
  getRenderer()->ResetCamera();
  render();
}

void WorkSpace::viewBack()
{
  bool use_blender = true;
  //getSet("General","use Blender definition for front, top, etc.", true, use_blender);
  getRenderer()->ResetCamera();
  double x[3];
  getRenderer()->GetActiveCamera()->GetFocalPoint(x);
  if (use_blender) {
    x[1] += 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,0,1);
  } else {
    x[2] -= 1;
    getRenderer()->GetActiveCamera()->SetPosition(x);
    getRenderer()->GetActiveCamera()->ComputeViewPlaneNormal();
    getRenderer()->GetActiveCamera()->SetViewUp(0,1,0);
  }
  getRenderer()->ResetCamera();
  render();
}







