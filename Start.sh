#!/bin/bash
sudo killall php-fpm
sudo nginx -s stop
sudo php-fpm&
#sudo nginx -s reload
sudo nginx
