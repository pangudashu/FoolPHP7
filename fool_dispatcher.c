/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_main.h"
#include "php_ini.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_exceptions.h"
#include "php_foolphp7.h"

#include "fool_dispatcher.h"
#include "fool_controller.h"
#include "fool_register.h"
#include "fool_view.h"
#include "fool_request.h"
#include "fool_exception.h"

zend_class_entry* fool_dispatcher_ce;

//usless
PHP_FOOLPHP7_API zval* fool_dispatcher_instance() /*{{{*/
{
	/*
	zval* instance;

	instance = zend_read_static_property(fool_dispatcher_ce,ZEND_STRL(FOOL_DISPATCHER_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_dispatcher_ce)) != 0){
		return instance;
	}

	//new Fool_Application
	object_init_ex(instance,fool_dispatcher_ce);

	if(!instance){
		return NULL;
	}
	zend_update_static_property(fool_dispatcher_ce,ZEND_STRL(FOOL_DISPATCHER_PROPERTY_NAME_INSTANCE),instance);

	return instance;
	*/
	return NULL;
}
/*}}}*/

PHP_FOOLPHP7_API zend_class_entry* fool_dispatcher_get_controller_entry(char* module, char *controller) /*{{{*/
{
	char *controller_name, *lower_controller_name;
	spprintf(&controller_name, 0, "%sController", controller);
	spprintf(&lower_controller_name, 0, "%sController", controller);

	php_strtolower(lower_controller_name, strlen(lower_controller_name) + 1);

	zend_class_entry *controller_entry;

	controller_entry = zend_hash_str_find_ptr(EG(class_table), lower_controller_name, strlen(lower_controller_name));

	if(NULL != controller_entry){
		efree(controller_name);
		efree(lower_controller_name);
		return controller_entry;
	}

	fool_loader_autoload(controller_name);	

	controller_entry = zend_hash_str_find_ptr(EG(class_table), lower_controller_name, strlen(lower_controller_name));
	
	efree(controller_name);
	efree(lower_controller_name);
	
	if(NULL != controller_entry){
		return controller_entry;
	}

	return NULL;
}
/*}}}*/

//include class map
PHP_FOOLPHP7_API int fool_dispatcher_load_classmap(char *module)/*{{{*/
{
	char *classmap_path;
	zval ret;

	spprintf(&classmap_path, 0, "%s/classmap/foolphp_class_%s.map", FOOLPHP7_G(root), module);

	int r = fool_loader_include(classmap_path, &ret);
	if(!r){
		efree(classmap_path);
		return 0;
	}

	fool_register_set(FOOL_REGISTER_PROPERTY_CLASSMAP, &ret);

	zval_ptr_dtor(&ret);
	efree(classmap_path);
	return 1;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_dispatcher_dispatch(void) /*{{{*/
{
	zval *module, *controller, *action, controller_instance;
	zend_class_entry* controller_ce;

	module = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_MODULE);
	controller = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER);
	action = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_ACTION);

	if(!module || !controller || !action){
		DEBUG("Request Parse Method Error");
		return 0;
	}

	//include class map by module
	if(fool_dispatcher_load_classmap(Z_STRVAL_P(module)) == 0){
		DEBUG("Load ClassMap Error");
		return 0;
	}

	controller_ce = fool_dispatcher_get_controller_entry(Z_STRVAL_P(module), Z_STRVAL_P(controller));
	if(NULL == controller_ce){
		DEBUG("404 controller");
		return 0;
	}

	char *action_method; 
	spprintf(&action_method, 0, "%saction", Z_STRVAL_P(action));

	if(!zend_hash_str_exists(&(controller_ce->function_table), action_method, strlen(action_method))){
		efree(action_method);
		DEBUG("404 action");
		return 0;
	}

	object_init_ex(&controller_instance, controller_ce);
	zval retval;

	zval *view_instance    = fool_view_instance();
	zval *request_instance = fool_request_instance();

	fool_controller_init(&controller_instance, view_instance, request_instance);

	//execute
	if(zend_hash_str_exists(&(controller_ce->function_table), "__construct", strlen("__construct"))){
		zend_call_method_with_0_params(&controller_instance, controller_ce, NULL, "__construct", &retval);
	}
	zend_call_method(&controller_instance, controller_ce, NULL, action_method, strlen(action_method), &retval, 0, NULL, NULL);
	if(zend_hash_str_exists(&(controller_ce->function_table), "__construct", strlen("__destruct"))){
		zend_call_method_with_0_params(&controller_instance, controller_ce, NULL, "__destruct", &retval);
	}


	zval_dtor(&controller_instance);
	//zval_dtor(view_instance);
	efree(action_method);
}
/*}}}*/

zend_function_entry fool_dispatcher_methods[] = {
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_dispatcher(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Dispatcher",fool_dispatcher_methods);
	fool_dispatcher_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_dispatcher_ce,ZEND_STRL(FOOL_DISPATCHER_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





