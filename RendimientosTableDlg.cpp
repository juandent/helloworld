// RendimientosTableDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "RendimientosTableDlg.h"

#include "RendimientoDlg.h"


// RendimientosTableDlg dialog

IMPLEMENT_DYNAMIC(RendimientosTableDlg, CDialog)

RendimientosTableDlg::RendimientosTableDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RendimientosTableDlg, pParent),
m_fondosCB(m_fondos_box, [](const Fondo& fondo)
{
		return Util::to_cstring(fondo.simple_dump());
})
{

}

RendimientosTableDlg::~RendimientosTableDlg()
{
}

void RendimientosTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_RENDIMIENTOS, m_rendimientos_grid);
	DDX_Control(pDX, IDC_CB_FONDOS, m_fondos_box);
	DDX_Control(pDX, IDC_DATETIME_INICIO, m_fecha_inicio);
	DDX_Control(pDX, IDC_DATETIME_FINAL, m_fecha_final);
	DDX_Control(pDX, IDC_CK_FILTER_BY_DATES, m_filter_by_dates);
}


BEGIN_MESSAGE_MAP(RendimientosTableDlg, CDialog)
	ON_BN_CLICKED(IDC_B_FILTER, &RendimientosTableDlg::OnBnClickedBFilter)
	ON_BN_CLICKED(IDC_B_FILTER_CLEAR, &RendimientosTableDlg::OnBnClickedBFilterClear)
	ON_BN_CLICKED(IDC_B_RENDIMIENTO, &RendimientosTableDlg::OnBnClickedBRendimiento)
END_MESSAGE_MAP()


// RendimientosTableDlg message handlers


BOOL RendimientosTableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_fondosCB.loadLBOrderBy(&Fondo::nombre);
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void RendimientosTableDlg::Refresh()
{
	OnBnClickedBFilter();
}
#include "CompositePersistentClasses.h"

template <typename T>
void RendimientosTableDlg::InitializeGridRendimientos(const T& t)
{
	auto rows = Storage::getStorage().select(columns(
		alias_column<als_r>(&Rendimiento::id),
		alias_column<als_f>(&Fondo::nombre),
		alias_column<als_r>(&Rendimiento::fecha),
		alias_column<als_r>(&Rendimiento::rendimiento_unitario)),

		inner_join<als_f>(on(c(alias_column<als_r>(&Rendimiento::fkey_fondo)) == alias_column<als_f>(&Fondo::id))),

		where(t),
		multi_order_by(order_by(alias_column<als_r>(&Rendimiento::fkey_fondo)), order_by(alias_column<als_r>(&Rendimiento::fecha)).desc()));

#define JDH
#ifdef JDH
	auto row = rows[0];
	auto guard = Storage::getStorage().transaction_guard();
	RendimientoComposite comp(std::get<0>(row));
	auto id = comp.get_id();
	comp.set_rendimiento_unitario(2.2);
	decltype(row)* p;
	auto col0 = std::get<0>(row);
	auto col1 = std::get<1>(row);
	guard.rollback();
#endif
	std::vector<std::string> headers{ "ID RENDIMIENTO", "FONDO ", "FECHA", "REND UNITARIO"};

	m_rendimientos_displayer.reset(new JoinedGridDisplayer<decltype(rows[0]), IntegerList<>, IntegerList<4>>(m_rendimientos_grid, std::move(rows), std::move(headers)));
	m_rendimientos_displayer->display();
}


void RendimientosTableDlg::OnBnClickedBFilter()
{
	auto fondo = m_fondosCB.current();
	auto fondo_id = fondo ? fondo->id : -1;

	auto fondoWhere = (not fondo or (c(alias_column<als_f>(&Fondo::id)) == fondo_id));

	bool filter_by_dates;
	m_filter_by_dates >> filter_by_dates;

	std::chrono::sys_days start_date, finish_date;
	m_fecha_inicio >> start_date;
	m_fecha_final >> finish_date;

	auto filter_by_dates_where = (not filter_by_dates or
		(c(alias_column<als_r>(&Rendimiento::fecha)) >= start_date
			&& (c(alias_column<als_r>(&Rendimiento::fecha)) <= finish_date)));

	auto whereClause = fondoWhere && filter_by_dates_where;

	InitializeGridRendimientos(whereClause);
}


void RendimientosTableDlg::OnBnClickedBFilterClear()
{
	// TODO: Add your control notification handler code here
	m_fondosCB.select(-1);
	m_filter_by_dates << false;
	OnBnClickedBFilter();
}


void RendimientosTableDlg::OnBnClickedBRendimiento()
{
	// TODO: Add your control notification handler code here
	RendimientoDlg dlg;
	dlg.DoModal();
}
