// InversionDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "InversionDlg.h"

#include "CFondoDlg.h"
#include "QueryInversionesDlg.h"

import Util;

// InversionDlg dialog

IMPLEMENT_DYNAMIC(InversionDlg, CDialog)

InversionDlg::InversionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_InversionDlg, pParent),
	m_list_inversionesLB(m_lista_inversiones, [](const Inversion& inv)
	{
			return Util::to_cstring(inv.simple_dump());
	}),
	m_list_fondosCB(m_lista_fondos, [](const Fondo& fondo)
	{
			return Util::to_cstring(fondo.simple_dump());
	})
{

}

InversionDlg::~InversionDlg()
{
}

void InversionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_INVERSION_ID, m_inversion_id);
	DDX_Control(pDX, IDC_E_INVERSION_NUM_PARTICIPACIONES, m_num_participaciones);
	DDX_Control(pDX, IDC_DT_FECHA_INVERSION, m_fecha_inversion);
	DDX_Control(pDX, IDC_CB_FONDOS, m_lista_fondos);
	DDX_Control(pDX, IDC_LIST_INVERSIONES, m_lista_inversiones);
}


BEGIN_MESSAGE_MAP(InversionDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &InversionDlg::OnBnClickedApply)
	ON_LBN_SELCHANGE(IDC_LIST_INVERSIONES, &InversionDlg::OnLbnSelchangeListInversiones)
	ON_BN_CLICKED(ID_NEW, &InversionDlg::OnBnClickedNew)
	ON_BN_CLICKED(ID_ERASE, &InversionDlg::OnBnClickedErase)
	ON_BN_CLICKED(IDC_B_FONDOS, &InversionDlg::OnBnClickedBFondos)
	ON_BN_CLICKED(IDC_B_QUERIES, &InversionDlg::OnBnClickedBQueries)
END_MESSAGE_MAP()


// InversionDlg message handlers


BOOL InversionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void InversionDlg::Refresh()
{
	m_list_inversionesLB.loadLBOrderByDesc(&Inversion::beginning_date);
	m_list_fondosCB.loadLBOrderBy(&Fondo::nombre);
}


void InversionDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	m_inversion = getCurrent<Inversion>(m_inversion_id);

	auto fondo = m_list_fondosCB.current();
	if(!fondo)
	{
		MessageBoxW(L"Falta escoger Fondo");
		return;
	}

	int num_part;
	m_num_participaciones >> num_part;

	std::chrono::sys_days fecha;
	m_fecha_inversion >> fecha;

	try
	{
		if (!m_inversion)	// insert
		{
			m_inversion = m_list_inversionesLB.insert(num_part, fecha, fondo->id);
		}
		else				// update
		{
			m_inversion->num_participaciones = num_part;
			m_inversion->beginning_date = fecha;
			m_inversion->fkey_fondo = fondo->id;
			m_list_inversionesLB.update(*m_inversion);
		}

		setIdFromRecord<Inversion>(m_inversion_id, m_inversion->id);
		Refresh();
	}
	catch (std::exception& exc)
	{
		handleApply(exc);
		OnLbnSelchangeListInversiones();
	}
}


void InversionDlg::OnLbnSelchangeListInversiones()
{
	// TODO: Add your control notification handler code here
	m_inversion = m_list_inversionesLB.current();
	if (!m_inversion)	return;
	SetText(m_inversion_id, m_inversion->id);
	m_list_fondosCB.select(m_inversion->fkey_fondo);
	SetDate(m_fecha_inversion, m_inversion->beginning_date);
	SetAmount(m_num_participaciones, m_inversion->num_participaciones);
}


void InversionDlg::OnBnClickedNew()
{
	// TODO: Add your control notification handler code here
	SetText(m_inversion_id, ""s);
	m_list_fondosCB.select(-1);

	using namespace std::chrono;
	const auto today = Today(); // sys_days{ floor<days>(system_clock::now()) };

	SetDate(m_fecha_inversion, today );
	SetAmount(m_num_participaciones, 0);
	m_inversion = std::nullopt;
}


void InversionDlg::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_list_inversionesLB.delete_current_sel();
		OnBnClickedNew();
	}
	catch(std::exception& exp)
	{
		handleErase(exp);
	}
}


void InversionDlg::OnBnClickedBFondos()
{
	// TODO: Add your control notification handler code here
	CFondoDlg dlg;
	dlg.DoModal();
	Refresh();
}


void InversionDlg::OnBnClickedBQueries()
{
	// TODO: Add your control notification handler code here
	QueryInversionesDlg dlg;
	dlg.DoModal();

}


LRESULT InversionDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	auto ret = Posting::get().WindowProc(message, wParam, lParam);
	if (ret != 0)
		return CDialog::WindowProc(message, wParam, lParam);
	return ret;
}
