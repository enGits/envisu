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

#include "reader.h"

#include <QFileDialog>

Reader::Reader(WorkSpace *ws) : GuiWsItem(ws)
{
  connect(m_Dlg.ui.apply_pb,  SIGNAL(clicked()), this, SLOT(apply()));
  connect(m_Dlg.ui.help_pb,   SIGNAL(clicked()), this, SLOT(help()));
  connect(m_Dlg.ui.browse_pb, SIGNAL(clicked()), this, SLOT(browse()));
  m_Formats = "";
}

void Reader::config()
{
  if (m_Dlg.exec()) {
    apply();
  }
}

void Reader::save(QTextStream &s)
{
  GuiWsItem<Ui::UnstructuredGridReaderConfig>::save(s);
  if (!m_Formats.isEmpty()) {
    s << m_Formats.replace(' ', '~') << "\n";
  } else {
    s << "N/A\n";
  }
  writeLineEdit(s, m_Dlg.ui.name_edit);
  writeLabel(s,m_Dlg.ui.file_label);
}

void Reader::load(QTextStream &s)
{
  GuiWsItem<Ui::UnstructuredGridReaderConfig>::load(s);
  s >> m_Formats;
  m_Formats = m_Formats.replace('~', ' ');
  readLineEdit(s, m_Dlg.ui.name_edit);
  readLabel(s,m_Dlg.ui.file_label);
  apply();
}

void Reader::browse()
{
  QString file_name = QFileDialog::getOpenFileName(NULL, "Choose a file to open", "./", "VTK files (*.vtk *.vtu)");
  if (!file_name.isEmpty()) {
    m_Dlg.ui.file_label->setText(file_name);
  }
}


