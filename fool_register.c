/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_main.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "php_foolphp7.h"

#include "fool_register.h"

zend_class_entry* fool_register_ce;


PHP_FOOLPHP7_API zval* fool_register_get(char* property)/*{{{*/
{
	zval *value;
	value = zend_read_static_property(fool_register_ce,property,strlen(property),1);
	return value;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_register_set(char* property,zval* value)/*{{{*/
{
	//zend_update_static_property会将值copy
	return zend_update_static_property(fool_register_ce,property,strlen(property),value);
}
/*}}}*/

zend_function_entry fool_register_methods[] = {
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_register(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Register",fool_register_methods);
	fool_register_ce = zend_register_internal_class(&ce);

	//declare property
	zend_declare_property_null(fool_register_ce,ZEND_STRL(FOOL_REGISTER_PROPERTY_CLASSMAP),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_register_ce,ZEND_STRL(FOOL_REGISTER_PROPERTY_CONFIG),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_register_ce,ZEND_STRL(FOOL_REGISTER_PROPERTY_REQUEST_MODULE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_register_ce,ZEND_STRL(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_register_ce,ZEND_STRL(FOOL_REGISTER_PROPERTY_REQUEST_ACTION),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





