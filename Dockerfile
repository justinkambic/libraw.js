FROM ubuntu:latest AS linux-build
SHELL ["/bin/bash", "-c"]
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata
RUN apt-get install -y libssl1.1 curl git g++ build-essential autotools-dev automake 
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash && apt-get install -y nodejs
RUN curl -XGET http://www.ijg.org/files/jpegsrc.v9d.tar.gz -o jpegsrc.v9d.tar.gz
RUN tar xofp jpegsrc.v9d.tar.gz && cd jpeg-9d && ./configure --with-pic && make && make install && cd ../
RUN curl https://www.libraw.org/data/LibRaw-0.21.1.tar.gz --output LibRaw-0.21.1.tar.gz
RUN tar xzvf LibRaw-0.21.1.tar.gz && cd LibRaw-0.21.1 && ./configure --with-pic --disable-openmp && touch * && make && make install && cd ../
RUN npm i -g node-gyp node-gyp-build prebuildify
RUN git clone https://github.com/justinkambic/libraw.js.git
RUN cd libraw.js && git pull origin master && npm install && npm run format-check && npm run lint && npm run build && npm test && prebuildify --napi

WORKDIR /libraw.js

FROM scratch AS export-stage
COPY --from=linux-build /libraw.js/prebuilds/linux-x64/node.napi.node .
