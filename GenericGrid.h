#pragma once
#include "afxdialogex.h"

#include "..\ORM_Extensions/IDisplayer.h"
#include "..\ORM_Extensions/GridDisplayer.h"
#include "Data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"

#include "..\ORM_Extensions/BoxContents.h"


// GenericGrid dialog

class GenericGrid : public CDialog
{
	DECLARE_DYNAMIC(GenericGrid)

public:
	GenericGrid(CWnd* pParent = nullptr);   // standard constructor
	virtual ~GenericGrid();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GenericGrid };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CJDGridCtrl m_generic_grid;
	std::unique_ptr<IDisplayer> m_grid_displayer;
	template<typename T>
	void InitializeGridRendimientos(const T& t);
};
