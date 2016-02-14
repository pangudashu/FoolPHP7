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

#include "fool_exception.h"
#include "fool_register.h"

zend_class_entry* fool_exception_ce;

PHP_FOOLPHP7_API zend_class_entry* fool_exception_get_exception_base(int root) /*{{{*/
{
#if can_handle_soft_dependency_on_SPL && defined(HAVE_SPL)
	if (!root) {
		if (!spl_ce_RuntimeException) {
			zend_class_entry *pce;

			if ((pce = zend_hash_str_find_ptr(CG(class_table), "runtimeexception", sizeof("RuntimeException") - 1))) {
				spl_ce_RuntimeException = pce;
				return pce; 
			}
		}else{
			return spl_ce_RuntimeException;
		}
	}
#endif
	return zend_ce_exception;
}
/*}}}*/

PHP_FOOLPHP7_API void fool_throw_exception(long code, char *msg)
{
	zend_throw_exception(fool_exception_ce, msg, code);
}

zend_function_entry fool_exception_methods[] = {
	PHP_FE_END
};

PHP_FOOLPHP7_API void register_class_exception(void)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"Fool_Exception",fool_exception_methods);
	fool_exception_ce = zend_register_internal_class_ex(&ce, fool_exception_get_exception_base(0));

	zend_declare_property_null(fool_exception_ce,ZEND_STRL(FOOL_EXCEPTION_PROPERTY_NAME_INSTANCE),ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);
}





