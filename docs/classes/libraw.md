# Class: LibRaw

Wraps LibRaw's functionality.

## Table of contents

### Constructors

- [constructor](libraw.md#constructor)

### Properties

- [libraw](libraw.md#libraw)

### Methods

- [accessLibRaw](libraw.md#accesslibraw)
- [cameraCount](libraw.md#cameracount)
- [cameraList](libraw.md#cameralist)
- [errorCount](libraw.md#errorcount)
- [getMetadata](libraw.md#getmetadata)
- [getThumbnail](libraw.md#getthumbnail)
- [getXmp](libraw.md#getxmp)
- [openBuffer](libraw.md#openbuffer)
- [openFile](libraw.md#openfile)
- [readBuffer](libraw.md#readbuffer)
- [recycle](libraw.md#recycle)
- [recycleDatastream](libraw.md#recycledatastream)
- [strerror](libraw.md#strerror)
- [unpack](libraw.md#unpack)
- [unpackThumb](libraw.md#unpackthumb)
- [version](libraw.md#version)
- [versionNumber](libraw.md#versionnumber)

## Constructors

### constructor

\+ **new LibRaw**(): [*LibRaw*](libraw.md)

**Returns:** [*LibRaw*](libraw.md)

Defined in: [libraw.ts:50](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L50)

## Properties

### libraw

• `Private` **libraw**: LibRawWrapper

Defined in: [libraw.ts:50](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L50)

## Methods

### accessLibRaw

▸ `Private`**accessLibRaw**<T\>(`executor`: () => T): *Promise*<T\>

Abstracts interactions with LibRaw to avoid boilerplate
async code repetitions in public methods.

#### Type parameters:

Name |
:------ |
`T` |

#### Parameters:

Name | Type | Description |
:------ | :------ | :------ |
`executor` | () => T | the interaction with LibRaw    |

**Returns:** *Promise*<T\>

Defined in: [libraw.ts:181](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L181)

___

### cameraCount

▸ **cameraCount**(): *Promise*<number\>

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:123](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L123)

___

### cameraList

▸ **cameraList**(): *Promise*<string[]\>

**Returns:** *Promise*<string[]\>

Defined in: [libraw.ts:127](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L127)

___

### errorCount

▸ **errorCount**(): *Promise*<number\>

This call returns count of non-fatal data errors (out of range, etc) occured in unpack() stage.

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:59](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L59)

___

### getMetadata

▸ **getMetadata**(): *Promise*<{ [key: string]: *unknown*;  }\>

Returns an object containing the RAW metadata.

**Returns:** *Promise*<{ [key: string]: *unknown*;  }\>

Defined in: [libraw.ts:74](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L74)

___

### getThumbnail

▸ **getThumbnail**(): *Promise*<Buffer\>

Unpacks and returns the bytes for the image's thumbnail.

**Returns:** *Promise*<Buffer\>

Defined in: [libraw.ts:88](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L88)

___

### getXmp

▸ **getXmp**(): *Promise*<Buffer\>

Helper function that returns the XMP data of the RAW file.

**Returns:** *Promise*<Buffer\>

Defined in: [libraw.ts:81](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L81)

___

### openBuffer

▸ **openBuffer**(`buffer`: *Buffer*): *Promise*<number\>

Created an LibRaw_buffer_datastream object, calls open_datastream().
If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
On failure, just created file_datastream destroyed immediately.

The function returns an integer number in accordance with the return code convention:
positive if any system call has returned an error, negative (from the LibRaw error list)
if there has been an error situation within LibRaw.

#### Parameters:

Name | Type | Description |
:------ | :------ | :------ |
`buffer` | *Buffer* | the image data    |

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:102](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L102)

___

### openFile

▸ **openFile**(`filename`: *string*, `bigFileSize?`: *number*): *Promise*<number\>

Creates an LibRaw_file_datastream object, calls open_datastream().
If succeed, sets internal flag which signals to destroy internal datastream object on recycle().
On failure, just created file_datastream destroyed immediately.

#### Parameters:

Name | Type | Description |
:------ | :------ | :------ |
`filename` | *string* | the file path to open   |
`bigFileSize?` | *number* | - |

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:114](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L114)

___

### readBuffer

▸ **readBuffer**(`buffer`: *Buffer*): *Promise*<void\>

Initializes a LibRaw instance from a file in memory.

#### Parameters:

Name | Type | Description |
:------ | :------ | :------ |
`buffer` | *Buffer* | the RAW file data    |

**Returns:** *Promise*<void\>

Defined in: [libraw.ts:67](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L67)

___

### recycle

▸ **recycle**(): *Promise*<void\>

Frees the allocated data of LibRaw instance, enabling one to process the next file using the same processor.
Repeated calls of recycle() are quite possible and do not conflict with anything.

**Returns:** *Promise*<void\>

Defined in: [libraw.ts:135](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L135)

___

### recycleDatastream

▸ **recycleDatastream**(): *Promise*<void\>

This call closes input datastream with associated data buffer and unblocks opened file.

**Returns:** *Promise*<void\>

Defined in: [libraw.ts:142](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L142)

___

### strerror

▸ **strerror**(`errorCode`: *number*): *Promise*<string\>

Analog of strerror(3) function: outputs the text descriptions of LibRaw error codes (in English).

#### Parameters:

Name | Type |
:------ | :------ |
`errorCode` | *number* |

**Returns:** *Promise*<string\>

Defined in: [libraw.ts:149](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L149)

___

### unpack

▸ **unpack**(): *Promise*<number\>

Unpacks the RAW files of the image, calculates the black level (not for all formats).

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:156](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L156)

___

### unpackThumb

▸ **unpackThumb**(): *Promise*<number\>

Reads (or unpacks) the image preview (thumbnail), placing the
result into the imgdata.thumbnail.thumb buffer.

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:164](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L164)

___

### version

▸ **version**(): *Promise*<string\>

**Returns:** *Promise*<string\>

Defined in: [libraw.ts:168](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L168)

___

### versionNumber

▸ **versionNumber**(): *Promise*<number\>

**Returns:** *Promise*<number\>

Defined in: [libraw.ts:172](https://github.com/justinkambic/libraw.js/blob/4d7dcc3/src/libraw.ts#L172)
