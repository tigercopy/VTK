/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXdmf3Writer.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPXdmf3Writer - write eXtensible Data Model and Format files
// .SECTION Description
// vtkPXdmf3Writer converts vtkDataObjects to XDMF format. This is intended to
// replace vtkXdmfWriter, which is not up to date with the capabilities of the
// newer XDMF3 library. This writer understands VTK's composite data types and
// produces full trees in the output XDMF files.
//
//
//     In the absence of the information provided by vtkModelMetadata,
//     if this writer is not part of a parallel application, we will use
//     reasonable defaults for all the values in the output XDMF file.
//     If you don't provide a block ID element array, we'll create a
//     block for each cell type that appears in the unstructured grid.
//
//     However if this writer is part of a parallel application (hence
//     writing out a distributed XDMF file), then we need at the very
//     least a list of all the block IDs that appear in the file.  And
//     we need the element array of block IDs for the input unstructured grid.
//
//     In the absence of a vtkModelMetadata object, you can also provide
//     time step information which we will include in the output XDMF
//     file.
//
//  .SECTION Caveats
//     We use the terms "point" and "node" interchangeably.
//     Also, we use the terms "element" and "cell" interchangeably.

#ifndef vtkPXdmf3Writer_h
#define vtkPXdmf3Writer_h

#include "vtkIOParallelXdmf3Module.h" // For export macro

#include "vtkXdmf3Writer.h"

#include <vector> // STL Header
#include <map>    // STL Header
#include <string> // STL Header

class vtkModelMetadata;
class vtkDoubleArray;
class vtkIntArray;
class vtkUnstructuredGrid;

class VTKIOPARALLELXDMF3_EXPORT vtkPXdmf3Writer : public vtkXdmf3Writer
{
public:
  static vtkPXdmf3Writer *New ();
  vtkTypeMacro(vtkPXdmf3Writer,vtkXdmf3Writer);
  void PrintSelf (ostream& os, vtkIndent indent);

protected:
  vtkPXdmf3Writer ();
  ~vtkPXdmf3Writer ();
  virtual int CheckParameters ();

  virtual int RequestUpdateExtent (vtkInformation* request,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector);
  virtual int GlobalContinueExecuting(int localContinue);

private:
  vtkPXdmf3Writer (const vtkPXdmf3Writer&) VTK_DELETE_FUNCTION;
  void operator= (const vtkPXdmf3Writer&) VTK_DELETE_FUNCTION;
};

#endif /* vtkPXdmf3Writer_h */
