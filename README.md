# libraw.js

## Installation

```
npm install --save libraw.js
```

## Usage

```typescript
import { LibRaw } from 'libraw.js';

const libRaw = new LibRaw();

await libRaw.openFile(rawImagePath);
const metadata = await libRaw.getMetadata();
await libRaw.unpackThumb();
const thumbnailBuffer = await libRaw.getThumbnail();
```

## Summary

This module provides bindings for the popular [LibRaw](https://www.libraw.org/) image library
via the [Node addon API](https://github.com/nodejs/node-addon-api). Releases are distributed with native builds for Ubuntu and macOS.
[Help with Windows support](https://github.com/justinkambic/libraw.js/issues/17) is solicited.

If you're trying to run this module on a different platform and are having trouble, please
create an issue or pull request explaining the desired solution.

This module is in an experimental state and is untested in production environments. The
primary focus at the moment is providing access to RAW metadata and thumbnails. We can add
access to additional LibRaw API functionality as those use cases emerge.

## Contributing

If you would like to contribute to the library please create a discussion issue first. Creating
a large pull request without input from the maintainer could result in wasted effort for the
author. There is plenty of work to be accomplished on this project, and contributions
are welcome, but we also want to make sure the work going into the project is in line with
its priorities.

### Testing

Any API you implement or update should include new or revised tests to cover the
functionality. The native code in the module is not tested. Because the native code is only
intended to run in the context of this addon, all unit tests are performed against the
TypeScript wrapper functions. Successfully running the test suite still requires
building/running the native code. Unit tests are run with [Jest](https://jestjs.io/).

The project includes two sample RAW images for use in testing.

## API

`libraw.js` exports a class, `LibRaw`, that wraps some of the functionality of the processor
class implemented by the native library. The class's API is documented in the [docs directory](docs/classes/libraw.md).

## Versioning

The major release versions of `libraw.js` mirror the major releases of LibRaw, starting with version 19.5.
`libraw.js` adopts the [recommended semantic versioning](https://docs.npmjs.com/about-semantic-versioning) from npm.

Every new major version of LibRaw that `libraw.js` supports will result in a major version bump; minor versions
are assigned likewise. For instance, increasing support from LibRaw version 0.19.5 to 0.19.6 would result in
`libraw.js` changing from 1.0.0 to 1.1.0. Patch releases are assigned in the typical way.
