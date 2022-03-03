#pragma once

#include <exception>

#include "Data.h"

#include "Typelist.h"



template <typename T, int T::* K> 			requires (is_persistent<T>::value)
struct TableKey;

template<typename T>
struct is_tableKey
{
	static constexpr bool value = false;
};

template<typename persistence_class, int persistence_class::* key>    requires (is_persistent<persistence_class>::value )
struct is_tableKey<TableKey<persistence_class, key>>
{
	static constexpr bool value = true;
};



// Examples:
//	Fondo with PK id
//  Rendimiento with FK fkey_fondo
//	Inversion with FK fkey_fondo
// 
// T is a persistent class
// K is its primary key
template <typename T, int T::* K>			requires (is_persistent<T>::value)
struct TableKey
{
	using Table = T;
	using KeyType = decltype(K);

	static constexpr KeyType Key = K;

	static void setRecord(const Table* const record)
	{
		tableData = record;
	}
	static int getKeyValue()
	{
		if (tableData == nullptr) throw std::exception{ "tableData is null" };
		return tableData->*Key;
	}
private:
	inline static const Table* tableData = nullptr;
};


#if 0
// template <typename DepPair, typename TargetPair>
// struct TableConnection;

template<typename T, typename Tuple>
struct TypeInTuple
{
	constexpr static bool exists()
	{
		constexpr unsigned size = std::tuple_size<Tuple>::value;
		bool match = matches<size>();
		return match;
	}

private:
	template<unsigned N>
	constexpr static bool matches()
	{
		using V = std::tuple_element_t < N - 1, Tuple>;
		bool match = std::is_same_v<T, V>;
		return match || matches<N - 1>();
	}
	template<>
	constexpr static bool matches<0>()
	{
		return false;
	}

};
#endif

// T and RefBy are TableKeys
// Target is special, has primary key
// The TableKeys in RefBy have foreign keys
template <typename T, typename ...RefBy>   requires ( is_tableKey<T>::value && (is_tableKey<RefBy>::value && ...))
struct PKDependencies
{
public:
	using Target = T;
	static std::tuple<RefBy...> reference_list;

	static bool has_links(const typename Target::Table& targetRec)
	{
		Target::setRecord(&targetRec);
		auto pk = Target::getKeyValue();
		if( pk == -1)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<typename Target::Table>() << " no está almacenado";
			throw std::exception(ss.str().c_str());
		}
		constexpr size_t size = std::tuple_size_v<decltype(reference_list)>;
		bool has = has_links<size>();
		if(has)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<typename Target::Table>() << " tiene dependientes";
			throw std::exception(ss.str().c_str());
		}
		return has;
	}
private:
	template<size_t index>
	static bool has_links()
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		// is a TableKey
		using DependentClass = std::tuple_element_t<index - 1, decltype(reference_list)>;

		bool has = storage.count<DependentClass::Table>(where(is_equal(DependentClass::Key, Target::getKeyValue()))) > 0;

		return has || has_links<index - 1>();
	}
	template<>
	static bool has_links<0>()  { return false; }
};

#if 1
// T is TableKey we are looking if present in Tuple
// Tuple is a std::tuple<TableKeys>
//  

template<typename T, typename Tuple>   requires (is_tableKey<T>::value)
struct TableKeyInTuple
{
	using tableKey = T;
	constexpr static int index()
	{
		constexpr unsigned size = std::tuple_size<Tuple>::value;
		int index = find<size>();
		return index;
	}

	// using type = typename std::enable_if<position() != -1, tableKey>::type;

private:
	template<unsigned N>
	constexpr static int find()
	{
		using V = std::tuple_element_t < N-1, Tuple>;
		bool match = std::is_same_v<T, V>;
		if (match)
			return N-1;
		else 
			return find<N - 1>();
	}
	template<>
	constexpr static int find<0>()
	{
		return -1;
	}
};

#if 0
template<typename list>
struct List
{
	using type = list;
};

template<typename list, typename T>
struct ListAppend : List<list>
{
	using Result = typename Loki::TL::Append<list, T>::Result;
};

template<typename List, typename T>
struct IncreasingCollection
{
	using type = typename Loki::TL::Append<List, T>::Result;

};

struct IncCollectionImpl
{
	using Collection = typename Loki::Typelist < Loki::NullType, Loki::NullType >;
	using list = IncreasingCollection::impl<Collection, int>::list;
	using list2 = IncreasingCollection::impl<list, double>::list;
};
#endif

template<typename Col, typename T, int N>
struct static_if
{
	using type = typename Loki::TL::Append<Col, T>::Result;
};

