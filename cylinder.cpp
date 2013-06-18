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

#include "cylinder.h"

Cylinder::Cylinder(WorkSpace *mws) : GuiWsItem(mws)
{
  loadIcon("cylinder.png");
  m_Ws->addItem(this);
  setName("cylinder");
  cyl = vtkCylinderSource::New();
  transf = vtkTransformPolyDataFilter::New();
  A = vtkGeneralTransform::New();
  A->Identity();
  transf->SetTransform(A);
  transf->SetInput(cyl->GetOutput());
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  x1 = vec3_t(0,0,0);
  x2 = vec3_t(0,0,0);
  setVector(x1, m_Dlg.ui.v1);
  setVector(x2, m_Dlg.ui.v2);
  apply();
}

Cylinder::~Cylinder()
{
}

void Cylinder::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  cyl->SetResolution(m_Dlg.ui.res->text().toInt());
  cyl->SetRadius(m_Dlg.ui.radius->text().toDouble());
  A->Identity();
  vec3_t v1, v2, v3, n;
  x1 = getVector(m_Dlg.ui.v1);
  x2 = getVector(m_Dlg.ui.v2);
  v1 = vec3_t(0,1,0);
  v2 = x2 - x1;
  n = v1.cross(v2);
  double cos_alpha = (v1*v2) / (v1.abs()*v2.abs());
  double alpha = acos(cos_alpha)*180/M_PI;
  v3 = 0.5*(x1 + x2);
  A->Translate(v3.c_array());
  A->RotateWXYZ(alpha,n.c_array());
  cyl->SetHeight(v2.abs());
  cyl->SetCapping(m_Dlg.ui.capped_cb->isChecked());
  m_Ws->render();
}

void Cylinder::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Cylinder::load(QTextStream &s)
{
  WsItem::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
  readLineEdit(s, m_Dlg.ui.res);
  readLineEdit(s, m_Dlg.ui.radius);
  readLineEdit(s, m_Dlg.ui.v1);
  readLineEdit(s, m_Dlg.ui.v2);
  readCheckBox(s, m_Dlg.ui.capped_cb);
  apply();
}

void Cylinder::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
  writeLineEdit(s, m_Dlg.ui.res);
  writeLineEdit(s, m_Dlg.ui.radius);
  writeLineEdit(s, m_Dlg.ui.v1);
  writeLineEdit(s, m_Dlg.ui.v2);
  writeCheckBox(s, m_Dlg.ui.capped_cb);
  s << '\n';
}

