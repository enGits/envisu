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
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <QColorDialog>

VtkDisplay::VtkDisplay(WorkSpace *ws) : GuiWsItem<Ui::DisplayConfig>(ws)
{
  m_Data = NULL;
  setName("camera");
  loadIcon("camera.png");
  m_Ws->addItem(this);
  m_HasOutput = false;
  addInput("poly data", "vtkPolyData");
  m_Mapper = vtkPolyDataMapper::New();
  m_Normals = vtkPolyDataNormals::New();
  m_Actor  = vtkLODActor::New();
  m_LegendActor = NULL;
  m_LookupTable = vtkLookupTable::New();
  m_LookupTable->SetHueRange(0.667, 0.0);
  m_LookupTable->SetVectorModeToMagnitude();
  m_LookupTable->Build();
  m_Mapper->SetLookupTable(m_LookupTable);
  m_Actor->SetMapper(m_Mapper);
  m_Ws->getRenderer()->AddActor(m_Actor);
  connect(m_Dlg.ui.pushButtonApply,     SIGNAL(clicked()),     this, SLOT(apply()));
  connect(m_Dlg.ui.pushButtonAutoScale, SIGNAL(clicked()),     this, SLOT(autoScale()));
  connect(m_Dlg.ui.checkBoxScalars,     SIGNAL(toggled(bool)), this, SLOT(scalarsToggled(bool)));
  connect(m_Dlg.ui.radioButtonNodes,    SIGNAL(toggled(bool)), this, SLOT(updateScalars()));
  connect(m_Dlg.ui.pushButtonColour,    SIGNAL(clicked()),     this, SLOT(selectColour()));
  m_Dlg.ui.lineEditName->setText(name());
  apply();
}

VtkDisplay::~VtkDisplay()
{
  m_Actor->Delete();
  m_Mapper->Delete();
}

void VtkDisplay::autoScale()
{
  m_Mapper->SelectColorArray(qPrintable(m_Dlg.ui.comboBoxScalar->currentText()));
  m_Mapper->Update();
  double min_value = 0;
  double max_value = 1;
  QString scalar_name = m_Mapper->GetArrayName();

  vtkDataSetAttributes* dataset_attributes;
  if (m_Dlg.ui.radioButtonCells->isChecked()) {
    dataset_attributes = m_Mapper->GetInput()->GetCellData();
  } else {
    dataset_attributes = m_Mapper->GetInput()->GetPointData();
  }
  if (dataset_attributes) {
    vtkDataArray* data_array = dataset_attributes->GetArray(qPrintable(scalar_name));
    if (data_array) {
      for (vtkIdType i = 0; i < data_array->GetNumberOfTuples(); ++i) {
        double value = 0;
        for (int j = 0; j < data_array->GetNumberOfComponents(); ++j) {
          double component = data_array->GetComponent(i, j);
          value += component*component;
        }
        value = sqrt(value);
        min_value = min(value, min_value);
        max_value = max(value, max_value);
      }
    }
  }

  QString min_txt, max_txt;
  min_txt.setNum(min_value);
  max_txt.setNum(max_value);
  m_Dlg.ui.lineEditRangeMin->setText(min_txt);
  m_Dlg.ui.lineEditRangeMax->setText(max_txt);
}

void VtkDisplay::config()
{
  m_Dlg.ui.lineEditName->setText(name());
  if (m_Dlg.exec()) {
    apply();
  }
}

void VtkDisplay::setPolyData(int i, vtkPolyData *poly_data)
{
  if (i != 0) {
    cerr << "error in void MpDisplay::setPolyData(int i, vtkPolyData *poly_data)" << endl;
  }
  m_Data = poly_data;
  update();
}

void VtkDisplay::apply()
{
  changeName(m_Dlg.ui.lineEditName->text());

  if (m_Dlg.ui.radioButtonFlat->isChecked())            m_Actor->GetProperty()->SetInterpolationToFlat();
  if (m_Dlg.ui.radioButtonGouraud->isChecked())         m_Actor->GetProperty()->SetInterpolationToGouraud();
  if (m_Dlg.ui.radioButtonPhong->isChecked())           m_Actor->GetProperty()->SetInterpolationToPhong();
  if (m_Dlg.ui.radioButtonRenderPoints->isChecked())    m_Actor->GetProperty()->SetRepresentationToPoints();
  if (m_Dlg.ui.radioButtonRenderWireFrame->isChecked()) m_Actor->GetProperty()->SetRepresentationToWireframe();
  if (m_Dlg.ui.radioButtonRenderSurface->isChecked())   m_Actor->GetProperty()->SetRepresentationToSurface();

  m_Actor->GetProperty()->BackfaceCullingOff();
  m_Actor->GetProperty()->FrontfaceCullingOff();
  m_Actor->GetProperty()->SetColor(m_Dlg.ui.lineEditRed->text().toFloat(),
                                   m_Dlg.ui.lineEditGreen->text().toFloat(),
                                   m_Dlg.ui.lineEditGreen->text().toFloat());
  m_Actor->GetProperty()->SetOpacity(m_Dlg.ui.lineEditAlpha->text().toFloat());

  update();
  m_Ws->render();
}

