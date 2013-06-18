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

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

#include "ui_inputdialog.h"

class InputDialog : public QDialog
{
  Q_OBJECT

private: // attributes

  Ui::InputDialog m_Ui;


public:

  explicit InputDialog(QWidget *parent = 0);

  void addInputItem(QString item);
  int  currentItem() { return m_Ui.m_CheckBoxInput->currentItem(); }

  
signals:
  
public slots:
  
};

#endif // INPUTDIALOG_H
