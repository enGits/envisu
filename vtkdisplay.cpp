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

#include "vtkdisplay.h"

#include <vtkProperty.h>
#include <vtkDataSetAttributes.h>

VtkDisplay::VtkDisplay(WorkSpace *ws) : GuiWsItem<Ui::DisplayConfig>(ws)
{
  data = NULL;
  setName("camera");
  loadIcon("camera.png");
  m_Ws->addItem(this);
  m_HasOutput = false;
  addInput("poly data", "vtkPolyData");
  mapper = vtkPolyDataMapper::New();
  normals = vtkPolyDataNormals::New();
  actor  = vtkLODActor::New();
  legend_actor = NULL;
  actor->SetMapper(mapper);
  lookup_table = vtkLookupTable::New();
  lookup_table->SetHueRange(0.667, 0.0);
  mapper->SetLookupTable(lookup_table);
  m_Ws->getRenderer()->AddActor(actor);
  connect(m_Dlg.ui.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));
  connect(m_Dlg.ui.auto_pb,SIGNAL(clicked()),this,SLOT(autoScale()));
  m_Dlg.ui.name_edit->setText(name());
  apply();
}

VtkDisplay::~VtkDisplay()
{
  actor->Delete();
  mapper->Delete();
}

void VtkDisplay::autoScale()
{
  mapper->Update();
  double range[2];
  mapper->GetInput()->GetScalarRange(range);
  QString min,max;
  min.setNum(range[0]);
  max.setNum(range[1]);
  m_Dlg.ui.min_edit->setText(min);
  m_Dlg.ui.max_edit->setText(max);
}

void VtkDisplay::config()
{
  m_Dlg.ui.name_edit->setText(name());
  if (m_Dlg.exec()) apply();
}

void VtkDisplay::setPolyData(int i, vtkPolyData *poly_data)
{
  if (i != 0) {
    cerr << "error in void MpDisplay::setPolyData(int i, vtkPolyData *poly_data)" << endl;
  };
  data = poly_data;
  update();
}

void VtkDisplay::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  if (m_Dlg.ui.flat_rb->isChecked())    actor->GetProperty()->SetInterpolationToFlat();
  if (m_Dlg.ui.gouraud_rb->isChecked()) actor->GetProperty()->SetInterpolationToGouraud();
  if (m_Dlg.ui.phong_rb->isChecked())   actor->GetProperty()->SetInterpolationToPhong();
  if (m_Dlg.ui.points_rb->isChecked())  actor->GetProperty()->SetRepresentationToPoints();
  if (m_Dlg.ui.wire_rb->isChecked())    actor->GetProperty()->SetRepresentationToWireframe();
  if (m_Dlg.ui.surface_rb->isChecked()) actor->GetProperty()->SetRepresentationToSurface();
  actor->GetProperty()->BackfaceCullingOff();
  actor->GetProperty()->FrontfaceCullingOff();
  actor->GetProperty()->SetColor(m_Dlg.ui.red_edit->text().toFloat(),
                                 m_Dlg.ui.green_edit->text().toFloat(),
                                 m_Dlg.ui.blue_edit->text().toFloat());
  actor->GetProperty()->SetOpacity(m_Dlg.ui.alpha_edit->text().toFloat());
  mapper->SetScalarVisibility(m_Dlg.ui.scalars_cb->isChecked());
  mapper->SetScalarRange(m_Dlg.ui.min_edit->text().toFloat(),m_Dlg.ui.max_edit->text().toFloat());
  if (legend_actor) {
    m_Ws->getRenderer()->RemoveActor(legend_actor);
    legend_actor->Delete();
    legend_actor = NULL;
  }
  if (m_Dlg.ui.legend_cb->isChecked()) {
    if (data) {
      legend_actor = vtkScalarBarActor::New();
      legend_actor->SetLookupTable(lookup_table);
      legend_actor->SetNumberOfLabels(m_Dlg.ui.legend_spin->value());
      legend_actor->SetMaximumNumberOfColors(m_Dlg.ui.legend_spin->value()-1);
      if (m_Dlg.ui.legend_combo->currentText() == "left") {
        legend_actor->SetPosition(0.02,0.1);
        legend_actor->SetWidth(0.1);
        legend_actor->SetHeight(0.9);
        legend_actor->SetOrientationToVertical();
      } else if (m_Dlg.ui.legend_combo->currentText() == "top") {
        legend_actor->SetOrientationToHorizontal();
        legend_actor->SetPosition(0.10,0.88);
        legend_actor->SetWidth(0.8);
        legend_actor->SetHeight(0.1);
      } else if (m_Dlg.ui.legend_combo->currentText() == "right") {
        legend_actor->SetPosition(0.88,0.10);
        legend_actor->SetWidth(0.1);
        legend_actor->SetHeight(0.8);
        legend_actor->SetOrientationToVertical();
      } else if (m_Dlg.ui.legend_combo->currentText() == "bottom") {
        legend_actor->SetPosition(0.10,0.02);
        legend_actor->SetWidth(0.8);
        legend_actor->SetHeight(0.1);
        legend_actor->SetOrientationToHorizontal();
      };
      legend_actor->SetTitle("Test");
      if (data->GetPointData()) {
        vtkDataSetAttributes *pd = (vtkDataSetAttributes*) data->GetPointData();
        legend_actor->SetTitle(pd->GetScalars()->GetName());
      } else if (data->GetCellData()) {
        vtkDataSetAttributes *cd = (vtkDataSetAttributes*) data->GetCellData();
        legend_actor->SetTitle(cd->GetScalars()->GetName());
      } else {
        legend_actor->SetTitle("N/A");
      }
    }
    m_Ws->getRenderer()->AddActor(legend_actor);
  };
  m_Ws->render();
}

