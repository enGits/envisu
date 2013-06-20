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

#ifndef MULTIBLOCKREADER_H
#define MULTIBLOCKREADER_H

#include "reader.h"

#include <vtkXMLMultiBlockDataReader.h>
#include <vtkCompositeDataSet.h>
#include <vtkAppendFilter.h>

class MultiBlockReader : public Reader
{

  Q_OBJECT

private:

  vtkXMLMultiBlockDataReader *m_Vtm;
  vtkAppendFilter            *m_Append;

public:

  MultiBlockReader(WorkSpace* ws);
  ~MultiBlockReader();

  virtual vtkDataSet* getDataSet();


public slots:

  virtual void apply ();
};

#endif // MULTIBLOCKREADER_H
