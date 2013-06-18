#include "mpcummulate.h"

MpCummulate::MpCummulate(MpWorkSpace *mws) :
	MpWsItem(mws)
{
    loadIcon("cummulate.png");
    ws->addItem(this);
    setName("cummulate");
    output_type = "vtkPolyData";
    cummulate = vtk::New();
    dummy = vtkPolyData::New();
    c2n->SetInput(dummy);
    addInput("poly data", "vtkPolyData/vtkDataSet");
};

MpCell2Node::~MpCell2Node()
{
    c2n->Delete();
};
