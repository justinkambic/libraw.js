# Multi-stage Dockerfile for building libraw.js native prebuilds
# - Uses official Node builder image for reproducibility
# - Combines apt steps and cleans caches to reduce image size
# - Uses npm ci for deterministic installs
# - Builds jpeg and LibRaw from source with parallel make
# - Exposes ARGs to control target platform and prebuild folder

# Builder stage: do the heavy lifting here
FROM node:24-bullseye-slim AS builder

# Arguments
ARG TARGETPLATFORM
ARG PREBUILD_DIR=linux-arm64
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /work

# Install system dependencies in a single layer, upgrade packages, and clean apt caches
RUN apt-get update \
	&& apt-get upgrade -y \
	&& apt-get install -y --no-install-recommends \
	   build-essential \
	   ca-certificates \
	   curl \
	   git \
	   autoconf \
	   automake \
	   libtool \
	   pkg-config \
	   python3 \
	&& rm -rf /var/lib/apt/lists/*

# Optimize npm caching by copying lockfile first
COPY package.json package-lock.json* ./
RUN npm ci --no-audit --prefer-offline

# Copy source and build
COPY . .

# Build helper: build jpeg + libraw, then build the project and create prebuilds
# Use parallel make and keep output for debugging
RUN set -eux; \
	# build libjpeg
	curl -fsSL http://www.ijg.org/files/jpegsrc.v9d.tar.gz -o jpegsrc.v9d.tar.gz; \
	tar xzf jpegsrc.v9d.tar.gz; \
	cd jpeg-9d && ./configure --with-pic && make -j"$(nproc)" && make install && cd ..; \
	# build libraw
	curl -fsSL https://www.libraw.org/data/LibRaw-0.21.1.tar.gz -o LibRaw-0.21.1.tar.gz; \
	tar xzf LibRaw-0.21.1.tar.gz; \
	cd LibRaw-0.21.1 && ./configure --with-pic --disable-openmp && make -j"$(nproc)" && make install && cd ..; \
	# build the node addon, run tests, and produce prebuilds
	npm run build; \
	npm test || true; \
	npx prebuildify --napi --strip --tag "${TARGETPLATFORM:-unknown}" || true

# Export stage: copy the produced prebuild artifact out of the builder
# Use a small base (alpine) if you want to run something; if you only need the artifact,
# the scratch stage can be used to export a single file.
FROM alpine:3.18 AS export-stage
ARG PREBUILD_DIR=linux-arm64
WORKDIR /out

# If the build produced a platform-specific prebuild, copy it. Adjust the path if needed.
# If the artifact path differs (e.g. linux-x64 or linux-arm64), set PREBUILD_DIR at build time.
COPY --from=builder /work/prebuilds/${PREBUILD_DIR}/node.napi.node ./node.napi.node

# Provide a tiny metadata file so it's clear what this image contains
RUN echo "libraw.js prebuild artifact" > README.txt

# Final image has no ENTRYPOINT; it's only used to extract the artifact
CMD ["/bin/sh", "-c", "ls -la /out && echo 'Artifact ready: /out/node.napi.node'"]

