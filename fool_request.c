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

#include "fool_request.h"
#include "fool_register.h"

zend_class_entry* fool_request_ce;

PHP_FOOLPHP7_API zval* fool_request_instance() /*{{{*/
{
	zval* instance;

	instance = zend_read_static_property(fool_request_ce,ZEND_STRL(FOOL_REQUEST_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_request_ce)) != 0){
		return instance;
	}

	//new Fool_Application
	object_init_ex(instance,fool_request_ce);

	if(!instance){
		return NULL;
	}
	zend_update_static_property(fool_request_ce,ZEND_STRL(FOOL_REQUEST_PROPERTY_NAME_INSTANCE),instance);

	DEBUG("getRequestInstance()");

	return instance;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_request_parse_method(void) /*{{{*/
{
	zval* parame_get;
	zval module, controller, action;

	parame_get = zend_hash_str_find(&EG(symbol_table),ZEND_STRL("_GET"));

	if(!parame_get){
		DEBUG("符号表错误");
		return 0;
	}
	
	if(Z_TYPE_P(parame_get) != IS_ARRAY){
		goto set_default_value;
	}
	
	zval* method = zend_hash_str_find(Z_ARRVAL_P(parame_get),ZEND_STRL("m"));
	if(!method || Z_TYPE_P(method) != IS_STRING){
		goto set_default_value;
	}

	char *str_module, *str_controller, *str_action, *tmp;
	str_module = php_strtok_r(Z_STRVAL_P(method), "/", &tmp);

	if(strlen(tmp) > 0){
		ZVAL_STRING(&module, str_module);

		str_controller = php_strtok_r(tmp, ".", &tmp);

		if(strlen(tmp) > 0){
			//有controller、action
			str_action = tmp;
		}else{
			str_action = "index";
		}
	}else{
		//默认module
		ZVAL_STRING(&module, "main");
		
		str_controller = php_strtok_r(str_module, ".", &tmp);
		if(strlen(tmp) > 0){
			str_action = tmp;
		}else{
			str_action = "index";
		}
	}

	//controller首字母大写,action全小写
	str_controller = php_strtolower(str_controller, strlen(str_controller) + 1);
	*str_controller = toupper((unsigned char)*str_controller);
	str_action = php_strtolower(str_action, strlen(str_action) + 1);

	ZVAL_STRING(&controller, str_controller);
	ZVAL_STRING(&action, str_action);

	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_MODULE, &module);
	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER, &controller);
	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_ACTION, &action);

	return 1;

set_default_value:
	//set default module controller action
	ZVAL_STRING(&module, "main");
	ZVAL_STRING(&controller, "Index");
	ZVAL_STRING(&action, "index");
	
	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_MODULE, &module);
	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER, &controller);
	fool_register_set(FOOL_REGISTER_PROPERTY_REQUEST_ACTION, &action);
	
	return 1;
}
/*}}}*/

ZEND_METHOD(fool_request, getController)
{
	zval* controller = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER);

	RETURN_ZVAL(controller,0,NULL);
}

ZEND_METHOD(fool_request, getAction)
{
	zval* action = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_ACTION);

	RETURN_ZVAL(action,0,NULL);
}

ZEND_METHOD(fool_request, getModule)
{
	zval* module = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_MODULE);

	printf("%s\n", FOOLPHP7_G(root));

	RETURN_ZVAL(module,0,NULL);
}

zend_function_entry fool_request_methods[] = {
	PHP_ME(fool_request, getModule,         NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_request, getController,         NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_request, getAction,         NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_request(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Request",fool_request_methods);
	fool_request_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_request_ce,ZEND_STRL(FOOL_REQUEST_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





