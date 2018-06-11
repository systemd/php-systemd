php-systemd
===========

PHP extension allowing native interaction with systemd and journald

Installation
------------

### Prerequisites

.deb based

    sudo apt install php5-dev  libsystemd-dev

.rpm based

    sudo dnf install php-devel systemd-devel

### Build

    phpize
    ./configure --with-systemd
    make

### Setup

    sudo make install

Fedora

    echo "extension=systemd.so" | sudo tee /etc/php.d/systemd.ini

Debian (PHP 5)

    echo "extension=systemd.so" | sudo tee /etc/php5/mods-available/systemd.ini
    sudo php5enmod systemd

### Basic Test

    echo "<?php echo sd_journal_send('MESSAGE=hello world');" | php

Usage
-----

Quick example:

``` {.php}
<?php
sd_journal_send('MESSAGE=Hello world.');
// message with priority "3" (warning) and identifier (also called *TAG*) set to "appname" 
sd_journal_send('MESSAGE=Error message','PRIORITY=3', 'SYSLOG_IDENTIFIER=appname');"
```

Notes:

-   Each argument must be in the form of a KEY=value pair, environmental
    variable style.
-   Unlike the native C version of journald's `sd_journal_send()`,
    printf-style substitution is not supported. Perform any substitution
    using PHP's `sprintf()` or similar capabilities first.
-   The base message is usually sent in the form MESSAGE=hello. The
    MESSAGE field is, however, not required.
-   Invalid arguments result in nothing recorded in the journal.

### Viewing Output

Quick way to view output with all fields as it comes in:

    sudo journalctl -f --output=json
