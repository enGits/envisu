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

#include "tubes.h"

Tubes::Tubes(WorkSpace *mws) : GuiWsItem<Ui::TubesConfig>(mws)
{
  loadIcon("tubes.png");
  setName("tubes");
  m_Ws->addItem(this);
  m_OutputTypes = "vtkPolyData";
  tubes = vtkTubeFilter::New();
  addInput("poly data", "vtkPolyData");
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  m_Dlg.ui.name_edit->setText(name());
  apply();
}

Tubes::~Tubes()
{
  tubes->Delete();
}

void Tubes::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readLineEdit(s,m_Dlg.ui.res_edit);
  readLineEdit(s,m_Dlg.ui.radius_edit);
  apply();
}

void Tubes::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeLineEdit(s,m_Dlg.ui.res_edit);
  writeLineEdit(s,m_Dlg.ui.radius_edit);
}

void Tubes::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  tubes->SetRadius(m_Dlg.ui.radius_edit->text().toFloat());
  tubes->SetNumberOfSides(m_Dlg.ui.res_edit->text().toInt());
  m_Ws->render();
}

void Tubes::config()
{
  m_Dlg.ui.name_edit->setText(name());
  if (m_Dlg.exec()) {
    apply();
  }
}
