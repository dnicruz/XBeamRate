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

#include "StdAfx.h"
#include "XBeamRateReportBuilder.h"
#include "XBeamRateReportSpecification.h"

#include <\ARP\PGSuper\Include\IFace\Project.h>
#include <XBeamRateExt\XBeamRateUtilities.h>
#include <PgsExt\GirderLabel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXBeamRateReportBuilder::CXBeamRateReportBuilder(LPCTSTR strName,bool bHidden,bool bIncludeTimingChapter) :
CReportBuilder(strName,bHidden,bIncludeTimingChapter)
{
}

boost::shared_ptr<rptReport> CXBeamRateReportBuilder::CreateReport(boost::shared_ptr<CReportSpecification>& pRptSpec)
{
   CXBeamRateReportSpecification* pXBRRptSpec = dynamic_cast<CXBeamRateReportSpecification*>(pRptSpec.get());

   CComPtr<IBroker> pBroker;
   pXBRRptSpec->GetBroker(&pBroker);

   PierIDType pierID = pXBRRptSpec->GetPierID();

   if ( IsPGSExtension() )
   {
      GET_IFACE2(pBroker,IBridgeDescription,pIBridgeDesc);
      const CPierData2* pPier = pIBridgeDesc->FindPier(pierID);
      if ( pPier->GetPierModelType() == pgsTypes::pmtIdealized )
      {
         boost::shared_ptr<rptReport> pReport( new rptReport(pRptSpec->GetReportName()) );
         rptChapter* pChapter = new rptChapter;
         (*pReport) << pChapter;

         rptParagraph* pPara = new rptParagraph;
         *pChapter << pPara;

         CString strMsg;
         strMsg.Format(_T("Pier %d is modeled as idealized. Idealized piers cannot be load rated."),LABEL_PIER(pPier->GetIndex()));
         *pPara << strMsg << rptNewLine;
         
         return pReport;
      }
   }
#if defined _DEBUG
   else
   {
      ATLASSERT(pierID == INVALID_ID);
   }
#endif

   return CReportBuilder::CreateReport(pRptSpec);
}