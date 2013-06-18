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

#include <QFile>
#include <QTextStream>

#include "newfilterdialog.h"
#include "ui_inputeditdialog.h"

NewFilterDialog::NewFilterDialog(QWidget *parent) : QDialog(parent)
{
  m_Ui.setupUi(this);
  connect(m_Ui.pushButtonDelete, SIGNAL(clicked()), this, SLOT(deleteInput()));
  connect(m_Ui.pushButtonEdit, SIGNAL(clicked()), this, SLOT(editInput()));
  connect(m_Ui.pushButtonNew, SIGNAL(clicked()), this, SLOT(newInput()));
}

void NewFilterDialog::newInput()
{
  m_Ui.inputs_lb->insertItem("vtkPolyData/DataSet : new_input");
  //                          1234567890123456789012345678901
  //                                   1         2         3
}

void NewFilterDialog::editInput()
{
  if (m_Ui.inputs_lb->currentItem() != -1) {
    Ui::InputEditDialog ui;
    QDialog dlg(this);
    ui.setupUi(&dlg);
    QString type = m_Ui.inputs_lb->currentText().left(22);
    if        (type == "vtkPolyData         : ") {
      ui.polydata_rb->setChecked(true);
    } else if (type == "vtkPolyData/DataSet : ") {
      ui.polyset_rb->setChecked(true);
    } else if (type == "vtkDataSet          : ") {
      ui.dataset_rb->setChecked(true);
    } else if (type == "vtkImplicitFunction : ") {
      ui.function_rb->setChecked(true);
    }
    QString name = m_Ui.inputs_lb->currentText().right(m_Ui.inputs_lb->currentText().length()-22);
    ui.name_edit->setText(name);
    if (dlg.exec()) {
      int i = m_Ui.inputs_lb->currentItem();
      if (ui.polydata_rb->isChecked()) {
        m_Ui.inputs_lb->changeItem("vtkPolyData         : " + ui.name_edit->text(),i);
      } else if (ui.polyset_rb->isChecked()) {
        m_Ui.inputs_lb->changeItem("vtkPolyData/DataSet : " + ui.name_edit->text(),i);
      } else if (ui.dataset_rb->isChecked()) {
        m_Ui.inputs_lb->changeItem("vtkDataSet          : " + ui.name_edit->text(),i);
      } else if (ui.function_rb->isChecked()) {
        m_Ui.inputs_lb->changeItem("vtkImplicitFunction : " + ui.name_edit->text(),i);
      }
    }
  }
}

void NewFilterDialog::deleteInput()
{
  m_Ui.inputs_lb->removeItem(m_Ui.inputs_lb->currentItem());
}

