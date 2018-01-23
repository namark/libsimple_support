#ifndef _SIMPLE_ENUM_HPP
#define _SIMPLE_ENUM_HPP

#include <type_traits>
#include <string>
#include <algorithm>
#include <array>
#include "function_utils.hpp"

namespace simple::support
{

	namespace Enum_Details
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

	} // namespace Enum_Details

	template<typename GutsType>
	class enum_wrapper : public GutsType
	{

		public:
		using Guts = GutsType;
		using Type = typename Guts::Type;

		template<typename G=Guts, std::enable_if_t<Enum_Details::hasDefault<G>::value>* = nullptr>
		enum_wrapper(Type type = Guts::Default) : _type{type}
		{ }

		template<typename G=Guts, std::enable_if_t<!Enum_Details::hasDefault<G>::value>* = nullptr>
		enum_wrapper(Type type) : _type{type}
		{ }

		template<typename G=Guts, std::enable_if_t<!Enum_Details::hasDefault<G>::value>* = nullptr>
		enum_wrapper() : _type{}
		{ }

		enum_wrapper& operator=(Type type)
		{
			_type = type;
			return *this;
		}

		operator Type() const
		{
			return _type;
		}

		template<typename G=Guts, std::enable_if_t<Enum_Details::hasFromConversion<G>::value>* = nullptr>
		explicit enum_wrapper(const typename support::fun_param_t<decltype(G::from), 0>& name)
		{
			// TODO: static assert one parameter
			// TODO: static assert return type to be correct
			_type = Guts::from(name);
		}

		template<typename G=Guts, std::enable_if_t<Enum_Details::hasToConversion<G>::value>* = nullptr>
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

	template <typename EnumType,
			 EnumType DefaultEnumValue,
			 size_t MappedValueCount = 1,
			 typename MappedType = std::string>
	struct mapped_enum_guts
	{
		using Type = EnumType;
		using UType = std::underlying_type_t<Type>;
		using map_type = const std::array<
			std::array<const MappedType, MappedValueCount>,
			static_cast<std::size_t>(DefaultEnumValue)>;
		static constexpr auto Default = DefaultEnumValue;
		static map_type map;

		static Type from(const MappedType& value)
		{
			for(UType i = 0; i < static_cast<UType>(Default); ++i)
			{
				auto&& variants = map[i];
				auto end = std::end(variants);
				auto begin = std::begin(variants);
				auto found = std::find(begin, end, value);
				if(found != end)
					return Type(i);
			}
			return Default;
		}

		static MappedType to(const Type& value)
		{
			return map[static_cast<UType>(value)][0];
		}
	};

	template <typename EnumType,
			 EnumType DefaultEnumValue,
			 size_t MappedValueCount = 1,
			 typename MappedType = std::string>
	using MappedEnum = enum_wrapper<mapped_enum_guts<EnumType, DefaultEnumValue, MappedValueCount, MappedType>>;

	template <typename Enum, typename Int = std::underlying_type<Enum>>
	constexpr Int to_integer(const Enum& value)
	{ return static_cast<Int>(value); };

} // namespace simple::support

#endif /* end of include guard */
