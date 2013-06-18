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

#include "polydatareader.h"

PolyDataReader::PolyDataReader(WorkSpace *ws) : GuiWsItem<Ui::PolyDataReaderConfig>(ws)
{
  loadIcon("polydatareader.png");
  m_Ws->addItem(this);
  setName("PolyDataReader");
  vtk = vtkPolyDataReader::New();
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.help_pb,SIGNAL(clicked()),this,SLOT(help()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkPolyData";
  apply();
}

PolyDataReader::~PolyDataReader()
{
  vtk->Delete();
}

void PolyDataReader::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  vtk->SetFileName(m_Dlg.ui.file_label->text().ascii());
  m_Ws->render();
}

void PolyDataReader::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void PolyDataReader::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
  writeLabel(s,m_Dlg.ui.file_label);
}

void PolyDataReader::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
  readLabel(s,m_Dlg.ui.file_label);
  apply();
}

vtkPolyData* PolyDataReader::getPolyData()
{
  return vtk->GetOutput();
}

void PolyDataReader::setPolyData(int i, vtkPolyData *poly_data)
{
}

void PolyDataReader::setDataSet(int i, vtkDataSet *data_set)
{
}

void PolyDataReader::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}

void PolyDataReader::connectInput(int i)
{
}

void PolyDataReader::disconnectInput(int i)
{
}

void PolyDataReader::connectOutput(int i)
{
}

void PolyDataReader::disconnectOutput(int i)
{
}

void PolyDataReader::help()
{
}

