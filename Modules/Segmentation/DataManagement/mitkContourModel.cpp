/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#include <mitkContourModel.h>

mitk::ContourModel::ContourModel()
{
  //set to initial state
  this->InitializeEmpty();
}



mitk::ContourModel::ContourModel(const mitk::ContourModel &other) :
m_ContourSeries(other.m_ContourSeries), m_lineInterpolation(other.m_lineInterpolation)
{
  m_SelectedVertex = NULL;
}



mitk::ContourModel::~ContourModel()
{
  m_SelectedVertex = NULL;
  this->m_ContourSeries.clear();//TODO check destruction
}



void mitk::ContourModel::AddVertex(mitk::Point3D &vertex, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep) )
  {
    this->AddVertex(vertex, false, timestep);
  }
}



void mitk::ContourModel::AddVertex(mitk::Point3D &vertex, bool isActive, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
      this->m_ContourSeries[timestep]->AddVertex(vertex, isActive);
      this->InvokeEvent( ContourModelSizeChangeEvent() );
      this->Modified();
  }
}



void mitk::ContourModel::InsertVertexAtIndex(mitk::Point3D &vertex, int index, bool isActive, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    if(index > 0 && this->m_ContourSeries[timestep]->GetSize() > index)
    {
      this->m_ContourSeries[timestep]->InsertVertexAtIndex(vertex, isActive, index);
      this->InvokeEvent( ContourModelSizeChangeEvent() );
      this->Modified();
    }
  }
}



int mitk::ContourModel::GetNumberOfVertices( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->GetSize();
  }
  return -1;
}



const mitk::ContourModel::VertexType* mitk::ContourModel::GetVertexAt(int index, int timestep) const
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->GetVertexAt(index);
  }
  return NULL;
}



void mitk::ContourModel::Close( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    this->m_ContourSeries[timestep]->Close();
    this->InvokeEvent( ContourModelClosedEvent() );
    this->Modified();
  }
}


void mitk::ContourModel::Open( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    this->m_ContourSeries[timestep]->Open();
    this->InvokeEvent( ContourModelClosedEvent() );
    this->Modified();
  }
}


void mitk::ContourModel::SetIsClosed(bool isClosed, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    this->m_ContourSeries[timestep]->SetIsClosed(isClosed);
    this->InvokeEvent( ContourModelClosedEvent() );
    this->Modified();
  }
}



bool mitk::ContourModel::IsEmptyTimeStep( int t) const
{
  return (t < 0) || (this->m_ContourSeries.size() <= t);
}



void mitk::ContourModel::Concatenate(mitk::ContourModel* other, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    if( !this->m_ContourSeries[timestep]->IsClosed() )
    {
      this->m_ContourSeries[timestep]->Concatenate(other->m_ContourSeries[timestep]);
      this->InvokeEvent( ContourModelSizeChangeEvent() );
      this->Modified();
    }
  }
}



mitk::ContourModel::VertexIterator mitk::ContourModel::IteratorBegin( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->ConstIteratorBegin();
  }
  else
  {
    throw std::exception("invalid timestep");// << timestep << "only " << this->GetTimeSteps() << " available.");
  }
}



mitk::ContourModel::VertexIterator mitk::ContourModel::IteratorEnd( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->ConstIteratorEnd();
  }
  else
  {
    throw std::exception("invalid timestep");// + timestep + "only " + this->GetTimeSteps() + " available.");
  }
}



bool mitk::ContourModel::IsClosed( int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->IsClosed();
  }
  return false;
}



bool mitk::ContourModel::SelectVertexAt(int index, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return (this->m_SelectedVertex = this->m_ContourSeries[timestep]->GetVertexAt(index));
  }
  return false;
}



bool mitk::ContourModel::SelectVertexAt(mitk::Point3D &point, float eps, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    this->m_SelectedVertex = this->m_ContourSeries[timestep]->GetVertexAt(point, eps);
  }
  return this->m_SelectedVertex != NULL;
}



bool mitk::ContourModel::RemoveVertexAt(int index, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    this->m_ContourSeries[timestep]->RemoveVertexAt(index);
    this->Modified();
    this->InvokeEvent( ContourModelSizeChangeEvent() );
    return true;
  }
  return false;
}



bool mitk::ContourModel::RemoveVertexAt(mitk::Point3D &point, float eps, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    return this->m_ContourSeries[timestep]->RemoveVertexAt(point, eps);
    this->Modified();
    this->InvokeEvent( ContourModelSizeChangeEvent() );
    return true;
  }
  return false;
}



void mitk::ContourModel::ShiftSelectedVertex(mitk::Vector3D &translate)
{
  if(this->m_SelectedVertex)
  {
    this->ShiftVertex(this->m_SelectedVertex,translate);
    this->Modified();
  }
}



