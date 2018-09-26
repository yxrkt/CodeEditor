#pragma once

#include <type_traits>

template <typename EnumT, typename = typename std::enable_if<std::is_enum<EnumT>::value>::type>
struct is_flags_enum
{
    static const bool value = false;
};

template <typename EnumT, typename = typename std::enable_if<is_flags_enum<EnumT>::value>::type>
EnumT operator|(EnumT l, EnumT r)
{
    typedef typename std::underlying_type<EnumT>::type underlying_type;
    return static_cast<EnumT>(static_cast<underlying_type>(l) | static_cast<underlying_type>(r));
}

template <typename EnumT, typename = typename std::enable_if<is_flags_enum<EnumT>::value>::type>
EnumT operator|=(EnumT& l, EnumT r)
{
    typedef typename std::underlying_type<EnumT>::type underlying_type;
    l = static_cast<EnumT>(static_cast<underlying_type>(l) | static_cast<underlying_type>(r));
    return l;
}

template <typename EnumT, typename = typename std::enable_if<is_flags_enum<EnumT>::value>::type>
EnumT operator&(EnumT l, EnumT r)
{
    typedef typename std::underlying_type<EnumT>::type underlying_type;
    return static_cast<EnumT>(static_cast<underlying_type>(l) & static_cast<underlying_type>(r));
}

template <typename EnumT, typename = typename std::enable_if<is_flags_enum<EnumT>::value>::type>
EnumT operator&=(EnumT& l, EnumT r)
{
    typedef typename std::underlying_type<EnumT>::type underlying_type;
    l = static_cast<EnumT>(static_cast<underlying_type>(l) & static_cast<underlying_type>(r));
    return l;
}

template <typename EnumT, typename = typename std::enable_if<is_flags_enum<EnumT>::value>::type>
bool has_flags(EnumT value, EnumT flags)
{
    return (value & flags) == flags;
}