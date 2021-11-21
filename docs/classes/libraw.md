# Class: LibRaw

Wraps LibRaw's functionality.

## Table of contents

### Constructors

- [constructor](LibRaw.md#constructor)

### Properties

- [libraw](LibRaw.md#libraw)

### Methods

- [accessLibRaw](LibRaw.md#accesslibraw)
- [cameraCount](LibRaw.md#cameracount)
- [cameraList](LibRaw.md#cameralist)
- [errorCount](LibRaw.md#errorcount)
- [getMetadata](LibRaw.md#getmetadata)
- [getThumbnail](LibRaw.md#getthumbnail)
- [getXmp](LibRaw.md#getxmp)
- [openBuffer](LibRaw.md#openbuffer)
- [openFile](LibRaw.md#openfile)
- [readBuffer](LibRaw.md#readbuffer)
- [recycle](LibRaw.md#recycle)
- [recycleDatastream](LibRaw.md#recycledatastream)
- [strerror](LibRaw.md#strerror)
- [unpack](LibRaw.md#unpack)
- [unpackThumb](LibRaw.md#unpackthumb)
- [version](LibRaw.md#version)
- [versionNumber](LibRaw.md#versionnumber)

## Constructors

### constructor

• **new LibRaw**()

#### Defined in

[libraw.ts:55](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L55)

## Properties

### libraw

• `Private` **libraw**: `LibRawWrapper`

#### Defined in

[libraw.ts:53](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L53)

## Methods

### accessLibRaw

▸ `Private` **accessLibRaw**<`T`\>(`executor`): `Promise`<`T`\>

Abstracts interactions with LibRaw to avoid boilerplate
async code repetitions in public methods.

#### Type parameters

| Name |
| :------ |
| `T` |

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `executor` | () => `T` | the interaction with LibRaw |

#### Returns

`Promise`<`T`\>

#### Defined in

[libraw.ts:184](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L184)

___

### cameraCount

▸ **cameraCount**(): `Promise`<`number`\>

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:126](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L126)

___

### cameraList

▸ **cameraList**(): `Promise`<`string`[]\>

#### Returns

`Promise`<`string`[]\>

#### Defined in

[libraw.ts:130](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L130)

___

### errorCount

▸ **errorCount**(): `Promise`<`number`\>

This call returns count of non-fatal data errors (out of range, etc) occured in unpack() stage.

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:62](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L62)

___

### getMetadata

▸ **getMetadata**(): `Promise`<{ [key: string]: `unknown`;  }\>

Returns an object containing the RAW metadata.

#### Returns

`Promise`<{ [key: string]: `unknown`;  }\>

#### Defined in

[libraw.ts:77](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L77)

___

### getThumbnail

▸ **getThumbnail**(): `Promise`<`Buffer`\>

Unpacks and returns the bytes for the image's thumbnail.

#### Returns

`Promise`<`Buffer`\>

#### Defined in

[libraw.ts:91](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L91)

___

### getXmp

▸ **getXmp**(): `Promise`<`Buffer`\>

Helper function that returns the XMP data of the RAW file.

#### Returns

`Promise`<`Buffer`\>

#### Defined in

[libraw.ts:84](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L84)

___

### openBuffer

▸ **openBuffer**(`buffer`): `Promise`<`number`\>

Created an LibRaw_buffer_datastream object, calls open_datastream().
If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
On failure, just created file_datastream destroyed immediately.

The function returns an integer number in accordance with the return code convention:
positive if any system call has returned an error, negative (from the LibRaw error list)
if there has been an error situation within LibRaw.

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `buffer` | `Buffer` | the image data |

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:105](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L105)

___

### openFile

▸ **openFile**(`filename`, `bigFileSize?`): `Promise`<`number`\>

Creates an LibRaw_file_datastream object, calls open_datastream().
If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
On failure, just created file_datastream destroyed immediately.

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `filename` | `string` | the file path to open |
| `bigFileSize?` | `number` | - |

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:117](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L117)

___

### readBuffer

▸ **readBuffer**(`buffer`): `Promise`<`void`\>

Initializes a LibRaw instance from a file in memory.

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `buffer` | `Buffer` | the RAW file data |

#### Returns

`Promise`<`void`\>

#### Defined in

[libraw.ts:70](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L70)

___

### recycle

▸ **recycle**(): `Promise`<`void`\>

Frees the allocated data of LibRaw instance, enabling one to process the next file using the same processor.
Repeated calls of recycle() are quite possible and do not conflict with anything.

#### Returns

`Promise`<`void`\>

#### Defined in

[libraw.ts:138](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L138)

___

### recycleDatastream

▸ **recycleDatastream**(): `Promise`<`void`\>

This call closes input datastream with associated data buffer and unblocks opened file.

#### Returns

`Promise`<`void`\>

#### Defined in

[libraw.ts:145](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L145)

___

### strerror

▸ **strerror**(`errorCode`): `Promise`<`string`\>

Analog of strerror(3) function: outputs the text descriptions of LibRaw error codes (in English).

#### Parameters

| Name | Type |
| :------ | :------ |
| `errorCode` | `number` |

#### Returns

`Promise`<`string`\>

#### Defined in

[libraw.ts:152](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L152)

___

### unpack

▸ **unpack**(): `Promise`<`number`\>

Unpacks the RAW files of the image, calculates the black level (not for all formats).

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:159](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L159)

___

### unpackThumb

▸ **unpackThumb**(): `Promise`<`number`\>

Reads (or unpacks) the image preview (thumbnail), placing the
result into the imgdata.thumbnail.thumb buffer.

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:167](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L167)

___

### version

▸ **version**(): `Promise`<`string`\>

#### Returns

`Promise`<`string`\>

#### Defined in

[libraw.ts:171](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L171)

___

### versionNumber

▸ **versionNumber**(): `Promise`<`number`\>

#### Returns

`Promise`<`number`\>

#### Defined in

[libraw.ts:175](https://github.com/justinkambic/libraw.js/blob/1e09310/src/libraw.ts#L175)
