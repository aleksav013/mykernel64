/* SPDX-License-Identifier: GPL-2.0 */
#ifndef CONTAINER_OF
#define CONTAINER_OF

#define static_assert(expr, ...) __static_assert(expr, ##__VA_ARGS__, #expr)
#define __static_assert(expr, msg, ...) _Static_assert(expr, msg)
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member)                                   \
	({                                                                \
		void *__mptr = (void *)(ptr);                             \
		static_assert(__same_type(*(ptr), ((type *)0)->member) || \
				      __same_type(*(ptr), void),          \
			      "pointer type mismatch in container_of()"); \
		((type *)(__mptr - offsetof(type, member)));              \
	})

#endif
