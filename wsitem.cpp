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

#include "wsitem.h"

WsItem::WsItem(WorkSpace *ws)
{
  m_Ws = ws;
  m_Icon        = NULL;
  m_Pixmaps     = NULL;
  m_ItemName   = QDateTime::currentDateTime().toString();
  m_ItemName   = " (created on " + m_ItemName + ")";
  m_HasOutput  = true;
  m_OutputTypes = "vtkPolyData";
  m_DummyVtkImplicitFunction = vtkSphere::New();
  m_DummyVtkDataSet          = vtkUnstructuredGrid::New();
  m_DummyVtkPolyData         = vtkPolyData::New();
}

WsItem::~WsItem()
{
  if (m_Icon)    delete m_Icon;
  if (m_Pixmaps) delete m_Pixmaps;
}

void WsItem::error(QString txt)
{
  QMessageBox::warning (m_Ws, "warning", txt, QMessageBox::Ok, QMessageBox::NoButton);
}

void WsItem::changeName(const QString &new_name)
{
  typename list<WsItem*>::iterator i;
  for (i = m_Ws->getItems()->begin(); i != m_Ws->getItems()->end(); ++i) {
    if ((*i) != this) {
      if ((*i)->name() == new_name) {
        error("This name exists already!");
        break;
      };
    };
  };
  m_ItemName = new_name;
  checkName();
}

void WsItem::checkName()
{
  list<WsItem*>::iterator i;
  for (i = m_Ws->getItems()->begin(); i != m_Ws->getItems()->end(); ++i) {
    if ((*i) != this) {
      if ((*i)->name() == m_ItemName) {
        m_ItemName += "!";
      }
    }
  }
  if (m_ItemName.stripWhiteSpace() == "") m_ItemName = "unknown";
}

void WsItem::setName(QString txt)
{
  m_ItemName = txt + m_ItemName;
  checkName();
}

void WsItem::loadIcon(QString file)
{
  m_Pixmaps = new Q3CanvasPixmapArray(QString(":/icons/icons/") + file);
  m_Icon    = new Q3CanvasSprite(m_Pixmaps,m_Ws->canvas());
}

WsItem *WsItem::findInput(int i)
{
  WsItem *item = NULL;
  list<WsConnection*>::iterator j;
  list<WsConnection*>* conn = m_Ws->getConnections();
  for (j = conn->begin(); j != conn->end(); ++j) {
    if ((*j)->inputSlot() == i) {
      if ((*j)->toItem() == this) {
        item = (*j)->fromItem();
        break;
      }
    }
  }
  return item;
}

void WsItem::addInput(QString input_name, QString type)
{
  m_Inputs.push_back(input_name);
  m_InputTypes.push_back(type);
}

void WsItem::save(QTextStream &s)
{
  QString save_name = name().replace(QChar(' '),QString("_"));
  s << save_name.ascii() << ' ' << getIcon()->x() << ' ' << getIcon()->y() << '\n';
}

void WsItem::load(QTextStream &s)
{
  double xr, yr;
  QString namer;
  s >> namer >> xr >> yr;
  m_ItemName = namer.replace(QChar('_'),QString(" "));
  getIcon()->setX(xr);
  getIcon()->setY(yr);
}

void WsItem::setDummyDataSet(int i)
{
  setDataSet(i,m_DummyVtkDataSet);
}

void WsItem::setDummyPolyData(int i)
{
  setPolyData(i,m_DummyVtkPolyData);
}

void WsItem::setDummyImplicitFunction(int i)
{
  setImplicitFunction(i,m_DummyVtkImplicitFunction);
}

void WsItem::readLineEdit(QTextStream &s, QLineEdit *edit)
{
  QString t;
  s >> t;
  edit->setText(t.replace('~', ' '));
}

void WsItem::writeLineEdit(QTextStream &s, QLineEdit *edit)
{
  s << edit->text().replace(' ', '~') << "\n";
}

void WsItem::readLabel(QTextStream &s, QLabel *label)
{
  QString t;
  s >> t;
  label->setText(t);
}

void WsItem::writeSlider(QTextStream &s, QSlider *slider)
{
  s << slider->maxValue() << '\n';
  s << slider->value()    << '\n';
}

void WsItem::writeCheckBox(QTextStream &s, QCheckBox *cb)
{
  if (cb->isChecked()) {
    s << "checked\n";
  } else {
    s << "not-checked\n";
  }
}

void WsItem::writeRadioButton(QTextStream &s, QRadioButton *rb)
{
  if (rb->isChecked()) {
    s << "checked\n";
  } else {
    s << "not-checked\n";
  }
}

void WsItem::readSlider(QTextStream &s, QSlider *slider)
{
  int t;
  s >> t;
  slider->setMaxValue(t);
  s >> t;
  slider->setValue(t);
}

void WsItem::readRadioButton(QTextStream &s, QRadioButton *rb)
{
  QString txt;
  s >> txt;
  if (txt == "checked") {
    rb->setChecked(true);
  } else {
    rb->setChecked(false);
  }
}

void WsItem::readCheckBox(QTextStream &s, QCheckBox *cb)
{
  QString txt;
  s >> txt;
  if (txt == "checked") {
    cb->setChecked(true);
  } else {
    cb->setChecked(false);
  }
}

void WsItem::writeComboBox(QTextStream &s, QComboBox *combo)
{
  s << combo->count() << '\n';
  for (int i = 0; i < combo->count(); ++i) {
    s << combo->text(i) << '\n';
  }
  s << combo->currentItem() << '\n';
}

void WsItem::writeSpinBox(QTextStream &s, QSpinBox *spin)
{
  s << spin->minValue() << '\n';
  s << spin->maxValue() << '\n';
  s << spin->value() << '\n';
}

void WsItem::readComboBox(QTextStream &s, QComboBox *combo)
{
  combo->clear();
  int N;
  s >> N;
  for (int i = 0; i < N; ++i) {
    QString t;
    s >> t;
    combo->insertItem(t);
  }
  s >> N;
  combo->setCurrentItem(N);
}

void WsItem::readSpinBox(QTextStream &s, QSpinBox *spin)
{
  int v;
  s >> v;
  spin->setMinValue(v);
  s >> v;
  spin->setMaxValue(v);
  s >> v;
  spin->setValue(v);
}

vec3_t WsItem::getVector(QLineEdit *edit)
{
  vec3_t x(0,0,0);
  QStringList words = edit->text().split(",", QString::SkipEmptyParts);
  for (int i = 0; i < min(3, words.size()); ++i) {
    x[i] = words[i].toDouble();
  }
}

void WsItem::setVector(vec3_t x, QLineEdit *edit)
{
  QString num;
  QString txt = "";
  num.setNum(x[0]);
  txt += num + ", ";
  num.setNum(x[1]);
  txt += num + ", ";
  num.setNum(x[2]);
  edit->setText(txt);
}
