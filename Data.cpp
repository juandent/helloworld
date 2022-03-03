

#include "pch.h"
#include "Data.h"


void Storage::initialize()
{
	// initialize tz library asynchronously
	std::thread{ std::chrono::get_tzdb }.detach();

	//fill_db_with_test_data();
//	empty_database();
}

void Storage::upgrade_database()
{
	Storage_Impl::copy_old_to_new();
}
///////////////////////////////////
/// Order for insert:
///
/// Location
/// Password
/// 
/// 

void Storage_Impl::copy_old_to_new()
{
	// starts full
	auto old = get_old_storage();
	// starts empty
	auto fresh = get_new_storage();

	auto fondos = old.get_all<Fondo>();
	for (auto& record : fondos)
	{
		fresh.replace(record);
	}

	auto rendimientos = old.get_all<Rendimiento>();
	for (auto& record : rendimientos)
	{
		fresh.replace(record);
	}

	auto inversiones = old.get_all<Inversion>();
	for (auto& record : inversiones )
	{
		fresh.replace(record);
	}


}

void Storage::backup_db()
{
	namespace fs = std::filesystem;

	auto path_to_db_name = fs::path(Storage_Impl::db_name);
	auto stem = path_to_db_name.stem().string();
	auto backup_stem = stem + "_backup1.sqlite";
	auto backup_full_path = path_to_db_name.parent_path().append(backup_stem).string();
	getStorage().backup_to(backup_full_path);
}



void Storage::fill_db_with_test_data()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();

	storage.begin_transaction();

	//empty_database();

	year_month_day ymd{ year{2021}, month{10}, day{13} };

	const auto today = Today(); // sys_days{ floor<days>(system_clock::now()) };

	sys_days tod = today;
	sys_days daybefore = tod - days{ 1 };

	// Fondo fondo{ -1, "inm1", "FCI", Fondo::trimestral };
	// fondo.id = storage.insert(fondo);
	//
	// Inversion inv{ -1, 100, daybefore, fondo.id };
	// inv.id = storage.insert(inv);
	std::string name = storage.tablename<Fondo>();
	try
	{
		Rendimiento rend{ -1, 1000, 7.45, tod };
		rend.id = storage.insert(rend);
	}
	catch(const std::system_error& exc)
	{
		auto code = handle(exc);
		if( code == std::error_code(SQLITE_CONSTRAINT, get_sqlite_error_category()))
		{
			//MessageBoxA();
		}
	}
	catch(std::exception& exc)
	{
		
	}
	//storage.has_dependent_rows(rend);
	storage.rollback();
}

void Storage::test_syntax()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();


//  SELECT doctor_id
//  FROM visits
//  WHERE LENGTH(patient_name) > 8
	auto selectStatement = storage.prepare(select(&Fondo::id, where(length(&Fondo::nombre) > 2)));
	ostringstream ss;
	ss << "selectStatement = " << selectStatement.sql() << endl;  //  prints "SELECT doctor_id FROM ..."
	auto rows = storage.execute(selectStatement); //  rows is std::vector<decltype(Fondo::id)>

	//  SELECT doctor_id
