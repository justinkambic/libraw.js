import { LibRaw } from '../src/libraw';
import path from 'path';
import fs from 'fs';

describe('LibRaw', () => {
  test('throws exception for non-existent file', () => {
    const lr = new LibRaw();
    expect(lr.readFile('./some/non-existent-path')).rejects.toEqual(
      new Error('File does not exist')
    );
  });

  test('basic metadata fields supported', async () => {
    expect.assertions(3);
    const lr = new LibRaw();
    await lr.readFile(
      path.join(__dirname, 'test_images/RAW_SONY_ILCA-77M2.ARW')
    );
    const metadata = await lr.getMetadata();
    expect(metadata.idata).toMatchSnapshot();
    expect(metadata.lens).toMatchSnapshot();
    expect(metadata.other).toMatchSnapshot();
  });

  test('reads img data from buffer', async () => {
    expect.assertions(3);
    const lr = new LibRaw();
    const buffer = fs.readFileSync(
      path.join(__dirname, 'test_images/RAW_SONY_ILCA-77M2.ARW')
    );
    await lr.readBuffer(buffer);
    const metadata = await lr.getMetadata();
    expect(metadata.idata).toMatchSnapshot();
    expect(metadata.lens).toMatchSnapshot();
    expect(metadata.other).toMatchSnapshot();
  });

  test('parses xmp', async () => {
    const lr = new LibRaw();
    await lr.readFile(path.join(__dirname, 'test_images/RAW_NIKON_Z6.NEF'));
    const xmp = (await lr.getXmp()).toString('utf8');
    expect(xmp).toMatchSnapshot();
  });
});
