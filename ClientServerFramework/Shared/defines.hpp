/// Used to inherit typenames from template base classes.
/// using base<T>::type syntax is still not working in gcc 4.9.
/// Assumes that the type base_type has been defined.
#define INHERIT(T) typedef typename base_type::T T