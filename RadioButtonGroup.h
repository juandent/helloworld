#pragma once

#include "framework.h"

class RadioButtonGroup
{
	const std::vector<CButton*> m_radios;
	const std::vector<int> m_values;
public:
	RadioButtonGroup(std::initializer_list<CButton*> list, std::initializer_list<int> values)
		: m_radios{ list }, m_values{values}
	{
	}
	void Initialize()
	{
		m_radios[0]->SetCheck(BST_CHECKED);
	}
	int get_value() const
	{
		for (int idx = 0; idx < m_radios.size(); ++idx)
		{
			if (m_radios[idx]->GetCheck())
			{
				int value = m_values[idx];
				return value;
			}
		}
		throw std::logic_error("There must be 1 selected radio button in a group");
	}
	void set_value(int idx_select)
	{
		for (int idx = 0; idx < m_radios.size(); ++idx)
		{
			int value = m_values[idx];
			m_radios[idx]->SetCheck(value == idx_select);
		}
	}
	RadioButtonGroup& operator<<(int idx_select)
	{
		set_value(idx_select);
		return *this;
	}
	RadioButtonGroup& operator>>(int& value)
	{
		value = get_value();
		return *this;
	}
};

