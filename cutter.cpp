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

#include "cutter.h"

Cutter::Cutter(WorkSpace *ws) : GuiWsItem<Ui::WsItemConfig>(ws)
{
  loadIcon("cutter.png");
  m_Ws->addItem(this);
  setName("Cutter");
  vtk = vtkCutter::New();
  addInput("cut function","vtkImplicitFunction");
  addInput("data","vtkPolyData/DataSet");
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  m_Dlg.ui.name_edit->setText(name());
  apply();
}

Cutter::~Cutter()
{
  vtk->Delete();
}

void Cutter::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
}

void Cutter::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Cutter::save(QTextStream &s)
{
  WsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
}

void Cutter::load(QTextStream &s)
{
  WsItem::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
}

vtkPolyData* Cutter::getPolyData()
{
  return vtk->GetOutput();
}

void Cutter::setPolyData(int i, vtkPolyData *poly_data)
{
  vtk->SetInput(poly_data);
}

void Cutter::setDataSet(int i, vtkDataSet *data_set)
{
  vtk->SetInput(data_set);
}

void Cutter::setImplicitFunction(int i, vtkImplicitFunction *function)
{
  vtk->SetCutFunction(function);
}

