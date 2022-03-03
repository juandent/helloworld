#pragma once


// template<typename ... DataTypes>
struct IDisplayer
{
	virtual void display() = 0;
};

namespace Util
{
	inline CString to_cstring(const std::optional<int>& o)
	{
		if (o)
		{
			return to_cstring(*o);
		}
		else
		{
			return CString{ "" };
		}
	}
}



template<int N>
struct Integral
{
	static constexpr int Int = N;
};


template<int Value, typename List, int Index>
struct Find
{
	constexpr static bool same = std::get<Index>(List::Values).Int == Value;
	constexpr static bool Check()
	{
		// if same, found it - end recursion
		return same || Find<Value, List, Index - 1>::Check();	// not found at Index, try at Index - 1
	}
};

template<int Value, typename List>
struct Find<Value, List, 0>
{
	constexpr static bool same = std::get<0>(List::Values).Int == Value;
	constexpr static bool Check()
	{
		return same;
	}
};


/// <summary>
/// is Value in values list?
/// </summary>
template<int...N>
struct IntegerList
{
	constexpr inline static std::tuple<Integral<N>...> Values;
	constexpr static unsigned Count = std::tuple_size_v<decltype(Values)>;

	template<int Value>
	constexpr static bool found()
	{
		using lista = IntegerList;

		bool found = Find<Value, lista, lista::Count - 1>::Check();
		return found;
	}
};

template<>
struct IntegerList<>
{
	template<int Value>
	constexpr static bool found()
	{
		return false;
	}
};

#if 0
template<int...N>
using ColonesCols = IntegerList;

template<int...N>
using DollarsCols = IntegerList;
#endif
