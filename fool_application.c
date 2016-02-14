/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_globals.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "php_foolphp7.h"

#include "fool_application.h"
#include "fool_loader.h"
#include "fool_register.h"
#include "fool_dispatcher.h"

zend_class_entry* fool_application_ce;

ZEND_DECLARE_MODULE_GLOBALS(foolphp7)


PHP_FOOLPHP7_API zval* fool_application_instance() /*{{{*/
{
	zval *instance, *loader_obj;

	instance = zend_read_static_property(fool_application_ce,ZEND_STRL(FOOL_APPLICATION_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_application_ce)) != 0){
		return instance;
	}

	//new Fool_Application
	object_init_ex(instance,fool_application_ce);
	if(!instance){
		return NULL;
	}

	//new Fool_Loader
	loader_obj = fool_loader_instance();
	if(!loader_obj){
		return NULL;
	}
	//call spl_autoload_register()
	fool_loader_register_autoload(loader_obj);

	zend_update_static_property(fool_application_ce,ZEND_STRL(FOOL_APPLICATION_PROPERTY_NAME_INSTANCE),instance);
	DEBUG("new Fool_Application()");

	return instance;
}
/*}}}*/

ZEND_METHOD(fool_application, getInstance) /*{{{*/
{
	DEBUG("start!");
	zval rv;
	zend_string *root;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "S", &root) == FAILURE){
		RETURN_FALSE;
	}

	/*
	 * global.application_path = 应用根目录
	 * global.config_path = 配置目录
	 */
	FOOLPHP7_G(root) = ZSTR_VAL(root);

	zval* instance = fool_application_instance();

	if(!instance){
		RETURN_FALSE;
	}
	
	RETURN_ZVAL(instance,1,0);
}
/*}}}*/

ZEND_METHOD(fool_application, setModule)
{
	zval *modules;
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "|a", &modules) == FAILURE){
		RETURN_FALSE;
	}

	Z_TRY_ADDREF_P(modules);
	RETURN_ZVAL(modules, 0, NULL);
}

ZEND_METHOD(fool_application, run)
{
	//fool_loader_include("/alidata/web/php7/foolphp/src/config/config.php",&result);

	//step1.map router
	
	//step2.解析method
	int res = fool_request_parse_method();

	if(!res){
		RETURN_FALSE;
	}
	//step3.dispatch
	fool_dispatcher_dispatch();

	zval *obj = fool_register_get(FOOL_REGISTER_PROPERTY_CLASSMAP);
	RETURN_ZVAL(obj,1,NULL);
}

zend_function_entry fool_application_methods[] = {
	PHP_ME(fool_application, getInstance, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(fool_application, run,         NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_application, setModule,   NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_applcation(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Application",fool_application_methods);
	fool_application_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_application_ce,ZEND_STRL(FOOL_APPLICATION_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





