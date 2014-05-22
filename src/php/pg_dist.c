#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
 
#define PHP_PG_DIST_VERSION "1.0"
#define PHP_PG_DIST_EXTNAME "pg_dist"
 
extern zend_module_entry pg_dist_module_entry;
#define phpext_pg_dist_ptr &pg_dist_module_entry
 
// declaration of a custom pg_execute_query()
PHP_FUNCTION(pg_execute_query);
 
// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static function_entry my_functions[] = {
    PHP_FE(pg_execute_query, NULL)
    {NULL, NULL, NULL}
};
 
// the following code creates an entry for the module and registers it with Zend.
zend_module_entry pg_dist_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_PG_DIST_EXTNAME,
    my_functions,
    NULL, // name of the MINIT function or NULL if not applicable
    NULL, // name of the MSHUTDOWN function or NULL if not applicable
    NULL, // name of the RINIT function or NULL if not applicable
    NULL, // name of the RSHUTDOWN function or NULL if not applicable
    NULL, // name of the MINFO function or NULL if not applicable
#if ZEND_MODULE_API_NO >= 20010901
    PHP_PG_DIST_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
 
ZEND_GET_MODULE(pg_dist)
 
// implementation of a custom pg_execute_query()
PHP_FUNCTION(pg_execute_query)
{
    	char *name;
    	int name_len;
	zval *array;

    	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
        	RETURN_NULL();
    	}


    	array_init(return_value);

	add_assoc_string(return_value, "a", "return a", 1);
	add_assoc_string(return_value, "b", "return b", 1);
	add_assoc_string(return_value, "c", "return c", 1);

	ALLOC_INIT_ZVAL(array);
	array_init(array);
	add_assoc_zval(return_value, "sub", array);
    	//RETURN_STRING(name, 1);
}
