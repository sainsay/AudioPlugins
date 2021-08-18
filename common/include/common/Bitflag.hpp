
#define FLAG_IS_SET(var, flag) ((var) & (flag)) != static_cast<decltype(var)>(0)
#define SET_FLAG(var, flag) var |= (flag) 
#define TOGGLE_FLAG(var, flag) var ^= (flag)
#define CLEAR_FLAG(var, flag) var &= ~(flag)

#define DEF_FLAG_OR_OP(type, internal_type) \
inline type operator|(const type& a, const type& b)   \
{                                       \
    return static_cast<type>(static_cast<internal_type>(a) | static_cast<internal_type>(b)); \
}

#define DEF_FLAG_ASSIGN_OR_OP(type, internal_type) \
inline type& operator|=(type& a, const type& b)   \
{                                       \
    a = static_cast<type>(static_cast<internal_type>(a) | static_cast<internal_type>(b)); \
    return a; \
}

#define DEF_FLAG_AND_OP(type, internal_type) \
inline type operator&(const type& a, const type& b)   \
{                                       \
    return static_cast<type>(static_cast<internal_type>(a) & static_cast<internal_type>(b)); \
}

#define DEF_FLAG_ASSIGN_AND_OP(type, internal_type) \
inline type operator&=(type& a, const type& b)   \
{                                       \
    a = static_cast<type>(static_cast<internal_type>(a) & static_cast<internal_type>(b)); \
    return a; \
}

#define DEF_FLAG_XOR_OP(type, internal_type) \
inline type operator^(const type& a, const type& b)   \
{                                       \
    return static_cast<type>(static_cast<internal_type>(a) ^ static_cast<internal_type>(b)); \
}

#define DEF_FLAG_ASSIGN_XOR_OP(type, internal_type) \
inline type operator^=(type& a, const type& b)   \
{                                       \
    a = static_cast<type>(static_cast<internal_type>(a) ^ static_cast<internal_type>(b)); \
    return a; \
}

#define DEF_FLAG_NOT_OP(type, internal_type) \
inline type operator~(const type& a)   \
{                                       \
    return static_cast<type>(~static_cast<internal_type>(a)); \
}
