#ifndef SIMPLE_SUPPORT_ENUM_FLAGS_OPERATORS_HPP
#define SIMPLE_SUPPORT_ENUM_FLAGS_OPERATORS_HPP

#include <type_traits>

namespace simple::support
{

	template <typename Enum>
	struct define_enum_flags_operators : public std::false_type {};

} // namespace simple::support

#define SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR(op_sym, underlying_op_sym, return_type) \
template <typename Enum, std::enable_if_t<simple::support::define_enum_flags_operators<Enum>{}>* = nullptr> \
constexpr return_type operator op_sym (const Enum& one, const Enum& other) \
{ \
	using UType = std::underlying_type_t<Enum>; \
	return static_cast<return_type>(static_cast<UType>(one) underlying_op_sym static_cast<UType>(other)); \
}

SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR(|, |, Enum)
SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR(&, &, Enum)
SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR(^, ^, Enum)
SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR(&&, &, bool)

#undef SIMPLE_SUPPORT_DEFINE_ENUM_FLAGS_OPERATOR

#endif /* end of include guard */
