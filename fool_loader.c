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

#include "fool_loader.h"
#include "fool_register.h"

zend_class_entry* fool_loader_ce;


PHP_FOOLPHP7_API zval* fool_loader_instance() /*{{{*/
{
	zval* instance;

	instance = zend_read_static_property(fool_loader_ce,ZEND_STRL(FOOL_LOADER_PROPERTY_NAME_INSTANCE),1);

	if(Z_TYPE_P(instance) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(instance), fool_loader_ce)) != 0){
		return instance;
	}

	//new Fool_Application
	object_init_ex(instance,fool_loader_ce);

	if(!instance){
		return NULL;
	}
	zend_update_static_property(fool_loader_ce,ZEND_STRL(FOOL_LOADER_PROPERTY_NAME_INSTANCE),instance);

	//char *class_map_path;
	//spprintf(&class_map_path, 0, "%s/%s/%s", FOOLPHP7_G(root), ROOT_DIR_CLASSMAP, FOOLPHP7_G(class_map));
	
	//include class map
	/*
	int res = fool_loader_include(class_map_path,NULL);

	DEBUG(class_map_path);

	efree(class_map_path);
	if(res == 0){
		DEBUG("inlude class map error");
		return NULL;
	}
	*/

	DEBUG("getLoaderInstance()");

	return instance;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_loader_register_autoload(zval* loader) /*{{{*/
{
	zval spl_func_name, ret, var_1, auto_load;
	zval params[1];

	ZVAL_STRING(&spl_func_name,"spl_autoload_register");
	ZVAL_STRING(&auto_load,"__autoload");

	array_init(&var_1);

	//加引用
	Z_TRY_ADDREF_P(loader);
	Z_TRY_ADDREF_P(&auto_load);

	add_next_index_zval(&var_1,loader);
	add_next_index_zval(&var_1,&auto_load);
	
	params[0] = var_1;

	zend_fcall_info fci = {
		sizeof(fci),
		//EG(function_table),
		spl_func_name,        //函数名
		//NULL,
		&ret,                 //函数返回值
		params,               //参数数组
		//NULL,                 //对象
		1,    
		1                     //参数数量
	};

	if(zend_call_function(&fci, NULL) == FAILURE) {
		DEBUG("SPL加载失败");
	}

	zval_ptr_dtor(&ret);
	zval_ptr_dtor(&spl_func_name);
	zval_ptr_dtor(&auto_load);
	zval_ptr_dtor(&var_1);
}
/*}}}*/

//include
PHP_FOOLPHP7_API int fool_loader_include(const char *class_path,zval *result) /*{{{*/
{
	zval dummy;
	zend_file_handle file_handle;
	zend_op_array *new_op_array;
	int ret;

	ret = php_stream_open_for_zend_ex(class_path,&file_handle,USE_PATH|STREAM_OPEN_FOR_INCLUDE);

	if(ret == FAILURE){
		return 0;
	}

	zend_string *opened_path;
	if (!file_handle.opened_path) {
		file_handle.opened_path = zend_string_init(class_path, sizeof(class_path), 0);
	}

	opened_path = zend_string_copy(file_handle.opened_path);
	ZVAL_NULL(&dummy);
	//将clas_path加入EG(included_files)
	if (zend_hash_add(&EG(included_files), opened_path, &dummy)){
		//compile
		new_op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
		zend_destroy_file_handle(&file_handle);
	}else{
		new_op_array = NULL;
		zend_file_handle_dtor(&file_handle);
	}

	zend_string_release(opened_path);

	if (!new_op_array){
		return 0;
	}

	//excute
	if(result != NULL){
		ZVAL_UNDEF(result);
	}
	zend_execute(new_op_array, result);
	destroy_op_array(new_op_array); 
	efree(new_op_array);
	//if (!EG(exception)){
		//zval_ptr_dtor(&result);
	//}

	return 1;
}
/*}}}*/

PHP_FOOLPHP7_API int fool_loader_autoload(const char* class_name) /*{{{*/
{
	zval *class_map = fool_register_get(FOOL_REGISTER_PROPERTY_CLASSMAP);
	
	if(!class_map){
		return 0;
	}

	zval *path = zend_hash_str_find(Z_ARRVAL_P(class_map), class_name, strlen(class_name));

	if(!path){
		return 0;
	}

	return fool_loader_include(Z_STRVAL_P(path), NULL);
}
/*}}}*/

ZEND_METHOD(fool_loader,__autoload) /*{{{*/
{
	zend_string *class_name;

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "S", &class_name) == FAILURE){
		return;
	}

	fool_loader_autoload(ZSTR_VAL(class_name));
}
/*}}}*/


zend_function_entry fool_loader_methods[] = {
	PHP_ME(fool_loader, __autoload,         NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_loader(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Loader",fool_loader_methods);
	fool_loader_ce = zend_register_internal_class(&ce);

	zend_declare_property_null(fool_loader_ce,ZEND_STRL(FOOL_LOADER_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





