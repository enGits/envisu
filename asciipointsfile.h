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

#ifndef mpasciipointsfile_H
#define mpasciipointsfile_H

class AsciiPointsFile;

#include "guiwsitem.h"
#include "ui_asciipointsfileconfig.h"

class AsciiPointsFile : public GuiWsItem<Ui::AsciiPointsFileConfig>
{

    Q_OBJECT

private:

    vtkPolyData             *data;
    vtkIdList               *ids;
    
public:

    AsciiPointsFile (WorkSpace *ws);
    ~AsciiPointsFile();


    virtual void setPolyData        (int i, vtkPolyData         *poly_data);
    virtual void setDataSet         (int i, vtkDataSet          *data_set);
    virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function);
    virtual void connectInput       (int i);
    virtual void disconnectInput    (int i);
    virtual void connectOutput      (int i);
    virtual void disconnectOutput   (int i);

public slots:

    virtual void apply    ();
    virtual void config   ();
    virtual void writeFile();
    virtual void load     (QTextStream &s);
    virtual void save     (QTextStream &s);

};

#endif
