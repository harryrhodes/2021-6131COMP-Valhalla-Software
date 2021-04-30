# embedded-home-hvac-system

A Final Year group embedded Software Engineering project developed for an ESP-32 Arduino MCU.

## HTTP Endpoint

The HTTP endpoint code present in _/webserver_ is deployed to Azure using docker containers and is accessable at *http://harryrhodes.com*

### Accessing logs

In order to access logs. Send a HTTP GET request to: *http://harryrhodes.com/logs* either through a client or a browser to download the file.
