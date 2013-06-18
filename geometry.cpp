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

#include "geometry.h"

Geometry::Geometry(WorkSpace *ws) :	WsItem(ws)
{
  loadIcon("geometry.png");
  m_Ws->addItem(this);
  setName("surface");
  geom = vtkGeometryFilter::New();
  addInput("data", "vtkPolyData/DataSet");
}

Geometry::~Geometry()
{
  geom->Delete();
}

void Geometry::load(QTextStream &s)
{
  WsItem::load(s);
}

void Geometry::save(QTextStream &s)
{
  WsItem::save(s);
}

