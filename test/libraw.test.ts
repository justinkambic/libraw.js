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

import { LibRaw } from '../src/libraw';
import path from 'path';
import fs, { promises as fsPromises } from 'fs';
import * as t from 'io-ts';
import { isRight } from 'fp-ts/Either';

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
const SAMPLE_TIFF = path.join(__dirname, 'test_images', 'sample.tiff');
const TEST_TIFF_OUTPUT_PATH = path.join(__dirname, 'test_images', 'test.tiff');

const __2dNumArray = t.array(t.array(t.number));
const __dngColor = t.array(
  t.type({
    calibration: __2dNumArray,
    colormatrix: __2dNumArray,
    forwardmatrix: __2dNumArray,
    illuminant: t.number,
    parsedfields: t.number,
  })
);
const __dngLevels = t.type({
  LinearResponseLimit: t.number,
  analogbalance: t.array(t.number),
  asshotneutral: t.array(t.number),
  baseline_exposure: t.number,
  default_crop: t.array(t.number),
  dng_black: t.number,
  dng_cblack: t.array(t.number),
  dng_fblack: t.number,
  dng_fcblack: t.array(t.number),
  dng_whitelevel: t.array(t.number),
  parsedfields: t.number,
  preview_colorspace: t.number,
});
const __color = t.type({
  P1_color: t.array(t.unknown),
  WB_Coeffs: __2dNumArray,
  WBCT_Coeffs: __2dNumArray,
  cblack: t.array(t.number),
  ccm: __2dNumArray,
  cmatrix: __2dNumArray,
  curve: t.array(t.number),
  dng_color: __dngColor,
  dng_levels: __dngLevels,
  white: __2dNumArray,
});
/**
 * This is not intended to be a definitive representation of the full
 * compliment of metadata fields LibRaw could return. It provides some
 * of the expected fields so it is easier for the tests to use the
 * interpolated types `io-ts` creates. This dependency should only be used
 * for testing purposes.
 */
const metadataType = t.type({
  color: __color,
  idata: t.type({
    make: t.string,
    model: t.string,
    xmplen: t.number,
    xmpdata: t.unknown,
    xtrans: __2dNumArray,
    xtrans_abs: __2dNumArray,
  }),
  lens: t.type({
    Lens: t.string,
  }),
  makernotes: t.type({
    canon: t.type({
      AFAreaHeights: t.array(t.number),
      AFAreaWidths: t.array(t.number),
      AFAreaXPositions: t.array(t.number),
      AFAreaYPositions: t.array(t.number),
    }),
    common: t.type({
      CameraTemperature: t.number,
    }),
  }),
  other: t.type({
    gpsdata: t.array(t.number),
    timestamp: t.number,
  }),
  rawdata: t.type({
    color: __color,
    iparams: t.type({
      xtrans: __2dNumArray,
      xtrans_abs: __2dNumArray,
    }),
  }),
});

type Metadata = t.TypeOf<typeof metadataType>;

function decodeLibRawMetadata(metadata: unknown) {
  const decoded = metadataType.decode(metadata);
  // `isRight` will return truthy if the provided metadata object corresponds to
  // the type as defined above, otherwise, it will error log the context of each
  // parsing failure that `io-ts` encountered. Typically this is enough to debug
  // issues, but there are additional fields you can log when debugging tests.
  if (!isRight(decoded)) {
    console.error(JSON.stringify(decoded.left.map(({ context }) => context)));
    throw new Error('Decoded metadata does not conform to expected type');
  }
  return decoded.right;
}

