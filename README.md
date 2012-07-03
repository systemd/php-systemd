systemd-php
============

PHP extension allowing native interaction with systemd and journald

Installation
============

    yum install php-devel systemd-devel
    phpize
    ./configure --enable-systemd
    make
    make install
    echo "extension=systemd.so" | sudo tee /etc/php.d/systemd.ini
    echo "<?php echo sd_journal_send('MESSAGE=hello world');" | php
    
Usage
=====

Quick example:

    <?php
    sd_journal_send('MESSAGE=Hello world.');
    sd_journal_send('MESSAGE=Hello, again, world.', 'FIELD2=Greetings!', 'FIELD3=Guten tag.');
    sd_journal_send('ARBITRARY=anything', 'FIELD3=Greetings!');

Notes:

 * Each argument must be in the form of a KEY=value pair, environmental variable style.
 * Unlike the native C version of journald's sd_journal_send(), printf-style substitution is not supported. Perform any substitution using PHP's sprintf() or similar capabilities first.
 * The base message is usually sent in the form MESSAGE=hello. The MESSAGE field is, however, not required.
 * Invalid arguments result in nothing recorded in the journal.

Viewing Output
==============

Quick way to view output with all fields as it comes in:

    sudo journalctl -f --output=json
