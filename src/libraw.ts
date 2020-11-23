const binary = require('node-pre-gyp');
const path = require('path');
const bindingPath = binary.find(path.resolve(path.join(__dirname, '../package.json')));
const librawAddon = require(bindingPath);

interface LibRawWrapper {
  getMetadata: () => { [key: string]: unknown };
  getThumbnail: () => Buffer;
  getXmp: () => Buffer;
}

/**
 * Wraps LibRaw's functionality.
 */
export class LibRaw {
  private libraw: LibRawWrapper;

  constructor() {
    this.libraw = null;
  }

  /**
   * Initializes a LibRaw instance from a file in
   * the specified path.
   * @param filePath path to the RAW file to process
   */
  readFile(filePath: string): Promise<void> {
    return this.accessLibRaw<void>(() => {
      this.libraw = new librawAddon.LibRawWrapper(filePath);
    });
  }

  /**
   * Initializes a LibRaw instance from a file in memory.
   * @param buffer the RAW file data
   */
  readBuffer(buffer: Buffer) {
    return this.accessLibRaw<void>(() => {
      this.libraw = new librawAddon.LibRawWrapper(buffer);
    });
  }

  /**
   * Returns an object containing the RAW metadata.
   */
  getMetadata(): Promise<{ [key: string]: unknown }> {
    return this.accessLibRaw(() => this.libraw.getMetadata());
  }

  /**
   * Returns the XMP data of the RAW file.
   */
  getXmp(): Promise<Buffer> {
    return this.accessLibRaw(() => this.libraw.getXmp());
  }

  /**
   * Unpacks and returns the bytes for the image's thumbnail.
   */
  getThumbnail(): Promise<Buffer> {
    return this.accessLibRaw(() => this.libraw.getThumbnail());
  }

  /**
   * Abstracts interactions with LibRaw to avoid boilerplate
   * async code repetitions in public methods.
   * @param executor the interaction with LibRaw
   */
  private accessLibRaw<T>(executor): Promise<T> {
    return new Promise((resolve, reject) => {
      try {
        resolve(executor());
      } catch (e) {
        reject(e);
      }
    });
  }
}
