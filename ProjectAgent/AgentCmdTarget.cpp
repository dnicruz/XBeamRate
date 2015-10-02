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

#include "stdafx.h"
#include "resource.h"
#include "AgentCmdTarget.h"

#include <MFCTools\Prompts.h>
#include <EAF\EAFTransactions.h>
#include <txnEditProject.h>
#include <txnEditPier.h>
#include <txnEditOptions.h>

#include "PierDlg.h"
#include "OptionsDlg.h"

#include <XBeamRateExt\XBeamRateUtilities.h>

#include <IFace\Project.h>
#include <IFace\RatingSpecification.h>


BEGIN_MESSAGE_MAP(CAgentCmdTarget,CCmdTarget)
   ON_COMMAND(ID_EDIT_PROJECT_NAME,OnEditProjectName)
   ON_COMMAND(ID_EDIT_PIER, &CAgentCmdTarget::OnEditPier)
   ON_COMMAND(ID_EDIT_OPTIONS,&CAgentCmdTarget::OnEditOptions)
END_MESSAGE_MAP()

CAgentCmdTarget::CAgentCmdTarget()
{
}

void CAgentCmdTarget::Init(IBroker* pBroker)
{
   m_pBroker = pBroker;
}

void CAgentCmdTarget::OnEditProjectName()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   //CString strOldProjectName, strNewProjectName;
   //GET_IFACE(IXBRProject,pProject);
   //strOldProjectName = pProject->GetProjectName();

   //if ( AfxQuestion(_T("Project Name"),_T("Enter project name"),strOldProjectName,strNewProjectName) )
   //{
   //   txnEditProject txn(strOldProjectName,strNewProjectName);
   //   GET_IFACE(IEAFTransactions,pTransactions);
   //   pTransactions->Execute(txn);
   //}
}

void CAgentCmdTarget::OnEditPier()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GET_IFACE(IXBRProject,pProject);
   CPierDlg dlg(_T("Edit Pier"));

   PierIDType pierID = INVALID_ID;

   txnEditPierData oldPierData;
   oldPierData.m_PierData = pProject->GetPierData(pierID);

   IndexType nBearingLines = oldPierData.m_PierData.GetBearingLineCount();
   for ( IndexType brgLineIdx = 0; brgLineIdx < nBearingLines; brgLineIdx++ )
   {
      oldPierData.m_DeadLoadReactionType[brgLineIdx] = pProject->GetBearingReactionType(pierID,brgLineIdx);
      IndexType nBearings = oldPierData.m_PierData.GetBearingCount(brgLineIdx);
      for ( IndexType brgIdx = 0; brgIdx < nBearings; brgIdx++ )
      {
         txnDeadLoadReaction reaction;
         pProject->GetBearingReactions(pierID,brgLineIdx,brgIdx,&reaction.m_DC,&reaction.m_DW,&reaction.m_CR,&reaction.m_SH,&reaction.m_PS,&reaction.m_RE,&reaction.m_W);
         oldPierData.m_DeadLoadReactions[brgLineIdx].push_back(reaction);
      }
   }

   // we are in stand-alone mode.... we only don't have load factors per limit state
   // so just use a dummy limit state here
   ATLASSERT(IsStandAlone());
   oldPierData.m_gDC = pProject->GetDCLoadFactor(pgsTypes::StrengthI_Inventory);
   oldPierData.m_gDW = pProject->GetDWLoadFactor(pgsTypes::StrengthI_Inventory);
   oldPierData.m_gCR = pProject->GetCRLoadFactor(pgsTypes::StrengthI_Inventory);
   oldPierData.m_gSH = pProject->GetSHLoadFactor(pgsTypes::StrengthI_Inventory);
   oldPierData.m_gPS = pProject->GetPSLoadFactor(pgsTypes::StrengthI_Inventory);

   oldPierData.m_gLL[0] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthI_Inventory,INVALID_INDEX);
   oldPierData.m_gLL[1] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthI_Operating,INVALID_INDEX);
   oldPierData.m_gLL[2] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthI_LegalRoutine,INVALID_INDEX);
   oldPierData.m_gLL[3] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthI_LegalSpecial,INVALID_INDEX);
   oldPierData.m_gLL[4] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthII_PermitRoutine,INVALID_INDEX);
   oldPierData.m_gLL[5] = pProject->GetLiveLoadFactor(pierID,pgsTypes::StrengthII_PermitSpecial,INVALID_INDEX);

   oldPierData.m_LiveLoadReactionApplication  = pProject->GetReactionLoadApplicationType(pierID);
   oldPierData.m_DesignLiveLoad.m_LLIM        = pProject->GetLiveLoadReactions(pierID,pgsTypes::lrDesign_Inventory);
   oldPierData.m_LegalRoutineLiveLoad.m_LLIM  = pProject->GetLiveLoadReactions(pierID,pgsTypes::lrLegal_Routine);
   oldPierData.m_LegalSpecialLiveLoad.m_LLIM  = pProject->GetLiveLoadReactions(pierID,pgsTypes::lrLegal_Special);
   oldPierData.m_PermitRoutineLiveLoad.m_LLIM = pProject->GetLiveLoadReactions(pierID,pgsTypes::lrPermit_Routine);
   oldPierData.m_PermitSpecialLiveLoad.m_LLIM = pProject->GetLiveLoadReactions(pierID,pgsTypes::lrPermit_Special);

   dlg.SetEditPierData(oldPierData);
   if ( dlg.DoModal() == IDOK )
   {
      txnEditPierData newPierData = dlg.GetEditPierData();
      txnEditPier txn(oldPierData,newPierData);
      GET_IFACE(IEAFTransactions,pTransactions);
      pTransactions->Execute(txn);
   }
}

void CAgentCmdTarget::OnEditOptions()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GET_IFACE(IXBRRatingSpecification,pRatingSpec);
   
   txnEditOptionsData oldOptions;
   oldOptions.m_bDesignRating = pRatingSpec->IsRatingEnabled(pgsTypes::lrDesign_Inventory);
   oldOptions.m_bDesignRateForShear = pRatingSpec->RateForShear(pgsTypes::lrDesign_Inventory);

   oldOptions.m_bLegalRating  = pRatingSpec->IsRatingEnabled(pgsTypes::lrLegal_Routine);
   oldOptions.m_bLegalRateForShear  = pRatingSpec->RateForShear(pgsTypes::lrLegal_Routine);

   oldOptions.m_bPermitRating = pRatingSpec->IsRatingEnabled(pgsTypes::lrPermit_Routine);
   oldOptions.m_bPermitRateForShear = pRatingSpec->RateForShear(pgsTypes::lrPermit_Routine);
   oldOptions.m_bCheckYieldStress = pRatingSpec->CheckYieldStressLimit();
   oldOptions.m_YieldStressCoefficient = pRatingSpec->GetYieldStressLimitCoefficient();
   oldOptions.m_PermitRatingMethod = pRatingSpec->GetPermitRatingMethod();

   COptionsDlg dlg;
   dlg.SetOptions(oldOptions);
   if ( dlg.DoModal() == IDOK )
   {
      txnEditOptionsData newOptions = dlg.GetOptions();

      txnEditOptions txn(oldOptions,newOptions);
      GET_IFACE(IEAFTransactions,pTransactions);
      pTransactions->Execute(txn);
   }
}
