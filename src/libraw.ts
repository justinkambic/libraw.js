/*
 * libraw.js - node wrapper for LibRaw
 * Copyright (C) 2020-2021  Justin Kambic
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 * Direct further questions to justinkambic.github@gmail.com.
 */

import * as path from 'path';
import nodeGypBuild from 'node-gyp-build';

// `prebuildify` import magic, handles loading pre-built bins or will
// try to `node-gyp build` if none are found. If you cannot get this to work
// for your platform, you can do a dynamic install of LibRaw and the package should work.
const librawAddon = nodeGypBuild(path.join(__dirname, '..'));

interface LibRawWrapper {
  error_count: () => number;
  getMetadata: () => { [key: string]: unknown };
  getThumbnail: () => Buffer;
  getXmp: () => Buffer;
  cameraCount: () => number;
  cameraList: () => string[];
  open_file: (filename: string, bigfile_size?: number) => number;
  open_buffer: (buffer: Buffer) => number;
  recycle: () => void;
  recycle_datastream: () => void;
  strerror: (errorCode: number) => string;
  unpack: () => number;
  unpack_thumb: () => number;
  version: () => string;
  versionNumber: () => number;
}

// Stronger alias for metadata returned from the native wrapper. Keep this
// intentionally permissive: native keys and values vary by camera format.
export type Metadata = Record<string, unknown>;

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
  async errorCount(): Promise<number> {
    // direct call to native binding; if it throws, the async function will
    // return a rejected Promise automatically.
    return this.libraw.error_count();
  }

  /**
   * Initializes a LibRaw instance from a file in memory.
   * @param buffer the RAW file data
   */
  async readBuffer(buffer: Buffer): Promise<void> {
    return this.libraw.open_buffer(buffer) as unknown as void;
  }

  /**
   * Returns an object containing the RAW metadata.
   */
  async getMetadata(): Promise<Metadata> {
    return this.libraw.getMetadata() as Metadata;
  }

  /**
   * Helper function that returns the XMP data of the RAW file.
   */
  async getXmp(): Promise<Buffer> {
    return this.libraw.getXmp() as Buffer;
  }

  /**
   * Unpacks and returns the bytes for the image's thumbnail.
   */
  async getThumbnail(): Promise<Buffer> {
    return this.libraw.getThumbnail() as Buffer;
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
  async openBuffer(buffer: Buffer): Promise<number> {
    return this.libraw.open_buffer(buffer);
  }

  /**
   * Creates an LibRaw_file_datastream object, calls open_datastream().
   * If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
   * On failure, just created file_datastream destroyed immediately.
   *
   * @param filename the file path to open
   * @param bigfileSize optional parameter bigfile_size controls background I/O interface used for file operations
   */
  async openFile(filename: string, bigFileSize?: number): Promise<number> {
    if (bigFileSize === undefined) {
      return this.libraw.open_file(filename);
    }
    return this.libraw.open_file(filename, bigFileSize);
  }

  async cameraCount(): Promise<number> {
    return this.libraw.cameraCount();
  }

  async cameraList(): Promise<string[]> {
    return this.libraw.cameraList() as string[];
  }

  /**
   * Frees the allocated data of LibRaw instance, enabling one to process the next file using the same processor.
   * Repeated calls of recycle() are quite possible and do not conflict with anything.
   */
  async recycle(): Promise<void> {
    return this.libraw.recycle() as unknown as void;
  }

  /**
   * This call closes input datastream with associated data buffer and unblocks opened file.
   */
  async recycleDatastream(): Promise<void> {
    return this.libraw.recycle_datastream() as unknown as void;
  }

  /**
   * Analog of strerror(3) function: outputs the text descriptions of LibRaw error codes (in English).
   */
  async strerror(errorCode: number): Promise<string> {
    return this.libraw.strerror(errorCode);
  }

  /**
   * Unpacks the RAW files of the image, calculates the black level (not for all formats).
   */
  async unpack(): Promise<number> {
    return this.libraw.unpack();
  }

  /**
   * Reads (or unpacks) the image preview (thumbnail), placing the
   * result into the imgdata.thumbnail.thumb buffer.
   */
  async unpackThumb(): Promise<number> {
    return this.libraw.unpack_thumb();
  }

  async version(): Promise<string> {
    return this.libraw.version();
  }

  async versionNumber(): Promise<number> {
    return this.libraw.versionNumber();
  }
}
