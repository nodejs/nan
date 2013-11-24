#!/bin/bash

#############################################################
# Run NAN tests in Docker containers for many Node versions #
#############################################################

__dirname="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# How many tests can we run at once without making the computer grind to a halt
# or causing other unwanted resource problems:
SIMULTANEOUS=4

# The versions of Node to test, this assumes that we have a Docker image
# set up with the name "node_dev-<version>"
NODE_VERSIONS=`cat ${__dirname}/node_versions.list`

START_TS=`date +%s`

test_node() {
  local OUT=/tmp/nan-test-${NV}.out
  local NV=$1

  # Run test in a Docker container
  docker run -v ${__dirname}/../../:/nan-src/:ro node_dev-$NV /bin/bash -c " \
    rsync -aAXx --delete /nan-src/ /nan/ --exclude .git; \
    cd /nan/test/; \
    npm install; \
    node_modules/.bin/node-gyp --nodedir /usr/src/node/ rebuild; \
    node_modules/.bin/tap js/*-test.js; \
  " &> $OUT

  # Print status
  printf "Node@\033[1m\033[33m%-8s\033[39m\033[22m: " $NV
  if [[ `tail -1 /tmp/nan-test-${NV}.out` == "ok" ]]; then
    echo -ne "\033[1m\033[32mPASS\033[39m\033[22m"
  else
    echo -ne "\033[1m\033[31mFAIL\033[39m\033[22m"
  fi
  echo -e " \033[3mwrote output to ${OUT}\033[23m"
}

# Run all tests
_C=0
for NV in $NODE_VERSIONS; do
  test_node $NV &
  # Small break between each start, gives Docker breathing room
  sleep 0.5

  # Crude limiting on the number of simultaneous runs
  let _C=_C+1
  if [[ $((_C % ${SIMULTANEOUS})) == 0 ]]; then
    wait
  fi
done

wait

END_TS=`date +%s`
DURATION=$((END_TS-START_TS))
VERSIONS=$(echo $NODE_VERSIONS | wc -w)

echo "Took ${DURATION}s to run ${VERSIONS} versions of Node"
