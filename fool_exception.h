#ifndef PHP_FOOL_EXCEPTION_H
#define PHP_FOOL_EXCEPTION_H

#define FOOL_EXCEPTION_PROPERTY_NAME_INSTANCE "_fool_exception_obj"

extern zend_class_entry* fool_exception_ce;

/*
typedef enum{
	NOT_UU,
}error_code;
*/

//注册class
PHP_FOOLPHP7_API void register_class_exception(void);

PHP_FOOLPHP7_API void fool_throw_exception(long code, char *msg);

#endif
