
// planeloaderDoc.cpp : implementation of the CplaneloaderDoc class
//

#include "stdafx.h"
#include "planeloader.h"

#include "planeloaderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CplaneloaderDoc

IMPLEMENT_DYNCREATE(CplaneloaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CplaneloaderDoc, CDocument)
END_MESSAGE_MAP()


// CplaneloaderDoc construction/destruction

CplaneloaderDoc::CplaneloaderDoc()
{
	// TODO: add one-time construction code here

}

CplaneloaderDoc::~CplaneloaderDoc()
{
}

BOOL CplaneloaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CplaneloaderDoc serialization

void CplaneloaderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CplaneloaderDoc diagnostics

#ifdef _DEBUG
void CplaneloaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CplaneloaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CplaneloaderDoc commands
