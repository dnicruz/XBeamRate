///////////////////////////////////////////////////////////////////////
// XBeamRate - Cross Beam Load Rating
// Copyright � 1999-2015  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// PGSuperExporter.cpp : Implementation of CPGSuperExporter
#include "stdafx.h"
#include "XBeamRateAgent.h"
#include "PGSuperExporter.h"

#include <IFace\Project.h>

HRESULT CPGSuperDataExporter::FinalConstruct()
{
   return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CPGSuperDataExporter

STDMETHODIMP CPGSuperDataExporter::Init(UINT nCmdID)
{
   return S_OK;
}

STDMETHODIMP CPGSuperDataExporter::GetMenuText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("Pier to XBeam Rate");
   return S_OK;
}

STDMETHODIMP CPGSuperDataExporter::GetBitmapHandle(HBITMAP* phBmp)
{
   *phBmp = NULL;
   return S_OK;
}

STDMETHODIMP CPGSuperDataExporter::GetCommandHintText(BSTR*  bstrText)
{
   *bstrText = CComBSTR("Export a pier to XBeam Rate\nTool tip text");
   return S_OK;   
}

STDMETHODIMP CPGSuperDataExporter::Export(IBroker* pBroker)
{
   GET_IFACE2(pBroker,IXBRExport,pExport);
   return pExport->Export(INVALID_INDEX); // INVALID_INDEX means prompt for pier
}