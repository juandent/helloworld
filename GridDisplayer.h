#pragma once

import Util;


template<typename T>
class GridDisplayer
{
	CJDGridCtrl& grid;
	using Container = std::vector<std::remove_reference_t<T>>;
	Container lines;
	using ValueType = typename Container::value_type;

	// std::tuple<DataTypes...> tuple;
	std::vector<std::string>	headers;
public:
	// template<typename ...DataTypes>
	GridDisplayer(CJDGridCtrl& grid,
		Container&& lines_,
		std::vector<std::string>&& headers_)
		: lines{ std::move(lines_) }, grid{ grid }, headers{ std::move(headers_) }
	{
		grid.SetColumnCount(headers.size() + 1);
		grid.SetRowCount(lines.size() + 1);
		grid.SetFixedRowCount();
		grid.SetFixedColumnCount();
		grid.SetHeaderSort(true);
		grid.SetSingleRowSelection(true);

		int col = 1;
		for (auto& str : headers)
		{
			auto head = Util::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
	}

	template<typename ...DataTypes>
	void display(DataTypes... dts)
	{
		RECT rect;
		grid.GetWindowRect(&rect);

		for (int i = 0; i < lines.size(); ++i)
		{
			printDataInGrid(i, 1, lines[i], dts...);
		}
		grid.SetColumnWidth(0, 100);
		int width = grid.GetColumnWidth(0);
		for (int i = 0; i < headers.size(); ++i)
		{
			grid.AutoSizeColumn(i + 1);	// skip vertical headers
			width += grid.GetColumnWidth(i + 1);
		}
		auto frame = grid.GetParentFrame();
		width += GetSystemMetrics(SM_CXVSCROLL);
		grid.SetWindowPos(frame, 10, 10, width, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
	}

private:
	template<typename DataType>
	void printDataInGrid(int row, int col, T& z, DataType p)
	{
		auto value = z.*p;
		auto cs = format(value);
		grid.SetItemText(row + 1, col, cs);
	}


	template<typename DataType, typename ... RestDataTypes>
	void printDataInGrid(int row, int col, T& z, DataType p, RestDataTypes...qs)
	{
		printDataInGrid(row, col, z, p);
		printDataInGrid(row, ++col, z, qs...);
	}
	template<typename T>
	CString format(const T& t)
	{
		return Util::to_cstring(t);
	}
	template<typename T>
	static CString format(const std::shared_ptr<T>& t)
	{
		if (t)
		{
			return Util::to_cstring(*t);
		}
		return L"";
	}
};
