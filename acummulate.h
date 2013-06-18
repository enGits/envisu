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
#ifndef ACUMMULATE_H
#define ACUMMULATE_H

class Acummulate;

#include "wsitem.h"

#include <vtkAppendPolyData.h>

class Acummulate : public WsItem
{
    
private:
    
    static const int N = 10;
    vector<vtkPolyData*> m_Inputs;
    vtkAppendPolyData *cummulate;
    
public:
    
    Acummulate(WorkSpace *ws);
    ~Acummulate();
    virtual vtkPolyData* getPolyData() { return cummulate->GetOutput(); }
    virtual void setPolyData(int i, vtkPolyData *data);
    virtual void setDummyPolyData(int i);
    virtual void load(QTextStream &s);
    virtual void save(QTextStream &s);
    virtual void disconnectInput(int i);
    
};

#endif