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

#ifndef mpplane_H
#define mpplane_H

class Plane;

#include "guiwsitem.h"
#include "ui_planeconfig.h"

#include <vtkPlane.h>

class Plane : public GuiWsItem<Ui::PlaneConfig>
{

    Q_OBJECT

private:

    vtkPlane      *vtk;

public:

    Plane (WorkSpace *mws);
    ~Plane();

    virtual vtkImplicitFunction* getImplicitFunction();

    virtual void setPolyData        (int i, vtkPolyData         *poly_data);
    virtual void setDataSet         (int i, vtkDataSet          *data_set);
    virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function);

public slots:

    virtual void apply    ();
    virtual void autoApply();
    virtual void config   ();
    virtual void load     (QTextStream &s);
    virtual void save     (QTextStream &s);

};

#endif
