#pragma once

#include "sqlite_orm/sqlite_orm.h"
#include <afxstr.h>
#include <system_error>

inline std::error_code handle(const std::system_error& systemError, bool display = true)
{
	using namespace sqlite_orm;
	using namespace std::literals;

	const HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();

	if (display)
	{
		if (systemError.code().category() == get_orm_error_category()) {
			std::ostringstream ss;
			ss << "orm error happened with code " << systemError.code().value() << " and message " << systemError.code().message();
			std::string s = ss.str();
			MessageBoxA(hwnd, s.c_str(), "ORM Error", MB_OK);
		}
		else if (systemError.code().category() == get_sqlite_error_category()) {
			if (systemError.code() == std::error_code(SQLITE_CONSTRAINT, get_sqlite_error_category()))
			{
				// FK constraint validation
				MessageBoxA(hwnd, "Constraint failed", "SQLITE_CONSTRAINT Error", MB_OK);
			}
			else
			{
				std::ostringstream ss;
				ss << "sqlite error happened with code " << systemError.code().value() << " and message " << systemError.code().message();
				std::string s = ss.str();
				MessageBoxA(hwnd, s.c_str(), "SQLITE Error", MB_OK);
			}
		}
	}
	return systemError.code();
}

inline const char* const OnErrorApply = "No se puede actualizar";
inline const char* const OnErrorErase = "No se puede borrar";

inline void handleApply(std::exception& exc)
{
	MessageBoxA(AfxGetMainWnd()->GetSafeHwnd(), exc.what(), OnErrorApply, MB_OK);
}

inline void handleErase(std::exception& exc)
{
	MessageBoxA(AfxGetMainWnd()->GetSafeHwnd(), exc.what(), OnErrorErase, MB_OK);
}
