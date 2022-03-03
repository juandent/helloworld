#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/BoxContents.h"
#include "Data.h"

// InversionDlg dialog

class InversionDlg : public CDialog
{
	DECLARE_DYNAMIC(InversionDlg)

	BoxContents<Inversion, &Inversion::id> m_list_inversionesLB;
	BoxContents<Fondo, &Fondo::id, CComboBox> m_list_fondosCB;
	std::optional<Inversion> m_inversion;

public:
	InversionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InversionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InversionDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Refresh();
	virtual BOOL OnInitDialog();
private:
	CEdit m_inversion_id;
	CEdit m_num_participaciones;
	CDateTimeCtrl m_fecha_inversion;
	CComboBox m_lista_fondos;
	CListBox m_lista_inversiones;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnLbnSelchangeListInversiones();
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedErase();
	afx_msg void OnBnClickedBFondos();
	afx_msg void OnBnClickedBQueries();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
