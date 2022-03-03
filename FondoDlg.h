#pragma once

#include "..\ORM_Extensions/BoxContents.h"
#include "Data.h"
#include "..\ORM_Extensions/RadioButtonGroup.h"


// FondoDlg form view

class FondoDlg : public CDialog
{
	DECLARE_DYNAMIC(FondoDlg)

	BoxContents<Fondo, &Fondo::id> m_list_all_fondosLB;
	std::optional<Fondo> m_fondo{};

	RadioButtonGroup m_tipo_cupon;
public:
	FondoDlg(CWnd* pParent = nullptr);           //////////// protected constructor used by dynamic creation
	virtual ~FondoDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FondoDlg };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListBox m_listFondos;
public:
	afx_msg void OnBnClickedApply();
private:
	CEdit m_fondo_id;
	CEdit m_fondo_abrev;
	CEdit m_fondo_name;
	CButton m_r_mensual;
	CButton m_r_trimestral;
};


