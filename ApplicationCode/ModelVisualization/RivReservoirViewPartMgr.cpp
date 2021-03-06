/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RIStdInclude.h"
#include "RivReservoirViewPartMgr.h"
#include "RivGridPartMgr.h"
#include "RimReservoirView.h"
#include "RigReservoir.h"
#include "RigGridBase.h"
#include "RigReservoirCellResults.h"
#include "RigGridScalarDataAccess.h"

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RivReservoirViewPartMgr::RivReservoirViewPartMgr(RimReservoirView * resv) :
m_reservoirView(resv)
{
    m_scaleTransform = new cvf::Transform();
    clearGeometryCache();
}


//--------------------------------------------------------------------------------------------------
/// Clears the geometry cache for the given, and the dependent geometryTypes (from a visibility standpoint)
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::scheduleGeometryRegen(ReservoirGeometryCacheType geometryType)
{
    switch (geometryType)
    {
    case INACTIVE:
        clearGeometryCache(INACTIVE);
        clearGeometryCache(RANGE_FILTERED_INACTIVE);
        break;  
    case RANGE_FILTERED_INACTIVE:
        clearGeometryCache(RANGE_FILTERED_INACTIVE);
        break;
    case ACTIVE:
        clearGeometryCache(ACTIVE);
        clearGeometryCache(ALL_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS);
        clearGeometryCache(RANGE_FILTERED);
        clearGeometryCache(RANGE_FILTERED_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case ALL_WELL_CELLS:
        clearGeometryCache(ALL_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS);
        clearGeometryCache(RANGE_FILTERED);
        clearGeometryCache(RANGE_FILTERED_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case VISIBLE_WELL_CELLS:
        clearGeometryCache(VISIBLE_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case VISIBLE_WELL_FENCE_CELLS:
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case RANGE_FILTERED:
        clearGeometryCache(RANGE_FILTERED);
        clearGeometryCache(RANGE_FILTERED_INACTIVE);
        clearGeometryCache(RANGE_FILTERED_WELL_CELLS);
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case RANGE_FILTERED_WELL_CELLS:
        clearGeometryCache(RANGE_FILTERED_WELL_CELLS);
        clearGeometryCache(RANGE_FILTERED);
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER:
        clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER:
        clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case PROPERTY_FILTERED:
        clearGeometryCache(PROPERTY_FILTERED);
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    case PROPERTY_FILTERED_WELL_CELLS:
        clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
        break;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::clearGeometryCache(ReservoirGeometryCacheType geomType)
{
    RigReservoir* reservoir = NULL;
    if (m_reservoirView != NULL && m_reservoirView->eclipseCase())
    {
        reservoir = m_reservoirView->eclipseCase()->reservoirData();
    }

    if (geomType == PROPERTY_FILTERED)
    {
        for (size_t i = 0; i < m_propFilteredGeometryFramesNeedsRegen.size(); ++i)
        {
            m_propFilteredGeometryFramesNeedsRegen[i] = true;
            if (m_propFilteredGeometryFrames[i].notNull())
            {
                m_propFilteredGeometryFrames[i]->clearAndSetReservoir(reservoir);
                m_propFilteredGeometryFrames[i]->setTransform(m_scaleTransform.p());
            }
        }
    }
    else if (geomType == PROPERTY_FILTERED_WELL_CELLS)
    {
        for (size_t i = 0; i < m_propFilteredWellGeometryFramesNeedsRegen.size(); ++i)
        {
            m_propFilteredWellGeometryFramesNeedsRegen[i] = true;
            if (m_propFilteredWellGeometryFrames[i].notNull())
            {
                m_propFilteredWellGeometryFrames[i]->clearAndSetReservoir(reservoir);
                m_propFilteredWellGeometryFrames[i]->setTransform(m_scaleTransform.p());
            }
        }
    }
    else
    {
        m_geometriesNeedsRegen[geomType] = true;
        m_geometries[geomType].clearAndSetReservoir(reservoir);
        m_geometries[geomType].setTransform(m_scaleTransform.p());
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::clearGeometryCache()
{
    clearGeometryCache(ACTIVE);
    clearGeometryCache(ALL_WELL_CELLS);
    clearGeometryCache(VISIBLE_WELL_CELLS);
    clearGeometryCache(VISIBLE_WELL_FENCE_CELLS);
    clearGeometryCache(INACTIVE);
    clearGeometryCache(RANGE_FILTERED);
    clearGeometryCache(RANGE_FILTERED_WELL_CELLS);
    clearGeometryCache(RANGE_FILTERED_INACTIVE);
    clearGeometryCache(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
    clearGeometryCache(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);
    clearGeometryCache(PROPERTY_FILTERED);
    clearGeometryCache(PROPERTY_FILTERED_WELL_CELLS);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::appendStaticGeometryPartsToModel(cvf::ModelBasicList* model, ReservoirGeometryCacheType geometryType, 
                                                                const std::vector<size_t>& gridIndices)
{
    if (m_geometriesNeedsRegen[geometryType])
    {
        createGeometry( geometryType);
    }
    m_geometries[geometryType].appendPartsToModel(model, gridIndices);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::appendDynamicGeometryPartsToModel(cvf::ModelBasicList* model, ReservoirGeometryCacheType geometryType, 
                                                                 size_t frameIndex, const std::vector<size_t>& gridIndices)
{
    if (geometryType == PROPERTY_FILTERED)
    {
        if (frameIndex >= m_propFilteredGeometryFramesNeedsRegen.size() || m_propFilteredGeometryFramesNeedsRegen[frameIndex])
        {
            createPropertyFilteredGeometry(frameIndex);
        }
        m_propFilteredGeometryFrames[frameIndex]->appendPartsToModel(model, gridIndices);
    }
    else if (geometryType == PROPERTY_FILTERED_WELL_CELLS)
    {
        if (frameIndex >= m_propFilteredWellGeometryFramesNeedsRegen.size() || m_propFilteredWellGeometryFramesNeedsRegen[frameIndex])
        {
            createPropertyFilteredWellGeometry(frameIndex);
        }
        m_propFilteredWellGeometryFrames[frameIndex]->appendPartsToModel(model, gridIndices);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::createGeometry(ReservoirGeometryCacheType geometryType)
{
    RigReservoir* res = m_reservoirView->eclipseCase()->reservoirData();
    m_geometries[geometryType].clearAndSetReservoir(res);
    m_geometries[geometryType].setTransform(m_scaleTransform.p());
    std::vector<RigGridBase*> grids;
    res->allGrids(&grids);

    for (size_t i = 0; i < grids.size(); ++i)
    {
        cvf::ref<cvf::UByteArray> cellVisibility = m_geometries[geometryType].cellVisibility(i); 
        computeVisibility(cellVisibility.p(), geometryType, grids[i], i);

        m_geometries[geometryType].setCellVisibility(i, cellVisibility.p());
    }

    m_geometriesNeedsRegen[geometryType] = false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::computeVisibility(cvf::UByteArray* cellVisibility, ReservoirGeometryCacheType geometryType, RigGridBase* grid, size_t gridIdx)
{
    switch (geometryType)
    {
    case ACTIVE:
        computeNativeVisibility(cellVisibility, grid, false, false, true, m_reservoirView->showMainGrid() );
        break;
    case ALL_WELL_CELLS:
        computeAllWellCellsVisibility(cellVisibility, grid);
        break;
    case VISIBLE_WELL_CELLS:
        {
            cvf::ref<cvf::UByteArray> allWellCellsVisibility;
            if (m_geometriesNeedsRegen[ALL_WELL_CELLS]) createGeometry(ALL_WELL_CELLS);

            allWellCellsVisibility = m_geometries[ALL_WELL_CELLS].cellVisibility(gridIdx);

            m_reservoirView->calculateVisibleWellCellsIncFence(cellVisibility, grid);

#pragma omp parallel for
            for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
            {
                (*cellVisibility)[cellIdx] = (*allWellCellsVisibility)[cellIdx] && (*cellVisibility)[cellIdx];
            }
        }
        break;
    case VISIBLE_WELL_FENCE_CELLS:
        {
            cvf::ref<cvf::UByteArray> allWellCellsVisibility;
            if (m_geometriesNeedsRegen[ALL_WELL_CELLS]) createGeometry(ALL_WELL_CELLS);

            allWellCellsVisibility = m_geometries[ALL_WELL_CELLS].cellVisibility(gridIdx);

            m_reservoirView->calculateVisibleWellCellsIncFence(cellVisibility, grid);

#pragma omp parallel for
            for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
            {
                (*cellVisibility)[cellIdx] = !(*allWellCellsVisibility)[cellIdx] && (*cellVisibility)[cellIdx];
            }
        }
        break;
    case INACTIVE:
        computeNativeVisibility(cellVisibility, grid, m_reservoirView->showInvalidCells(), true, false, m_reservoirView->showMainGrid());
        break;
    case RANGE_FILTERED:
        {
            cvf::ref<cvf::UByteArray> nativeVisibility;
            if (m_geometriesNeedsRegen[ACTIVE]) createGeometry(ACTIVE);

            nativeVisibility = m_geometries[ACTIVE].cellVisibility(gridIdx);
            computeRangeVisibility(cellVisibility, grid, nativeVisibility.p(), m_reservoirView->rangeFilterCollection());
        }
        break;
    case RANGE_FILTERED_INACTIVE:
        {
            cvf::ref<cvf::UByteArray> nativeVisibility;
            if (m_geometriesNeedsRegen[INACTIVE]) createGeometry(INACTIVE);

            nativeVisibility = m_geometries[INACTIVE].cellVisibility(gridIdx);
            computeRangeVisibility(cellVisibility, grid, nativeVisibility.p(), m_reservoirView->rangeFilterCollection());
        }
        break;
    case RANGE_FILTERED_WELL_CELLS:
        {
            cvf::ref<cvf::UByteArray> nativeVisibility;
            if (m_geometriesNeedsRegen[ALL_WELL_CELLS]) createGeometry(ALL_WELL_CELLS);

            nativeVisibility = m_geometries[ALL_WELL_CELLS].cellVisibility(gridIdx);
            computeRangeVisibility(cellVisibility, grid, nativeVisibility.p(), m_reservoirView->rangeFilterCollection());
        }
        break;
    case VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER:
        {
            cvf::ref<cvf::UByteArray> visibleWellCells;
            cvf::ref<cvf::UByteArray> rangeFilteredWellCells;

            if (m_geometriesNeedsRegen[VISIBLE_WELL_CELLS]) createGeometry(VISIBLE_WELL_CELLS);
            if (m_geometriesNeedsRegen[RANGE_FILTERED_WELL_CELLS]) createGeometry(RANGE_FILTERED_WELL_CELLS);

            visibleWellCells = m_geometries[VISIBLE_WELL_CELLS].cellVisibility(gridIdx);
            rangeFilteredWellCells = m_geometries[RANGE_FILTERED_WELL_CELLS].cellVisibility(gridIdx);

            cellVisibility->resize(visibleWellCells->size());

#pragma omp parallel for
            for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
            {
                (*cellVisibility)[cellIdx] = (*visibleWellCells)[cellIdx] && !(*rangeFilteredWellCells)[cellIdx];
            }
        }
        break;
    case VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER:
        {
            cvf::ref<cvf::UByteArray> visibleWellCells;
            cvf::ref<cvf::UByteArray> rangeFilteredWellCells;

            if (m_geometriesNeedsRegen[VISIBLE_WELL_FENCE_CELLS]) createGeometry(VISIBLE_WELL_FENCE_CELLS);
            if (m_geometriesNeedsRegen[RANGE_FILTERED]) createGeometry(RANGE_FILTERED);

            visibleWellCells = m_geometries[VISIBLE_WELL_FENCE_CELLS].cellVisibility(gridIdx);
            rangeFilteredWellCells = m_geometries[RANGE_FILTERED].cellVisibility(gridIdx);

            cellVisibility->resize(visibleWellCells->size());

#pragma omp parallel for
            for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
            {
                (*cellVisibility)[cellIdx] = (*visibleWellCells)[cellIdx] && !(*rangeFilteredWellCells)[cellIdx];
            }
        }
        break;
    default:
        CVF_ASSERT(false); // Call special function for property filtered stuff
        break;
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::createPropertyFilteredGeometry(size_t frameIndex)
{
    RigReservoir* res = m_reservoirView->eclipseCase()->reservoirData();

    if ( frameIndex >= m_propFilteredGeometryFrames.size())
    { 
        m_propFilteredGeometryFrames.resize(frameIndex + 1);
        m_propFilteredGeometryFramesNeedsRegen.resize(frameIndex + 1, true);
    }
    if ( m_propFilteredGeometryFrames[frameIndex].isNull())  m_propFilteredGeometryFrames[frameIndex] = new RivReservoirPartMgr;
    m_propFilteredGeometryFrames[frameIndex]->clearAndSetReservoir(res);
    m_propFilteredGeometryFrames[frameIndex]->setTransform(m_scaleTransform.p());
    std::vector<RigGridBase*> grids;
    res->allGrids(&grids);

    bool hasActiveRangeFilters  = m_reservoirView->rangeFilterCollection()->hasActiveFilters() || m_reservoirView->wellCollection()->hasVisibleWellCells();

    for (size_t i = 0; i < grids.size(); ++i)
    {
        cvf::ref<cvf::UByteArray> cellVisibility = m_propFilteredGeometryFrames[frameIndex]->cellVisibility(i); 
        cvf::ref<cvf::UByteArray> rangeVisibility; 
        cvf::ref<cvf::UByteArray> fenceVisibility; 

        if (m_geometriesNeedsRegen[RANGE_FILTERED]) createGeometry(RANGE_FILTERED);
        if (m_geometriesNeedsRegen[VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER]) createGeometry(VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER);

        rangeVisibility = m_geometries[RANGE_FILTERED].cellVisibility(i);
        fenceVisibility = m_geometries[VISIBLE_WELL_FENCE_CELLS_OUTSIDE_RANGE_FILTER].cellVisibility(i);

        cellVisibility->resize(rangeVisibility->size());

#pragma omp parallel for
        for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
        {
            (*cellVisibility)[cellIdx] = (!hasActiveRangeFilters && !grids[i]->cell(cellIdx).isWellCell()) || (*rangeVisibility)[cellIdx] || (*fenceVisibility)[cellIdx];
        }
        computePropertyVisibility(cellVisibility.p(), grids[i], frameIndex, cellVisibility.p(), m_reservoirView->propertyFilterCollection()); 

        m_propFilteredGeometryFrames[frameIndex]->setCellVisibility(i, cellVisibility.p());
    } 

    m_propFilteredGeometryFramesNeedsRegen[frameIndex] = false;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::createPropertyFilteredWellGeometry(size_t frameIndex)
{
    RigReservoir* res = m_reservoirView->eclipseCase()->reservoirData();

    if ( frameIndex >= m_propFilteredWellGeometryFrames.size())
    { 
        m_propFilteredWellGeometryFrames.resize(frameIndex + 1);
        m_propFilteredWellGeometryFramesNeedsRegen.resize(frameIndex + 1, true);
    }

    if ( m_propFilteredWellGeometryFrames[frameIndex].isNull())  m_propFilteredWellGeometryFrames[frameIndex] = new RivReservoirPartMgr;

    m_propFilteredWellGeometryFrames[frameIndex]->clearAndSetReservoir(res);
    m_propFilteredWellGeometryFrames[frameIndex]->setTransform(m_scaleTransform.p());

    std::vector<RigGridBase*> grids;
    res->allGrids(&grids);

    bool hasActiveRangeFilters  = m_reservoirView->rangeFilterCollection()->hasActiveFilters() || m_reservoirView->wellCollection()->hasVisibleWellCells();

    for (size_t i = 0; i < grids.size(); ++i)
    {
        cvf::ref<cvf::UByteArray> cellVisibility = m_propFilteredWellGeometryFrames[frameIndex]->cellVisibility(i); 
        cvf::ref<cvf::UByteArray> rangeVisibility; 
        cvf::ref<cvf::UByteArray> wellCellsOutsideVisibility; 

        if (m_geometriesNeedsRegen[RANGE_FILTERED_WELL_CELLS]) createGeometry(RANGE_FILTERED_WELL_CELLS);
        rangeVisibility = m_geometries[RANGE_FILTERED_WELL_CELLS].cellVisibility(i);

        if (m_geometriesNeedsRegen[VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER]) createGeometry(VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER);
        wellCellsOutsideVisibility = m_geometries[VISIBLE_WELL_CELLS_OUTSIDE_RANGE_FILTER].cellVisibility(i);

        cellVisibility->resize(rangeVisibility->size());
#pragma omp parallel for
        for (int cellIdx = 0; cellIdx < static_cast<int>(cellVisibility->size()); ++cellIdx)
        {
            (*cellVisibility)[cellIdx] = (!hasActiveRangeFilters && grids[i]->cell(cellIdx).isWellCell()) || (*rangeVisibility)[cellIdx] || (*wellCellsOutsideVisibility)[cellIdx];
        }
        computePropertyVisibility(cellVisibility.p(), grids[i], frameIndex, cellVisibility.p(), m_reservoirView->propertyFilterCollection()); 
        m_propFilteredWellGeometryFrames[frameIndex]->setCellVisibility(i, cellVisibility.p());
    }

    m_propFilteredWellGeometryFramesNeedsRegen[frameIndex] = false;
}

//--------------------------------------------------------------------------------------------------
/// Evaluate visibility based on cell state
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::computeNativeVisibility(cvf::UByteArray* cellVisibility, const RigGridBase* grid, 
    bool invalidCellsIsVisible, 
    bool inactiveCellsIsVisible, 
    bool activeCellsIsVisible,
    bool mainGridIsVisible) 
{
    CVF_ASSERT(cellVisibility != NULL);
    CVF_ASSERT(grid != NULL);
    cellVisibility->resize(grid->cellCount());

#pragma omp parallel for 
    for (int cellIndex = 0; cellIndex < static_cast<int>(grid->cellCount()); cellIndex++)
    {
        const RigCell& cell = grid->cell(cellIndex);

        if (   !invalidCellsIsVisible && cell.isInvalid() 
            || !inactiveCellsIsVisible && !cell.isActiveInMatrixModel()
            || !activeCellsIsVisible && cell.isActiveInMatrixModel()
            || mainGridIsVisible && (cell.subGrid() != NULL)
            || cell.isWellCell()
            )
        {
            (*cellVisibility)[cellIndex] = false;
        }
        else
        {
            (*cellVisibility)[cellIndex] = true;
        }
    }
}


//--------------------------------------------------------------------------------------------------
/// Evaluate Well cell visibility based on cell state
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::computeAllWellCellsVisibility(cvf::UByteArray* cellVisibility, const RigGridBase* grid ) 
{
    CVF_ASSERT(cellVisibility != NULL);
    CVF_ASSERT(grid != NULL);
    cellVisibility->resize(grid->cellCount());

#pragma omp parallel for 
    for (int cellIndex = 0; cellIndex < static_cast<int>(grid->cellCount()); cellIndex++)
    {
        const RigCell& cell = grid->cell(cellIndex);

        if (  cell.isWellCell() )
        {
            (*cellVisibility)[cellIndex] = true;
        }
        else
        {
            (*cellVisibility)[cellIndex] = false;
        }
    }
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::computeRangeVisibility(cvf::UByteArray* cellVisibility, const RigGridBase* grid, 
    const cvf::UByteArray* nativeVisibility, const RimCellRangeFilterCollection* rangeFilterColl) 
{
    CVF_ASSERT(cellVisibility != NULL);
    CVF_ASSERT(nativeVisibility != NULL);
    CVF_ASSERT(rangeFilterColl != NULL);

    CVF_ASSERT(grid != NULL);
    CVF_ASSERT(nativeVisibility->size() == grid->cellCount());


    if (rangeFilterColl->hasActiveFilters())
    {
        if (cellVisibility != nativeVisibility) (*cellVisibility) = (*nativeVisibility);

        // Build range filter for current grid
        cvf::CellRangeFilter mainGridCellRangeFilter;
        rangeFilterColl->compoundCellRangeFilter(&mainGridCellRangeFilter);

#pragma omp parallel for 
        for (int cellIndex = 0; cellIndex < static_cast<int>(grid->cellCount()); cellIndex++)
        {
            if ( (*nativeVisibility)[cellIndex] )
            {
                const RigCell& cell = grid->cell(cellIndex);
                size_t mainGridCellIndex = cell.mainGridCellIndex();
                size_t mainGridI;
                size_t mainGridJ;
                size_t mainGridK;

                grid->mainGrid()->ijkFromCellIndex(mainGridCellIndex, &mainGridI, &mainGridJ, &mainGridK);
                (*cellVisibility)[cellIndex] = !mainGridCellRangeFilter.isCellRejected(mainGridI, mainGridJ, mainGridK);
            }
        }
    }
    else
    {
        cellVisibility->resize(grid->cellCount());
        cellVisibility->setAll(false);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::computePropertyVisibility(cvf::UByteArray* cellVisibility, const RigGridBase* grid, size_t timeStepIndex, 
    const cvf::UByteArray* rangeFilterVisibility, RimCellPropertyFilterCollection* propFilterColl)
{
    CVF_ASSERT(cellVisibility != NULL);
    CVF_ASSERT(rangeFilterVisibility != NULL);
    CVF_ASSERT(propFilterColl != NULL);

    CVF_ASSERT(grid->cellCount() > 0);
    CVF_ASSERT(rangeFilterVisibility->size() == grid->cellCount());

    // Copy if not equal
    if (cellVisibility != rangeFilterVisibility ) (*cellVisibility) = *rangeFilterVisibility;

    if (propFilterColl->hasActiveFilters())
    {
        std::list< caf::PdmPointer< RimCellPropertyFilter > >::const_iterator pfIt;
        for (pfIt = propFilterColl->propertyFilters().begin(); pfIt !=  propFilterColl->propertyFilters().end(); ++pfIt)
        {
            if ((*pfIt)->active()&& (*pfIt)->resultDefinition->hasResult())
            {
                const double lowerBound = (*pfIt)->lowerBound();
                const double upperBound = (*pfIt)->upperBound();

                size_t scalarResultIndex = (*pfIt)->resultDefinition->gridScalarIndex();

                // Set time step to zero for static results
                if ((*pfIt)->resultDefinition()->hasStaticResult())
                {
                    timeStepIndex = 0;
                }

                const RimCellFilter::FilterModeType filterType = (*pfIt)->filterMode();

                RifReaderInterface::PorosityModelResultType porosityModel = RigReservoirCellResults::convertFromProjectModelPorosityModel((*pfIt)->resultDefinition()->porosityModel());
                cvf::ref<RigGridScalarDataAccess> dataAccessObject = grid->dataAccessObject(porosityModel, timeStepIndex, scalarResultIndex);
                CVF_ASSERT(dataAccessObject.notNull());

                #pragma omp parallel for schedule(dynamic)
                for (int cellIndex = 0; cellIndex < static_cast<int>(grid->cellCount()); cellIndex++)
                {
                    if ( (*cellVisibility)[cellIndex] )
                    {
                        size_t resultValueIndex = cellIndex;
                        
                        double scalarValue = dataAccessObject->cellScalar(resultValueIndex);
                        if (lowerBound <= scalarValue && scalarValue <= upperBound)
                        {
                            if (filterType == RimCellFilter::EXCLUDE)
                            {
                                (*cellVisibility)[cellIndex] = false;
                            }
                        }
                        else
                        {
                            if (filterType == RimCellFilter::INCLUDE)
                            {
                                (*cellVisibility)[cellIndex] = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::updateCellColor(ReservoirGeometryCacheType geometryType, size_t timeStepIndex, cvf::Color4f color)
{
    if (geometryType == PROPERTY_FILTERED)
    {
        m_propFilteredGeometryFrames[timeStepIndex]->updateCellColor(color );
    }
    else if (geometryType == PROPERTY_FILTERED_WELL_CELLS)
    {
        m_propFilteredWellGeometryFrames[timeStepIndex]->updateCellColor(color );
    }    
    else
    {
        m_geometries[geometryType].updateCellColor(color);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::updateCellColor(ReservoirGeometryCacheType geometryType, cvf::Color4f color)
{
    //CVF_ASSERT(geometryType != PROPERTY_FILTERED);
    //CVF_ASSERT(geometryType != PROPERTY_FILTERED_WELL_CELLS);

    updateCellColor(geometryType, 0 , color);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::updateCellResultColor(ReservoirGeometryCacheType geometryType, size_t timeStepIndex, RimResultSlot* cellResultSlot)
{
    if (geometryType == PROPERTY_FILTERED)
    {
        m_propFilteredGeometryFrames[timeStepIndex]->updateCellResultColor(timeStepIndex, cellResultSlot);
    }
    else if (geometryType == PROPERTY_FILTERED_WELL_CELLS)
    {
        m_propFilteredWellGeometryFrames[timeStepIndex]->updateCellResultColor(timeStepIndex, cellResultSlot);
    }
    else
    {
        m_geometries[geometryType].updateCellResultColor(timeStepIndex, cellResultSlot);
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RivReservoirViewPartMgr::updateCellEdgeResultColor(ReservoirGeometryCacheType geometryType, size_t timeStepIndex, RimResultSlot* cellResultSlot, RimCellEdgeResultSlot* cellEdgeResultSlot)
{
    if (geometryType == PROPERTY_FILTERED)
    {
        m_propFilteredGeometryFrames[timeStepIndex]->updateCellEdgeResultColor( timeStepIndex, cellResultSlot, cellEdgeResultSlot );
    }
    else if (geometryType == PROPERTY_FILTERED_WELL_CELLS)
    {
        m_propFilteredWellGeometryFrames[timeStepIndex]->updateCellEdgeResultColor( timeStepIndex, cellResultSlot, cellEdgeResultSlot );
    }
    else
    {
        m_geometries[geometryType].updateCellEdgeResultColor(timeStepIndex, cellResultSlot, cellEdgeResultSlot );
    }
}
