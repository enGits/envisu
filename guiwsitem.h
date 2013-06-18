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

#ifndef GUIWSITEM_H
#define GUIWSITEM_H

template <typename UI> class GuiWsItem;

#include "wsitem.h"

template <typename UI>
class GuiWsItem : public WsItem
{
protected: // data types

  struct dlg_t : public QDialog
  {
    UI ui;
    dlg_t()
    {
      ui.setupUi(this);
    }
  };
        
protected:
    
  dlg_t m_Dlg;

public:
    
  GuiWsItem(WorkSpace *ws) : WsItem(ws) {}

};

#endif
