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

#ifndef CELL2NODE_H
#define CELL2NODE_H

class Cell2Node;

#include "wsitem.h"

#include <vtkCellDataToPointData.h>

class Cell2Node : public WsItem
{
    
private:
    
    vtkCellDataToPointData *c2n;
    vtkPolyData            *dummy;
    
public:
    
    Cell2Node(WorkSpace *ws);
    ~Cell2Node();
    virtual vtkDataSet* getDataSet() { return c2n->GetOutput(); }
    virtual void setPolyData(int i, vtkPolyData *data) { c2n->SetInput(data); }
    virtual void setDataSet(int i, vtkDataSet *data) { c2n->SetInput(data); }
    
    virtual void setDummyDataSet(int i) { c2n->SetInput(dummy); }
    virtual void setDummyPolyData(int i) { c2n->SetInput(dummy); }
    
    virtual void load(QTextStream &s);
    virtual void save(QTextStream &s);
    
};

#endif
