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

#ifndef PHP_FOOLPHP7_H
#define PHP_FOOLPHP7_H

extern zend_module_entry foolphp7_module_entry;
#define phpext_foolphp7_ptr &foolphp7_module_entry

#define PHP_FOOLPHP7_VERSION "2.0.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_FOOLPHP7_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FOOLPHP7_API __attribute__ ((visibility("default")))
#else
#	define PHP_FOOLPHP7_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#define _DEBUG_ 1
#define DEBUG(errmsg) debug(__FILE__,__LINE__,__FUNCTION__,errmsg)


void fool_php_foreach(HashTable *ht);

//项目目录
#define ROOT_DIR_APPLICATION  "application"
#define ROOT_DIR_CONFIG       "config"
#define ROOT_DIR_CLASSMAP     "classmap"
#define ROOT_DIR_WEB          "www"


ZEND_BEGIN_MODULE_GLOBALS(foolphp7)
	char *root;
	char *class_map;
ZEND_END_MODULE_GLOBALS(foolphp7)

ZEND_EXTERN_MODULE_GLOBALS(foolphp7)
#define FOOLPHP7_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(foolphp7, v)



void debug(const char *file,int line,const char *func,const char* errmsg);

#if defined(ZTS) && defined(COMPILE_DL_FOOLPHP7)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_FOOLPHP7_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
