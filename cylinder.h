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

#ifndef CYLINDER_H
#define CYLINDER_H

class Cylinder;

#include "guiwsitem.h"
#include "ui_cylconfigdialog.h"
#include "envisu.h"

#include <vtkCylinderSource.h>
#include <vtkDataSet.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkGeneralTransform.h>


class Cylinder : public GuiWsItem<Ui::CylConfigDialog>
{
    
    Q_OBJECT
    
private:
    
    vtkCylinderSource          *cyl;
    vtkTransformPolyDataFilter *transf;
    vtkGeneralTransform        *A;
    vec3_t                      x1;
    vec3_t                      x2;
    
public:
    
    Cylinder(WorkSpace *mws);
    ~Cylinder();
    virtual vtkPolyData* getPolyData() { return transf->GetOutput(); }
    
public slots:
    
    virtual void config();
    virtual void apply();
    virtual void load(QTextStream &s);
    virtual void save(QTextStream &s);
    
};

#endif