void VtkDisplay::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readRadioButton(s, m_Dlg.ui.flat_rb);
  readRadioButton(s, m_Dlg.ui.gouraud_rb);
  readRadioButton(s, m_Dlg.ui.phong_rb);
  readRadioButton(s, m_Dlg.ui.points_rb);
  readRadioButton(s, m_Dlg.ui.wire_rb);
  readRadioButton(s, m_Dlg.ui.surface_rb);
  readCheckBox(s, m_Dlg.ui.scalars_cb);
  readLineEdit(s, m_Dlg.ui.min_edit);
  readLineEdit(s, m_Dlg.ui.max_edit);
  readLineEdit(s,m_Dlg.ui.red_edit);
  readLineEdit(s,m_Dlg.ui.green_edit);
  readLineEdit(s,m_Dlg.ui.blue_edit);
  readLineEdit(s,m_Dlg.ui.alpha_edit);
  readCheckBox(s,m_Dlg.ui.legend_cb);
  readComboBox(s,m_Dlg.ui.legend_combo);
  readSpinBox(s,m_Dlg.ui.legend_spin);
  apply();
}

void VtkDisplay::save(QTextStream &s)
{
  GuiWsItem::save(s);
  s << m_Dlg.ui.flat_rb->isChecked() << ' ';
  s << m_Dlg.ui.gouraud_rb->isChecked() << ' ';
  s << m_Dlg.ui.phong_rb->isChecked() << ' ';
  s << m_Dlg.ui.points_rb->isChecked() << ' ';
  s << m_Dlg.ui.wire_rb->isChecked() << ' ';
  s << m_Dlg.ui.surface_rb->isChecked() << ' ';
  s << m_Dlg.ui.scalars_cb->isChecked() << ' ';
  s << m_Dlg.ui.min_edit->text().ascii() << ' ';
  s << m_Dlg.ui.max_edit->text().ascii() << ' ';
  writeLineEdit(s,m_Dlg.ui.red_edit);
  writeLineEdit(s,m_Dlg.ui.green_edit);
  writeLineEdit(s,m_Dlg.ui.blue_edit);
  writeLineEdit(s,m_Dlg.ui.alpha_edit);
  writeCheckBox(s,m_Dlg.ui.legend_cb);
  writeComboBox(s,m_Dlg.ui.legend_combo);
  writeSpinBox(s,m_Dlg.ui.legend_spin);
  s << '\n';
}

void VtkDisplay::update()
{
  if (data) {
    if (data->GetNumberOfPolys() || data->GetNumberOfStrips()) {
      normals->SetInput(data);
      mapper->SetInput(normals->GetOutput());
    } else {
      mapper->SetInput(data);
    }
    if (m_Dlg.ui.autoscale_cb->isChecked()) {
      autoScale();
      mapper->SetScalarRange(m_Dlg.ui.min_edit->text().toFloat(),m_Dlg.ui.max_edit->text().toFloat());
    }
    if (legend_actor) {
      legend_actor->SetTitle("Test");
      if (data->GetPointData()) {
        vtkDataSetAttributes *pd = (vtkDataSetAttributes*) data->GetPointData();
        legend_actor->SetTitle(pd->GetScalars()->GetName());
      } else if (data->GetCellData()) {
        vtkDataSetAttributes *cd = (vtkDataSetAttributes*) data->GetCellData();
        legend_actor->SetTitle(cd->GetScalars()->GetName());
      } else {
        legend_actor->SetTitle("N/A");
      }
    }
  }
}



