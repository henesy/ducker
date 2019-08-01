# Ducker (WIP)

It's not docker.

Builds processes into namespace containers with no hope of escape™.

## Build

	mk

## Usage

	usage: ducker [-D] [-p proto | -a arch ] [-r root] [-c cfg] [-u user] [cmd args…]

_Note_: Setting `-u` requires having sufficient permissions to utilize the cap(3) device as the invoking user. 

### Examples

Touch(1) a file in the current directory:

	ducker `{whatis touch} `{pwd}^/test

Run a web server with a proto as the fs:

	ducker -p web.proto -c web.cfg /bin/httpd

Run a web server with an archive as the fs:

	ducker -d web.arch -c web.cfg /bin/httpd

Run touch(1) in /tmp:

	ducker -p examples/tmp.proto -c examples/tmp.cfg
