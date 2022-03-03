
// InvestmentsView.cpp : implementation of the CInvestmentsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Investments.h"
#endif

#include "InvestmentsDoc.h"
#include "InvestmentsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInvestmentsView

IMPLEMENT_DYNCREATE(CInvestmentsView, CView)

BEGIN_MESSAGE_MAP(CInvestmentsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CInvestmentsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CInvestmentsView construction/destruction

CInvestmentsView::CInvestmentsView() noexcept
{
	// TODO: add construction code here

}

CInvestmentsView::~CInvestmentsView()
{
}

BOOL CInvestmentsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CInvestmentsView drawing

void CInvestmentsView::OnDraw(CDC* /*pDC*/)
{
	CInvestmentsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CInvestmentsView printing


void CInvestmentsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CInvestmentsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CInvestmentsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CInvestmentsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CInvestmentsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CInvestmentsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CInvestmentsView diagnostics

#ifdef _DEBUG
void CInvestmentsView::AssertValid() const
{
	CView::AssertValid();
}

void CInvestmentsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInvestmentsDoc* CInvestmentsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInvestmentsDoc)));
	return (CInvestmentsDoc*)m_pDocument;
}
#endif //_DEBUG


// CInvestmentsView message handlers