void NewFilterDialog::createFiles()
{
  QString class_name     = m_Ui.class_edit->text();
  QString h_file_name    = "new_filters/" + class_name.lower() + ".h";
  QString cpp_file_name  = "new_filters/" + class_name.lower() + ".cpp";
  QFile h_file(h_file_name);
  QFile cpp_file(cpp_file_name);
  h_file.open(QIODevice::WriteOnly);
  cpp_file.open(QIODevice::WriteOnly);
  {
    QTextStream h(&h_file);
    QTextStream cpp(&cpp_file);
    h << "//                                                                         \n";
    h << "// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    h << "// +                                                                      +\n";
    h << "// + This file is part of enVisu.                                         +\n";
    h << "// +                                                                      +\n";
    h << "// + Copyright 2013 O. Gloth, enGits GmbH                                 +\n";
    h << "// +                                                                      +\n";
    h << "// + enGrid is free software: you can redistribute it and/or modify       +\n";
    h << "// + it under the terms of the GNU General Public License as published by +\n";
    h << "// + the Free Software Foundation, either version 3 of the License, or    +\n";
    h << "// + (at your option) any later version.                                  +\n";
    h << "// +                                                                      +\n";
    h << "// + enGrid is distributed in the hope that it will be useful,            +\n";
    h << "// + but WITHOUT ANY WARRANTY; without even the implied warranty of       +\n";
    h << "// + MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        +\n";
    h << "// + GNU General Public License for more details.                         +\n";
    h << "// +                                                                      +\n";
    h << "// + You should have received a copy of the GNU General Public License    +\n";
    h << "// + along with enVisu. If not, see <http://www.gnu.org/licenses/>.       +\n";
    h << "// +                                                                      +\n";
    h << "// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    h << "//                                                                         \n";

    h << "#ifndef " << m_Ui.class_edit->text().lower() << "_H\n";
    h << "#define " << m_Ui.class_edit->text().lower() << "_H\n";
    h << "\n";
    h << "class " << class_name << ";\n";
    h << "\n";
    h << "#include \"mpwsitem.h\"\n";
    h << "#include \"" << class_name.lower() << "config.h\"\n";
    h << "\n";
    h << "#include <" << m_Ui.vtk_edit->text() << ".h>\n";
    h << "\n";
    h << "class " << class_name << " : public MpWsItem\n";
    h << "{\n";
    h << "\n";
    h << "    Q_OBJECT\n";
    h << "\n";
    h << "private:\n";
    h << "\n";
    h << "    " << class_name << "Config dlg;\n";
    h << "    " << m_Ui.vtk_edit->text() << " *vtk;\n";
    h << "\n";
    h << "public:\n";
    h << "\n";
    h << "    " << class_name << " (MpWorkSpace *mws);\n";
    h << "    ~" << class_name << "();\n";
    h << "\n";
    if (m_Ui.polydata_rb->isChecked()) {
      h << "    virtual vtkPolyData* getPolyData();\n";
    } else if (m_Ui.dataset_rb->isChecked()) {
      h << "    virtual vtkDataSet* getDataSet();\n";
    } else if (m_Ui.function_rb->isChecked()) {
      h << "    virtual vtkImplicitFunction* getImplicitFunction();\n";
    };
    h << "\n";
    h << "    virtual void setPolyData        (int i, vtkPolyData         *poly_data);\n";
    h << "    virtual void setDataSet         (int i, vtkDataSet          *data_set);\n";
    h << "    virtual void setImplicitFunction(int i, vtkImplicitFunction *implicit_function);\n";
    h << "    virtual void connectInput       (int i);\n";
    h << "    virtual void disconnectInput    (int i);\n";
    h << "    virtual void connectOutput      (int i);\n";
    h << "    virtual void disconnectOutput   (int i);\n";
    h << "\n";
    h << "public slots:\n";
    h << "\n";
    h << "    virtual void apply ();\n";
    h << "    virtual void config();\n";
    h << "    virtual void help  ();\n";
    h << "    virtual void load  (istream &s);\n";
    h << "    virtual void save  (ostream &s);\n";
    h << "\n";
    h << "};\n";
    h << "\n";
    h << "#endif\n";
    cpp << "#include \"" << m_Ui.class_edit->text().lower() << ".h\"\n";
    cpp << "\n";
    cpp << class_name << "::" << class_name << "(MpWorkSpace *mws) :\n";
    cpp << "        MpWsItem(mws)\n";
    cpp << "{\n";
    cpp << "    loadIcon(\"" << class_name.lower() << ".png\");\n";
    cpp << "    ws->addItem(this);\n";
    cpp << "    setName(\"" << class_name << "\");\n";
    cpp << "    vtk = " << m_Ui.vtk_edit->text() << "::New();\n";
    for (int i = 0; i < m_Ui.inputs_lb->count(); ++i) {
      QString text = m_Ui.inputs_lb->text(i);
      QString type = text.left(19).stripWhiteSpace();
      QString name = text.right(text.length()-22).stripWhiteSpace();
      cpp << "    addInput(\"" << name << "\",\"" << type << "\");\n";
    }
    cpp << "    connect(dlg.apply_pb,SIGNAL(clicked()),this,SLOT(apply()));\n";
    cpp << "    connect(dlg.help_pb,SIGNAL(clicked()),this,SLOT(help()));\n";
    cpp << "    dlg.name_edit->setText(name());\n";
    if (m_Ui.polydata_rb->isChecked()) {
      cpp << "    has_output = true;\n";
      cpp << "    output_type = \"vtkPolyData\";\n";
    } else if (m_Ui.dataset_rb->isChecked()) {
      cpp << "    has_output = true;\n";
      cpp << "    output_type = \"vtkDataSet\";\n";
    } else if (m_Ui.function_rb->isChecked()) {
      cpp << "    has_output = true;\n";
      cpp << "    output_type = \"vtkImplicitFunction\";\n";
    } else {
      cpp << "    has_output = false;\n";
    };
    cpp << "    apply();\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << class_name << "::~" << class_name << "()\n";
    cpp << "{\n";
    cpp << "    vtk->Delete();\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::apply()\n";
    cpp << "{\n";
    cpp << "    changeName(dlg.name_edit->text());\n";
    cpp << "    ws->render();\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::config()\n";
    cpp << "{\n";
    cpp << "    if (dlg.exec()) apply();\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::save(ostream &s)\n";
    cpp << "{\n";
    cpp << "    MpWsItem::save(s);\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::load(istream &s)\n";
    cpp << "{\n";
    cpp << "    MpWsItem::load(s);\n";
    cpp << "    apply();\n";
    cpp << "};\n";
    cpp << "\n";

    if (m_Ui.polydata_rb->isChecked()) {
      cpp << "vtkPolyData* " << class_name;
      cpp << "::getPolyData()\n";
      cpp << "{\n";
      cpp << "    return vtk->GetOutput();\n";
      cpp << "};\n";
    } else if (m_Ui.dataset_rb->isChecked()) {
      cpp << "vtkDataSet* " << class_name;
      cpp << "::getDataSet()\n";
      cpp << "{\n";
      cpp << "    return vtk->GetOutput();\n";
      cpp << "};\n";
    } else if (m_Ui.function_rb->isChecked()) {
      cpp << "vtkImplicitFunction* " << class_name;
      cpp << "::getImplicitFunction()\n";
      cpp << "{\n";
      cpp << "    return vtk;\n";
      cpp << "};\n";
    };
    cpp << "\n";
    cpp << "void " << class_name << "::setPolyData(int i, vtkPolyData *poly_data)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::setDataSet(int i, vtkDataSet *data_set)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::setImplicitFunction(int i, vtkImplicitFunction *function)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::connectInput(int i)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::disconnectInput(int i)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::connectOutput(int i)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::disconnectOutput(int i)\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
    cpp << "void " << class_name << "::help()\n";
    cpp << "{\n";
    cpp << "};\n";
    cpp << "\n";
  }
}
