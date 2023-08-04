## How to Use Ninja

1. Install Ninja
```
sudo apt install ninja-build
```

2. Configure CMake to create Ninja build files
```
mkdir build && cd build
PARALLEL_LEVEL=N cmake -GNinja ..
```
   - Fill in `N` with an appropriate number of threads for your system.
Or
```
cmake -GNinja -S . -B $build -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=ON 
```

3. Build
```
ninja
```
or 
```
ninja -j 1
```

That's it!

## Suppliments
- `ninja -t targets all`: list all targets

- `ninja -t query <targetName>`: query the target

- `ninja <targetName>`: run the target

## How to create a Ninja Trace

1. Get `ninjatracing`

```
git clone https://github.com/nico/ninjatracing.git
```

2. Use `ninjatracing` to convert log file into a Chrome trace file

```
./ninjatracing.py build/.ninja_log > trace.json
```

3. Open trace file in Chrome
   - Enter `chrome://tracing` in the URL bar of Chrome
   - Click "Load" button on top left and select the `trace.json` file 


![ninja_trace](https://user-images.githubusercontent.com/15221289/60206274-ff7bb080-9807-11e9-9861-2ecb253201f0.png)
