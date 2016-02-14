/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_foolphp7.h"

#include "fool_application.h"
#include "fool_loader.h"
#include "fool_register.h"
#include "fool_request.h"
#include "fool_exception.h"

/* True global resources - no need for thread safety here */
static int le_foolphp7;

ZEND_DECLARE_MODULE_GLOBALS(foolphp7)

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("foolphp.class_map","fool_php_class.map", PHP_INI_ALL,OnUpdateString,class_map, zend_foolphp7_globals, foolphp7_globals)
PHP_INI_END();
/* }}} */

int debug_no;
void debug(const char *file,int line,const char *func,const char* errmsg)/*{{{*/
{
#if _DEBUG_ == 1
	if(debug_no == 0){
		printf("======================[debug trace]===================\n");
	}
	printf("#%d %s #%d [%s] \n\t%s\n",debug_no, file, line, func, errmsg);
	++debug_no;
#endif
}
/*}}}*/

void fool_php_foreach(HashTable *ht)
{
	zval *entry;
	zend_ulong num_key;
	zend_string *var_name;

	ZEND_HASH_FOREACH_KEY_VAL_IND(ht, num_key, var_name, entry){
		printf("key:%s\n",ZSTR_VAL(var_name));
	}ZEND_HASH_FOREACH_END();
}


PHP_MINIT_FUNCTION(foolphp7)
{
	//register php.ini
	REGISTER_INI_ENTRIES();

	//register internal class
	register_class_applcation();
	register_class_loader();
	register_class_register();
	register_class_dispatcher();
	register_class_controller();
	register_class_view();
	register_class_request();
	register_class_exception();

	return SUCCESS;
}

PHP_GINIT_FUNCTION(foolphp7) /*{{{*/
{
#if defined(COMPILE_DL_FOOLPHP7) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	foolphp7_globals->root = NULL;
}
/*}}}*/

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(foolphp7)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(foolphp7)
{
#if defined(COMPILE_DL_FOOLPHP7) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(foolphp7)
{
	//clear register data
	zval* module = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_MODULE);
	zval* controller = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_CONTROLLER);
	zval* action = fool_register_get(FOOL_REGISTER_PROPERTY_REQUEST_ACTION);
	if(module){
		zval_ptr_dtor(module);
	}
	if(controller){
		zval_ptr_dtor(controller);
	}
	if(action){
		zval_ptr_dtor(action);
	}
	DEBUG("request shutdown");
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(foolphp7)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "foolphp7 support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

const zend_function_entry foolphp7_functions[] = {
	PHP_FE_END	/* Must be the last line in foolphp7_functions[] */
};

zend_module_entry foolphp7_module_entry = {
	STANDARD_MODULE_HEADER,
	"foolphp7",
	foolphp7_functions,
	PHP_MINIT(foolphp7),
	PHP_MSHUTDOWN(foolphp7),
	PHP_RINIT(foolphp7),
	PHP_RSHUTDOWN(foolphp7),
	PHP_MINFO(foolphp7),
	PHP_FOOLPHP7_VERSION,
	PHP_MODULE_GLOBALS(foolphp7),
	PHP_GINIT(foolphp7),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_FOOLPHP7
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(foolphp7)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
