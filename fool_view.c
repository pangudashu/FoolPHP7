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

#include "fool_view.h"
#include "fool_register.h"
#include "fool_loader.h"

zend_class_entry* fool_view_ce;

PHP_FOOLPHP7_API zval* fool_view_instance() /*{{{*/
{
	zval *instance;
	zval tpl_data;
	char *view_root;

	instance = zend_read_static_property(fool_view_ce,ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_view_ce)) != 0){
		return instance;
	}

	object_init_ex(instance,fool_view_ce);

	if(!instance){
		return NULL;
	}
	array_init(&tpl_data);
	
	spprintf(&view_root, 0, "%s/application/view/", FOOLPHP7_G(root));

	zend_update_static_property(fool_view_ce, ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_INSTANCE), instance);
	zend_update_property(fool_view_ce, instance, ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA), &tpl_data);
	zend_update_property_string(fool_view_ce, instance, ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_ROOT), view_root);

	zval_ptr_dtor(&tpl_data);
	efree(view_root);
	return instance;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_view_assign(zval* instance, char* key, zval* value) /*{{{*/
{
	zval *tpl_data = zend_read_property(fool_view_ce, instance, ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA), 1, NULL);
	
	if(zend_hash_str_update(Z_ARRVAL_P(tpl_data), key, strlen(key), value) != NULL){
		Z_TRY_ADDREF_P(value);
		return 1;
	}
	return 0;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_view_extract(zval *tpl_values, zval *view_root) /*{{{*/
{
	zval *val;
	zend_string *key;

	//add variable to symbol
	ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(tpl_values), key, val){
		//filter view root
		if (zend_string_equals_literal(key, FOOL_VIEW_VAR_TPL_ROOT)){
			continue;
		}

		if(zend_set_local_var(key, val, 1) == SUCCESS){
			Z_TRY_ADDREF_P(val);
		}
	}ZEND_HASH_FOREACH_END();

	zend_set_local_var_str(ZEND_STRL(FOOL_VIEW_VAR_TPL_ROOT), view_root, 1);

	return 1;
}
/*}}}*/

ZEND_METHOD(fool_view, setViewRoot) /*{{{*/
{
	zend_string *view_path;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "S", &view_path) == FAILURE){
		RETURN_FALSE;
	}
	
	zend_update_property_string(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_ROOT), ZSTR_VAL(view_path));
	
	RETURN_TRUE;
}
/*}}}*/

ZEND_METHOD(fool_view, assign) /*{{{*/
{
	zend_string *key;
	zval* value;
	zval rv;
	int ret;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "Sz", &key, &value) == FAILURE){
		RETURN_FALSE;
	}

	ret = fool_view_assign(getThis(), ZSTR_VAL(key), value);
	if(ret){
		RETURN_TRUE;
	}
	RETURN_FALSE;
}/*}}}*/

ZEND_METHOD(fool_view, render) /*{{{*/
{
	zend_string *render_path;
	zval *controller, *action, *view_root, *tpl_values;
	char *view_path;
	int args_num;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "|S", &render_path) == FAILURE){
		RETURN_FALSE;
	}

	view_root = zend_read_property(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_ROOT), 1, NULL);

	args_num = ZEND_NUM_ARGS();

	if(args_num == 1 && !zend_string_equals_literal(render_path, "")){
		spprintf(&view_path, 0, "%s/%s%s", Z_STRVAL_P(view_root), ZSTR_VAL(render_path), FOOL_VIEW_TPL_SUFFIX);
	}else{
		controller = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER);
		action = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_ACTION);
		spprintf(&view_path, 0, "%s/%s/%s%s", Z_STRVAL_P(view_root), Z_STRVAL_P(controller), Z_STRVAL_P(action), FOOL_VIEW_TPL_SUFFIX);
	}

	tpl_values = zend_read_property(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA), 1, NULL);
	
	fool_view_extract(tpl_values, view_root);

	if (php_output_start_user(NULL, 0, PHP_OUTPUT_HANDLER_STDFLAGS) == FAILURE) {
		efree(view_path);
		RETURN_FALSE;
	}
	
	if(!fool_loader_include(view_path, NULL)){
		efree(view_path);
		php_output_end();
		RETURN_FALSE;
	}
	efree(view_path);

	if(php_output_get_contents(return_value) == FAILURE){
		php_output_end();
		RETURN_FALSE;
	}

	if(php_output_discard() == FAILURE){
		RETURN_FALSE;
	}
}
/*}}}*/

ZEND_METHOD(fool_view, display) /*{{{*/
{
	zend_string *render_path;
	zval *controller, *action, *view_root, *tpl_values;
	char *view_path;
	int args_num;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "|S", &render_path) == FAILURE){
		RETURN_FALSE;
	}

	view_root = zend_read_property(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_ROOT), 1, NULL);

	args_num = ZEND_NUM_ARGS();

	if(args_num == 1 && !zend_string_equals_literal(render_path, "")){
		spprintf(&view_path, 0, "%s/%s%s", Z_STRVAL_P(view_root), ZSTR_VAL(render_path), FOOL_VIEW_TPL_SUFFIX);
	}else{
		controller = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER);
		action = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_ACTION);
		spprintf(&view_path, 0, "%s/%s/%s%s", Z_STRVAL_P(view_root), Z_STRVAL_P(controller), Z_STRVAL_P(action), FOOL_VIEW_TPL_SUFFIX);
	}

	tpl_values = zend_read_property(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA), 1, NULL);
	
	fool_view_extract(tpl_values, view_root);

	if(!fool_loader_include(view_path, NULL)){
		printf("view %s not exist!!!",view_path);
	}

	efree(view_path);
}
/*}}}*/

ZEND_METHOD(fool_view, getViewData) /*{{{*/
{
	zval* tpl_data = zend_read_property(fool_view_ce, getThis(), ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA), 1, NULL);

	Z_TRY_ADDREF_P(tpl_data);
	RETURN_ZVAL(tpl_data,0,NULL);
}
/*}}}*/

zend_function_entry fool_view_methods[] = {
	PHP_ME(fool_view, setViewRoot,     NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_view, assign,          NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_view, render,          NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_view, display,         NULL, ZEND_ACC_PUBLIC)
	PHP_ME(fool_view, getViewData,     NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_view(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_View",fool_view_methods);
	fool_view_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_view_ce,ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
	zend_declare_property_null(fool_view_ce,ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_DATA),ZEND_ACC_PRIVATE);
	zend_declare_property_null(fool_view_ce,ZEND_STRL(FOOL_VIEW_PROPERTY_NAME_TPL_ROOT),ZEND_ACC_PRIVATE);
}





