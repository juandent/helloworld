#pragma once



class RendimientoCompositeImpl
{
	friend class RendimientoComposite;
	// Primary Key must exist!
	inline	static auto query(int pk)
	{
		using namespace std::literals;
		auto rows = Storage::getStorage().select(columns(
			alias_column<als_r>(&Rendimiento::id),
			alias_column<als_f>(&Fondo::nombre),
			alias_column<als_r>(&Rendimiento::fecha),
			alias_column<als_r>(&Rendimiento::rendimiento_unitario)),

			inner_join<als_f>(on(c(alias_column<als_r>(&Rendimiento::fkey_fondo)) == alias_column<als_f>(&Fondo::id))),

			where(is_equal(alias_column<als_r>(&Rendimiento::id), pk)),
			multi_order_by(order_by(alias_column<als_r>(&Rendimiento::fkey_fondo)), order_by(alias_column<als_r>(&Rendimiento::fecha)).desc()));

		if( rows.empty())
		{
			std::ostringstream ss;
			ss << "Primary Key " << pk << " does not exist!";
			throw std::exception{ ss.str().c_str()};
		}
		return rows[0];
	}
};



class RendimientoComposite
{
	using row_type = decltype(RendimientoCompositeImpl::query(1));
	row_type row;
	const auto& clause() const noexcept	{ return is_equal(&Rendimiento::id, get_id());	}
public:
	RendimientoComposite(int pk) : row{ RendimientoCompositeImpl::query(pk) }	{}

	int get_id() const 	{	return std::get<0>(row); }
	std::string get_nombre_fondo() const { 	return std::get<1>(row);}
	std::chrono::sys_days get_rendimiento_fecha() const { return std::get<2>(row);}

	double get_rendimiento_unitario() const { return std::get<3>(row); }

	void set_rendimiento_unitario(double rend)
	{
		Storage::getStorage().update_all(set(assign(&Rendimiento::rendimiento_unitario, rend)), where(clause()));
	}
};
