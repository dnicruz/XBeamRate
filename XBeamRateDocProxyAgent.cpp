#include "stdafx.h"
#include "resource.h"
#include "XBeamRateDocProxyAgent.h"
#include "XBeamRateDoc.h"

#include <IFace\Project.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************************************
CLASS
   CXBeamRateDocProxyAgent
****************************************************************************/

CXBeamRateDocProxyAgent::CXBeamRateDocProxyAgent()
{
   m_pMyDocument = NULL;
}

CXBeamRateDocProxyAgent::~CXBeamRateDocProxyAgent()
{
}

void CXBeamRateDocProxyAgent::SetDocument(CXBeamRateDoc* pDoc)
{
   m_pMyDocument = pDoc;
}

void CXBeamRateDocProxyAgent::AdviseEventSinks()
{
   //
   // Attach to connection points
   //
   CComQIPtr<IBrokerInitEx2,&IID_IBrokerInitEx2> pBrokerInit(m_pBroker);
   CComPtr<IConnectionPoint> pCP;
   HRESULT hr = S_OK;

   hr = pBrokerInit->FindConnectionPoint( IID_IProjectEventSink, &pCP );
   ATLASSERT( SUCCEEDED(hr) );
   hr = pCP->Advise( GetUnknown(), &m_dwProjectCookie );
   ATLASSERT( SUCCEEDED(hr) );
   pCP.Release(); // Recycle the IConnectionPoint smart pointer so we can use it again.
}

void CXBeamRateDocProxyAgent::UnadviseEventSinks()
{
   //
   // Detach to connection points
   //
   CComQIPtr<IBrokerInitEx2,&IID_IBrokerInitEx2> pBrokerInit(m_pBroker);
   CComPtr<IConnectionPoint> pCP;
   HRESULT hr = S_OK;

   hr = pBrokerInit->FindConnectionPoint( IID_IProjectEventSink, &pCP );
   ATLASSERT( SUCCEEDED(hr) );
   hr = pCP->Unadvise( m_dwProjectCookie );
   ATLASSERT( SUCCEEDED(hr) );
   pCP.Release(); // Recycle the IConnectionPoint smart pointer so we can use it again.
}

//////////////////////////////////////////////////////////
// IAgentEx
STDMETHODIMP CXBeamRateDocProxyAgent::SetBroker(IBroker* pBroker)
{
   EAF_AGENT_SET_BROKER(pBroker);
   return S_OK;
}

STDMETHODIMP CXBeamRateDocProxyAgent::RegInterfaces()
{
   CComQIPtr<IBrokerInitEx2> pBrokerInit(m_pBroker);
 //  pBrokerInit->RegInterface( IID_IEditByUI,           this );
   return S_OK;
}

STDMETHODIMP CXBeamRateDocProxyAgent::Init()
{
   //EAF_AGENT_INIT;

   return AGENT_S_SECONDPASSINIT;
}

STDMETHODIMP CXBeamRateDocProxyAgent::Init2()
{
   AdviseEventSinks();
   return S_OK;
}

STDMETHODIMP CXBeamRateDocProxyAgent::Reset()
{
   txnTxnManager::GetInstance()->Clear();
   return S_OK;
}

STDMETHODIMP CXBeamRateDocProxyAgent::ShutDown()
{
   UnadviseEventSinks();

   EAF_AGENT_CLEAR_INTERFACE_CACHE;
   //CLOSE_LOGFILE;

   return S_OK;
}

STDMETHODIMP CXBeamRateDocProxyAgent::GetClassID(CLSID* pCLSID)
{
   *pCLSID = CLSID_XBeamRateDocProxyAgent;
   return S_OK;
}

////////////////////////////////////////////////////////////////////
// IAgentUIIntegration

//STDMETHODIMP CPGSuperDocProxyAgent::IntegrateWithUI(BOOL bIntegrate)
//{
//   if ( bIntegrate )
//   {
//      RegisterViews();
//      CreateToolBars();
//      CreateAcceleratorKeys();
//      CreateStatusBar();
//   }
//   else
//   {
//      ResetStatusBar();
//      RemoveAcceleratorKeys();
//      RemoveToolBars();
//      UnregisterViews();
//   }
//
//   return S_OK;
//}

////////////////////////////////////////////////////////////////////
// IProjectEventSink
HRESULT CXBeamRateDocProxyAgent::OnProjectChanged()
{
   AFX_MANAGE_STATE(AfxGetAppModuleState());
   m_pMyDocument->SetModifiedFlag();

   m_pMyDocument->UpdateAllViews(0,0,0);

   //boost::shared_ptr<CObject> pnull;
   //FireEvent( 0, HINT_PROJECTPROPERTIESCHANGED, pnull );
   return S_OK;
}

////////////////////////////////////////////////////////////////////
// IEAFDisplayUnitsEventSink
//HRESULT CXBeamRateDocProxyAgent::OnUnitsChanged(eafTypes::UnitMode newUnitMode)
//{
//   AFX_MANAGE_STATE(AfxGetAppModuleState());
//   m_pMyDocument->SetModifiedFlag();
//
//   GET_IFACE(IEAFDisplayUnits,pDisplayUnits);
//
//   CComPtr<IDocUnitSystem> pDocUnitSystem;
//   m_pMyDocument->GetDocUnitSystem(&pDocUnitSystem);
//   pDocUnitSystem->put_UnitMode(UnitModeType(pDisplayUnits->GetUnitMode()));
//
//   boost::shared_ptr<CObject> pnull;
//   FireEvent( 0, HINT_UNITSCHANGED, pnull );
//   return S_OK;
//}

///////////////////////////////////////////////////////////////////////////////////
// IVersionInfo
//CString CXBeamRateDocProxyAgent::GetVersionString(bool bIncludeBuildNumber)
//{
//   CString str(_T("Version "));
//   str += GetVersion(bIncludeBuildNumber);
//#if defined _BETA_VERSION
//   str += CString(_T(" BETA"));
//#endif
//
//   str += CString(_T(" - Built on "));
//   str += CString(__DATE__);
//   return str;
//}
//
//CString CXBeamRateDocProxyAgent::GetVersion(bool bIncludeBuildNumber)
//{
//   AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//   CWinApp* pApp = AfxGetApp();
//   CString strExe( pApp->m_pszExeName );
//   strExe += _T(".dll");
//
//   CVersionInfo verInfo;
//   verInfo.Load(strExe);
//   
//   CString strVersion = verInfo.GetProductVersionAsString();
//
//#if defined _DEBUG || defined _BETA_VERSION
//   // always include the build number in debug and beta versions
//   bIncludeBuildNumber = true;
//#endif
//
//   if (!bIncludeBuildNumber)
//   {
//      // remove the build number
//      int pos = strVersion.ReverseFind(_T('.')); // find the last '.'
//      strVersion = strVersion.Left(pos);
//   }
//
//   return strVersion;
//}