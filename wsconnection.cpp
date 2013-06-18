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

#include "wsconnection.h"

WsConnection::WsConnection(WorkSpace *ws, WsItem *item1, WsItem *item2, int islot)
{
  m_Ws = ws;
  from = item1;
  to = item2;
  line = new Q3CanvasLine(ws->canvas());
  line->setZ(-20);
  line->setPen(QPen(QColor(0,0,255),2,Qt::SolidLine));
  arr_line1 = new Q3CanvasLine(ws->canvas());
  arr_line1->setZ(-20);
  arr_line1->setPen(QPen(QColor(0,0,255),2,Qt::SolidLine));
  arr_line2 = new Q3CanvasLine(ws->canvas());
  arr_line2->setZ(-20);
  arr_line2->setPen(QPen(QColor(0,0,255),2,Qt::SolidLine));
  update();
  line->show();
  arr_line1->show();
  arr_line2->show();
  input_slot = islot;
  if (to->inputType(islot) == "vtkPolyData/DataSet") {
    if (from->outputType() == "vtkPolyData") {
      to->setPolyData(islot,from->getPolyData());
    } else if (from->outputType() == "vtkDataSet") {
      to->setDataSet(islot,from->getDataSet());
    };
  } else if (to->inputType(islot) == "vtkDataSet") {
    to->setDataSet(islot,from->getDataSet());
  } else if (to->inputType(islot) == "vtkPolyData") {
    to->setPolyData(islot,from->getPolyData());
  } else if (to->inputType(islot) == "vtkImplicitFunction") {
    to->setImplicitFunction(islot,from->getImplicitFunction());
  };
  from->connectOutput();
  to->connectInput(islot);
  m_Ws->addConnection(this);
  //ws->render();
}

WsConnection::~WsConnection()
{
  if (to->inputType(input_slot) == "vtkPolyData") {
    to->setDummyPolyData(input_slot);
  } else if (to->inputType(input_slot) == "vtkDataSet") {
    to->setDummyDataSet(input_slot);
  } else if (to->inputType(input_slot) == "vtkPolyData/vtkDataSet") {
    to->setDummyPolyData(input_slot);
    to->setDummyDataSet(input_slot);
  };
  from->disconnectOutput();
  to->disconnectInput(input_slot);
  delete line;
  delete arr_line1;
  delete arr_line2;
}

void WsConnection::update()
{
  int x1 = int(from->getIcon()->x()) + WorkSpace::iconw/2;
  int x2 = int(to->getIcon()->x())   + WorkSpace::iconw/2;
  int y1 = int(from->getIcon()->y()) + WorkSpace::iconh/2;
  int y2 = int(to->getIcon()->y())   + WorkSpace::iconh/2;
  line->setPoints(x1,y1,x2,y2);
  int xm = (x1+x2)/2;
  int ym = (y1+y2)/2;
  double l = sqrt(double(x2-x1)*double(x2-x1) + double(y2-y1)*double(y2-y1));
  int dx = int((double(x2-x1)/l)*10);
  int dy = int((double(y2-y1)/l)*10);
  int x3 = xm+dx;
  int y3 = ym+dy;
  int x4 = xm+dy;
  int y4 = ym-dx;
  int x5 = xm-dy;
  int y5 = ym+dx;
  arr_line1->setPoints(x3,y3,x4,y4);
  arr_line2->setPoints(x3,y3,x5,y5);
}


