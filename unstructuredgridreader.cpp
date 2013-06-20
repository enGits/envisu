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

#include "unstructuredgridreader.h"

UnstructuredGridReader::UnstructuredGridReader(WorkSpace *ws) : Reader(ws)
{
  loadIcon("unstructuredgridreader.png");
  m_Ws->addItem(this);
  setName("UnstructuredGridReader");
  m_Vtk = vtkUnstructuredGridReader::New();
  m_Xml = vtkXMLUnstructuredGridReader::New();
  m_UseXml = false;
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkDataSet";
  m_Formats = "VTK files (*.vtk *.vtu)";
  apply();
}

UnstructuredGridReader::~UnstructuredGridReader()
{
  m_Vtk->Delete();
  m_Xml->Delete();
}

void UnstructuredGridReader::apply()
{
  changeName(m_Dlg.ui.name_edit->text());
  QString file_name = m_Dlg.ui.file_label->text();
  if (file_name.size() >= 4) {
    if (file_name.right(4) == ".vtu") {
      m_UseXml = true;
      m_Xml->SetFileName(qPrintable(file_name));
    } else {
      m_UseXml = false;
      m_Vtk->SetFileName(qPrintable(file_name));
    }
  }
  m_Ws->render();
}

vtkDataSet* UnstructuredGridReader::getDataSet()
{
  if (m_UseXml) {
    return m_Xml->GetOutput();
  }
  return m_Vtk->GetOutput();
}

