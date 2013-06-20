#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QMainWindow>
#include <QVTKWidget.h>
#include <QStatusBar>

#include <vtkRenderWindow.h>

#include "ui_mainwindow.h"

#include "workspace.h"

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

#include "items.h"

class MainWindow : public QMainWindow
{

  Q_OBJECT


private: // attributes

  Ui::MainWindow m_Ui;

  WorkSpace *m_WorkSpace;
  QLabel    *m_StatusLabel;
  QString    m_FileName;


private: // methods


public:

  MainWindow();

  QVTKWidget* getQVtkWidget() { return m_Ui.m_QVtkWidget; }
  void        renderVtk()     { m_Ui.m_QVtkWidget->GetRenderWindow()->Render(); }
  void        clearStatusText() { m_StatusLabel->setText(""); }
  void        setStatusText(QString status_txt) { m_StatusLabel->setText(status_txt); }


public:

  QString version();

  
public slots:

  void fileOpen();
  void fileNew();
  void fileSave();
  void fileSaveAs();
  void filePrint();
  void fileExit();

  void render() { m_WorkSpace->render(); }

  void filterCreate();

  void pipelineFloatStateChanged(bool floating);
  void vtkFloatStateChanged(bool floating);

  // filter slots

  void gridNewPolyDataReader()
  {
    new PolyDataReader(m_WorkSpace);
  }

  void gridNewUnstructuredGridReader()
  {
    new UnstructuredGridReader(m_WorkSpace);
  }

  void gridNewMultiBlockReader()
  {
    new MultiBlockReader(m_WorkSpace);
  }

  void displayNewDisplay()
  {
    new VtkDisplay(m_WorkSpace);
  }

  void displayNewAsciiPointsFile()
  {
    new AsciiPointsFile(m_WorkSpace);
  }

  void objectsNewCylinder()              { new Cylinder(m_WorkSpace); }
  void filterNewCutter()                 { new Cutter(m_WorkSpace); }
  void filterNewClipper()                { new Clipper(m_WorkSpace); }
  void filterNewCell2Node()              { new Cell2Node(m_WorkSpace); }
  void filterNewCummulate()              { new Acummulate(m_WorkSpace); }
  void filterNewGeometry()               { new Geometry(m_WorkSpace); }
  void filterNewTubes()                  { new Tubes(m_WorkSpace); }
  void filterNewContour()                { new Contour(m_WorkSpace); }
  void filterNewAxisymmetry()            { new Axisymmetry(m_WorkSpace); }
  void functionNewPlane()                { new Plane(m_WorkSpace); }
  void functionNewSphere()               { new Sphere(m_WorkSpace); }
  
};

#endif // GUIMAINWINDOW_H
