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

#include "plane.h"

Plane::Plane(WorkSpace *ws) : GuiWsItem<Ui::PlaneConfig>(ws)
{
  loadIcon("plane.png");
  m_Ws->addItem(this);
  setName("Plane");
  vtk = vtkPlane::New();
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.n_slider,SIGNAL(valueChanged(int)),this,SLOT(autoApply()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkImplicitFunction";
  apply();
}

Plane::~Plane()
{
  vtk->Delete();
}

void Plane::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  vec3_t x1 = getVector(m_Dlg.ui.x1);
  vec3_t x2 = getVector(m_Dlg.ui.x2);
  vec3_t n  = x2 - x1;
  double slider_value = double(m_Dlg.ui.n_slider->value());
  double slider_max   = double(m_Dlg.ui.n_slider->maxValue());
  vec3_t x = x1 + (slider_value/slider_max)*n;
  vtk->SetOrigin(x[0],x[1],x[2]);
  vtk->SetNormal(n[0],n[1],n[2]);
  setVector(x, m_Dlg.ui.m_CurrentX);
  m_Ws->render();
}

void Plane::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Plane::autoApply()
{
  if (m_Dlg.ui.auto_cb->isChecked()) apply();
}

void Plane::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.x1);
  writeLineEdit(s, m_Dlg.ui.x2);
  writeSlider(s,m_Dlg.ui.n_slider);
}

void Plane::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readLineEdit(s, m_Dlg.ui.x1);
  readLineEdit(s, m_Dlg.ui.x2);
  readSlider(s,m_Dlg.ui.n_slider);
  apply();
}

vtkImplicitFunction* Plane::getImplicitFunction()
{
  return vtk;
}

void Plane::setPolyData(int i, vtkPolyData *poly_data)
{
}

void Plane::setDataSet(int i, vtkDataSet *data_set)
{
}

void Plane::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}
