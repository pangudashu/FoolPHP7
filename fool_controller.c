/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_main.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_exceptions.h"
#include "php_foolphp7.h"

#include "fool_controller.h"
#include "fool_view.h"
#include "fool_register.h"
#include "fool_request.h"

zend_class_entry* fool_controller_ce;

PHP_FOOLPHP7_API zval* fool_controller_instance() /*{{{*/
{
	zval *instance,*view_instance;

	instance = zend_read_static_property(fool_controller_ce,ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_controller_ce)) != 0){
		return instance;
	}

	object_init_ex(instance,fool_controller_ce);

	if(!instance){
		return NULL;
	}
	zend_update_static_property(fool_controller_ce,ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_INSTANCE),instance);

	view_instance = fool_view_instance();
	if(view_instance){
		zend_update_static_property(fool_controller_ce,ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_VIEW),view_instance);
	}

	DEBUG("getControllerInstance()");

	return instance;
}
/*}}}*/

PHP_FOOLPHP7_API zval* fool_controller_init(zval* controller, zval* view, zval* request) /*{{{*/
{
	zend_update_property(fool_controller_ce, controller, ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_VIEW), view);
	zend_update_property(fool_controller_ce, controller, ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_REQUEST), request);
}
/*}}}*/

zend_function_entry fool_controller_methods[] = {
	//PHP_ME(fool_controller, assign,         NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_controller(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Controller",fool_controller_methods);
	fool_controller_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_controller_ce,ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_controller_ce,ZEND_STRL(FOOL_CONTROLLER_PROPERTY_NAME_VIEW), ZEND_ACC_PROTECTED);
}