function deleteLargeFields(m: Metadata) {
  // these fields are very large - `io-ts` verifies they conform to the
  // expected type/shape, and then we delete them to keep snapshots manageable
  delete m.color.P1_color;
  delete m.color.WB_Coeffs;
  delete m.color.WBCT_Coeffs;
  delete m.color.cblack;
  delete m.color.ccm;
  delete m.color.cmatrix;
  delete m.color.curve;
  delete m.color.dng_color;
  delete m.color.dng_levels;
  delete m.color.white;
  delete m.idata.xtrans;
  delete m.idata.xtrans_abs;
  delete m.other.gpsdata;
  delete m.makernotes.canon.AFAreaHeights;
  delete m.makernotes.canon.AFAreaWidths;
  delete m.makernotes.canon.AFAreaXPositions;
  delete m.makernotes.canon.AFAreaYPositions;
  delete m.rawdata.color.WB_Coeffs;
  delete m.rawdata.color.WBCT_Coeffs;
  delete m.rawdata.color.cblack;
  delete m.rawdata.color.ccm;
  delete m.rawdata.color.cmatrix;
  delete m.rawdata.color.curve;
  delete m.rawdata.color.dng_color;
  delete m.rawdata.color.dng_levels;
  delete m.rawdata.color.white;
  delete m.rawdata.iparams.xmpdata;
  delete m.rawdata.iparams.xtrans;
  delete m.rawdata.iparams.xtrans_abs;
}

/*
 * To avoid issues with mismatched clocks on CI server
 * we're replacing the value for the snapshot with a hardcoded one
 * and ensuring the date has the appropriate d/m/y.
 */
function normalizeTimestampAndTest(
  metadata: Metadata,
  expectedDateVals: { day: number; month: number; year: number }
) {
  const { day, month, year } = expectedDateVals;
  const metadataDate = new Date(metadata.other.timestamp * 1000);

  expect(metadataDate.getDate()).toBe(day);
  expect(metadataDate.getMonth()).toBe(month);
  expect(metadataDate.getFullYear()).toBe(year);

  metadata.other.timestamp = 1414528361;

  expect(metadata.other).toMatchSnapshot();
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
      const metadata = decodeLibRawMetadata(await lr.getMetadata());

      deleteLargeFields(metadata);

      expect(metadata.idata).toMatchSnapshot();
      expect(metadata.lens).toMatchSnapshot();
      normalizeTimestampAndTest(metadata, { day: 28, month: 9, year: 2014 });
      expect(metadata.makernotes.common).toMatchSnapshot();
    });

    test('track key names to identify and prevent typo injection', async () => {
      await lr.openFile(RAW_NIKON_FILE_PATH);
      const metadata = decodeLibRawMetadata(await lr.getMetadata());

      deleteLargeFields(metadata);

      normalizeTimestampAndTest(metadata, { day: 26, month: 6, year: 2019 });

      expect(metadata.idata.xmplen).toEqual(12289);
      expect(metadata).toMatchSnapshot();
    });

    test('reads img data from buffer', async () => {
      const buffer = fs.readFileSync(RAW_SONY_FILE_PATH);
      await lr.readBuffer(buffer);
      await lr.unpack();
      const metadata = decodeLibRawMetadata(await lr.getMetadata());

      deleteLargeFields(metadata);

      expect(metadata.idata).toMatchSnapshot();
      expect(metadata.lens).toMatchSnapshot();
      normalizeTimestampAndTest(metadata, { day: 28, month: 9, year: 2014 });
      expect(metadata.other).toMatchSnapshot();
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

  describe('extractTiff', () => {
    test('throws error if there is an invalid path', async () => {
      expect(await lr.openFile(RAW_SONY_FILE_PATH)).toBe(0);
      // @ts-expect-error testing C++ error logic
      await expect(lr.extract_tiff(0)).rejects.toThrow(
        'extract_tiff received an invalid argument, tiffpath must be a string.'
      );
    });

    test('extracts a tiff', async () => {
      jest.setTimeout(20000);
      expect(await lr.openFile(RAW_SONY_FILE_PATH)).toBe(0);
      expect(await lr.extract_tiff(TEST_TIFF_OUTPUT_PATH)).toBe(0);
      const [output, sample] = await Promise.all([
        fsPromises.readFile(TEST_TIFF_OUTPUT_PATH, { encoding: 'binary' }),
        fsPromises.readFile(SAMPLE_TIFF, { encoding: 'binary' }),
      ]);
      expect(output).toEqual(sample);
    });
  });

  describe('cameraCount', () => {
    test('gives the number of supported cameras', async () => {
      expect(await lr.cameraCount()).toBe(1117);
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
      expect(await lr.version()).toEqual('0.20.2-Release');
    });
  });

  describe('versionNumber', () => {
    test('returns a version number >= version at time of add-on creation', async () => {
      expect(await lr.versionNumber()).toBeGreaterThanOrEqual(4869);
    });
  });
});
