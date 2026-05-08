#!/bin/sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
CONTAINER="$("$ROOT/docker_start_builder.sh" | tail -n 1)"
UID_HOST="$(id -u)"
GID_HOST="$(id -g)"
MAKE_TARGETS="${EXACT_FOUR_IN_A_ROW_MAKE_TARGETS:-exact-four-in-a-row smoke-test}"
DO_PACKAGE="${EXACT_FOUR_IN_A_ROW_PACKAGE:-1}"

docker exec "$CONTAINER" chown -R "$UID_HOST:$GID_HOST" /src/exact-four-in-a-row
docker exec --user "$UID_HOST:$GID_HOST" "$CONTAINER" /bin/sh -lc "make $MAKE_TARGETS && ./smoke-test"

if [ "$DO_PACKAGE" = "1" ]; then
    EXACT_FOUR_IN_A_ROW_DOCKER_CONTAINER="$CONTAINER" "$ROOT/package_extension.sh"
fi

echo "Builder container: $CONTAINER"
echo "Binary: $ROOT/exact-four-in-a-row"
if [ "$DO_PACKAGE" = "1" ]; then
    echo "Package: $ROOT/dist/exact-four-in-a-row-extension.zip"
fi
