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

#ifndef mpcutter_H
#define mpcutter_H

class Cutter;

#include "guiwsitem.h"
#include "ui_wsitemconfig.h"

#include <vtkCutter.h>

class Cutter : public GuiWsItem<Ui::WsItemConfig>
{

    Q_OBJECT

private:

    vtkCutter      *vtk;

public:

    Cutter (WorkSpace *ws);
    ~Cutter();

    virtual vtkPolyData* getPolyData();

    virtual void setPolyData        (int i, vtkPolyData         *poly_data);
    virtual void setDataSet         (int i, vtkDataSet          *data_set);
    virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function);

public slots:

    virtual void apply ();
    virtual void config();
    virtual void load  (QTextStream &s);
    virtual void save  (QTextStream &s);

};

#endif
