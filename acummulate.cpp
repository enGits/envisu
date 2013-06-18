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

#include "acummulate.h"

Acummulate::Acummulate(WorkSpace *ws) : GuiWsItem<Ui::WsItemConfig>(ws)
{
  loadIcon("cummulate.png");
  m_Ws->addItem(this);
  setName("cummulate");
  m_OutputTypes = "vtkPolyData";
  cummulate = vtkAppendPolyData::New();
  m_Inputs.resize(N,NULL);
  addInput("data-01", "vtkPolyData");
  addInput("data-02", "vtkPolyData");
  addInput("data-03", "vtkPolyData");
  addInput("data-04", "vtkPolyData");
  addInput("data-05", "vtkPolyData");
  addInput("data-06", "vtkPolyData");
  addInput("data-07", "vtkPolyData");
  addInput("data-08", "vtkPolyData");
  addInput("data-09", "vtkPolyData");
  addInput("data-10", "vtkPolyData");
}

Acummulate::~Acummulate()
{
  cummulate->Delete();
}

void Acummulate::setPolyData(int i, vtkPolyData *data)
{
  m_Inputs[i] = data;
  cummulate->AddInput(data);
}

void Acummulate::setDummyPolyData(int i)
{
  cummulate->RemoveInput(m_Inputs[i]);
  m_Inputs[i] = NULL;
}

void Acummulate::load(QTextStream &s)
{
  WsItem::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
}

void Acummulate::save(QTextStream &s)
{
  WsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
}

void Acummulate::disconnectInput(int i)
{
  setDummyPolyData(i);
}


