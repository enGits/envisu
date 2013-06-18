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

#ifndef GEOMETRY_H
#define GEOMETRY_H

class Geometry;

#include "guiwsitem.h"
#include "ui_wsitemconfig.h"

#include <vtkGeometryFilter.h>

class Geometry : public GuiWsItem<Ui::WsItemConfig>
{
    
private:
    
    vtkGeometryFilter *geom;
    
public:
    
    Geometry(WorkSpace *ws);
    ~Geometry();
    virtual vtkPolyData* getPolyData() { return geom->GetOutput(); }
    virtual void setPolyData(int i, vtkPolyData *poly_data) { geom->SetInput(poly_data); }
    virtual void setDataSet (int i, vtkDataSet  *data_set)  { geom->SetInput(data_set); }
    virtual void load(QTextStream &s);
    virtual void save(QTextStream &s);
    
};

#endif