void VtkDisplay::load(QTextStream &s)
{
  GuiWsItem::load(s);
  readLineEdit   (s, m_Dlg.ui.lineEditName);
  readRadioButton(s, m_Dlg.ui.radioButtonFlat);
  readRadioButton(s, m_Dlg.ui.radioButtonGouraud);
  readRadioButton(s, m_Dlg.ui.radioButtonPhong);
  readRadioButton(s, m_Dlg.ui.radioButtonRenderPoints);
  readRadioButton(s, m_Dlg.ui.radioButtonRenderWireFrame);
  readRadioButton(s, m_Dlg.ui.radioButtonRenderSurface);
  readRadioButton(s, m_Dlg.ui.radioButtonCells);
  readRadioButton(s, m_Dlg.ui.radioButtonNodes);
  readCheckBox   (s, m_Dlg.ui.checkBoxScalars);
  readLineEdit   (s, m_Dlg.ui.lineEditRangeMin);
  readLineEdit   (s, m_Dlg.ui.lineEditRangeMax);
  readLineEdit   (s, m_Dlg.ui.lineEditRed);
  readLineEdit   (s, m_Dlg.ui.lineEditGreen);
  readLineEdit   (s, m_Dlg.ui.lineEditBlue);
  readLineEdit   (s, m_Dlg.ui.lineEditAlpha);
  readCheckBox   (s, m_Dlg.ui.checkBoxLegend);
  readComboBox   (s, m_Dlg.ui.comboBoxPosition);
  readComboBox   (s, m_Dlg.ui.comboBoxScalar);
  readSpinBox    (s ,m_Dlg.ui.spinBoxLevels);
  apply();
}

void VtkDisplay::save(QTextStream &s)
{
  GuiWsItem::save(s);
  writeLineEdit   (s, m_Dlg.ui.lineEditName);
  writeRadioButton(s, m_Dlg.ui.radioButtonFlat);
  writeRadioButton(s, m_Dlg.ui.radioButtonGouraud);
  writeRadioButton(s, m_Dlg.ui.radioButtonPhong);
  writeRadioButton(s, m_Dlg.ui.radioButtonRenderPoints);
  writeRadioButton(s, m_Dlg.ui.radioButtonRenderWireFrame);
  writeRadioButton(s, m_Dlg.ui.radioButtonRenderSurface);
  writeRadioButton(s, m_Dlg.ui.radioButtonCells);
  writeRadioButton(s, m_Dlg.ui.radioButtonNodes);
  writeCheckBox   (s, m_Dlg.ui.checkBoxScalars);
  writeLineEdit   (s, m_Dlg.ui.lineEditRangeMin);
  writeLineEdit   (s, m_Dlg.ui.lineEditRangeMax);
  writeLineEdit   (s, m_Dlg.ui.lineEditRed);
  writeLineEdit   (s, m_Dlg.ui.lineEditGreen);
  writeLineEdit   (s, m_Dlg.ui.lineEditBlue);
  writeLineEdit   (s, m_Dlg.ui.lineEditAlpha);
  writeCheckBox   (s, m_Dlg.ui.checkBoxLegend);
  writeComboBox   (s, m_Dlg.ui.comboBoxPosition);
  writeComboBox   (s, m_Dlg.ui.comboBoxScalar);
  writeSpinBox    (s ,m_Dlg.ui.spinBoxLevels);
  s << '\n';
}

