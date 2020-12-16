import binary from 'node-pre-gyp';
import path from 'path';
const bindingPath = binary.find(
  path.resolve(path.join(__dirname, '../package.json'))
);
// weirdness related to node-pre-gyp https://github.com/mapbox/node-pre-gyp#3-dynamically-require-your-node
// eslint-disable-next-line @typescript-eslint/no-var-requires
const librawAddon = require(bindingPath);

interface LibRawWrapper {
  error_count: () => number;
  getMetadata: () => { [key: string]: unknown };
  getThumbnail: () => Buffer;
  getXmp: () => Buffer;
  cameraCount: () => number;
  cameraList: () => string[];
  open_file: (filename: string, bifile_size?: number) => number;
  open_buffer: (buffer: Buffer) => number;
  recycle: () => void;
  recycle_datastream: () => void;
  strerror: (errorCode: number) => string;
  unpack: () => number;
  unpack_thumb: () => number;
  version: () => string;
  versionNumber: () => number;
}

/**
 * Wraps LibRaw's functionality.
 */
export class LibRaw {
  private libraw: LibRawWrapper;

  constructor() {
    this.libraw = new librawAddon.LibRawWrapper();
  }

  /**
   * This call returns count of non-fatal data errors (out of range, etc) occured in unpack() stage.
   */
  errorCount(): Promise<number> {
    return this.accessLibRaw<number>(() => this.libraw.error_count());
  }

  /**
   * Initializes a LibRaw instance from a file in
   * the specified path.
   * @param filePath path to the RAW file to process
   */
  readFile(filePath: string): Promise<void> {
    return this.accessLibRaw<void>(() => this.libraw.open_file(filePath));
  }

  /**
   * Initializes a LibRaw instance from a file in memory.
   * @param buffer the RAW file data
   */
  readBuffer(buffer: Buffer): Promise<void> {
    return this.accessLibRaw<void>(() => this.libraw.open_buffer(buffer));
  }

  /**
   * Returns an object containing the RAW metadata.
   */
  getMetadata(): Promise<{ [key: string]: unknown }> {
    return this.accessLibRaw(() => this.libraw.getMetadata());
  }

  /**
   * Helper function that returns the XMP data of the RAW file.
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
   * Created an LibRaw_buffer_datastream object, calls open_datastream().
   * If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
   * On failure, just created file_datastream destroyed immediately.
   *
   * The function returns an integer number in accordance with the return code convention:
   * positive if any system call has returned an error, negative (from the LibRaw error list)
   * if there has been an error situation within LibRaw.
   * @param buffer the image data
   */
  openBuffer(buffer: Buffer): Promise<number> {
    return this.accessLibRaw(() => this.libraw.open_buffer(buffer));
  }

  /**
   * Creates an LibRaw_file_datastream object, calls open_datastream().
   * If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
   * On failure, just created file_datastream destroyed immediately.
   *
   * @param filename the file path to open
   * @param bigfileSize optional parameter bigfile_size controls background I/O interface used for file operations
   */
  openFile(filename: string, bigFileSize?: number): Promise<number> {
    return this.accessLibRaw(() => {
      if (bigFileSize === undefined) {
        return this.libraw.open_file(filename);
      }
      return this.libraw.open_file(filename, bigFileSize);
    });
  }

  cameraCount(): Promise<number> {
    return this.accessLibRaw(() => this.libraw.cameraCount());
  }

  cameraList(): Promise<string[]> {
    return this.accessLibRaw(() => this.libraw.cameraList());
  }

  /**
   * Frees the allocated data of LibRaw instance, enabling one to process the next file using the same processor.
   * Repeated calls of recycle() are quite possible and do not conflict with anything.
   */
  recycle(): Promise<void> {
    return this.accessLibRaw(() => this.libraw.recycle());
  }

  /**
   * This call closes input datastream with associated data buffer and unblocks opened file.
   */
  recycleDatastream(): Promise<void> {
    return this.accessLibRaw(() => this.libraw.recycle_datastream());
  }

  /**
   * Analog of strerror(3) function: outputs the text descriptions of LibRaw error codes (in English).
   */
  strerror(errorCode: number): Promise<string> {
    return this.accessLibRaw(() => this.libraw.strerror(errorCode));
  }

  /**
   * Unpacks the RAW files of the image, calculates the black level (not for all formats).
   */
  unpack(): Promise<number> {
    return this.accessLibRaw(() => this.libraw.unpack());
  }

  /**
   * Reads (or unpacks) the image preview (thumbnail), placing the
   * result into the imgdata.thumbnail.thumb buffer.
   */
  unpackThumb(): Promise<number> {
    return this.accessLibRaw(() => this.libraw.unpack_thumb());
  }

  version(): Promise<string> {
    return this.accessLibRaw(() => this.libraw.version());
  }

  versionNumber(): Promise<number> {
    return this.accessLibRaw(() => this.libraw.versionNumber());
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
