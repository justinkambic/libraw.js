FROM ubuntu:latest
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata
RUN apt-get install -y nodejs npm libssl1.1 curl git g++ build-essential autotools-dev automake 
RUN curl -XGET http://www.ijg.org/files/jpegsrc.v9d.tar.gz -o jpegsrc.v9d.tar.gz
RUN tar xofp jpegsrc.v9d.tar.gz && cd jpeg-9d && ./configure --with-pic && make && make install && cd ../
RUN curl https://www.libraw.org/data/LibRaw-0.20.2.tar.gz --output LibRaw-0.20.2.tar.gz
RUN tar xzvf LibRaw-0.20.2.tar.gz && cd LibRaw-0.20.2 && ./configure --with-pic && touch * && make && make install && cd ../
RUN npm i -g node-gyp
RUN npm i -g prebuildify
RUN git clone https://github.com/justinkambic/libraw.js.git
RUN cd libraw.js && npm install && npm run format-check && npm run lint && npm run build && npm test && prebuildify --napi
