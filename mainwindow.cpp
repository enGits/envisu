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

#include "mainwindow.h"
#include "newfilterdialog.h"

#include <QFileDialog>
#include <QPrinter>
#include <QPainter>

MainWindow::MainWindow() // : QMainWindow(NULL)
{
  m_Ui.setupUi(this);
  m_WorkSpace = new WorkSpace(m_Ui.dockWidgetPipeline);
  m_Ui.m_WorkSpaceLayout->addWidget(m_WorkSpace);
  m_StatusLabel = new QLabel(this);
  m_StatusLabel->setText("");
  statusBar()->addWidget(m_StatusLabel);
  m_WorkSpace->setMainWindow(this);
  setCaption(version() + ": unknown.envisu");

  if (centralWidget()) {
    delete centralWidget();
    setCentralWidget(NULL);
  }

  connect(m_Ui.dockWidget3D,       SIGNAL(topLevelChanged(bool)), this, SLOT(vtkFloatStateChanged(bool)));
  connect(m_Ui.dockWidgetPipeline, SIGNAL(topLevelChanged(bool)), this, SLOT(pipelineFloatStateChanged(bool)));
  connect(m_Ui.pushButtonMaximize3D, SIGNAL(clicked()), m_Ui.dockWidget3D, SLOT(showMaximized()));
  connect(m_Ui.pushButtonMinimize3D, SIGNAL(clicked()), m_Ui.dockWidget3D, SLOT(showNormal()));
  connect(m_Ui.pushButtonMaximizePipeline, SIGNAL(clicked()), m_Ui.dockWidgetPipeline, SLOT(showMaximized()));
  connect(m_Ui.pushButtonMinimizePipeline, SIGNAL(clicked()), m_Ui.dockWidgetPipeline, SLOT(showNormal()));

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
}

void MainWindow::pipelineFloatStateChanged(bool floating)
{
  if (floating) {
    m_Ui.dockWidgetPipeline->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint);
    m_Ui.pushButtonMaximizePipeline->setEnabled(true);
    m_Ui.pushButtonMinimizePipeline->setEnabled(true);
    m_Ui.dockWidgetPipeline->show();
  } else {
    m_Ui.dockWidgetPipeline->setWindowFlags(Qt::Widget);
    m_Ui.pushButtonMaximizePipeline->setEnabled(false);
    m_Ui.pushButtonMinimizePipeline->setEnabled(false);
  }
}

void MainWindow::vtkFloatStateChanged(bool floating)
{
  if (floating) {
    m_Ui.dockWidget3D->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint);
    m_Ui.pushButtonMaximize3D->setEnabled(true);
    m_Ui.pushButtonMinimize3D->setEnabled(true);
    m_Ui.dockWidget3D->show();
  } else {
    m_Ui.dockWidget3D->setWindowFlags(Qt::Widget);
    m_Ui.pushButtonMaximize3D->setEnabled(false);
    m_Ui.pushButtonMinimize3D->setEnabled(false);
  }
}

void MainWindow::fileNew()
{
  for (list<WsItem*>::iterator i = m_WorkSpace->getItems()->begin(); i != m_WorkSpace->getItems()->end(); ++i) {
    m_WorkSpace->delItem(*i);
  }
}

QString MainWindow::version()
{
  return QString("enVisu 1.0dev");
}

void MainWindow::fileOpen()
{
  m_FileName = QFileDialog::getOpenFileName(this, "Choose a file to open", "./", "enVisu files (*.envisu)");
  if (!m_FileName.isEmpty()) {
    fileNew();
    m_WorkSpace->load(m_FileName);
    setCaption(version() + ": " + m_FileName);
    m_WorkSpace->zoomAll();
  }
}

void MainWindow::fileSave()
{
  if (!m_FileName.isEmpty()) {
    m_WorkSpace->save(m_FileName);
  } else {
    fileSaveAs();
  }
}

void MainWindow::fileSaveAs()
{
  m_FileName = QFileDialog::getSaveFileName(this, "Choose a file name for saving", "./", "enVisu files (*.envisu)");
  if (!m_FileName.isEmpty()) {
    if (m_FileName.right(7) != ".envisu") {
      m_FileName += ".envisu";
    }
    m_WorkSpace->save(m_FileName);
    setCaption(version() + ": " + m_FileName);
  }
}

void MainWindow::filePrint()
{
  QPrinter pr;
  if (pr.setup()) {
    QPainter p(&pr);
    m_WorkSpace->hideBin();
    m_WorkSpace->canvas()->drawArea(m_WorkSpace->canvas()->rect(),&p);
    m_WorkSpace->showBin();
  }
}

void MainWindow::fileExit()
{
  QCoreApplication::exit();
}

void MainWindow::filterCreate()
{
  NewFilterDialog dlg;
  if (dlg.exec()) {
    dlg.createFiles();
  }
}


