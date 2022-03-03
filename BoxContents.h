#pragma once

#include <unordered_set>
#include "RecordLinks.h"
#include "RefIntegrity.h"
#include <cassert>

using namespace sqlite_orm;



template<typename Table>
using TableStringizer = CString(*)(const Table&);

struct Posting
{
private:
	Posting() = default;	// singleton
	std::unordered_set<HWND> m_postingWindows;
	void AddWindow(HWND hwnd)
	{
		m_postingWindows.insert(hwnd);
	}
	bool exists(LPARAM lParam)
	{
		HWND hwnd = reinterpret_cast<HWND>(lParam);
		auto f = m_postingWindows.find(hwnd);
		if (f != m_postingWindows.end())
		{
			m_postingWindows.erase(hwnd);
			return true;
		}
		return false;
	}
public:

	static constexpr int WindowProcNotHandled = 1;

	static Posting& get()	// access Posting only instance
	{
		static Posting posting;
		return posting;
	}

	template<typename Box>
	void PostSelChangeNotification(Box& box)
	{
		AddWindow(box.m_hWnd);
		box.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(box.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)box.m_hWnd);
	}

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_COMMAND)
		{
			auto loword = LOWORD(wParam);
			auto hiword = HIWORD(wParam);

			if ( hiword == LBN_SELCHANGE)
			{
				if (Posting::get().exists(lParam))		// this window already responded to initial LBN_SELCHANGE
				{
					return 0;
				}
			}
		}
		return WindowProcNotHandled;
	}

};


template<typename Table, int Table::* keyCol, typename BoxType = CListBox>
class BoxContents
{
private:

	BoxType& m_box;
	TableStringizer<Table> asString;


	RefIntegrityManager<Table, keyCol> refIntManager;
	Accessor<Table, keyCol> accessor;
	Storage::Storage_t& storage;

public:

	BoxContents(BoxType& listbox, TableStringizer<Table>  f) : m_box(listbox), asString(f), storage(Storage::getStorage()) {}

	template<typename ...Cols>
	std::optional<Table> insert(Cols&&... cols)
	{
		Table record{ -1, cols... };

		refIntManager.throwIfcannotInsertUpdate(record);
		record.*keyCol = storage.insert(record);
		this->insert_into_listbox(record);
		return record;
	}

	void ResetContent()
	{
		m_box.ResetContent();
		m_box.SetCurSel(-1);
	}

	void update(const Table& record)
	{
		refIntManager.throwIfcannotInsertUpdate(record);
		storage.update(record);
	}

	template<typename WhereClause>
	std::optional<Table> exists(WhereClause& clause)
	{
		using namespace sqlite_orm;

		std::optional<Table> record;
		auto e = storage.select(columns(keyCol), where(clause));
		if (e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get<Table>(id);
		}

		return record;
	}

	std::optional<Table> current()
	{
		std::optional<Table> record;
		int cur_sel = m_box.GetCurSel();
		if (cur_sel != npos)
		{
			auto id = static_cast<long long>(m_box.GetItemData(cur_sel));
			record = refIntManager.get(id);
		}
		return record;
	}
	int GetCurSel() const
	{
		return m_box.GetCurSel();
	}
	void SetCurSel(int index) const
	{
		m_box.SetCurSel(index);
	}
	std::optional<Table> select(std::optional<int> key)
	{
		std::optional<Table> record;
		if (key)
		{
			record = select(*key);
		}
		else
		{
			m_box.SetCurSel(-1);
			CString buffer;
			m_box.GetWindowTextW(buffer);
			m_box.SetWindowTextW(L"");
		}
		return record;
	}

	std::optional<Table> select(int pk)
	{
		std::optional<Table> record;
		if (pk < 0)
		{
			m_box.SetCurSel(npos);
			return record;
		}
		int index = find_index_in_listbox(pk);
		if (index != npos)
		{
			m_box.SetCurSel(index);
			record = refIntManager.get(pk);
			
			Posting::get().PostSelChangeNotification(m_box);
		}
		return record;
	}
private:
	void remove(Table& record)
	{
		refIntManager.remove(record);
	}
public:

	int get_pk(Table& record)
	{
		return record.*keyCol;
	}

	int get_pk(std::optional<Table>& record)
	{
		return record ? record.*keyCol : -1;
	}

	// will check if record exists in listbox and do nothing if so
	int insert_into_listbox(Table& record)
	{
		int index = find_in_listbox(record);
		if (index != npos)	return index;	// exists already
		auto displayStr = asString(record);
		index = m_box.AddString(displayStr);
		m_box.SetItemData(index, get_pk(record));
		m_box.SetCurSel(index);
		return index;
	}

	bool delete_current_sel()
	{
		using namespace sqlite_orm;
		auto current = this->current();
		if (!current)
		{
			throw std::exception("No hay registro seleccionado");
		}
		refIntManager.throwIfcannotDelete(*current);
		storage.remove<Table>(get_pk(*current));
		int cur_sel = m_box.GetCurSel();
		m_box.DeleteString(cur_sel);
		return true;
	}

	bool delete_from_box(Table& record)
	{
		int index = find_in_listbox(record);
		if (index != npos)
		{
			m_box.DeleteString(index);
			return true;
		}
		return false;
	}

	int find_in_listbox(Table& record)
	{
		const int pk = record.*keyCol;
		return find_index_in_listbox(pk);
	}
	template<typename T>
	void moveVectorIntoBox(T& vec)
	{
		m_box.ResetContent();
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		// ???
		SetCurSel(-1);
	}
	// Accesor methods
	void loadLB()
	{
		m_box.ResetContent();
		auto vec = accessor.getAll();

		moveVectorIntoBox(vec);
	}
	template<typename whereClause>
	void loadLB(whereClause clause)
	{
		m_box.ResetContent();

		auto vec = accessor.getAll(clause);
		moveVectorIntoBox(vec);
	}

	template<typename orderByClause>
	void loadLBOrderBy(orderByClause clause)
	{
		m_box.ResetContent();

		auto vec = accessor.getAllOrderBy(clause);
		moveVectorIntoBox(vec);
	}

	template<typename orderByClause>
	void loadLBOrderByDesc(orderByClause clause)
	{
		m_box.ResetContent();
		auto vec = accessor.getAllOrderByDesc(clause);
		moveVectorIntoBox(vec);
	}

	template<typename orderByClause, typename whereClause>
	void loadLBOrderByWhere(orderByClause clause, whereClause where_clause)
	{
		m_box.ResetContent();

		auto vec = accessor.getAllOrderByWhere(clause, where_clause);
		moveVectorIntoBox(vec);
	}

	template<typename orderByClause, typename whereClause>
	void loadLBOrderByDescWhere(orderByClause clause, whereClause where_clause)
	{
		m_box.ResetContent();

		auto vec = accessor.getAllOrderByDescWhere(clause, where_clause);
		moveVectorIntoBox(vec);
	}



	constexpr static const int npos = -1;
private:
	int find_index_in_listbox(const int pk)
	{
		int index = m_box.GetCount();
		while (index >= 0)
		{
			auto id = m_box.GetItemData(index);
			if (pk == id)
			{
				return index;
			}
			--index;
		}
		return npos;
	}

};
