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

#ifndef mpdisplay_H
#define mpdisplay_H

class VtkDisplay;

#include "guiwsitem.h"
#include "ui_displayconfig.h"

#include <vtkPolyDataMapper.h>
#include <vtkLODActor.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataNormals.h>
#include <vtkScalarBarActor.h>

class VtkDisplay : public GuiWsItem<Ui::DisplayConfig>
{
    
  Q_OBJECT

  vtkLODActor        *actor;
  vtkScalarBarActor  *legend_actor;
  vtkPolyData        *data;
  vtkPolyDataMapper  *mapper;
  vtkPolyDataNormals *normals;
  vtkLookupTable     *lookup_table;

public:

  VtkDisplay(WorkSpace *ws);
  ~VtkDisplay();
  virtual void setPolyData(int i, vtkPolyData *poly_data);
  virtual void disconnectInput  (int i) { data = NULL; }

public slots:

  virtual void config();
  virtual void apply();
  virtual void autoScale();
  virtual void load(QTextStream &s);
  virtual void save(QTextStream &s);
  virtual void update();

};


#endif
