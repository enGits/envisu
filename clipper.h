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

#ifndef mpclipper_H
#define mpclipper_H

class Clipper;

#include "guiwsitem.h"
#include "ui_clipperconfig.h"

#include <vtkClipDataSet.h>
#include <vtkClipPolyData.h>

class Clipper : public GuiWsItem<Ui::ClipperConfig>
{

    Q_OBJECT

private:

    vtkClipDataSet  *vtk_ds;
    vtkClipPolyData *vtk_pd;
    bool             input_connected;
    bool             output_connected;

public:

    Clipper (WorkSpace *mws);
    ~Clipper();

    virtual vtkPolyData* getPolyData();
    virtual vtkDataSet*  getDataSet();

    virtual void setPolyData        (int i, vtkPolyData         *poly_data);
    virtual void setDataSet         (int i, vtkDataSet          *data_set);
    virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function);
    
    virtual void disconnectOutput();
    virtual void disconnectInput (int i);
    virtual void connectOutput   ();
    virtual void connectInput    (int i);

public slots:

    virtual void apply ();
    virtual void config();
    virtual void load  (QTextStream &s);
    virtual void save  (QTextStream &s);

};

#endif
