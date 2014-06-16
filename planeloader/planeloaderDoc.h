
// planeloaderDoc.h : interface of the CplaneloaderDoc class
//


#pragma once


class CplaneloaderDoc : public CDocument
{
protected: // create from serialization only
	CplaneloaderDoc();
	DECLARE_DYNCREATE(CplaneloaderDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CplaneloaderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


