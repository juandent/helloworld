#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/BoxContents.h"
#include "Data.h"


// QueryInversionesDlg dialog

class QueryInversionesDlg : public CDialog
{
	DECLARE_DYNAMIC(QueryInversionesDlg)

	BoxContents<Fondo, &Fondo::id, CComboBox> m_list_fondosCB;

public:
	QueryInversionesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~QueryInversionesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QueryInversionesDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CDateTimeCtrl m_fecha_calculo;
	CComboBox m_lista_fondos;
	CEdit m_num_participaciones;
public:
	afx_msg void OnBnClickedBCalculate();
	void Refresh();
	virtual BOOL OnInitDialog();
private:
	CEdit m_rendimiento_participaciones;
	CEdit m_rendimiento_unitario;
};
