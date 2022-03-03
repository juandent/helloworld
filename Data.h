#pragma once
#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
#include <chrono>



import Util;
//#include "..\JDMFCLibrary/Util.h"

// #include "../FixedPoint/Money.h"
#if 1
// #include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\ORM_Extensions/DateBinding.h"
#else
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"
#endif

#include "PersistentClasses.h"

class Storage_Impl
{
private:
	Storage_Impl() = delete;	// prohibit instantiation
	static auto& get_storage();
	static auto& get_old_storage();
	static auto& get_new_storage();
	static void copy_old_to_new();

	friend class Storage;

	
#ifdef _DEBUG_DB
	static constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Finances\\InvestmentsDebug.sqlite" };
#else
	static constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Finances\\Investments.sqlite" };
#endif
};


inline 	auto& Storage_Impl::get_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;


	static auto storage =
		make_storage(db_name,
			make_trigger("validate_fields_before_insert_fondos",
				before()
				.insert()
				.on<Fondo>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Fondo::abreviacion),""),
						then(raise_abort("Fondo abreviacion empty")))
					.when(is_equal(length(new_(&Fondo::nombre)), 0),
						then(raise_abort("Fondo nombre empty")))
					.end()))
				.end()),

			make_trigger("validate_num_participaciones_before_insert_inversion",
				before()
				.insert()
				.on<Inversion>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Inversion::num_participaciones), 0),
						then(raise_abort("Numero de participaciones no puede ser 0")))
					.end()))
				.end()),
			make_trigger("validate_rendimiento_unitario_before_insert_rendimiento",
				before()
				.insert()
				.on<Rendimiento>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Rendimiento::rendimiento_unitario), 0.0),
						then(raise_abort("Rendimiento unitario no puede ser 0")))
					.end()))
				.end()),
			make_trigger("validate_fields_before_update_fondos",
				before()
				.update()
				.on<Fondo>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Fondo::abreviacion), ""),
						then(raise_abort("Fondo abreviacion empty")))
					.when(is_equal(length(new_(&Fondo::nombre)), 0),
						then(raise_abort("Fondo nombre empty")))
					.end()))
				.end()),
			make_trigger("validate_num_participaciones_before_update_inversion",
				before()
				.update()
				.on<Inversion>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Inversion::num_participaciones), 0),
						then(raise_abort("Numero de participaciones no puede ser 0")))
					.end()))
				.end()),
			make_trigger("validate_rendimiento_unitario_before_update_rendimiento",
				before()
				.update()
				.on<Rendimiento>()
				.begin(select(case_<int>()
					.when(is_equal(new_(&Rendimiento::rendimiento_unitario), 0.0),
						then(raise_abort("Rendimiento unitario no puede ser 0")))
					.end()))
				.end()),



			make_table("Fondos",
				make_column("id_fondo", &Fondo::id, autoincrement(), primary_key()),
				make_column("nombre", &Fondo::nombre),
				make_column("abrev", &Fondo::abreviacion),
				make_column("tipo_cupon", &Fondo::tipo_cupon)),
			make_table("Inversiones",
				make_column("id_inversion", &Inversion::id, autoincrement(), primary_key()),
				make_column("fkey_fondo", &Inversion::fkey_fondo),
				make_column("begin_date", &Inversion::beginning_date),
				make_column("num_participaciones", &Inversion::num_participaciones),
				foreign_key(&Inversion::fkey_fondo).references(&Fondo::id)),
			make_table("Rendimientos",
				make_column("id_rendimiento", &Rendimiento::id, autoincrement(), primary_key()),
				make_column("fecha", &Rendimiento::fecha),
				make_column("rend_unitario", &Rendimiento::rendimiento_unitario),
				make_column("fkey_fondo", &Rendimiento::fkey_fondo),
				foreign_key(&Rendimiento::fkey_fondo).references(&Fondo::id)));


	if (flag == 0)
	{
		flag = 1;
		auto syncSchemaRes = storage.sync_schema(true);
		ostringstream oss;
		for (auto& p : syncSchemaRes) {
			oss << p.first << " " << p.second << endl;
		}
		auto s = oss.str();
		std::ignore = s;
	}

	return storage;
}


inline auto& Storage_Impl::get_old_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;

	constexpr int version = 2;

	// this file will start full
	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Finances\\old.sqlite" };

	static auto storage =
		make_storage(db_name,
			make_table("Fondos",
				make_column("id_fondo", &Fondo::id, autoincrement(), primary_key()),
				make_column("nombre", &Fondo::nombre),
				make_column("abrev", &Fondo::abreviacion),
				make_column("tipo_cupon", &Fondo::tipo_cupon)),
			make_table("Inversiones",
				make_column("id_inversion", &Inversion::id, autoincrement(), primary_key()),
				make_column("fkey_fondo", &Inversion::fkey_fondo),
				make_column("begin_date", &Inversion::beginning_date),
				make_column("num_participaciones", &Inversion::num_participaciones),
				foreign_key(&Inversion::fkey_fondo).references(&Fondo::id)),
			make_table("Rendimientos",
				make_column("id_rendimiento", &Rendimiento::id, autoincrement(), primary_key()),
				make_column("fecha", &Rendimiento::fecha),
				make_column("rend_unitario", &Rendimiento::rendimiento_unitario),
				make_column("fkey_fondo", &Rendimiento::fkey_fondo),
				foreign_key(&Rendimiento::fkey_fondo).references(&Fondo::id)));


	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(false);
	}

	return storage;
}

inline auto& Storage_Impl::get_new_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;
	// this file will start empty
	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Finances\\New.sqlite" };

	constexpr int version = 3;

	static auto storage =
		make_storage(db_name,
			make_table("Fondos",
				make_column("id_fondo", &Fondo::id, autoincrement(), primary_key()),
				make_column("nombre", &Fondo::nombre),
				make_column("abrev", &Fondo::abreviacion),
				make_column("tipo_cupon", &Fondo::tipo_cupon)),
			make_table("Inversiones",
				make_column("id_inversion", &Inversion::id, autoincrement(), primary_key()),
				make_column("fkey_fondo", &Inversion::fkey_fondo),
				make_column("begin_date", &Inversion::beginning_date),
				make_column("num_participaciones", &Inversion::num_participaciones),
				foreign_key(&Inversion::fkey_fondo).references(&Fondo::id)),
			make_table("Rendimientos",
				make_column("id_rendimiento", &Rendimiento::id, autoincrement(), primary_key()),
				make_column("fecha", &Rendimiento::fecha),
				make_column("rend_unitario", &Rendimiento::rendimiento_unitario),
				make_column("fkey_fondo", &Rendimiento::fkey_fondo),
				foreign_key(&Rendimiento::fkey_fondo).references(&Fondo::id)));

	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(true);
	}

	return storage;

}

class Storage
{
public:
	using Storage_t = decltype(Storage_Impl::get_storage());

	Storage();
	static void initialize();
	static Storage_t& getStorage() { return Storage_Impl::get_storage(); }
	static void fill_db_with_test_data();
	static void empty_database();
	static void upgrade_database();
	static void backup_db();
	static void test_syntax();
};

//inline Storage::Storage_t& storage = Storage::getStorage();

#include "..\ORM_Extensions/ORM_Utilities.h"

