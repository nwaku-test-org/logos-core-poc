# logos-core-poc

Setup:

```bash
git submodule update --init --recursive
```

Build & run all:

```bash
./run_app.sh all
```

For core & plugins:

```bash
./run_core.sh all
```

Build Core only:

```bash
./run_core.sh build
```

Build Container:

```bash
docker build -t logos-core .
```

Run Container:

```bash
docker run -it logos-core
```

## Requirements

- QT 6.4

  Ubuntu
  ```
  apt-get install qt6-base-dev protobuf-compiler patchelf
  ```
- CMake

For some plugins
- Rust
- protobuf
