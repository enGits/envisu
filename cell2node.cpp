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

#include "cell2node.h"

Cell2Node::Cell2Node(WorkSpace *ws) :	GuiWsItem<Ui::WsItemConfig>(ws)
{
  loadIcon("cell2node.png");
  m_Ws->addItem(this);
  setName("cell to node data");
  m_OutputTypes = "vtkDataSet";
  c2n = vtkCellDataToPointData::New();
  dummy = vtkPolyData::New();
  c2n->SetInput(dummy);
  addInput("data", "vtkPolyData/DataSet");
}

Cell2Node::~Cell2Node()
{
  c2n->Delete();
}

void Cell2Node::load(QTextStream &s)
{
  WsItem::load(s);
  readLineEdit(s, m_Dlg.ui.name_edit);
}

void Cell2Node::save(QTextStream &s)
{
  WsItem::save(s);
  writeLineEdit(s, m_Dlg.ui.name_edit);
}
