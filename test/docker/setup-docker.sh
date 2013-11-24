#!/bin/bash

##########################################################
# Set up Docker containers with basic dev tools and Node #
##########################################################

__dirname="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# For `make -j X`
BUILD_THREADS=`grep '^processor\s*\:\s*[0-9][0-9]*$' /proc/cpuinfo | wc -l`

# The versions of Node to test, edit the node_versions.list file to change
# NOTE: to re-make "master" just `sudo docker rmi node_dev-master` and run
# this agai`sudo docker rmi node_dev-master` and run
# this again
NODE_VERSIONS=`cat ${__dirname}/node_versions.list`

# Simple setup function for a container:
#  setup_container(image id, base image, commands to run to set up)
setup_container() {
  local ID=$1
  local BASE=$2
  local RUN=$3

  # Does this image exist? If yes, ignore
  docker inspect "$ID" &> /dev/null
  if [[ $? -eq 0 ]]; then
    echo "Found existing container [$ID]"
    return
  fi

  # No such image, so make it
  echo "Did not find container container [$ID], creating..."
  docker run $BASE /bin/bash -c "$RUN"
  sleep 2
  docker commit `docker ps -l -q` $ID
}

# A basic dev image with the build tools needed for Node
setup_container "dev_base" "ubuntu" " \
  apt-get update; \
  apt-get install -y make gcc g++ python git"

# The main Node repo in an image by itself
setup_container "node_dev" "dev_base" " \
  git clone https://github.com/joyent/node.git /usr/src/node/"

# For each version of Node, make an image by checking out that branch
# on the repo, building it and installing it
for NV in $NODE_VERSIONS; do
  setup_container "node_dev-$NV" "node_dev" " \
    cd /usr/src/node; \
    git checkout $NV; \
    git pull origin $NV; \
    ./configure; \
    make -j${BUILD_THREADS}; \
    make install"
done