void VtkDisplay::update()
{
  m_Mapper->ScalarVisibilityOff();
  m_Mapper->SetScalarRange(m_Dlg.ui.lineEditRangeMin->text().toFloat(), m_Dlg.ui.lineEditRangeMax->text().toFloat());
  if (m_Data) {
    if (m_Dlg.ui.checkBoxScalars->isChecked()) {
      m_Mapper->ScalarVisibilityOn();
      if (m_Dlg.ui.radioButtonCells->isChecked()) {
        m_Mapper->SetScalarModeToUseCellFieldData();
      } else {
        m_Mapper->SetScalarModeToUsePointFieldData();
      }
      m_Mapper->SelectColorArray(qPrintable(m_Dlg.ui.comboBoxScalar->currentText()));
    }
    m_Mapper->SelectColorArray(qPrintable(m_Dlg.ui.comboBoxScalar->currentText()));
    if (m_Data->GetNumberOfPolys() || m_Data->GetNumberOfStrips()) {
      m_Normals->SetInput(m_Data);
      m_Normals->Update();
      m_Mapper->SetInput(m_Normals->GetOutput());
    } else {
      m_Mapper->SetInput(m_Data);
    }
    if (m_Dlg.ui.checkBoxAutoScale->isChecked()) {
      autoScale();
      m_Mapper->SetScalarRange(m_Dlg.ui.lineEditRangeMin->text().toFloat(), m_Dlg.ui.lineEditRangeMax->text().toFloat());
    }
    if (m_LegendActor) {
      m_Ws->getRenderer()->RemoveActor(m_LegendActor);
      m_LegendActor->Delete();
      m_LegendActor = NULL;
    }
    if (m_Dlg.ui.checkBoxLegend->isChecked()) {
      if (m_Data && m_Dlg.ui.checkBoxScalars->isChecked()) {
        m_LegendActor = vtkScalarBarActor::New();
        m_LegendActor->SetLookupTable(m_LookupTable);
        m_LegendActor->SetNumberOfLabels(m_Dlg.ui.spinBoxLevels->value());
        m_LegendActor->SetMaximumNumberOfColors(m_Dlg.ui.spinBoxLevels->value()-1);
        if (m_Dlg.ui.comboBoxPosition->currentText() == "left") {
          m_LegendActor->SetPosition(0.02,0.1);
          m_LegendActor->SetWidth(0.1);
          m_LegendActor->SetHeight(0.9);
          m_LegendActor->SetOrientationToVertical();
        } else if (m_Dlg.ui.comboBoxPosition->currentText() == "top") {
          m_LegendActor->SetOrientationToHorizontal();
          m_LegendActor->SetPosition(0.10,0.88);
          m_LegendActor->SetWidth(0.8);
          m_LegendActor->SetHeight(0.1);
        } else if (m_Dlg.ui.comboBoxPosition->currentText() == "right") {
          m_LegendActor->SetPosition(0.88,0.10);
          m_LegendActor->SetWidth(0.1);
          m_LegendActor->SetHeight(0.8);
          m_LegendActor->SetOrientationToVertical();
        } else if (m_Dlg.ui.comboBoxPosition->currentText() == "bottom") {
          m_LegendActor->SetPosition(0.10,0.02);
          m_LegendActor->SetWidth(0.8);
          m_LegendActor->SetHeight(0.1);
          m_LegendActor->SetOrientationToHorizontal();
        }
        m_LegendActor->SetTitle(m_Mapper->GetArrayName());
      }
      m_Ws->getRenderer()->AddActor(m_LegendActor);
    }
    if (m_LegendActor) {
      m_LegendActor->SetTitle(m_Mapper->GetArrayName());
    }
  }
}

void VtkDisplay::scalarsToggled(bool scalars_on)
{
  m_Dlg.ui.checkBoxAutoScale->setEnabled(scalars_on);
  m_Dlg.ui.radioButtonNodes->setEnabled(scalars_on);
  m_Dlg.ui.radioButtonCells->setEnabled(scalars_on);
  m_Dlg.ui.comboBoxScalar->setEnabled(scalars_on);
  m_Dlg.ui.lineEditRangeMin->setEnabled(scalars_on);
  m_Dlg.ui.lineEditRangeMax->setEnabled(scalars_on);
  m_Dlg.ui.checkBoxLegend->setEnabled(scalars_on);
  m_Dlg.ui.spinBoxLevels->setEnabled(scalars_on);
  m_Dlg.ui.comboBoxPosition->setEnabled(scalars_on);
  m_Dlg.ui.pushButtonAutoScale->setEnabled(scalars_on);
  updateScalars();
}

void VtkDisplay::updateScalars()
{
  m_Dlg.ui.comboBoxScalar->clear();
  if (m_Data) {
    vtkDataSetAttributes *dataset_attributes;
    if (m_Dlg.ui.radioButtonNodes->isChecked()) {
      dataset_attributes = m_Data->GetPointData();
    } else {
      dataset_attributes = m_Data->GetCellData();
    }
    if (dataset_attributes) {
      for (int i = 0; i < dataset_attributes->GetNumberOfArrays(); ++i) {
        vtkDataArray* data_array = dataset_attributes->GetArray(i);
        if (data_array) {
          m_Dlg.ui.comboBoxScalar->addItem(data_array->GetName());
        }
      }
    }
  }
}

void VtkDisplay::selectColour()
{
  int r = int(255*m_Dlg.ui.lineEditRed->text().toDouble());
  int g = int(255*m_Dlg.ui.lineEditGreen->text().toDouble());
  int b = int(255*m_Dlg.ui.lineEditBlue->text().toDouble());
  QColor colour(r,g,b);
  colour = QColorDialog::getColor(colour);
  QString txt;
  txt.setNum(double(colour.red())/255);
  m_Dlg.ui.lineEditRed->setText(txt);
  txt.setNum(double(colour.green())/255);
  m_Dlg.ui.lineEditGreen->setText(txt);
  txt.setNum(double(colour.blue())/255);
  m_Dlg.ui.lineEditBlue->setText(txt);
}


