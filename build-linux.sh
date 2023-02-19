CURRENT_LIBRAWJS_VERSION=2.3.0
PREBUILD_PATH="prebuilds/linux-x64"
NODE_FILE_NAME="node.napi.node"
OUTPUT_PATH="./${PREBUILD_PATH}/${NODE_FILE_NAME}"
IMAGE_TAG_NAME="libraw.js:${CURRENT_LIBRAWJS_VERSION}"

if [ ! -d ${PREBUILD_PATH} ]; then
    echo "Linux prebuild dir does not exist, creating.";
    mkdir -p ${PREBUILD_PATH};
else
    echo "Linux prebuild dir exists.";
fi

DOCKER_BUILDKIT=1

docker build -t ${IMAGE_TAG_NAME} --output ${OUTPUT_PATH} .
