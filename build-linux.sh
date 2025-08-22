#!/bin/bash

CPU_ARCH=$(uname -m)
echo "Current CPU Architecture: $CPU_ARCH, build will be for this arch."

CURRENT_LIBRAWJS_VERSION=3.0.0
PREBUILD_PATH="prebuilds/linux-${CPU_ARCH}"
NODE_FILE_NAME="node.napi.node"
OUTPUT_PATH="./${PREBUILD_PATH}/${NODE_FILE_NAME}"
BACKUP_PATH="./${PREBUILD_PATH}/bak.${NODE_FILE_NAME}"
IMAGE_TAG_NAME="libraw.js:${CURRENT_LIBRAWJS_VERSION}"

echo "Output path is ${OUTPUT_PATH}";

if [ ! -d ${PREBUILD_PATH} ]; then
    echo "Linux prebuild dir does not exist, creating.";
    mkdir -p ${PREBUILD_PATH};
else
    echo "Linux prebuild dir exists.";
fi

if [ -f ${OUTPUT_PATH} ]; then
  echo "Linux prebuild file exists. Backing up.";
  mv ${OUTPUT_PATH} ${BACKUP_PATH};
fi

DOCKER_BUILDKIT=1

#docker build --build-arg archversion=arm64 --output ${PREBUILD_PATH} --no-cache -t ${IMAGE_TAG_NAME} .


docker build --build-arg archversion=arm64 --output "prebuilds/linux-amd64" -t ${IMAGE_TAG_NAME} .

DOCKER_EXIT_CODE=$?

if [ -f ${BACKUP_PATH} ] && [ $DOCKER_EXIT_CODE -eq 0 ]; then
  echo "Binary build succeeded. Deleting backup file."
  rm ${BACKUP_PATH};
fi

if [ $DOCKER_EXIT_CODE -eq 0 ]; then
  echo "Linux build complete.";
else
  echo "Docker build failed."
  exit 1;
fi
