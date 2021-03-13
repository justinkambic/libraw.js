# libraw.js

## Installation

```
npm install --save libraw.js
```

## Usage

```typescript
import { LibRaw } from 'libraw';

const libRaw = new LibRaw();

await libRaw.openFile(rawImagePath);
const metadata = await libRaw.getMetadata();
await libRaw.unpackThumb();
const thumbnailBuffer = await libRaw.getThumbnail();
```

## Summary

This module provides bindings for the popular [LibRaw](https://www.libraw.org/) image library
via the Node addon API. Releases are distributed with native builds for Ubuntu and macOS.
[Help with Windows support](https://github.com/justinkambic/libraw.js/issues/17) is solicited.

If you're trying to run this module on a different platform and are having trouble, please
create an issue or pull request explaining the desired solution.

This module is in an experimental state and is untested in production environments. The
primary focus at the moment is providing access to RAW metadata and thumbnails. We can add
access to additional LibRaw API functionality if those use cases emerge.

## Contributing

If you would like to contribute to the library please create a discussion issue first. Creating
a large pull request with no input from maintainers can result in wasted effort for the
author. There is plenty of work to be accomplished on this project, and contributions
are welcome, but we also want to make sure the work going into the project is in line with
its priorities.

### Testing

Any API you implement or update should include new or revised tests to cover the
functionality. The native code in the module is not tested. Because the native code is only
intended to run in the context of this addon, all unit tests are performed against the
TypeScript wrapper functions. Successfully running the test suite still requires
building/running the native code. Unit tests are run by Jest.

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

### Compatibility matrix

| `libraw.js` version | LibRaw version | NODE_MODULE version    |
| :------------------ | :------------- | :--------------------- |
| 1.0.0               | 0.19.5         | 64, 67, 72, 79, 83, 88 |
| 2.0.0               | 0.20.1         | 64, 67, 72, 79, 83, 88 |

### Version branching

The `master` branch reflects the current development version of the package. Additionally, there are
development branches for each release in progress, with a naming convention of `{MAJOR_VERSION}.x`. For example,
the development branch for major version `1` is named `1.x`.

When a minor version is ready for release, it is cut from the development release branch. To release version
`1.1.0`, a new branch called `1.1` is cut from `1.x`. Only fixes are merged to release branches.
When it is time to publish a release, (in this case, version `1.1.0`), the release is created using the latest
commit on the release branch. Additional patch releases are created from the same release branch, so a
version `1.1.1` would be created from a later commit to the `1.1` branch.

#### Backporting

Changes propagate from the `master` branch to development and release branches via `cherry-pick`. New features
and enhancements are only ever backported to development branches, and bug fixes are backported to development
and any appropriate release branches.

For example, a new feature intended for release in version `2.0.0` follows the path:

```
master -> 2.x
```

A bug fix that applies to `2.0.0`, `1.0.1`, _and_ an upcoming minor release `1.1.0` follows the path:

```
master -> 2.x
master -> 1.x
master -> 1.0
```
