### Overview 
Simple fake HTTP 1.1 daemon used for experimental work with libraries:
* pthread 
* [libev] (http://) 
* [protobuf] (http://)
* [p7] (http://)
Curently supports only **GET** HTTP request.

### Build
```
mkdir tmp
cd tmp
cmake ..
``` 

### Run
```
cd tmp
fakehttpd -h<ip> -p<port> -d<http directory> -s
```