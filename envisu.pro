#
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +                                                                      +
# + This file is part of enVisu.                                         +
# +                                                                      +
# + Copyright 2013 O. Gloth, enGits GmbH                                 +
# +                                                                      +
# + enGrid is free software: you can redistribute it and/or modify       +
# + it under the terms of the GNU General Public License as published by +
# + the Free Software Foundation, either version 3 of the License, or    +
# + (at your option) any later version.                                  +
# +                                                                      +
# + enGrid is distributed in the hope that it will be useful,            +
# + but WITHOUT ANY WARRANTY; without even the implied warranty of       +
# + MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        +
# + GNU General Public License for more details.                         +
# +                                                                      +
# + You should have received a copy of the GNU General Public License    +
# + along with enVisu. If not, see <http:#www.gnu.org/licenses/>.       +
# +                                                                      +
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#

TEMPLATE	= app
LANGUAGE	= C++

CONFIG	         += qt warn_off debug thread
QT               += qt3support
#QMAKE_CXXFLAGS += -std=c++0x

LIBS += -lvtkCommon -lvtkGraphics -lvtkImaging -lvtkHybrid -lvtkFiltering -lvtkRendering -lvtkIO -lQVTK
LIBS += -L$(VTKLIBDIR)

INCLUDEPATH += $(VTKINCDIR)

HEADERS	+= \
	qtincludes.h \
	items.h \
    math/smallsquarematrix.h \
    math/mathvector_structs.h \
    math/mathvector_operators.h \
    math/mathvector_methods.h \
    math/mathvector.h \
    math/linsolve.h \
    asciipointsfile.h \
    axisymmetry.h \
    cell2node.h \
    clipper.h \
    contour.h \
    acummulate.h \
    cutter.h \
    geometry.h \
    plane.h \
    polydatareader.h \
    sphere.h \
    tubes.h \
    unstructuredgridreader.h \
    wsconnection.h \
    cylinder.h \
    mainwindow.h \
    workspace.h \
    vtkdisplay.h \
    guiwsitem.h \
    wsitem.h \
    inputdialog.h \
    newfilterdialog.h \
    envisu.h \
    reader.h

SOURCES	+= main.cpp \
    cylinder.cpp \
    mainwindow.cpp \
    workspace.cpp \
    asciipointsfile.cpp \
    axisymmetry.cpp \
    cell2node.cpp \
    clipper.cpp \
    contour.cpp \
    acummulate.cpp \
    wsconnection.cpp \
    cutter.cpp \
    geometry.cpp \
    plane.cpp \
    polydatareader.cpp \
    sphere.cpp \
    tubes.cpp \
    unstructuredgridreader.cpp \
    vtkdisplay.cpp \
    inputdialog.cpp \
    wsitem.cpp \
    newfilterdialog.cpp \
    reader.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS	= \
    asciipointsfileconfig.ui \
    axisymmetryconfig.ui \
    clipperconfig.ui \
    contourconfig.ui \
    cylconfigdialog.ui \
    inputdialog.ui \
    inputeditdialog.ui \
    mainwindow.ui \
    planeconfig.ui \
    newfilterdialog.ui \
    polydatareaderconfig.ui \
    sphereconfig.ui \
    tubesconfig.ui \
    wsitemconfig.ui \
    vtkdisplayconfig.ui \
    readerconfig.ui

RESOURCES += \
    images.qrc


