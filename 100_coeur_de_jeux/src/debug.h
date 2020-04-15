/*
 * debug.h
 *
 *  Created on: Feb 27, 2020
 *      Author: ghermant
 */

#ifndef DEBUG_H_
#define DEBUG_H_

void debug_setlevel(int inNewLevel) ;
/*
 * debug_setlevel() : set current debug level
 * 0 => Display only messages with level 0
 * 1 => Display only messages with level 1 or less
 * 2 => Display only messages with level 2 or less
 * */

// call debug as printf with a first argument is level from which you want printf
// must first call

// ##__VA_ARGS__ : no replacement of variable arguments if empty
// see https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macroshtml#Concatenation
#define debug_printf( inLevel, format, ...) debug_pr_fn(inLevel, __FILE__ ":%d: " format , __LINE__ , ##__VA_ARGS__ )
#define error_printf(format, ...) debug_pr_fn(-1, format , ##__VA_ARGS__)

int debug_pr_fn(int inLevel, const char *format, ...) ;


#endif /* DEBUG_H_ */
