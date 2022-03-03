#include "pch.h"



#include "RecordLinks.h"

bool RecordLinks::has_links(const Fondo& fondo)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count[2];
	count [0] = storage.count<Rendimiento>(where(is_equal(&Rendimiento::fkey_fondo, fondo.id)));
	count [1] = storage.count<Inversion>(where(is_equal(&Inversion::fkey_fondo, fondo.id)));

	return count[0] || count[1];
}

bool RecordLinks::has_links(const Inversion& inv)
{
	return false;
}

bool RecordLinks::has_links(const Rendimiento& rend)
{
	return false;
}


bool RecordLinks::foreignKeysExist(const Inversion& inv)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count;

	count = storage.count<Fondo>(where(is_equal(&Fondo::id, inv.fkey_fondo)));

	return count > 0;
}

bool RecordLinks::foreignKeysExist(const Rendimiento& rend)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count;

	count = storage.count<Fondo>(where(is_equal(&Fondo::id, rend.fkey_fondo)));

	return count > 0;
}


bool RecordLinks::foreignKeysExist(const Fondo& fondo)
{
	return true;
}