//  FROM visits
//  WHERE LENGTH(patient_name) > 3
	get<0>(selectStatement) = 3;
	auto rows2 = storage.execute(selectStatement);


	auto allUsersList = storage.get_all<Fondo, std::list<Fondo>>(where(c(&Fondo::id) < 40));

	ss.str("");
	auto concatedUserIdWithDashes = storage.group_concat(&Fondo::id, "---");
	ss << "concatedUserIdWithDashes = " << concatedUserIdWithDashes << endl;

	ss.str("");
	auto countImageUrl = storage.count(&Fondo::nombre);
	ss << "countImageUrl = " << countImageUrl << endl;      //  countImageUrl = 5

	ss.str("");
	//  SELECT SUM(id) FROM users
	if (auto sumId = storage.sum(&Fondo::id)) {    //  sumId is std::unique_ptr<int>
		ss << "sumId = " << *sumId << endl;
	}
	else {
		ss << "sumId is null" << endl;
	}

	ss.str("");
	//  SELECT TOTAL(id) FROM users
	auto totalId = storage.total(&Fondo::id);
	ss << "totalId = " << totalId << endl;    //  totalId is double (always)

	ss.str("");
	//  SELECT * FROM users WHERE id < 43
	auto idLesserThan43 = storage.get_all<Fondo, std::list<Fondo>>(where(c(&Fondo::id) < 43));
	cout << "idLesserThan43 count = " << idLesserThan43.size() << endl;
	for (auto& user : idLesserThan43) {
		ss << storage.dump(user) << endl;
	}

	ss.str("");
	// where
	// In fact you can chain together any number of different conditions with any operator fromand, or and not.All conditions are templated so there is no runtime overhead.And this makes
	auto cuteConditions = storage.get_all<Fondo>(where((c(&Fondo::nombre) == "CFI" or c(&Fondo::abreviacion) == "Alex") and c(&Fondo::id) == 43));  //  where (first_name = 'John' or first_name = 'Alex') and id = 4
	ss << "cuteConditions count = " << cuteConditions.size() << endl; //  cuteConditions count = 1
	cuteConditions = storage.get_all<Fondo>(where(c(&Fondo::nombre) == "John" or (c(&Fondo::abreviacion) == "Alex" and c(&Fondo::id) == 44)));   //  where first_name = 'John' or (first_name = 'Alex' and id = 4)
	ss << "cuteConditions count = " << cuteConditions.size() << endl; //  cuteConditions count = 2


	ss.str("");
	//  SELECT * FROM users WHERE last_name IN ("Doe", "White")
	auto doesAndWhites = storage.get_all<Fondo>(where(in(&Fondo::nombre, { "FCI", "White" })));
	ss << "doesAndWhites count = " << doesAndWhites.size() << endl;
	for (auto& user : doesAndWhites) {
		ss << storage.dump(user) << endl;
	}


	vector<int> ids{ 43,44,45 };
	auto doesinVector = storage.get_all<Fondo>(where(in(&Fondo::nombre, ids)));
	ss << "doesinVector count = " << doesinVector.size() << endl;
	for (auto& user : doesinVector) {
		ss << storage.dump(user) << endl;
	}

	ss.str("");

	//  SELECT * FROM users WHERE id BETWEEN 66 AND 68
	auto betweenId = storage.get_all<Fondo>(where(between(&Fondo::id, 66, 68)));
	cout << "betweenId = " << betweenId.size() << endl;
	for (auto& user : betweenId) {
		cout << storage.dump(user) << endl;
	}

	ss.str("");
	//  SELECT * FROM users WHERE last_name LIKE 'D%'
	auto whereNameLike = storage.get_all<Fondo>(where(like(&Fondo::nombre, "F%")));
	ss << "whereNameLike = " << whereNameLike.size() << endl;
	for (auto& user : whereNameLike) {
		ss << storage.dump(user) << endl;
	}

	// Also you can use remove_all function to perform DELETE FROM ... WHERE query with the same type of conditions.
	storage.transaction([&storage]()->bool
		{
			storage.remove_all<Fondo>(where(c(&Fondo::id) < 30));
			return false;
		});

	ss.str("");
	//  SELECT last_name FROM users WHERE id < 300
	auto allLastNames = storage.select(&Fondo::nombre, where(c(&Fondo::id) < 300));
	ss << "allLastNames count = " << allLastNames.size() << endl; //  allLastNames is std::vector<std::string>
	for (auto& lastName : allLastNames) {
		ss << lastName << " ";
	}
	ss << endl;

	ss.str("");
	//  SELECT id FROM users WHERE image_url IS NULL
	auto idsWithoutUrls = storage.select(&Fondo::id, where(is_null(&Fondo::abreviacion)));
	for (auto id : idsWithoutUrls) {
		ss << "id without image url " << id << endl;
	}

	ss.str("");
	//  SELECT id FROM users WHERE image_url IS NOT NULL
	auto idsWithUrl = storage.select(&Fondo::id, where(is_not_null(&Fondo::abreviacion)));
	for (auto id : idsWithUrl) {
		ss << "id with image url " << id << endl;
	}

	//  `SELECT * FROM users WHERE id > 250 ORDER BY id LIMIT 2, 4`
	auto limited5comma10 = storage.get_all<Fondo>(where(c(&Fondo::id) < 250),
		order_by(&Fondo::id),
		limit(2, 4));	// offset 2, limit 4

	auto crossed = storage.select(columns(&Fondo::id, &Fondo::nombre, &Inversion::fkey_fondo, &Inversion::num_participaciones),
		cross_join<Inversion>());
}

