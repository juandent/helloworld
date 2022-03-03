// GenericGrid.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "GenericGrid.h"


// GenericGrid dialog

IMPLEMENT_DYNAMIC(GenericGrid, CDialog)

GenericGrid::GenericGrid(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GenericGrid, pParent)
{

}

GenericGrid::~GenericGrid()
{
}

void GenericGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_GENERIC, m_generic_grid);
}


BEGIN_MESSAGE_MAP(GenericGrid, CDialog)
END_MESSAGE_MAP()


// GenericGrid message handlers




struct Employee {
    int id;
    std::string name;
    int age;
    std::optional<std::string> address;
    std::optional<double> salary;

    bool operator==(const Employee& other) const {
        return this->id == other.id && this->name == other.name && this->age == other.age &&
            this->address == other.address && this->salary == other.salary;
    }
};

inline namespace ORM
{
    auto storage = make_storage({ "generic.sqlite" },
        make_table("COMPANY",
            make_column("ID", &Employee::id, primary_key()),
            make_column("NAME", &Employee::name),
            make_column("AGE", &Employee::age),
            make_column("ADDRESS", &Employee::address),
            make_column("SALARY", &Employee::salary)));

    void insertEmployees()
    {
        storage.sync_schema();

        storage.remove_all<Employee>();

        //  create employees..
        Employee paul{ -1, "Paul", 32, std::nullopt, 20000.0 };
        Employee allen{ -1, "Allen", 25, "Texas", 15000.0 };
        Employee teddy{ -1, "Teddy", 23, "Norway", 20000.0 };
        Employee mark{ -1, "Mark", 25, "Rich-Mond", 65000.0 };
        Employee david{ -1, "David", 27, "Texas", 85000.0 };
        Employee kim{ -1, "Kim", 22, "South-Hall", 45000.0 };
        Employee james{ -1, "James", 24, "Houston", 10000.0 };

        //  insert employees. `insert` function returns id of inserted object..
        paul.id = storage.insert(paul);
        allen.id = storage.insert(allen);
        teddy.id = storage.insert(teddy);
        mark.id = storage.insert(mark);
        david.id = storage.insert(david);
        kim.id = storage.insert(kim);
        james.id = storage.insert(james);

    }
}


BOOL GenericGrid::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    insertEmployees();
    InitializeGridRendimientos(true);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}



auto full_outer_join()
{
    struct Dog
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };


    struct Cat
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };

    using namespace sqlite_orm;

    auto storage = make_storage(
        { "full_outer.sqlite" },
        make_table("Dogs", make_column("type", &Dog::type), make_column("color", &Dog::color)),
        make_table("Cats", make_column("type", &Cat::type), make_column("color", &Cat::color)));

    storage.sync_schema();
    storage.remove_all<Dog>();
    storage.remove_all<Cat>();

    storage.insert(into<Dog>(), columns(&Dog::type, &Dog::color), values(std::make_tuple("Hunting", "Black"), std::make_tuple("Guard", "Brown")));
    storage.insert(into<Cat>(), columns(&Cat::type, &Cat::color), values(std::make_tuple("Indoor", "White"), std::make_tuple("Outdoor", "Black")));


    auto left = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), left_join<Cat>(using_(&Cat::color)));
    auto right = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(), left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)));




    /*  FULL OUTER JOIN simulation:
     *
            SELECT d.type,
            d.color,
            c.type,
            c.color
            FROM dogs d
            LEFT JOIN cats c USING(color)
            UNION ALL
            SELECT d.type,
            d.color,
            c.type,
            c.color
            FROM cats c
            LEFT JOIN dogs d USING(color)
            WHERE d.color IS NULL;
    */
    static auto rows = storage.select(
        union_all(select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color),
            left_join<Cat>(using_(&Cat::color))),
            select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(),
                left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)))));


    return rows;
}





template <typename T>
void GenericGrid::InitializeGridRendimientos(const T& t)
{
// #define distinct
    // auto rows = storage.select(distinct(columns(&Employee::address, &Employee::name)), from<Employee>(), where(t));
    auto rows = full_outer_join();

	std::vector<std::string> headers{ "TYPE", "COLOR", "TYPE", "COLOR" };

	m_grid_displayer.reset(new JoinedGridDisplayer<decltype(rows[0]), IntegerList<>, IntegerList<>>(m_generic_grid, std::move(rows), std::move(headers)));
	m_grid_displayer->display();
}
