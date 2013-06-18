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

#include "axisymmetry.h"

Axisymmetry::Axisymmetry(WorkSpace *ws) : GuiWsItem<Ui::AxisymmetryConfig>(ws)
{
  loadIcon("axisymmetry.png");
  m_Ws->addItem(this);
  setName("Axisymmetry");
  //vtk = vtkRotationFilter::New();
  //vtk->SetAngle(2.0*180/M_PI);
  //vtk->SetNumberOfCopies(10);
  addInput("data","vtkDataSet");
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.help_pb,SIGNAL(clicked()),this,SLOT(help()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkDataSet";
  apply();
}

Axisymmetry::~Axisymmetry()
{
  //vtk->Delete();
}

void Axisymmetry::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  //vtk->SetAngle(m_Dlg.ui.angle_edit->text().toDouble()*180/M_PI);
  //vtk->SetNumberOfCopies(m_Dlg.ui.steps_edit->text().toInt());
  m_Ws->render();
}

void Axisymmetry::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Axisymmetry::save(QTextStream &s)
{
  GuiWsItem<Ui::AxisymmetryConfig>::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
  writeRadioButton(s,m_Dlg.ui.xaxis_rb);
  writeRadioButton(s,m_Dlg.ui.yaxis_rb);
  writeRadioButton(s,m_Dlg.ui.zaxis_rb);
  writeLineEdit(s,m_Dlg.ui.angle_edit);
  writeLineEdit(s,m_Dlg.ui.steps_edit);
}

void Axisymmetry::load(QTextStream &s)
{
  GuiWsItem<Ui::AxisymmetryConfig>::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
  readRadioButton(s,m_Dlg.ui.xaxis_rb);
  readRadioButton(s,m_Dlg.ui.yaxis_rb);
  readRadioButton(s,m_Dlg.ui.zaxis_rb);
  readLineEdit(s,m_Dlg.ui.angle_edit);
  readLineEdit(s,m_Dlg.ui.steps_edit);
  apply();
}

vtkDataSet* Axisymmetry::getDataSet()
{
  //return vtk->GetOutput();
}

void Axisymmetry::setPolyData(int i, vtkPolyData *poly_data)
{
}

void Axisymmetry::setDataSet(int i, vtkDataSet *data_set)
{
  //vtk->SetInput(data_set);
}

void Axisymmetry::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}

void Axisymmetry::connectInput(int i)
{
}

void Axisymmetry::disconnectInput(int i)
{
}

void Axisymmetry::connectOutput(int i)
{
}

void Axisymmetry::disconnectOutput(int i)
{
}

void Axisymmetry::help()
{
}

