#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_systemd.h"
#include <systemd/sd-journal.h>

ZEND_BEGIN_ARG_INFO_EX(arginfo_sd_journal_send, 0, 0, 3)
ZEND_ARG_TYPE_INFO(0, tags, IS_STRING, 0)
ZEND_ARG_VARIADIC_INFO(0, 3)
ZEND_END_ARG_INFO()

zend_function_entry systemd_functions[] = {
    PHP_FE(sd_journal_send, arginfo_sd_journal_send)
    {NULL, NULL, NULL} // Sentinel
};

zend_module_entry systemd_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_SYSTEMD_EXTNAME,
    systemd_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_SYSTEMD_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SYSTEMD
ZEND_GET_MODULE(systemd)
#endif

PHP_FUNCTION(sd_journal_send)
{
    struct iovec *iov = NULL;
    zval *args;
    int argc, len, i;
    char *val;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "+", &args, &argc) != SUCCESS) {
        return;
    }
    
    // Allocate sufficient iovector space for the arguments.
    iov = safe_emalloc(argc, sizeof(struct iovec), 0);
    if (!iov) {
        // Should probably raise a more clear error.
        RETURN_FALSE;
    }

    // Iterate through the PHP arguments and fill the iovector.
    for (i = 0; i < argc; ++i) {
        convert_to_string_ex(&args[i]);
        val = Z_STRVAL(args[i]);
        len = Z_STRLEN(args[i]);
        iov[i].iov_base = val;
        iov[i].iov_len = len;
    }

    // Send the iovector to journald.
    sd_journal_sendv(iov, argc);

    // Free the iovector. The actual strings
    // are already managed by PHP.
    efree(iov);
    
    RETURN_TRUE;
}
