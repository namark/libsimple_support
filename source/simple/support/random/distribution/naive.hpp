#ifndef SIMPLE_SUPPORT_RANDOM_DISTRIBUTION_NAIVE_HPP
#define SIMPLE_SUPPORT_RANDOM_DISTRIBUTION_NAIVE_HPP
#include "../../range.hpp"

namespace simple::support::random::distribution
{

	template <typename Int>
	class naive_int
	{
		public:
		using param_type = range<Int>;
		using result_type = Int;

		explicit constexpr naive_int(param_type range) : r(std::move(range)) {}
		explicit constexpr naive_int(result_type min, result_type max)
			: r{std::move(min), std::move(max)} {}
		explicit constexpr naive_int(result_type max) : naive_int(0, max) {}


		constexpr const result_type& min() const noexcept { return r.lower(); }
		constexpr const result_type& max() const noexcept { return r.upper(); }

		template <typename Engine, std::enable_if_t<
			std::is_convertible_v<typename Engine::result_type, result_type> >* = nullptr>
		constexpr result_type operator()(Engine& engine, const param_type& range) const
		{
			return range.lower() + engine()%(range.upper() - range.lower());
		}

		template <typename Engine>
		constexpr result_type operator()(Engine& engine) const
		{
			return operator()(engine, r);
		}

		private:
		param_type r;
	};

	template <typename Real>
	class naive_real
	{
		public:
		using param_type = range<Real>;
		using result_type = Real;

		explicit constexpr naive_real(param_type range) : r(std::move(range)) {}
		explicit constexpr naive_real(result_type min, result_type max)
			: r{std::move(min), std::move(max)} {}
		explicit constexpr naive_real(result_type max) : naive_real(0, max) {}

		constexpr const result_type& min() const noexcept { return r.lower(); }
		constexpr const result_type& max() const noexcept { return r.upper(); }

		template <typename Engine, std::enable_if_t<
			std::is_convertible_v<typename Engine::result_type, result_type> >* = nullptr>
		constexpr result_type operator()(Engine& engine, const param_type& range) const
		{
			const auto min = get_min<Engine>();
			const auto max = get_max<Engine>();
			const auto diff = max - min;
			result_type random = engine() - min;
			random /= diff;
			return range.lower() + random * (range.upper() - range.lower());
		}

		template <typename Engine>
		constexpr result_type operator()(Engine& engine) const
		{
			return operator()(engine, r);
		}
		private:
		template <typename Engine, typename = std::nullptr_t >
		struct has_min_s { constexpr static bool value = false; };
		template <typename Engine>
		struct has_min_s<Engine, decltype(Engine::min(), nullptr)> { constexpr static bool value = true; };
		template <typename Engine>
		constexpr static bool has_min = has_min_s<Engine>::value;

		template <typename Engine, typename = std::nullptr_t>
		struct has_max_s { constexpr static bool value = false; };
		template <typename Engine>
		struct has_max_s<Engine, decltype(Engine::max(), nullptr)> { constexpr static bool value = true; };
		template <typename Engine>
		constexpr static bool has_max = has_max_s<Engine>::value;

		template <typename Engine, std::enable_if_t<has_min<Engine>>* = nullptr>
		constexpr static result_type get_min() { return Engine::min(); }
		template <typename Engine, std::enable_if_t<has_max<Engine>>* = nullptr>
		constexpr static result_type get_max() { return Engine::max(); }

		template <typename Engine, std::enable_if_t<!has_min<Engine>>* = nullptr>
		constexpr static result_type get_min()
		{ return std::numeric_limits<typename Engine::result_type>::min(); }
		template <typename Engine, std::enable_if_t<!has_min<Engine>>* = nullptr>
		constexpr static result_type get_max()
		{ return std::numeric_limits<typename Engine::result_type>::max(); }

		param_type r;
	};

} // namespace simple::support::random::distribution

#endif /* end of include guard */
