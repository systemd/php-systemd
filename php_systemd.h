#ifndef PHP_SYSTEMD_H
#define PHP_SYSTEMD_H 1
#define PHP_SYSTEMD_VERSION "0.1"
#define PHP_SYSTEMD_EXTNAME "systemd"

PHP_FUNCTION(sd_journal_send);

extern zend_module_entry systemd_module_entry;
#define phpext_systemd_ptr &systemd_module_entry

#endif
