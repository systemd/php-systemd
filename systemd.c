#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_systemd.h"
#include <systemd/sd-journal.h>

zend_function_entry systemd_functions[] = {
    PHP_FE(sd_journal_send, NULL)
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
    zval ***args;
    int argc, len, i;
    char *val;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "+", &args, &argc) == FAILURE) {
      return NULL;
    }
    
    // Allocate sufficient iovector space for the arguments.
    iov = safe_emalloc(argc, sizeof(struct iovec), 0);
    if (!iov) {
        // Should probably raise a more clear error.
        RETURN_FALSE;
    }

    // Iterate through the PHP arguments and fill the iovector.
    for (i = 0; i < ZEND_NUM_ARGS() TSRMLS_CC; ++i) {
      convert_to_string_ex(args[i]);
      val = Z_STRVAL_PP(args[i]);
      len = Z_STRLEN_PP(args[i]);
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
