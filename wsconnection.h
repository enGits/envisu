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

#ifndef mpwsconnection_H
#define mpwsconnection_H

class WsConnection;

#include "workspace.h"

class WsConnection
{
    
    WorkSpace     *m_Ws;
    Q3CanvasLine  *line;
    Q3CanvasLine  *arr_line1;
    Q3CanvasLine  *arr_line2;
    WsItem        *from;
    WsItem        *to;
    int            input_slot;
    
public:
    
    WsConnection(WorkSpace *mws, WsItem *item1, WsItem *item2, int islot);
    ~WsConnection();
    
    WsItem* fromItem()  { return from; }
    WsItem* toItem()    { return to; }
    int         inputSlot() { return input_slot; }
    void        update();
    
    
};

#endif
