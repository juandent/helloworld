#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/BoxContents.h"
#include "Data.h"


// RendimientoDlg dialog

class RendimientoDlg : public CDialog
{
	DECLARE_DYNAMIC(RendimientoDlg)

	BoxContents<Rendimiento, &Rendimiento::id> m_list_rendimientosLB;
	BoxContents<Fondo, &Fondo::id, CComboBox> m_list_fondosCB;
	std::optional<Rendimiento> m_rendimiento;

public:
	RendimientoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RendimientoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RendimientoDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Refresh();
	virtual BOOL OnInitDialog();
private:
	CEdit m_rendimiento_id;
	CEdit m_rendimiento_unitario;
	CDateTimeCtrl m_fecha_rendimiento;
	CComboBox m_list_fondos;
	CListBox m_list_rendimientos;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnLbnSelchangeListRendimientos();
	afx_msg void OnBnClickedBQueries();
	afx_msg void OnBnClickedErase();
	afx_msg void OnBnClickedNew();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
