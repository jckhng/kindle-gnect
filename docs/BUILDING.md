# Building Exact Four in a Row

## Requirements

- Docker.
- ARM binfmt support if your Docker setup does not already run ARM containers.

Install ARM binfmt support on Linux with:

```bash
docker run --privileged --rm tonistiigi/binfmt --install arm
```

## Build And Package

```bash
./docker_rebuild.sh
```

The persistent builder is:

```text
image:     exact-four-in-a-row-armhf-build:bullseye
container: exact-four-in-a-row-armhf-builder
```

Build outputs:

```text
exact-four-in-a-row
smoke-test
dist/exact-four-in-a-row-extension.zip
```

## Build Without Packaging

```bash
EXACT_FOUR_IN_A_ROW_PACKAGE=0 ./docker_rebuild.sh
```

## Builder Shell

```bash
./docker_shell.sh
```

Inside the container:

```bash
make clean
make exact-four-in-a-row smoke-test
./smoke-test
```

If you move the repository, recreate the persistent container:

```bash
docker rm -f exact-four-in-a-row-armhf-builder
./docker_rebuild.sh
```
