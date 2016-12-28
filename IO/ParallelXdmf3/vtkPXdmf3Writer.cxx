/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPXdmf3Writer.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "XdmfDomain.hpp"
#include "XdmfGridCollection.hpp"
#include "XdmfGridCollectionType.hpp"
#include "XdmfHeavyDataWriter.hpp"
#include "XdmfWriter.hpp"

#include "vtkPXdmf3Writer.h"

#include "vtkDataObject.h"
#include "vtkDirectedGraph.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkObjectFactory.h"
#include "vtkPointSet.h"
#include "vtkRectilinearGrid.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStructuredGrid.h"
#include "vtkXdmf3DataSet.h"

#include "vtkMultiProcessController.h"


#include <stack>

vtkStandardNewMacro (vtkPXdmf3Writer);

//----------------------------------------------------------------------------

vtkPXdmf3Writer::vtkPXdmf3Writer ()
{
}

vtkPXdmf3Writer::~vtkPXdmf3Writer ()
{
}

void vtkPXdmf3Writer::PrintSelf (ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
int vtkPXdmf3Writer::CheckParameters ()
{
  vtkMultiProcessController *c = vtkMultiProcessController::GetGlobalController();
  int numberOfProcesses = c ? c->GetNumberOfProcesses() : 1;
  int myRank = c ? c->GetLocalProcessId() : 0;

  if (this->GhostLevel > 0)
  {
    vtkWarningMacro(<< "Xdmf3Writer ignores ghost level request");
  }

  return this->Superclass::CheckParametersInternal(numberOfProcesses, myRank);
  //return 0;
}

//----------------------------------------------------------------------------
int vtkPXdmf3Writer::RequestUpdateExtent (
  vtkInformation* request,
  vtkInformationVector** inputVector,
  vtkInformationVector* outputVector)
{
  this->Superclass::RequestUpdateExtent(request, inputVector, outputVector);
  vtkMultiProcessController *c = vtkMultiProcessController::GetGlobalController();
  if (c)
  {
    int numberOfProcesses = c->GetNumberOfProcesses();
    int myRank = c->GetLocalProcessId();

    vtkInformation *info = inputVector[0]->GetInformationObject(0);
    info->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(), myRank);
    info->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(), numberOfProcesses);
  }

  return 1;
}

//----------------------------------------------------------------------------
int vtkPXdmf3Writer::GlobalContinueExecuting(int localContinue)
{
  vtkMultiProcessController *c =
    vtkMultiProcessController::GetGlobalController();
  int globalContinue = localContinue;
  if (c)
  {
    c->AllReduce (&localContinue, &globalContinue, 1, vtkCommunicator::MIN_OP);
  }
  return globalContinue;
}
