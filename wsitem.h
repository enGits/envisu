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

#ifndef GUIWSITEMBASE_H
#define GUIWSITEMBASE_H

class WsItem;

#include "envisu.h"
#include "workspace.h"

#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkSphere.h>

#include <vector>

#include <QDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>

//Added by qt3to4:
#include <QLabel>


using namespace std;

class WsItem : public QObject
{
    Q_OBJECT

protected:

  vtkPolyData         *m_DummyVtkPolyData;
  vtkUnstructuredGrid *m_DummyVtkDataSet;
  vtkSphere           *m_DummyVtkImplicitFunction;
  WorkSpace           *m_Ws;
  Q3CanvasSprite      *m_Icon;
  Q3CanvasPixmapArray *m_Pixmaps;
  QString              m_ImagePath;
  QString              m_ItemName;
  bool                 m_HasOutput;
  vector<QString>      m_Inputs;
  vector<QString>      m_InputTypes;
  QString              m_OutputTypes;

  void error   (QString txt);
  void setName (QString txt);
  void loadIcon(QString file);

  void writeLineEdit   (QTextStream &s, QLineEdit    *edit);
  void writeLabel      (QTextStream &s, QLabel       *label)  { s << label->text() << '\n'; }
  void writeSlider     (QTextStream &s, QSlider      *slider);
  void writeRadioButton(QTextStream &s, QRadioButton *rb);
  void writeCheckBox   (QTextStream &s, QCheckBox    *cb);
  void writeComboBox   (QTextStream &s, QComboBox    *combo);
  void writeSpinBox    (QTextStream &s, QSpinBox     *spin);
  void readLineEdit    (QTextStream &s, QLineEdit    *edit);
  void readLabel       (QTextStream &s, QLabel       *label);
  void readSlider      (QTextStream &s, QSlider      *slider);
  void readRadioButton (QTextStream &s, QRadioButton *rb);
  void readCheckBox    (QTextStream &s, QCheckBox    *cb);
  void readComboBox    (QTextStream &s, QComboBox    *combo);
  void readSpinBox     (QTextStream &s, QSpinBox     *spin);

  vec3_t getVector(QLineEdit *edit);
  void   setVector(vec3_t x, QLineEdit *edit);

  WsItem* findInput(int i);

public:

  WsItem(WorkSpace *mpws);
  ~WsItem();
  Q3CanvasSprite* getIcon() { return m_Icon; }
  bool    hasOutput() { return m_HasOutput; }
  void    addInput(QString input_name, QString type = "vtkPolyData");
  void    checkName();
  int     numInputs() { return m_Inputs.size(); }
  QString inputName(int i) { return m_Inputs[i]; }
  QString inputType(int i) { return m_InputTypes[i]; }
  QString outputType()     { return m_OutputTypes; }

  virtual void setPolyData        (int i, vtkPolyData         *poly_data)           {}
  virtual void setDataSet         (int i, vtkDataSet          *data_set)            {}
  virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function)   {}

  virtual void connectOutput    () {}
  virtual void connectInput     (int i) {}
  virtual void disconnectOutput () {}
  virtual void disconnectInput  (int i) {}

public slots:

  virtual void config() {}

  QString name      () { return m_ItemName; }
  void    changeName(const QString &new_name);

  virtual vtkDataSet*          getDataSet()          { return NULL; }
  virtual vtkPolyData*         getPolyData()         { return NULL; }
  virtual vtkImplicitFunction* getImplicitFunction() { return NULL; }

  virtual void setDummyDataSet         (int i);
  virtual void setDummyPolyData        (int i);
  virtual void setDummyImplicitFunction(int i);

  virtual void load(QTextStream &s) = 0;
  virtual void save(QTextStream &s) = 0;

  virtual void update() {}

};

#endif // GUIWSITEMBASE_H
