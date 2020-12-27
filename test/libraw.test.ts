import { LibRaw } from '../src/libraw';
import path from 'path';
import fs from 'fs';

const RAW_SONY_FILE_PATH = path.join(
  __dirname,
  'test_images/RAW_SONY_ILCA-77M2.ARW'
);
const RAW_NIKON_FILE_PATH = path.join(
  __dirname,
  'test_images/RAW_NIKON_Z6.NEF'
);
const TEST_THUMBNAIL_JPG = path.join(
  __dirname,
  'test_images',
  'test_thumb.jpg'
);

interface HasTimestamp {
  timestamp: number;
}

function asHasTimestamp(d: unknown): HasTimestamp {
  if (typeof d === 'object' && d !== null) {
    const testObj = d as HasTimestamp;
    if (Number.isInteger(testObj.timestamp)) {
      return testObj;
    }
  }
  throw new Error('Object does not contain a `timestamp` field');
}

describe('LibRaw', () => {
  let lr: LibRaw;

  beforeEach(() => {
    lr = new LibRaw();
  });

  afterEach(async () => {
    await lr.recycle();
  });

  describe('getMetadata', () => {
    test('basic metadata fields supported', async () => {
      await lr.openFile(RAW_SONY_FILE_PATH);
      const metadata = await lr.getMetadata();
      expect(metadata.idata).toMatchSnapshot();
      expect(metadata.lens).toMatchSnapshot();
      const other = asHasTimestamp(metadata.other);
      /*
       * To avoid issues with mismatched clocks on CI server
       * we're replacing the value for the snapshot with a hardcoded one
       * and ensuring the date has the appropriate d/m/y.
       *
       * Improvements to this test are welcome.
       */
      const metadataDate = new Date(other.timestamp * 1000);
      expect(metadataDate.getFullYear()).toBe(2014);
      expect(metadataDate.getMonth()).toBe(9);
      expect(metadataDate.getDate()).toBe(28);
      other.timestamp = 1414528361;
      expect(other).toMatchSnapshot();
    });

    test('reads img data from buffer', async () => {
      const buffer = fs.readFileSync(RAW_SONY_FILE_PATH);
      await lr.readBuffer(buffer);
      await lr.unpack();
      const metadata = await lr.getMetadata();
      expect(metadata.idata).toMatchSnapshot();
      expect(metadata.lens).toMatchSnapshot();
      const other = asHasTimestamp(metadata.other);
      /*
       * To avoid issues with mismatched clocks on CI server
       * we're replacing the value for the snapshot with a hardcoded one
       * and ensuring the date has the appropriate d/m/y.
       *
       * Improvements to this test are welcome.
       */
      const metadataDate = new Date(other.timestamp * 1000);
      expect(metadataDate.getFullYear()).toBe(2014);
      expect(metadataDate.getMonth()).toBe(9);
      expect(metadataDate.getDate()).toBe(28);
      other.timestamp = 1414528361;
      expect(other).toMatchSnapshot();
    });
  });

  describe('getXmp', () => {
    test('parses xmp', async () => {
      const lr = new LibRaw();
      await lr.openFile(path.join(__dirname, 'test_images/RAW_NIKON_Z6.NEF'));
      const xmp = (await lr.getXmp()).toString('utf8');
      expect(xmp).toMatchSnapshot();
    });
  });

  describe('errorCount', () => {
    test('returns 0 with no errors', async () => {
      await lr.openFile(RAW_SONY_FILE_PATH);
      await lr.unpack();
      expect(await lr.errorCount()).toBe(0);
    });

    test('returns non-zero code for error state', async () => {
      await lr.openFile(RAW_NIKON_FILE_PATH);
      await lr.unpack();
      expect(await lr.errorCount()).not.toBe(0);
    });
  });

  describe('openFile', () => {
    test('openFile opens file', async () => {
      expect(await lr.openFile(RAW_NIKON_FILE_PATH)).toEqual(0);
    });

    test('openFile returns error code for nonexistent file', async () => {
      expect(await lr.openFile('some nonexistent path')).not.toEqual(0);
    });

    test('throws exception if filename is not string', async () => {
      // ignore ban-ts-comment for testing purposes
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore testing C++ condition
      await expect(lr.openFile(23)).rejects.toThrow(
        'openFile received an invalid argument, filename must be a string.'
      );
    });

    test('throws exception if bigfile_size is not number', async () => {
      // ignore ban-ts-comment for testing purposes
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore testing C++ condition
      await expect(lr.openFile('a string', 'not a number')).rejects.toThrow(
        'openFile received an invalid argument, bigfile_size must be a number.'
      );
    });
  });

  describe('openBuffer', () => {
    test('throws exception if buffer is not provided', async () => {
      // ignore ban-ts-comment for testing purposes
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore testing C++ condition
      await expect(lr.openBuffer()).rejects.toThrow(
        'openBuffer received a null argument, buffer is required.'
      );
    });

    test('openBuffer accepts data', async () => {
      const buffer = fs.readFileSync(RAW_SONY_FILE_PATH);
      expect(await lr.openBuffer(buffer)).toEqual(0);
    });
  });

  describe('unpack', () => {
    test('unpacks image without error', async () => {
      expect(await lr.openFile(RAW_NIKON_FILE_PATH)).toEqual(0);
      expect(await lr.unpack()).toBe(0);
    });

    test('returns error code for un-opened file', async () => {
      expect(await lr.unpack()).not.toEqual(0);
    });
  });

  describe('unpackThumb', () => {
    test('unpacks thumbnail without error', async () => {
      expect(await lr.openFile(RAW_NIKON_FILE_PATH)).toBe(0);
      expect(await lr.unpackThumb()).toBe(0);
    });

    test('returns out-of-order error code when unpacking un-opened file', async () => {
      expect(await lr.unpackThumb()).toBe(-4);
    });
  });

  describe('getThumbnail', () => {
    test('throws error if there is no thumbnail', async () => {
      await expect(lr.getThumbnail()).rejects.toThrow(
        'Thumbnail is not unpacked or is null.'
      );
    });

    test('gets the thumbnail from a RAW file', async () => {
      expect(await lr.openFile(RAW_SONY_FILE_PATH)).toBe(0);
      expect(await lr.unpackThumb()).toBe(0);
      const thumbnail = await lr.getThumbnail();
      const testBuffer = fs.readFileSync(TEST_THUMBNAIL_JPG);
      expect(thumbnail.equals(testBuffer)).toBe(true);
    });
  });

  describe('cameraCount', () => {
    test('gives the number of supported cameras', async () => {
      expect(await lr.cameraCount()).toBe(1016);
    });
  });

  describe('cameraList', () => {
    test('gives a list of supported cameras', async () => {
      expect(JSON.stringify(await lr.cameraList(), null, 2)).toMatchSnapshot();
    });
  });

  describe('recycle', () => {
    test('runs without error', async () => {
      expect(await lr.openFile(RAW_SONY_FILE_PATH)).toBe(0);
      expect(await lr.recycle());
    });
  });

  describe('strerror', () => {
    test('handles error code 4', async () => {
      expect(await lr.strerror(-4)).toEqual(
        'Out of order call of libraw function'
      );
    });
  });

  describe('version', () => {
    test('returns version string', async () => {
      expect(await lr.version()).toEqual('0.19.5-Release');
    });
  });

  describe('versionNumber', () => {
    test('returns a version number >= version at time of add-on creation', async () => {
      expect(await lr.versionNumber()).toBeGreaterThanOrEqual(4869);
    });
  });
});
