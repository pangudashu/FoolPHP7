#ifndef PHP_FOOL_LOADER_H
#define PHP_FOOL_LOADER_H

#define FOOL_LOADER_PROPERTY_NAME_INSTANCE "_fool_loader_obj"

extern zend_class_entry* fool_loader_ce;

PHP_FOOLPHP7_API void register_class_loader(void);

PHP_FOOLPHP7_API zval* fool_loader_instance();
PHP_FOOLPHP7_API int fool_loader_include(const char *class_path, zval *result);
PHP_FOOLPHP7_API int fool_loader_register_autoload(zval* loader);

#endif
