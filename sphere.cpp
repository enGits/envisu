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

#include "sphere.h"

Sphere::Sphere(WorkSpace *ws) : GuiWsItem<Ui::SphereConfig>(ws)
{
  loadIcon("sphere.png");
  m_Ws->addItem(this);
  setName("Sphere");
  vtk = vtkSphere::New();
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.slider,SIGNAL(valueChanged(int)),this,SLOT(autoApply()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkImplicitFunction";
  apply();
}

Sphere::~Sphere()
{
  vtk->Delete();
}

void Sphere::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  vec3_t x = getVector(m_Dlg.ui.x);
  double slider_value = double(m_Dlg.ui.slider->value());
  double slider_max   = double(m_Dlg.ui.slider->maxValue());
  float r1 = m_Dlg.ui.rmin->text().toFloat();
  float r2 = m_Dlg.ui.rmax->text().toFloat();
  float r = r1 + (slider_value/slider_max)*(r2-r1);
  vtk->SetCenter(x[0],x[1],x[2]);
  vtk->SetRadius(r);
  m_Ws->render();
}

void Sphere::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Sphere::autoApply()
{
  if (m_Dlg.ui.auto_cb->isChecked()) {
    apply();
  }
}

void Sphere::save(QTextStream &s)
{
  GuiWsItem<Ui::SphereConfig>::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
  writeLineEdit(s, m_Dlg.ui.x);
  writeSlider(s, m_Dlg.ui.slider);
  writeLineEdit(s, m_Dlg.ui.rmin);
  writeLineEdit(s, m_Dlg.ui.rmax);
  writeCheckBox(s, m_Dlg.ui.auto_cb);
}

void Sphere::load(QTextStream &s)
{
  GuiWsItem<Ui::SphereConfig>::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
  readLineEdit(s, m_Dlg.ui.x);
  readSlider(s, m_Dlg.ui.slider);
  readLineEdit(s, m_Dlg.ui.rmin);
  readLineEdit(s, m_Dlg.ui.rmax);
  readCheckBox(s, m_Dlg.ui.auto_cb);
  apply();
}

vtkImplicitFunction* Sphere::getImplicitFunction()
{
  return vtk;
}

void Sphere::setPolyData(int i, vtkPolyData *poly_data)
{
}

void Sphere::setDataSet(int i, vtkDataSet *data_set)
{
}

void Sphere::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}

