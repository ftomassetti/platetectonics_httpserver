# platetectonics httpserver

A server to generate maps using plate-tectonics simulation. Written in C

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

## Dependencies

* libuv
