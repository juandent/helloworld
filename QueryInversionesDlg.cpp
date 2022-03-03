// QueryInversionesDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "QueryInversionesDlg.h"

import Util;

// QueryInversionesDlg dialog

IMPLEMENT_DYNAMIC(QueryInversionesDlg, CDialog)

QueryInversionesDlg::QueryInversionesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_QueryInversionesDlg, pParent),
	m_list_fondosCB(m_lista_fondos, [](const Fondo& fondo)
	{
			return Util::to_cstring(fondo.simple_dump());
	})
{

}

QueryInversionesDlg::~QueryInversionesDlg()
{
}

void QueryInversionesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DT_FECHA_INVERSION, m_fecha_calculo);
	DDX_Control(pDX, IDC_CB_FONDOS, m_lista_fondos);
	DDX_Control(pDX, IDC_E_INVERSION_NUM_PARTICIPACIONES, m_num_participaciones);
	DDX_Control(pDX, IDC_E_RENDIMIENTO_PARTICIPACIONES, m_rendimiento_participaciones);
	DDX_Control(pDX, IDC_E_RENDIMIENTO_UNITARIO, m_rendimiento_unitario);
}


BEGIN_MESSAGE_MAP(QueryInversionesDlg, CDialog)
	ON_BN_CLICKED(ID_B_CALCULATE, &QueryInversionesDlg::OnBnClickedBCalculate)
END_MESSAGE_MAP()


// QueryInversionesDlg message handlers


void QueryInversionesDlg::OnBnClickedBCalculate()
{
	// TODO: Add your control notification handler code here
	auto fondo = m_list_fondosCB.current();
	if(!fondo)
	{
		MessageBoxW(L"Fondo no escogido!");		return;
	}

	std::chrono::sys_days fecha;
	m_fecha_calculo >> fecha;

	int num_part = fondo->num_participaciones_al(fecha);
	m_num_participaciones << num_part;
	
	auto rend_unit = fondo->get_rendimiento_unitario_al(fecha);
	m_rendimiento_unitario << rend_unit;

	auto rendimiento = rend_unit * num_part;
	m_rendimiento_participaciones << rendimiento;
}


BOOL QueryInversionesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void QueryInversionesDlg::Refresh()
{
	m_list_fondosCB.loadLBOrderBy(&Fondo::nombre);
}
