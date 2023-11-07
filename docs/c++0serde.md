# SerDe

## Easy Way: 
- std::memcpy.
- construct an object without memory copy. (just change data type signature)
- example: modules/basis/serde/serde_vector.cpp

## Boost.serialization
- example: modules/boostorg/boost_serialization.cpp

## Google Protobuf

## [bitsery](https://github.com/fraillt/bitsery)
- Why use bitsery

Look at the numbers and features list, and decide yourself.

| library     | data size | ser time | des time |
| ----------- | --------- | -------- | -------- |
| bitsery     | 6913B     | 1119ms   | 1166ms   |
| boost       | 11037B    | 15391ms  | 12912ms  |
| cereal      | 10413B    | 10518ms  | 10245ms  |
| flatbuffers | 14924B    | 9075ms   | 3701ms   |
| msgpack     | 8857B     | 3340ms   | 13842ms  |
| protobuf    | 10018B    | 21229ms  | 22077ms  |
| yas         | 10463B    | 2107ms   | 1554ms   |

*benchmarked on Ubuntu with GCC 10.3.0, more details can be found [here](https://github.com/fraillt/cpp_serializers_benchmark.git)*

## [flatbuffers](https://github.com/google/flatbuffers)
FlatBuffers is an efficient cross platform serialization library for C++, C#, C, Go, Java, Kotlin, JavaScript, Lobster, Lua, TypeScript, PHP, Python, Rust and Swift. It was originally created at Google for game development and other performance-critical applications.

- Why not use Protocol Buffers, or .. ?

Protocol Buffers is indeed relatively similar to FlatBuffers, with the primary difference being that FlatBuffers does not need a parsing/ unpacking step to a secondary representation before you can access data, often coupled with per-object memory allocation. The code is an order of magnitude bigger, too. Protocol Buffers has no optional text import/export.

## [ThorsSerializer](https://github.com/Loki-Astari/ThorsSerializer)
