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

#include "multiblockreader.h"

#include <vtkCompositeDataIterator.h>

MultiBlockReader::MultiBlockReader(WorkSpace *ws) : Reader(ws)
{
  loadIcon("multiblockreader.png");
  m_Ws->addItem(this);
  setName("MultiBlockGridReader");
  m_Vtm = vtkXMLMultiBlockDataReader::New();
  m_Append = vtkAppendFilter::New();
  m_Dlg.ui.name_edit->setText(name());
  m_HasOutput = true;
  m_OutputTypes = "vtkDataSet";
  m_Formats = "VTK multi-block files (*.vtm)";
  apply();
}

MultiBlockReader::~MultiBlockReader()
{
  m_Append->Delete();
  m_Vtm->Delete();
}

void MultiBlockReader::apply()
{
  m_Append->RemoveAllInputs();
  changeName(m_Dlg.ui.name_edit->text());
  QString file_name = m_Dlg.ui.file_label->text();
  m_Vtm->SetFileName(qPrintable(file_name));
  m_Vtm->Update();
  vtkCompositeDataIterator *iter = m_Vtm->GetOutput()->NewIterator();
  iter->GoToFirstItem();
  iter->VisitOnlyLeavesOn();
  while (!iter->IsDoneWithTraversal()) {
    m_Append->AddInput(iter->GetCurrentDataObject());
    iter->GoToNextItem();
  }
  m_Ws->render();
}

vtkDataSet *MultiBlockReader::getDataSet()
{
  return m_Append->GetOutput();
}
