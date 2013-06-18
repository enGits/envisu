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

#include "contour.h"

Contour::Contour(WorkSpace *ws) : GuiWsItem<Ui::ContourConfig>(ws)
{
  loadIcon("contour.png");
  m_Ws->addItem(this);
  setName("Contour");
  vtk = vtkContourFilter::New();
  addInput("data","vtkPolyData/DataSet");
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.help_pb,SIGNAL(clicked()),this,SLOT(help()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkPolyData";
  apply();
}

Contour::~Contour()
{
  vtk->Delete();
}

void Contour::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  float min = m_Dlg.ui.min_edit->text().toFloat();
  float max = m_Dlg.ui.max_edit->text().toFloat();
  int   num = m_Dlg.ui.num_edit->text().toInt();
  vtk->GenerateValues(num,min,max);
  m_Ws->render();
}

void Contour::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Contour::save(QTextStream &s)
{
  GuiWsItem<Ui::ContourConfig>::save(s);
  writeLineEdit(s,m_Dlg.ui.num_edit);
  writeLineEdit(s,m_Dlg.ui.min_edit);
  writeLineEdit(s,m_Dlg.ui.max_edit);
}

void Contour::load(QTextStream &s)
{
  GuiWsItem<Ui::ContourConfig>::load(s);
  readLineEdit(s,m_Dlg.ui.num_edit);
  readLineEdit(s,m_Dlg.ui.min_edit);
  readLineEdit(s,m_Dlg.ui.max_edit);
  apply();
}

vtkPolyData* Contour::getPolyData()
{
  return vtk->GetOutput();
}

void Contour::setPolyData(int i, vtkPolyData *poly_data)
{
  vtk->SetInput(poly_data);
}

void Contour::setDataSet(int i, vtkDataSet *data_set)
{
  vtk->SetInput(data_set);
}

void Contour::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}

void Contour::connectInput(int i)
{
}

void Contour::disconnectInput(int i)
{
}

void Contour::connectOutput(int i)
{
}

void Contour::disconnectOutput(int i)
{
}

void Contour::help()
{
}

