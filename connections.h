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

connect(m_Ui.actionDisplayNewAsciiPointsFile, SIGNAL(activated()), this, SLOT(displayNewAsciiPointsFile()));
connect(m_Ui.actionDisplayNewDisplay, SIGNAL(activated()), this, SLOT(displayNewDisplay()));

connect(m_Ui.actionFileExit,   SIGNAL(activated()), this, SLOT(fileExit()));
connect(m_Ui.actionFileNew,    SIGNAL(activated()), this, SLOT(fileNew()));
connect(m_Ui.actionFileOpen,   SIGNAL(activated()), this, SLOT(fileOpen()));
connect(m_Ui.actionFilePrint,  SIGNAL(activated()), this, SLOT(filePrint()));
connect(m_Ui.actionFileSave,   SIGNAL(activated()), this, SLOT(fileSave()));
connect(m_Ui.actionFileSaveAs, SIGNAL(activated()), this, SLOT(fileSaveAs()));

connect(m_Ui.actionFilterCreate,         SIGNAL(activated()), this, SLOT(filterCreate()));
connect(m_Ui.actionFilterNewAxisymmetry, SIGNAL(activated()), this, SLOT(filterNewAxisymmetry()));
connect(m_Ui.actionFilterNewCell2Node,   SIGNAL(activated()), this, SLOT(filterNewCell2Node()));
connect(m_Ui.actionFilterNewClipper,     SIGNAL(activated()), this, SLOT(filterNewClipper()));
connect(m_Ui.actionFilterNewContour,     SIGNAL(activated()), this, SLOT(filterNewContour()));
connect(m_Ui.actionFilterNewCummulate,   SIGNAL(activated()), this, SLOT(filterNewCummulate()));
connect(m_Ui.actionFilterNewCutter,      SIGNAL(activated()), this, SLOT(filterNewCutter()));
connect(m_Ui.actionFilterNewGeometry,    SIGNAL(activated()), this, SLOT(filterNewGeometry()));
connect(m_Ui.actionFilterNewTubes,       SIGNAL(activated()), this, SLOT(filterNewTubes()));

connect(m_Ui.actionGridNewPolyDataReader,         SIGNAL(activated()), this, SLOT(gridNewPolyDataReader()));
connect(m_Ui.actionGridNewUnstructuredGridReader, SIGNAL(activated()), this, SLOT(gridNewUnstructuredGridReader()));
connect(m_Ui.actionGridNewMultiBlockReader,       SIGNAL(activated()), this, SLOT(gridNewMultiBlockReader()));

connect(m_Ui.actionFunctionNewPlane, SIGNAL(activated()), this, SLOT(functionNewPlane()));
connect(m_Ui.actionFunctionNewSphere, SIGNAL(activated()), this, SLOT(functionNewSphere()));

connect(m_Ui.actionObjectNewCylinder, SIGNAL(activated()), this, SLOT(objectsNewCylinder()));

connect(m_Ui.actionViewZoomAll, SIGNAL(activated()), m_WorkSpace, SLOT(zoomAll()));
connect(m_Ui.actionViewLeft,    SIGNAL(activated()), m_WorkSpace, SLOT(viewLeft()));
connect(m_Ui.actionViewRight,   SIGNAL(activated()), m_WorkSpace, SLOT(viewRight()));
connect(m_Ui.actionViewFront,   SIGNAL(activated()), m_WorkSpace, SLOT(viewFront()));
connect(m_Ui.actionViewBack,    SIGNAL(activated()), m_WorkSpace, SLOT(viewBack()));
connect(m_Ui.actionViewTop,     SIGNAL(activated()), m_WorkSpace, SLOT(viewTop()));
connect(m_Ui.actionViewBottom,  SIGNAL(activated()), m_WorkSpace, SLOT(viewBottom()));
