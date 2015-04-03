# platetectonics httpserver

A server to generate maps using plate-tectonics simulation. Written in C

## Requirements

* automake
* libtool

## Build

```
sh deps.sh
cmake .
cmake --build .
```

## Run

```
./ptserver
```

Now go to:

```
http://localhost:7890/map/
```

Be patient, it could be slow

You should get something like:

![](https://raw.githubusercontent.com/ftomassetti/platetectonics_httpserver/master/screenshots/screenshot.png)

## Dependencies

* libuv