void mitk::ContourModel::ShiftContour(mitk::Vector3D &translate, int timestep)
{
  if(!this->IsEmptyTimeStep(timestep))
  {
    VertexListType* vList = this->m_ContourSeries[timestep]->GetVertexList();
    VertexIterator it = vList->begin();
    VertexIterator end = vList->end();

    while(it != end)
    {
      this->ShiftVertex((*it),translate);
      it++;
    }

    this->Modified();
    this->InvokeEvent( ContourModelShiftEvent() );
  }
}



void mitk::ContourModel::ShiftVertex(VertexType* vertex, mitk::Vector3D &vector)
{
  vertex->Coordinates[0] += vector[0];
  vertex->Coordinates[1] += vector[1];
  vertex->Coordinates[2] += vector[2];
}



void mitk::ContourModel::Expand( int timeSteps )
{
  int oldSize = this->m_ContourSeries.size();

  if( timeSteps > 0 && timeSteps > oldSize )
  {
    Superclass::Expand(timeSteps);

    for( int i = oldSize; i < timeSteps; i++)
    {
      m_ContourSeries.push_back(mitk::ContourModelElement::New());
    }
    
    this->InvokeEvent( ContourModelExpandTimeBoundsEvent() );
  }
}



void mitk::ContourModel::SetRequestedRegionToLargestPossibleRegion ()
{
  //no support for regions
}



bool mitk::ContourModel::RequestedRegionIsOutsideOfTheBufferedRegion ()
{
  //no support for regions
  return false;
}



bool mitk::ContourModel::VerifyRequestedRegion ()
{
  //no support for regions
  return true;
}



const mitk::Geometry3D * mitk::ContourModel::GetUpdatedGeometry (int t)
{
  return Superclass::GetUpdatedGeometry(t);
}



mitk::Geometry3D* mitk::ContourModel::GetGeometry (int t)const
{
  return Superclass::GetGeometry(t);
}



void mitk::ContourModel::SetRequestedRegion (itk::DataObject *data)
{
  //no support for regions
}


void mitk::ContourModel::Clear()
{
  //clear data and set to initial state again
  this->ClearData();
  this->InitializeEmpty();
  this->Modified();
}



void mitk::ContourModel::ClearData()
{
  //call the superclass, this releases the data of BaseData
  Superclass::ClearData();

  //clear out the time resolved contours
  this->m_ContourSeries.clear();
}



void mitk::ContourModel::InitializeEmpty()
{
  //clear data at timesteps
  this->m_ContourSeries.resize(0);
  this->m_ContourSeries.push_back(mitk::ContourModelElement::New());

  //set number of timesteps to one
  this->InitializeTimeSlicedGeometry(1);

  m_SelectedVertex = NULL;
  this->m_lineInterpolation = ContourModel::LINEAR;
}



void mitk::ContourModel::UpdateOutputInformation()
{
  
  if ( this->GetSource() )
  {
    this->GetSource()->UpdateOutputInformation();
  }



  //update the bounds of the geometry according to the stored vertices
  float mitkBounds[6];


  //calculate the boundingbox at each timestep

  typedef itk::BoundingBox<unsigned long, 3, ScalarType>        BoundingBoxType;
  typedef BoundingBoxType::PointsContainer                      PointsContainer;

  int timesteps = this->GetTimeSteps();

  //iterate over the timesteps
  for(int currenTimeStep = 0; currenTimeStep < timesteps; currenTimeStep++)
  {
    //if no controlPoints are available the boundingbox is 0 in all dimensions
    if (this->GetMTime() > this->GetGeometry(currenTimeStep)->GetBoundingBox()->GetMTime())
    {
      mitkBounds[0] = 0.0;
      mitkBounds[1] = 0.0;
      mitkBounds[2] = 0.0;
      mitkBounds[3] = 0.0;
      mitkBounds[4] = 0.0;
      mitkBounds[5] = 0.0;

      BoundingBoxType::Pointer boundingBox = BoundingBoxType::New();

      PointsContainer::Pointer points = PointsContainer::New();

      VertexIterator it = this->IteratorBegin(currenTimeStep);
      VertexIterator end = this->IteratorEnd(currenTimeStep);

      //fill the boundingbox with the points
      while(it != end)
      {
        Point3D currentP = (*it)->Coordinates;
        BoundingBoxType::PointType p;
        p.CastFrom(currentP);
        points->InsertElement(points->Size(), p);

        it++;
      }

      //construct the new boundingBox
      boundingBox->SetPoints(points);
      boundingBox->ComputeBoundingBox();
      BoundingBoxType::BoundsArrayType tmp = boundingBox->GetBounds();
      mitkBounds[0] = tmp[0];
      mitkBounds[1] = tmp[1];
      mitkBounds[2] = tmp[2];
      mitkBounds[3] = tmp[3];
      mitkBounds[4] = tmp[4];
      mitkBounds[5] = tmp[5];

      Geometry3D* geometry3d = this->GetGeometry(currenTimeStep);
      geometry3d->SetBounds(mitkBounds);
    }

  }
  GetTimeSlicedGeometry()->UpdateInformation();
}




void mitk::ContourModel::ExecuteOperation(mitk::Operation* operation)
{

}