template<typename Col, typename T>
struct static_if<Col, T, -1>
{
	using type = Col;
};



#endif

// connection between 2 TableKey's:
// Dependent pair
//		DepPair::Table = Rendimiento
//		DepPair::Key =   getKeyValue()
// Target pair
//		TargetPair::Table = Fondo
//		TargetPair::Key = &Fondo.id

template <typename DepPair, typename TargetPair>   requires (is_tableKey<DepPair>::value && is_tableKey<TargetPair>::value)
struct TableConnection
{
	using Dependent = DepPair;
	using Target = TargetPair;

	static bool foreignKeyExists(const typename Dependent::Table& dep)
	{
		Dependent::setRecord(&dep);

		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		int count = storage.count<typename Target::Table>(where(is_equal(Target::Key, Dependent::getKeyValue())));
		return count > 0;
	}
};


// List of TableConnection instances
template <typename List>
struct TableConnectionList
{
	using typeAt0 = typename Loki::TL::TypeAt<List, 0>::Result;
	using DependentTable = typename typeAt0::Dependent::Table;

	static bool foreignKeysExist(const DependentTable& dep)
	{
		constexpr size_t size = Loki::TL::Length<List>::value;
		bool exists = foreignKeyExists<size>(dep);

		if (!exists)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<DependentTable>() << " contiene dangling FKs";
			throw std::exception(ss.str().c_str());
		}
		return exists;
	}
private:
	template<size_t index>
	static bool foreignKeyExists(const DependentTable& dep)
	{
		// is a TableConnection
		using Connection = typename Loki::TL::TypeAt<List, index - 1>::Result;
		bool exists = Connection::foreignKeyExists(dep);

		return exists && foreignKeyExists<index - 1>(dep);
	}
	template<>
	static bool foreignKeyExists<0>(const DependentTable& dep)
	{
		return true;
	}
};

template<>
struct TableConnectionList<Loki::NullType>
{
	static bool foreignKeysExist(...)
	{
		return true;
	}
};
// T is a TableKey, also known as T::Target or tableKey
// tblDefs are PKDependencies types
//		tblDefs[x]::reference_list is list of TableKeys
// we are going to search TableKey T in each tblDefs[x]::reference_list
//	for each found we will create a TableConnection<T, tblDefs[x]::Target>
//	we add such connection to TableConnections list by appending
//	(see struct append inside struct build and see how the list is "exported"
//	in struct construct: see how construct triggers the building of the list
//	and reports its final stage)

template<typename T, typename...tblDefs >
struct FKDependencies
{
	using tableKey = T;
	static std::tuple<tblDefs...> tblDefs_list;
private:
	template<typename Col>
	struct build
	{
		template <typename Col, int N>
		struct append
		{
		private:
			using tableDef = std::tuple_element_t<N - 1, decltype(tblDefs_list)>;
			using tableKeys = decltype(tableDef::reference_list);
			static constexpr int index = TableKeyInTuple < tableKey, tableKeys>::index();
			using typeToAdd = TableConnection<tableKey, typename tableDef::Target>;
			using newCol = static_if<Col, typeToAdd, index>::type;
		public:
			using result = typename append<newCol, N - 1>::result;
		};
		template<typename Col>
		struct append<Col, 0>
		{
			using result = Col;
		};
		static constexpr int size = std::tuple_size_v<decltype(tblDefs_list)>;
		using result = typename append<Col, size>::result;

	};
public:
	struct construct
	{
	private:
		using res = typename build<Loki::NullType>::result;
	public:
		using result = TableConnectionList<res>;
	};
};

#if 0
// T and Conns are TableConnection instances
template <typename T, typename ...Conns>
struct TableConnections
{
	using DependentTable = typename T::Dependent::Table;
	static std::tuple<T, Conns...> connections_list;

	static bool foreignKeysExist(const DependentTable& dep)
	{
		constexpr size_t size = std::tuple_size_v<decltype(connections_list)>;
		bool exists = foreignKeyExists<size>(dep);

		if( ! exists)
		{
			std::ostringstream ss;
			ss << "Registro de " << Storage::getStorage().tablename<DependentTable>() << " contiene dangling FKs";
			throw std::exception(ss.str().c_str());
		}
		return exists;
	}
private:
	template<size_t index>
	static bool foreignKeyExists(const DependentTable& dep)
	{
		// is a TableConnection
		using Connection = std::tuple_element_t<index - 1, decltype(connections_list)>;
		bool exists = Connection::foreignKeyExists(dep);

		return exists && foreignKeyExists<index - 1>(dep);
	}
	template<>
	static bool foreignKeyExists<0>(const DependentTable& dep)
	{
		return true;
	}
};
#endif
