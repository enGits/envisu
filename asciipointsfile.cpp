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

#include "asciipointsfile.h"

#include <iostream>
#include <fstream>

#include <vtkIdList.h>
#include <vtkPointData.h>

AsciiPointsFile::AsciiPointsFile(WorkSpace *ws) : GuiWsItem<Ui::AsciiPointsFileConfig>(ws)
{
  loadIcon("asciipointsfile.png");
  m_Ws->addItem(this);
  setName("AsciiPointsFile");
  addInput("data","vtkPolyData");
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.write_pb,SIGNAL(clicked()),this,SLOT(writeFile()));
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = false;
  ids = vtkIdList::New();
  apply();
}

AsciiPointsFile::~AsciiPointsFile()
{
  ids->Delete();
}

void AsciiPointsFile::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  m_Ws->render();
}

void AsciiPointsFile::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void AsciiPointsFile::writeFile()
{
  ofstream f(m_Dlg.ui.file_label->text().ascii());
  data->Update();
  data->BuildCells();
  data->BuildLinks();
  vtkIdType ncells = data->GetNumberOfCells();
  vtkIdType nnodes = data->GetNumberOfPoints();
  vtkIdType nlines = data->GetNumberOfLines();
  if (ncells != nlines) return;
  if (ncells == 0) return;
  if (nnodes == 0) return;
  vtkIdType  start = 0;
  vtkIdType  point = start;
  vtkIdType  cell  = ncells;
  vtkIdType  N;
  do {
    data->GetPointCells(point,ids);
    N = ids->GetNumberOfIds();
    if (N == 1) break;
    if (N != 2) return; // error
    if (cell == ids->GetId(0)) {
      cell = ids->GetId(1);
    } else {
      cell = ids->GetId(0);
    };
    data->GetCellPoints(cell,ids);
    N = ids->GetNumberOfIds();
    if (N != 2) return; // error
    if (point == ids->GetId(0)) {
      point = ids->GetId(1);
    } else {
      point = ids->GetId(0);
    };
  } while (point != start);
  vtkDataArray *scalars = data->GetPointData()->GetScalars();

  vector<double> var(nnodes);
  vector<double> xc(nnodes);
  vector<double> yc(nnodes);
  vector<double> zc(nnodes);
  vector<double> L(nnodes);
  for (vtkIdType i = 0; i < nnodes; ++i) {
    {
      double s = 0,x[3];
      if (scalars) scalars->GetTuple(point,&var[i]);
      data->GetPoint(point,x);
      xc[i] = x[0];
      yc[i] = x[1];
      zc[i] = x[2];
    }
    data->GetPointCells(point,ids);
    N = ids->GetNumberOfIds();
    if (N == 1) {
      cell = ids->GetId(0);
    } else {
      if (cell == ids->GetId(0)) {
        cell = ids->GetId(1);
      } else {
        cell = ids->GetId(0);
      }
    }
    data->GetCellPoints(cell,ids);
    if (point == ids->GetId(0)) {
      point = ids->GetId(1);
    } else {
      point = ids->GetId(0);
    }
  }

  if (m_Dlg.ui.reverse_cb->isChecked()) {
    L[nnodes-2] = 0;
    for (int i = nnodes-2; i >= 0; --i) {
      L[i]  = (xc[i]-xc[i+1])*(xc[i]-xc[i+1]);
      L[i] += (yc[i]-yc[i+1])*(yc[i]-yc[i+1]);
      L[i] += (zc[i]-zc[i+1])*(zc[i]-zc[i+1]);
      L[i]  = sqrt(L[i]);
      L[i] += L[i+1];
    }
  } else {
    L[0] = 0;
    for (int i = 1; i < nnodes; ++i) {
      L[i]  = (xc[i]-xc[i-1])*(xc[i]-xc[i-1]);
      L[i] += (yc[i]-yc[i-1])*(yc[i]-yc[i-1]);
      L[i] += (zc[i]-zc[i-1])*(zc[i]-zc[i-1]);
      L[i]  = sqrt(L[i]);
      L[i] += L[i-1];
    }
  }

  if (m_Dlg.ui.start_cb->isChecked()) {
    int i = m_Dlg.ui.start_spin->value() - 1;
    if (i+2 >= nnodes) i = nnodes - 3;
    while (i >= 0) {
      var[i] = var[i+1]+fabs((L[i+1]-L[i]) / (L[i+2]-L[i+1]))*(var[i+1]-var[i+2]);
      --i;
    }
  }
  if (m_Dlg.ui.end_cb->isChecked()) {
    int i = nnodes - m_Dlg.ui.end_spin->value();
    if (i <= 0) i = 0;
    while (i < nnodes) {
      var[i] = var[i-1]+fabs((L[i-1]-L[i]) / (L[i-2]-L[i-1]))*(var[i-1]-var[i-2]);
      ++i;
    }
  }
  for (int i = 0; i < nnodes; ++i) {
    f << L[i]   << ' '
      << xc[i]  << ' '
      << yc[i]  << ' '
      << zc[i]  << ' '
      << var[i] << '\n';
  }
}

void AsciiPointsFile::save(QTextStream &s)
{
  GuiWsItem<Ui::AsciiPointsFileConfig>::save(s);
  writeLabel(s,m_Dlg.ui.file_label);
  writeCheckBox(s,m_Dlg.ui.start_cb);
  writeCheckBox(s,m_Dlg.ui.end_cb);
  writeCheckBox(s,m_Dlg.ui.reverse_cb);
  writeSpinBox(s,m_Dlg.ui.start_spin);
  writeSpinBox(s,m_Dlg.ui.end_spin);
}

void AsciiPointsFile::load(QTextStream &s)
{
  GuiWsItem<Ui::AsciiPointsFileConfig>::load(s);
  readLabel(s,m_Dlg.ui.file_label);
  readCheckBox(s,m_Dlg.ui.start_cb);
  readCheckBox(s,m_Dlg.ui.end_cb);
  readCheckBox(s,m_Dlg.ui.reverse_cb);
  readSpinBox(s,m_Dlg.ui.start_spin);
  readSpinBox(s,m_Dlg.ui.end_spin);
  apply();
}


void AsciiPointsFile::setPolyData(int i, vtkPolyData *poly_data)
{
  data = poly_data;
}

void AsciiPointsFile::setDataSet(int i, vtkDataSet *data_set)
{
}

void AsciiPointsFile::setImplicitFunction(int i, vtkImplicitFunction *function)
{
}

void AsciiPointsFile::connectInput(int i)
{
}

void AsciiPointsFile::disconnectInput(int i)
{
}

void AsciiPointsFile::connectOutput(int i)
{
}

void AsciiPointsFile::disconnectOutput(int i)
{
}

