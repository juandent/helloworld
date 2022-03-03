#pragma once

#include "RecordLinks.h"
#include "Data.h"

template<typename Table, int Table::* keyCol>
struct RefIntegrityManager
{
public:
	Storage::Storage_t& storage;
	void throwIfcannotDelete(Table const& record)	// may throw std::exception
	{
		!RecordLinks::has_links(record);
	}
	void throwIfcannotInsertUpdate(Table const& record)
	{
		RecordLinks::foreignKeysExist(record);
	}
private:
	///
	template<typename ...Cols>
	Table do_insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		record.*keyCol = storage.insert(record);
		return record;
	}
	void do_update(const Table& record)
	{
		// TODO: we must ensure keyCol is not -1 in the caller! Remove this if...
		if (record.*keyCol == -1)	return;

		storage.update(record);
	}
public:
	RefIntegrityManager() : storage{ Storage::getStorage() } {}

	template<typename ...Cols>
	std::optional<Table> insert(Cols&&... cols)
	{
		Table record{ -1, cols... };

		throwIfcannotInsertUpdate(record);
		record.*keyCol = storage.insert(record);
		return record;
	}
	void update(const Table& record)
	{
		// if (record.*keyCol == -1)	return;	// not persisted yet!
		throwIfcannotInsertUpdate(record);
		storage.update(record);
	}

	Table get( long long id)
	{
		Table record;
		try
		{
			record = storage.get<Table>(id);
		}
		catch (std::exception& exp)
		{
			// handle(exp, false);
			std::ostringstream ss;
			ss << "PK = " << id << " no existe en tabla " << storage.tablename<Table>();
			throw std::exception(ss.str().c_str());
		}
		return record;
	}
private:
	bool remove(Table& record)
	{
		throwIfcannotDelete(record);

		storage.remove<Table>(get_pk(record));
		return true;
	}

	int get_pk(Table& record)
	{
		return record.*keyCol;
	}
};

template<typename Table, int Table::* keyCol >
struct Accessor
{
private:
	Storage::Storage_t& storage;
public:
	Accessor() : storage{ Storage::getStorage() } {}

	std::vector<Table> getAll()
	{
		return storage.get_all<Table>();
	}
	template<typename whereClause>
	std::vector<Table> getAll(whereClause clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(clause));
	}

	template<typename orderByClause>
	std::vector<Table> getAllOrderBy(orderByClause clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(order_by(clause).asc().collate_nocase());
	}

	template<typename orderByClause, typename whereClause>
	std::vector<Table> getAllOrderByWhere(orderByClause clause, whereClause where_clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(where_clause), order_by(clause).asc().collate_nocase());
	}

	template<typename orderByClause, typename whereClause>
	std::vector<Table> getAllOrderByDescWhere(orderByClause clause, whereClause where_clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(where_clause), order_by(clause).desc().collate_nocase());
	}

	template<typename orderByClause>
	std::vector<Table> getAllOrderByDesc(orderByClause clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(order_by(clause).desc().collate_nocase());
	}



	template<typename WhereClause> //, typename ...Cols>
	std::optional<Table> exists(WhereClause& clause) //, Cols&& ... cols)
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

};