void Storage::empty_database()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();

	// order is vital!
	storage.remove_all<Rendimiento>();
	storage.remove_all<Inversion>();
	storage.remove_all<Fondo>();
}

////////////////////////////////////////
///DB access
///
///

int Fondo::num_participaciones_al(std::chrono::sys_days fecha) const noexcept
{
	using namespace std::chrono;
	using namespace sqlite_orm;

	sys_days when = fecha;

	auto suma_participaciones = Storage::getStorage().select(sum(&Inversion::num_participaciones), where(c(&Inversion::beginning_date) <= when and (c(&Inversion::fkey_fondo) == this->id)), group_by(&Inversion::fkey_fondo));

	if (suma_participaciones.empty()) 	{ return 0; }

	auto suma = *std::move(suma_participaciones[0]);
	auto sum_participaciones = static_cast<int>(suma);

	return sum_participaciones;
}

double Fondo::get_rendimiento_unitario_al(std::chrono::year_month_day fecha) const noexcept
{
	using namespace std::chrono;
	using namespace sqlite_orm;

	sys_days when = fecha;

	auto rendimientos = Storage::getStorage().get_all<Rendimiento>(where(c(&Rendimiento::fecha) <= when and (c(&Rendimiento::fkey_fondo) == this->id)), order_by(&Rendimiento::fecha).desc());

	if (rendimientos.empty())
	{
		return 0.0;
	}

	return rendimientos[0].rendimiento_unitario;
}

#if 0

int Inversion::num_participaciones_en(int fondo, std::chrono::year_month_day fecha) noexcept
{
	using namespace std::chrono;
	using namespace sqlite_orm;

	sys_days when = fecha;

	auto suma_participaciones = Storage::getStorage().select( sum(&Inversion::num_participaciones), where(c(&Inversion::beginning_date) <= when and (c(&Inversion::fkey_fondo) == fondo)), group_by(&Inversion::fkey_fondo));

	if( suma_participaciones.empty())
	{
		return 0;
	}

	auto&& rec = suma_participaciones[0];
	
	auto* suma = rec.get();

	auto sum_participaciones = static_cast<int>(* suma);

	return sum_participaciones;

}


double Rendimiento::get_rendimiento_unitario(int fondo, std::chrono::year_month_day fecha) noexcept
{
	using namespace std::chrono;
	using namespace sqlite_orm;

	sys_days when = fecha;

	// auto rendimientos = Storage::getStorage().select( columns( &Rendimiento::rendimiento_unitario, &Rendimiento::fkey_fondo) , where(c(&Rendimiento::fecha) <= when ), order_by(&Rendimiento::fecha).desc());



	auto rendimientos = Storage::getStorage().get_all<Rendimiento>(where(c(&Rendimiento::fecha) <= when and (c(&Rendimiento::fkey_fondo) == fondo)), order_by(&Rendimiento::fecha).desc());

	if( rendimientos.empty())
	{
		return 0.0;
	}

	return rendimientos[0].rendimiento_unitario;
}
#endif 
