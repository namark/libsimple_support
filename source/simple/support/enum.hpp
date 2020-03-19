#ifndef _SIMPLE_ENUM_HPP
#define _SIMPLE_ENUM_HPP

#include <type_traits>
#include <string>
#include <algorithm>
#include <array>
#include "function_utils.hpp"
#include "type_traits.hpp"

namespace simple::support
{

	namespace enum_details
	{

		template<typename Type, typename = std::nullptr_t>
		struct has_default
		{ static constexpr bool value = false; };
		template<typename Type>
		struct has_default<Type, decltype((void)Type::default_value, nullptr)>
		{ static constexpr bool value = true; };

		template<typename Type, typename = std::nullptr_t>
		struct has_to_conversion
		{ static constexpr bool value = false; };
		template<typename Type>
		struct has_to_conversion<Type, decltype((void)Type::to, nullptr)>
		{ static constexpr bool value = true; };

		template<typename Type, typename = std::nullptr_t>
		struct has_from_conversion
		{ static constexpr bool value = false; };
		template<typename Type>
		struct has_from_conversion<Type, decltype((void)Type::from, nullptr)>
		{ static constexpr bool value = true; };

	} // namespace enum_details

	template<typename GutsType>
	class enum_wrapper : public GutsType
	{

		public:
		using Guts [[deprecated]] = GutsType;
		using Type [[deprecated]] = typename Guts::Type;
		using guts = GutsType;
		using type = typename guts::type;

		template<typename G=guts, std::enable_if_t<enum_details::has_default<G>::value>* = nullptr>
		enum_wrapper(type type = guts::default_value) : _type{type}
		{ }

		template<typename G=guts, std::enable_if_t<!enum_details::has_default<G>::value>* = nullptr>
		enum_wrapper(type type) : _type{type}
		{ }

		template<typename G=guts, std::enable_if_t<!enum_details::has_default<G>::value>* = nullptr>
		enum_wrapper() : _type{}
		{ }

		enum_wrapper& operator=(type type)
		{
			_type = type;
			return *this;
		}

		operator type() const
		{
			return _type;
		}

		template<typename G=guts, std::enable_if_t<enum_details::has_from_conversion<G>::value>* = nullptr>
		explicit enum_wrapper(const typename support::fun_param_t<decltype(G::from), 0>& name)
		{
			// TODO: static assert one parameter
			// TODO: static assert return type to be correct
			_type = guts::from(name);
		}

		template<typename G=guts, std::enable_if_t<enum_details::has_to_conversion<G>::value>* = nullptr>
		explicit operator typename support::fun_return_t<decltype(G::to)>() const
		{
			// TODO: static assert one parameter
			// TODO: static assert parameter type to be correct
			return guts::to(_type);
		}

#ifdef __clang__ // cause GCC can't handle the SFINAE below
		template<typename Integral,
			std::enable_if_t<std::is_integral<Integral>::value>* = nullptr>
		operator Integral() const = delete;
#endif

		private:
		type _type;

	};

	template <typename EnumType,
			 EnumType DefaultEnumValue,
			 size_t MappedValueCount = 1,
			 typename MappedType = std::string>
	struct mapped_enum_guts
	{
		using Type [[deprecated]] = EnumType;
		using type = EnumType;
		using UType [[deprecated]] = std::underlying_type_t<Type>;
		using underlying_type = std::underlying_type_t<type>;
		using map_type = const std::array<
			std::array<const MappedType, MappedValueCount>,
			static_cast<std::size_t>(DefaultEnumValue)>;
		static constexpr auto Default [[deprecated]] = DefaultEnumValue;
		static constexpr auto default_value = DefaultEnumValue;
		static map_type map;

		static type from(const MappedType& value)
		{
			for(underlying_type i = 0; i < static_cast<underlying_type>(default_value); ++i)
			{
				auto&& variants = map[i];
				auto end = std::end(variants);
				auto begin = std::begin(variants);
				auto found = std::find(begin, end, value);
				if(found != end)
					return type(i);
			}
			return default_value;
		}

		static MappedType to(const type& value)
		{
			return map[static_cast<underlying_type>(value)][0];
		}
	};

	template <typename EnumType,
			 EnumType DefaultEnumValue,
			 size_t MappedValueCount = 1,
			 typename MappedType = std::string>
	using MappedEnum [[deprecated]] = enum_wrapper<mapped_enum_guts<EnumType, DefaultEnumValue, MappedValueCount, MappedType>>;

	template <typename EnumType,
			 EnumType DefaultEnumValue,
			 size_t MappedValueCount = 1,
			 typename MappedType = std::string>
	using mapped_enum = enum_wrapper<mapped_enum_guts<EnumType, DefaultEnumValue, MappedValueCount, MappedType>>;

	template <typename Enum, typename Int,
		 std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
	constexpr Int to_integer(const Enum& value)
	{ return static_cast<Int>(value); };

	template <typename Enum,
		 std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
	constexpr auto to_integer(const Enum& value)
	{ return to_integer<Enum,std::underlying_type_t<Enum>>(value); };

	template <typename EnumWrapper,
		 std::enable_if_t<
			 is_template_instance<EnumWrapper,enum_wrapper>::value>
		 * = nullptr
	>
	constexpr auto to_integer(const EnumWrapper& value)
	{ return to_integer<typename EnumWrapper::type>(value);};

} // namespace simple::support

#endif /* end of include guard */
