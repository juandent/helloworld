#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/BoxContents.h"
#include "Data.h"
#include "..\ORM_Extensions/RadioButtonGroup.h"

// CFondoDlg dialog

class CFondoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFondoDlg)

	BoxContents<Fondo, &Fondo::id> m_list_all_fondosLB;
	std::optional<Fondo> m_fondo{};

	RadioButtonGroup m_tipo_cupon;
public:
	CFondoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFondoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFondoDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_list_fondos;
	CButton m_mensual;
	CButton m_trimestral;
	CEdit m_fondo_id;
	CEdit m_fondo_abrev;
	CEdit m_fondo_name;
public:
	virtual BOOL OnInitDialog();
	void Refresh();
	afx_msg void OnBnClickedApply();
	afx_msg void OnSelchangeListFondos();
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedErase();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
