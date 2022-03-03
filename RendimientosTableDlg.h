#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/IDisplayer.h"
#include "..\ORM_Extensions/GridDisplayer.h"
#include "Data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"

#include "..\ORM_Extensions/BoxContents.h"


// RendimientosTableDlg dialog

class RendimientosTableDlg : public CDialog
{
	DECLARE_DYNAMIC(RendimientosTableDlg)
	BoxContents<Fondo, &Fondo::id, CComboBox> m_fondosCB;

public:
	RendimientosTableDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RendimientosTableDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RendimientosTableDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Refresh();
	virtual BOOL OnInitDialog();
private:
	CJDGridCtrl m_rendimientos_grid;
	std::unique_ptr<IDisplayer> m_rendimientos_displayer;

	template<typename T>
	void InitializeGridRendimientos(const T& t);

	CComboBox m_fondos_box;
public:
	afx_msg void OnBnClickedBFilter();
private:
	CDateTimeCtrl m_fecha_inicio;
	CDateTimeCtrl m_fecha_final;
	CButton m_filter_by_dates;
public:
	afx_msg void OnBnClickedBFilterClear();
	afx_msg void OnBnClickedBRendimiento();
};
