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

#include "clipper.h"

Clipper::Clipper(WorkSpace *mws) : GuiWsItem<Ui::ClipperConfig>(mws)
{
  loadIcon("clipper.png");
  m_Ws->addItem(this);
  setName("Clipper");
  vtk_ds = vtkClipDataSet::New();
  vtk_pd = vtkClipPolyData::New();
  addInput("clip function","vtkImplicitFunction");
  addInput("data","vtkPolyData/DataSet");
  m_OutputTypes = "undefined";
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  input_connected  = false;
  output_connected = false;
  apply();
}

Clipper::~Clipper()
{
  vtk_pd->Delete();
  vtk_ds->Delete();
}

void Clipper::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  if (m_Dlg.ui.in_rb->isChecked()) {
    vtk_ds->InsideOutOn();
    vtk_pd->InsideOutOn();
  } else {
    vtk_ds->InsideOutOff();
    vtk_pd->InsideOutOff();
  };
  m_Ws->render();
}

void Clipper::config()
{
  if (m_Dlg.exec()) apply();
}

void Clipper::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeRadioButton(s,m_Dlg.ui.in_rb);
}

void Clipper::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readRadioButton(s,m_Dlg.ui.in_rb);
  apply();
}

vtkPolyData* Clipper::getPolyData()
{
  return vtk_pd->GetOutput();
}

vtkDataSet* Clipper::getDataSet()
{
  return vtk_ds->GetOutput();
}

void Clipper::setPolyData(int i, vtkPolyData *poly_data)
{
  vtk_pd->SetInput(poly_data);
  m_OutputTypes    = "vtkPolyData";
  m_InputTypes[1] = m_OutputTypes;
  input_connected = true;
}

void Clipper::setDataSet(int i, vtkDataSet *data_set)
{
  vtk_ds->SetInput(data_set);
  m_OutputTypes = "vtkDataSet";
  m_InputTypes[1] = m_OutputTypes;
  input_connected = true;
}

void Clipper::setImplicitFunction(int i, vtkImplicitFunction *function)
{
  vtk_pd->SetClipFunction(function);
  vtk_ds->SetClipFunction(function);
}

void Clipper::disconnectOutput()
{
  if (!input_connected) {
    m_OutputTypes = "undefined";
    m_InputTypes[1] = "vtkPolyData/DataSet";
  } else {
    m_OutputTypes = m_InputTypes[1];
  }
}

void Clipper::disconnectInput(int i)
{
  if (i == 1) {
    input_connected = false;
    vtk_pd->SetInput(m_DummyVtkPolyData);
    vtk_ds->SetInput(m_DummyVtkDataSet);
    if (output_connected) m_InputTypes[1] = m_OutputTypes;
    else m_InputTypes[1] = "vtkPolyData/DataSet";
  }
}

void Clipper::connectOutput()
{
  output_connected = true;
  if (!input_connected) {
    m_OutputTypes = "undefined";
    m_InputTypes[1] = "vtkPolyData/DataSet";
  }
}

void Clipper::connectInput(int i)
{
  if (i == 1) input_connected = true;
}
