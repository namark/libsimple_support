#ifndef _SIMPLE_ENUM_HPP
#define _SIMPLE_ENUM_HPP

#include <type_traits>
#include <string>
#include "function_utils.hpp"

namespace simple { namespace support
{

	namespace __Enum_Details
	{

		template<typename Type, typename = std::nullptr_t>
		struct hasDefault
		{ static constexpr bool value = false; };
		template<typename Type>
		struct hasDefault<Type, decltype((void)Type::Default, nullptr)>
		{ static constexpr bool value = true; };

		template<typename Type, typename = std::nullptr_t>
		struct hasToConversion
		{ static constexpr bool value = false; };
		template<typename Type>
		struct hasToConversion<Type, decltype((void)Type::to, nullptr)>
		{ static constexpr bool value = true; };

		template<typename Type, typename = std::nullptr_t>
		struct hasFromConversion
		{ static constexpr bool value = false; };
		template<typename Type>
		struct hasFromConversion<Type, decltype((void)Type::from, nullptr)>
		{ static constexpr bool value = true; };

	} // namespace __Enum_Impl

	template<typename GutsType>
	class Enum : public GutsType
	{

		public:
		using Guts = GutsType;
		using Type = typename Guts::Type;

		template<typename G=Guts, std::enable_if_t<__Enum_Details::hasDefault<G>::value>* = nullptr>
		Enum(Type type = Guts::Default) : _type{type}
		{ }

		template<typename G=Guts, std::enable_if_t<!__Enum_Details::hasDefault<G>::value>* = nullptr>
		Enum(Type type) : _type{type}
		{ }

		template<typename G=Guts, std::enable_if_t<!__Enum_Details::hasDefault<G>::value>* = nullptr>
		Enum() : _type{}
		{ }

		Enum& operator=(Type type)
		{
			_type = type;
			return *this;
		}

		operator Type() const
		{
			return _type;
		}

		template<typename G=Guts, std::enable_if_t<__Enum_Details::hasFromConversion<G>::value>* = nullptr>
		explicit Enum(const typename support::fun_param_t<decltype(G::from), 0>& name)
		{
			// TODO: static assert one parameter
			// TODO: static assert return type to be correct
			_type = Guts::from(name);
		}

		template<typename G=Guts, std::enable_if_t<__Enum_Details::hasToConversion<G>::value>* = nullptr>
		explicit operator typename support::fun_return_t<decltype(G::to)>() const
		{
			// TODO: static assert one parameters
			// TODO: static assert parameter type to be correct
			return Guts::to(_type);
		}

#ifdef __clang__ // cause GCC can't handle the SFINAE below
		template<typename Integral,
			std::enable_if_t<std::is_integral<Integral>::value>* = nullptr>
		operator Integral() const = delete;
#endif

		private:
		Type _type;

	};

}} // namespace simple::support


#endif /* end of include guard */
