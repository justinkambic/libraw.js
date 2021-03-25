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

#include <napi.h>
#include "wraptypes.h"

/* passing raw floats to v8 will cause a loss of precision.
 * simply casting to double does not seem to work either.
 * 
 * Example: providing a float with a value like 0.0125 will
 * yield a Number like 0.012500000186264515 in JavaScript.
 * 
 * Please improve if there is a more efficient solution.
 * 
 * More info: https://github.com/nodejs/node-addon-api/issues/836
*/
double convertFloat(float f)
{
  std::string s = std::to_string(f);
  std::size_t offset = 0;
  return std::stod(s, &offset);
}

template <class T>
Napi::Array WrapArray(Napi::Env* env, T ar[], size_t size)
{
  Napi::Array a = Napi::Array::New(*env, size);
  for (size_t i = 0; i < size; i++)
  {
    a[i] = ar[i];
  }
  return a;
}

Napi::Array MapFloatArrayToDouble(Napi::Env* env, float ar[], size_t size)
{
  Napi::Array a = Napi::Array::New(*env, size);
  for (size_t i = 0; i < size; i++)
  {
    a[i] = convertFloat(ar[i]);
  }
  return a;
}

Napi::Object Wrapidata(Napi::Env* env, libraw_iparams_t iparams)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("guard", iparams.guard);
  o.Set("make", iparams.make);
  o.Set("model", iparams.model);
  o.Set("software", iparams.software);
  o.Set("normalized_make", iparams.normalized_make);
  o.Set("normalized_model", iparams.normalized_model);
  o.Set("maker_index", iparams.maker_index);
  o.Set("raw_count", iparams.raw_count);
  o.Set("dng_version", iparams.dng_version);
  o.Set("is_foveon", iparams.is_foveon);
  o.Set("colors", iparams.colors);
  o.Set("filters", iparams.filters);
  Napi::Array xtrans = Napi::Array::New(*env, 6);
  Napi::Array xtrans_abs = Napi::Array::New(*env, 6);

  for (int i = 0; i < 6; i++)
  {
    xtrans[i] = WrapArray(env, iparams.xtrans[i], 6);
    xtrans_abs[i] = WrapArray(env, iparams.xtrans_abs[i], 6);
  }

  o.Set("xtrans", xtrans);
  o.Set("xtrans_abs", xtrans_abs);
  o.Set("cdesc", iparams.cdesc);
  o.Set("xmplen", iparams.xmplen);
  /*
   * There were some cross-platform inconsistencies with this code.
   * The Ubuntu container that creates the release versions works fine,
   * as does macOS, but the ubuntu-latest container provided by GitHub Actions
   * for the CI builds does not allow for this buffer creation at runtime.
   * 
   * Given that the API the package exposes has a dedicated XMP function, this
   * buffer is excluded from the default metadata wrapper code.
   */
  // if (iparams.xmplen)
  // {
  //   // o.Set("xmpdata", Napi::Buffer<char>::New(*env, iparams.xmpdata, (std::size_t)iparams.xmplen));
  // }

  return o;
}

Napi::Object WrapRawInsetCrop(Napi::Env* env, libraw_raw_inset_crop_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("cleft", t.cleft);
  o.Set("ctop", t.ctop);
  o.Set("cwidth", t.cwidth);
  o.Set("cheight", t.cheight);
  o.Set("aspect", t.aspect);

  return o;
}

Napi::Object WrapImageSizes(Napi::Env* env, libraw_image_sizes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("raw_height", t.raw_height);
  o.Set("raw_width", t.raw_width);
  o.Set("height", t.height);
  o.Set("width", t.width);
  o.Set("top_margin", t.top_margin);
  o.Set("left_margin", t.left_margin);
  o.Set("iheight", t.iheight);
  o.Set("iwidth", t.iwidth);
  o.Set("raw_pitch", t.raw_pitch);
  o.Set("pixel_aspect", t.pixel_aspect);
  o.Set("flip", t.flip);
  Napi::Array mask = Napi::Array::New(*env, 8);
  for (int j = 0; j < 8; j++) {
    mask[j] = WrapArray(env, t.mask[j], 4);
  }
  o.Set("mask", mask);
  o.Set("raw_inset_crop", WrapRawInsetCrop(env, t.raw_inset_crop));

  return o;
}

Napi::Object WrapDngLens(Napi::Env* env, libraw_dnglens_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("MinFocal", convertFloat(t.MinFocal));
  o.Set("MaxFocal", convertFloat(t.MaxFocal));
  o.Set("MaxAp4MinFocal",convertFloat(t.MaxAp4MinFocal));
  o.Set("MaxAp4MaxFocal", convertFloat(t.MaxAp4MaxFocal));

  return o;
}

Napi::Object WrapMakernotesLens(Napi::Env* env, libraw_makernotes_lens_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("LensID", t.Lens);
  o.Set("Lens", t.Lens);
  o.Set("LensFormat", t.LensFormat);
  o.Set("LensMount", t.LensMount);
  o.Set("CamID", t.CamID);
  o.Set("CameraFormat", t.CameraFormat);
  o.Set("CameraMount", t.CameraMount);
  o.Set("body", t.body);
  o.Set("FocalType", t.FocalType);
  o.Set("LensFeatures_pre", t.LensFeatures_pre);
  o.Set("LensFeatures_suf", t.LensFeatures_suf);
  o.Set("MinFocal", convertFloat(t.MinFocal));
  o.Set("MaxFocal", convertFloat(t.MaxFocal));
  o.Set("MaxAp4MinFocal",convertFloat( t.MaxAp4MinFocal));
  o.Set("MaxAp4MaxFocal",convertFloat( t.MaxAp4MaxFocal));
  o.Set("MinAp4MinFocal",convertFloat( t.MinAp4MinFocal));
  o.Set("MinAp4MaxFocal",convertFloat( t.MinAp4MaxFocal));
  o.Set("MaxAp", convertFloat(t.MaxAp));
  o.Set("MinAp", convertFloat(t.MinAp));
  o.Set("CurFocal", convertFloat(t.CurFocal));
  o.Set("CurAp", convertFloat(t.CurAp));
  o.Set("MaxAp4CurFocal",convertFloat( t.MaxAp4CurFocal));
  o.Set("MinAp4CurFocal",convertFloat( t.MinAp4CurFocal));
  o.Set("MinFocusDistance", convertFloat(t.MinFocusDistance));
  o.Set("FocusRangeIndex", convertFloat(t.FocusRangeIndex));
  o.Set("LensFStops", convertFloat(t.LensFStops));
  o.Set("TeleconverterID", t.TeleconverterID);
  o.Set("Teleconverter", t.Teleconverter);
  o.Set("AdapterID", t.AdapterID);
  o.Set("Adapter", t.Adapter);
  o.Set("AttachmentID", t.AttachmentID);
  o.Set("Attachment", t.Attachment);
  o.Set("FocalUnits", t.FocalUnits);
  o.Set("FocalLengthIn35mmFormat", convertFloat(t.FocalLengthIn35mmFormat));

  return o;
}

Napi::Object WrapNikonLens(Napi::Env* env, libraw_nikonlens_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("EffectiveMaxAp", convertFloat(t.EffectiveMaxAp));
  o.Set("LensIDNumber", t.LensIDNumber);
  o.Set("LensFStops", t.LensFStops);
  o.Set("MCUVersion", t.MCUVersion);
  o.Set("LensType", t.LensType);

  return o;
}

Napi::Object WrapLensInfo(Napi::Env* env, libraw_lensinfo_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("MinFocal", convertFloat(t.MinFocal));
  o.Set("MaxFocal", convertFloat(t.MaxFocal));
  o.Set("MaxAp4MinFocal", convertFloat(t.MaxAp4MinFocal));
  o.Set("MaxAp4MaxFocal",convertFloat( t.MaxAp4MaxFocal));
  o.Set("EXIF_MaxAp", convertFloat(t.EXIF_MaxAp));
  o.Set("LensMake", t.LensMake);
  o.Set("Lens", t.Lens);
  o.Set("LensSerial", t.LensSerial);
  o.Set("InternalLensSerial", t.InternalLensSerial);
  o.Set("FocalLengthIn35mmFormat", t.FocalLengthIn35mmFormat);
  o.Set("nikon", WrapNikonLens(env, t.nikon));
  o.Set("dng", WrapDngLens(env, t.dng));
  o.Set("makernotes", WrapMakernotesLens(env, t.makernotes));

  return o;
}

Napi::Object WrapCanonMakernotes(Napi::Env* env, libraw_canon_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("ColorDataVer", t.ColorDataVer);
  o.Set("ColorDataSubVer", t.ColorDataSubVer);
  o.Set("SpecularWhiteLevel", t.SpecularWhiteLevel);
  o.Set("NormalWhiteLevel", t.NormalWhiteLevel);
  o.Set("ChannelBlackLevel", WrapArray(env, t.ChannelBlackLevel, 4));
  o.Set("AverageBlackLevel", t.AverageBlackLevel);
  o.Set("multishot", WrapArray(env, t.multishot, 4));
  o.Set("MeteringMode", t.MeteringMode);
  o.Set("SpotMeteringMode", t.SpotMeteringMode);
  o.Set("FlashMeteringMode", t.FlashMeteringMode);
  o.Set("FlashExposureLock", t.FlashExposureLock);
  o.Set("ExposureMode", t.ExposureMode);
  o.Set("AESetting", t.AESetting);
  o.Set("HighlightTonePriority", t.HighlightTonePriority);
  o.Set("ImageStabilization", t.ImageStabilization);
  o.Set("FocusMode", t.FocusMode);
  o.Set("AFPoint", t.AFPoint);
  o.Set("FocusContinuous", t.FocusContinuous);
  o.Set("AFPointsInFocus30D", t.AFPointsInFocus30D);
  o.Set("AFPointsInFocus1D", WrapArray(env, t.AFPointsInFocus1D, 8));
  o.Set("AFPointsInFocus5D", t.AFPointsInFocus5D);
  o.Set("AFAreaMode", t.AFAreaMode);
  o.Set("NumAFPoints", t.NumAFPoints);
  o.Set("ValidAFPoints", t.ValidAFPoints);
  o.Set("AFImageWidth", t.AFImageWidth);
  o.Set("AFImageHeight", t.AFImageHeight);
  o.Set("AFAreaWidths", WrapArray(env, t.AFAreaWidths, 61));
  o.Set("AFAreaHeights", WrapArray(env, t.AFAreaHeights, 61));
  o.Set("AFAreaXPositions", WrapArray(env, t.AFAreaXPositions, 61));
  o.Set("AFAreaYPositions", WrapArray(env, t.AFAreaYPositions, 61));
  o.Set("AFPointsInFocus", WrapArray(env, t.AFPointsInFocus, 4));
  o.Set("AFPointsSelected", WrapArray(env, t.AFPointsSelected, 4));
  o.Set("PrimaryAFPoint", t.PrimaryAFPoint);
  o.Set("FlashMode", t.FlashMode);
  o.Set("FlashActivity", t.FlashActivity);
  o.Set("FlashBits", t.FlashBits);
  o.Set("ManualFlashOutput", t.ManualFlashOutput);
  o.Set("FlashOutput", t.FlashOutput);
  o.Set("FlashGuideNumber", t.FlashGuideNumber);
  o.Set("ContinuousDrive", t.ContinuousDrive);
  o.Set("SensorWidth", t.SensorWidth);
  o.Set("SensorHeight", t.SensorHeight);
  o.Set("SensorLeftBorder", t.SensorLeftBorder);
  o.Set("SensorTopBorder", t.SensorTopBorder);
  o.Set("SensorRightBorder", t.SensorRightBorder);
  o.Set("SensorBottomBorder", t.SensorBottomBorder);
  o.Set("BlackMaskLeftBorder", t.BlackMaskLeftBorder);
  o.Set("BlackMaskTopBorder", t.BlackMaskTopBorder);
  o.Set("BlackMaskRightBorder", t.BlackMaskRightBorder);
  o.Set("BlackMaskBottomBorder", t.BlackMaskBottomBorder);
  o.Set("AFMicroAdjMode", t.AFMicroAdjMode);
  o.Set("AFMicroAdjValue", convertFloat(t.AFMicroAdjValue));
  o.Set("MakernotesFlip", t.MakernotesFlip);
  o.Set("RecordMode", t.RecordMode);
  o.Set("SRAWQuality", t.SRAWQuality);
  o.Set("wbi", t.wbi);
  o.Set("firmware", t.firmware);
  o.Set("RF_lensID", t.RF_lensID);

  return o;
}

Napi::Object WrapSensorHighspeedCrop(Napi::Env* env, libraw_sensor_highspeed_crop_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("cleft", t.cleft);
  o.Set("ctop", t.ctop);
  o.Set("cwidth", t.cwidth);
  o.Set("cheight", t.cheight);

  return o;
}

Napi::Object WrapNikonMakernotes(Napi::Env* env, libraw_nikon_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("ExposureBracketValue", t.ExposureBracketValue);
  o.Set("ActiveDLighting", t.ActiveDLighting);
  o.Set("ShootingMode", t.ShootingMode);
  o.Set("ImageStabilization", WrapArray(env, t.ImageStabilization, 7));
  o.Set("VibrationReduction", t.VibrationReduction);
  o.Set("VRMode", t.VRMode);
  o.Set("FocusMode", WrapArray(env, t.FocusMode, 7));
  o.Set("AFPoint", t.AFPoint);
  o.Set("AFPointsInFocus", t.AFPointsInFocus);
  o.Set("ContrastDetectAF", t.ContrastDetectAF);
  o.Set("AFAreaMode", t.AFAreaMode);
  o.Set("PhaseDetectAF", t.PhaseDetectAF);
  o.Set("PrimaryAFPoint", t.PrimaryAFPoint);
  o.Set("AFPointsUsed", WrapArray(env, t.AFPointsUsed, 29));
  o.Set("AFImageWidth", t.AFImageWidth);
  o.Set("AFImageHeight", t.AFImageHeight);
  o.Set("AFAreaXPposition", t.AFAreaXPposition);
  o.Set("AFAreaYPosition", t.AFAreaYPosition);
  o.Set("AFAreaWidth", t.AFAreaWidth);
  o.Set("AFAreaHeight", t.AFAreaHeight);
  o.Set("ContrastDetectAFInFocus", t.ContrastDetectAFInFocus);
  o.Set("FlashSetting", WrapArray(env, t.FlashSetting, 13));
  o.Set("FlashType", WrapArray(env, t.FlashType, 20));
  o.Set("FlashExposureCompensation", WrapArray(env, t.FlashExposureCompensation, 4));
  o.Set("ExternalFlashExposureComp", WrapArray(env, t.ExternalFlashExposureComp, 4));
  o.Set("FlashExposureBracketValue", WrapArray(env, t.FlashExposureBracketValue, 4));
  o.Set("FlashMode", t.FlashMode);
  o.Set("FlashExposureCompensation2", t.FlashExposureCompensation2);
  o.Set("FlashExposureCompensation3", t.FlashExposureCompensation3);
  o.Set("FlashExposureCompensation4", t.FlashExposureCompensation4);
  o.Set("FlashSource", t.FlashSource);
  o.Set("FlashFirmware", WrapArray(env, t.FlashFirmware, 2));
  o.Set("ExternalFlashFlags", t.ExternalFlashFlags);
  o.Set("FlashControlCommanderMode", t.FlashControlCommanderMode);
  o.Set("FlashOutputAndCompensation", t.FlashOutputAndCompensation);
  o.Set("FlashFocalLength", t.FlashFocalLength);
  o.Set("FlashGNDistance", t.FlashGNDistance);
  o.Set("FlashGroupControlMode", WrapArray(env, t.FlashGroupControlMode, 4));
  o.Set("FlashGroupOutputAndCompensation", WrapArray(env, t.FlashGroupOutputAndCompensation, 4));
  o.Set("FlashColorFilter", t.FlashColorFilter);
  o.Set("NEFCompression", t.NEFCompression);
  o.Set("ExposureMode", t.ExposureMode);
  o.Set("ExposureProgram", t.ExposureProgram);
  o.Set("nMEshots", t.nMEshots);
  o.Set("MEgainOn", t.MEgainOn);
  o.Set("ME_WB", WrapArray(env, t.ME_WB, 4));
  o.Set("AFFineTune", t.AFFineTune);
  o.Set("AFFineTuneIndex", t.AFFineTuneIndex);
  o.Set("AFFineTuneAdj", t.AFFineTuneAdj);
  o.Set("LensDataVersion", t.LensDataVersion);
  o.Set("FlashInfoVersion", t.FlashInfoVersion);
  o.Set("ColorBalanceVersion", t.ColorBalanceVersion);
  o.Set("key", t.key);
  o.Set("NEFBitDepth", WrapArray(env, t.NEFBitDepth, 4));
  o.Set("HighSpeedCropFormat", t.HighSpeedCropFormat);
  o.Set("SensorWidth", t.SensorWidth);
  o.Set("SensorHighSpeedCrop", WrapSensorHighspeedCrop(env, t.SensorHighSpeedCrop));
  o.Set("SensorHeight", t.SensorHeight);

  return o;
}

Napi::Object WrapHasselbladMakernotes(Napi::Env* env, libraw_hasselblad_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("BaseISO", t.BaseISO);
  o.Set("Gain", t.Gain);
  o.Set("Sensor", t.Sensor);
  o.Set("SensorUnit", t.SensorUnit);
  o.Set("HostBody", t.HostBody);
  o.Set("SensorCode", t.SensorCode);
  o.Set("SensorSubCode", t.SensorSubCode);
  o.Set("CoatingCode", t.CoatingCode);
  o.Set("uncropped", t.uncropped);
  o.Set("CaptureSequenceInitiator", t.CaptureSequenceInitiator);
  o.Set("SensorUnitConnector", t.SensorUnitConnector);
  o.Set("format", t.format);
  o.Set("nIFD_CM", WrapArray(env, t.nIFD_CM, 2));
  o.Set("RecommendedCrop", WrapArray(env, t.RecommendedCrop, 2));
  Napi::Array mnColorMatrix = Napi::Array::New(*env, 4);
  for (std::size_t i = 0; i < 4; i++)
  {
    mnColorMatrix[i] = WrapArray(env, t.mnColorMatrix[i], 3);
  }
  o.Set("mnColorMatrix", mnColorMatrix);

  return o;
}

Napi::Object WrapFujiInfo(Napi::Env* env, libraw_fuji_info_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("ExpoMidPointShift", convertFloat(t.ExpoMidPointShift));
  o.Set("DynamicRange", t.DynamicRange);
  o.Set("FilmMode", t.FilmMode);
  o.Set("DynamicRangeSetting", t.DynamicRangeSetting);
  o.Set("DevelopmentDynamicRange", t.DevelopmentDynamicRange);
  o.Set("AutoDynamicRange", t.AutoDynamicRange);
  o.Set("DRangePriority", t.DRangePriority);
  o.Set("DRangePriorityAuto", t.DRangePriorityAuto);
  o.Set("DRangePriorityFixed", t.DRangePriorityFixed);
  o.Set("BrightnessCompensation", t.BrightnessCompensation);
  o.Set("FocusMode", t.FocusMode);
  o.Set("AFMode", t.AFMode);
  o.Set("FocusPixel", WrapArray(env, t.FocusPixel, 2));
  o.Set("ImageStabilization", WrapArray(env, t.ImageStabilization, 3));
  o.Set("FlashMode", t.FlashMode);
  o.Set("WB_Preset", t.WB_Preset);
  o.Set("ShutterType", t.ShutterType);
  o.Set("ExrMode", t.ExrMode);
  o.Set("Macro", t.Macro);
  o.Set("Rating", t.Rating);
  o.Set("CropMode", t.CropMode);
  o.Set("FrameRate", t.FrameRate);
  o.Set("FrameWidth", t.FrameWidth);
  o.Set("FrameHeight", t.FrameHeight);
  o.Set("SerialSignature", t.SerialSignature);
  o.Set("RAFVersion", t.RAFVersion);
  o.Set("RAFDataVersion", t.RAFDataVersion);
  o.Set("isTSNERDTS", t.isTSNERDTS);
  o.Set("DriveMode", t.DriveMode);

  return o;
}

Napi::Object WrapOlympusMakernotes(Napi::Env* env, libraw_olympus_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("SensorCalibration", WrapArray(env, t.SensorCalibration, 2));
  o.Set("FocusMode", WrapArray(env, t.FocusMode, 2));
  o.Set("AutoFocus", t.AutoFocus);
  o.Set("AFPoint", t.AFPoint);
  o.Set("AFAreas", WrapArray(env, t.AFAreas, 64));
  o.Set("AFPointSelected", WrapArray(env, t.AFPointSelected, 5));
  o.Set("AFResult", t.AFResult);
  o.Set("DriveMode", WrapArray(env, t.DriveMode, 5));
  o.Set("ColorSpace", t.ColorSpace);
  o.Set("AFFineTune", t.AFFineTune);
  o.Set("AFFineTuneAdj", WrapArray(env, t.AFFineTuneAdj, 3));
  o.Set("CameraType2", t.CameraType2);

  return o;
}

Napi::Object WrapSonyInfo(Napi::Env* env, libraw_sony_info_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("CameraType", t.CameraType);
  o.Set("Sony0x9400_version", t.Sony0x9400_version);
  o.Set("Sony0x9400_ReleaseMode2", t.Sony0x9400_ReleaseMode2);
  o.Set("Sony0x9400_SequenceImageNumber", t.Sony0x9400_SequenceImageNumber);
  o.Set("Sony0x9400_SequenceLength1", t.Sony0x9400_SequenceLength1);
  o.Set("Sony0x9400_SequenceFileNumber", t.Sony0x9400_SequenceFileNumber);
  o.Set("Sony0x9400_SequenceLength2", t.Sony0x9400_SequenceLength2);
  o.Set("AFAreaModeSetting", t.AFAreaModeSetting);
  o.Set("FlexibleSpotPosition", WrapArray(env, t.FlexibleSpotPosition, 2));
  o.Set("AFPointSelected", t.AFPointSelected);
  o.Set("AFPointsUsed", WrapArray(env, t.AFPointsUsed, 10));
  o.Set("AFTracking", t.AFTracking);
  o.Set("AFType", t.AFType);
  o.Set("FocusLocation", WrapArray(env, t.FocusLocation, 4));
  o.Set("AFMicroAdjValue", t.AFMicroAdjValue);
  o.Set("AFMicroAdjOn", t.AFMicroAdjOn);
  o.Set("AFMicroAdjRegisteredLenses", t.AFMicroAdjRegisteredLenses);
  o.Set("VariableLowPassFilter", t.VariableLowPassFilter);
  o.Set("LongExposureNoiseReduction", t.LongExposureNoiseReduction);
  o.Set("HighISONoiseReduction", t.HighISONoiseReduction);
  o.Set("HDR", WrapArray(env, t.HDR, 2));
  o.Set("group2010", t.group2010);
  o.Set("real_iso_offset", t.real_iso_offset);
  o.Set("MeteringMode_offset", t.MeteringMode_offset);
  o.Set("ExposureProgram_offset", t.ExposureProgram_offset);
  o.Set("ReleaseMode2_offset", t.ReleaseMode2_offset);
  o.Set("MinoltaCamID", t.MinoltaCamID);
  o.Set("firmware", convertFloat(t.firmware));
  o.Set("ImageCount3_offset", t.ImageCount3_offset);
  o.Set("ImageCount3", t.ImageCount3);
  o.Set("ElectronicFrontCurtainShutter", t.ElectronicFrontCurtainShutter);
  o.Set("MeteringMode2", t.MeteringMode2);
  o.Set("SonyDateTime", WrapArray(env, t.SonyDateTime, 20));
  o.Set("ShotNumberSincePowerUp", t.ShotNumberSincePowerUp);
  o.Set("PixelShiftGroupPrefix", t.PixelShiftGroupPrefix);
  o.Set("PixelShiftGroupID", t.PixelShiftGroupID);
  o.Set("nShotsInPixelShiftGroup", t.nShotsInPixelShiftGroup);
  o.Set("numInPixelShiftGroup", t.numInPixelShiftGroup);
  o.Set("prd_ImageHeight", t.prd_ImageHeight);
  o.Set("prd_ImageWidth", t.prd_ImageWidth);
  o.Set("prd_RawBitDepth", t.prd_RawBitDepth);
  o.Set("prd_StorageMethod", t.prd_StorageMethod);
  o.Set("prd_BayerPattern", t.prd_BayerPattern);
  o.Set("SonyRawFileType", t.SonyRawFileType);
  o.Set("RAWFileType", t.RAWFileType);
  o.Set("Quality", t.Quality);
  o.Set("FileFormat", t.FileFormat);

  return o;
}

Napi::Object WrapKodakMakernotes(Napi::Env* env, libraw_kodak_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("BlackLevelTop", t.BlackLevelTop);
  o.Set("BlackLevelBottom", t.BlackLevelBottom);
  o.Set("offset_left", t.offset_left);
  o.Set("offset_top", t.offset_top);
  o.Set("clipBlack", t.clipBlack);
  o.Set("clipWhite", t.clipWhite);

  std::size_t arraySize = 3;
  Napi::Array romm_camDaylight = Napi::Array::New(*env, arraySize);
  Napi::Array romm_camTungsten = Napi::Array::New(*env, arraySize);
  Napi::Array romm_camFluorescent = Napi::Array::New(*env, arraySize);
  Napi::Array romm_camFlash = Napi::Array::New(*env, arraySize);
  Napi::Array romm_camCustom = Napi::Array::New(*env, arraySize);
  Napi::Array romm_camAuto = Napi::Array::New(*env, arraySize);

  for (std::size_t i = 0; i < arraySize; i++)
  {
    romm_camDaylight[i] = MapFloatArrayToDouble(env, t.romm_camDaylight[i], arraySize);
    romm_camTungsten[i] = MapFloatArrayToDouble(env, t.romm_camTungsten[i], arraySize);
    romm_camFluorescent[i] = MapFloatArrayToDouble(env, t.romm_camFluorescent[i], arraySize);
    romm_camFlash[i] = MapFloatArrayToDouble(env, t.romm_camFlash[i], arraySize);
    romm_camCustom[i] = MapFloatArrayToDouble(env, t.romm_camCustom[i], arraySize);
    romm_camAuto[i] = MapFloatArrayToDouble(env, t.romm_camAuto[i], arraySize);
  }

  o.Set("romm_camDaylight", romm_camDaylight);
  o.Set("romm_camTungsten", romm_camTungsten);
  o.Set("romm_camFluorescent", romm_camFluorescent);
  o.Set("romm_camFlash", romm_camFlash);
  o.Set("romm_camCustom", romm_camCustom);
  o.Set("romm_camAuto", romm_camAuto);
  o.Set("val018percent", t.val018percent);
  o.Set("val100percent", t.val100percent);
  o.Set("val170percent", t.val170percent);
  o.Set("MakerNoteKodak8a", t.MakerNoteKodak8a);
  o.Set("ISOCalibrationGain", convertFloat(t.ISOCalibrationGain));
  o.Set("AnalogISO", convertFloat(t.AnalogISO));

  return o;
}

Napi::Object WrapPanasonicMakernotes(Napi::Env* env, libraw_panasonic_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("Compression", t.Compression);
  o.Set("BlackLevelDim", t.BlackLevelDim);
  o.Set("BlackLevel", MapFloatArrayToDouble(env, t.BlackLevel, 8));
  o.Set("Multishot", t.Multishot);
  o.Set("gamma", convertFloat(t.gamma));
  o.Set("HighISOMultiplier", WrapArray(env, t.HighISOMultiplier, 3));

  return o;
}

Napi::Object WrapPentaxMakernotes(Napi::Env* env, libraw_pentax_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("FocusMode", t.FocusMode);
  o.Set("AFPointSelected", t.AFPointSelected);
  o.Set("AFPointsInFocus", t.AFPointsInFocus);
  o.Set("FocusPosition", t.FocusPosition);
  o.Set("DriveMode", WrapArray(env, t.DriveMode, 4));
  o.Set("AFAdjustment", t.AFAdjustment);
  o.Set("MultiExposure", t.MultiExposure);
  o.Set("Quality", t.Quality);

  return o;
}

Napi::Object WrapPhaseoneMakernotes(Napi::Env* env, libraw_p1_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("Software", t.Software);
  o.Set("SystemType", t.SystemType);
  o.Set("FirmwareString", t.FirmwareString);
  o.Set("SystemModel", t.SystemModel);

  return o;
}

Napi::Object WrapSamsungMakernotes(Napi::Env* env, libraw_samsung_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("ImageSizeFull", WrapArray(env, t.ImageSizeFull, 4));
  o.Set("ImageSizeCrop", WrapArray(env, t.ImageSizeCrop, 4));
  o.Set("ColorSpace", WrapArray(env, t.ColorSpace, 2));
  o.Set("key", WrapArray(env, t.key, 11));
  o.Set("DigitalGain", t.DigitalGain);
  o.Set("DeviceType", t.DeviceType);
  o.Set("LensFirmware", t.LensFirmware);

  return o;
}

Napi::Object WrapCommonMakernotes(Napi::Env* env, libraw_metadata_common_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("FlashEC", convertFloat(t.FlashEC));
  o.Set("FlashGN", convertFloat(t.FlashGN));
  o.Set("CameraTemperature", convertFloat(t.CameraTemperature));
  o.Set("SensorTemperature", convertFloat(t.SensorTemperature));
  o.Set("SensorTemperature2", convertFloat(t.SensorTemperature2));
  o.Set("LensTemperature", convertFloat(t.LensTemperature));
  o.Set("AmbientTemperature", convertFloat(t.AmbientTemperature));
  o.Set("BatteryTemperature", convertFloat(t.BatteryTemperature));
  o.Set("exifAmbientTemperature", convertFloat(t.exifAmbientTemperature));
  o.Set("exifHumidity", convertFloat(t.exifHumidity));
  o.Set("exifPressure", convertFloat(t.exifPressure));
  o.Set("exifWaterDepth", convertFloat(t.exifWaterDepth));
  o.Set("exifAcceleration", convertFloat(t.exifAcceleration));
  o.Set("exifCameraElevationAngle", convertFloat(t.exifCameraElevationAngle));
  o.Set("real_ISO", convertFloat(t.real_ISO));
  o.Set("exifExposureIndex", convertFloat(t.exifExposureIndex));
  o.Set("ColorSpace", t.ColorSpace);
  o.Set("firmware", t.firmware);

  return o;
}

Napi::Object WrapMakernotes(Napi::Env* env, libraw_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("canon", WrapCanonMakernotes(env, t.canon));
  o.Set("nikon", WrapNikonMakernotes(env, t.nikon));
  o.Set("hasselblad", WrapHasselbladMakernotes(env, t.hasselblad));
  o.Set("fuji", WrapFujiInfo(env, t.fuji));
  o.Set("olympus", WrapOlympusMakernotes(env, t.olympus));
  o.Set("sony", WrapSonyInfo(env, t.sony));
  o.Set("kodak", WrapKodakMakernotes(env, t.kodak));
  o.Set("panasonic", WrapPanasonicMakernotes(env, t.panasonic));
  o.Set("pentax", WrapPentaxMakernotes(env, t.pentax));
  o.Set("phaseone", WrapPhaseoneMakernotes(env, t.phaseone));
  o.Set("samsung", WrapSamsungMakernotes(env, t.samsung));
  o.Set("common", WrapCommonMakernotes(env, t.common));

  return o;
}

Napi::Object WrapShootinginfo(Napi::Env* env, libraw_shootinginfo_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("DriveMode", t.DriveMode);
  o.Set("FocusMode", t.FocusMode);
  o.Set("MeteringMode", t.MeteringMode);
  o.Set("AFPoint", t.AFPoint);
  o.Set("ExposureMode", t.ExposureMode);
  o.Set("ExposureProgram", t.ExposureProgram);
  o.Set("ImageStabilization", t.ImageStabilization);
  o.Set("BodySerial", t.BodySerial);
  o.Set("InternalBodySerial", t.InternalBodySerial);

  return o;
}

Napi::Object WrapOutputParams(Napi::Env* env, libraw_output_params_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("greybox", WrapArray(env, t.greybox, 4));
  o.Set("cropbox", WrapArray(env, t.cropbox, 4));
  o.Set("aber", WrapArray(env, t.aber, 4));
  o.Set("gamm", WrapArray(env, t.gamm, 6));
  o.Set("user_mul", MapFloatArrayToDouble(env, t.user_mul, 4));
  o.Set("shot_select", t.shot_select);
  o.Set("bright", convertFloat(t.bright));
  o.Set("threshold", convertFloat(t.threshold));
  o.Set("half_size", t.half_size);
  o.Set("four_color_rgb", t.four_color_rgb);
  o.Set("highlight", t.highlight);
  o.Set("use_auto_wb", t.use_auto_wb);
  o.Set("use_camera_wb", t.use_camera_wb);
  o.Set("use_camera_matrix", t.use_camera_matrix);
  o.Set("output_color", t.output_color);
  o.Set("output_bps", t.output_bps);
  o.Set("output_tiff", t.output_tiff);
  o.Set("user_flip", t.user_flip);
  o.Set("user_qual", t.user_qual);
  o.Set("user_black", t.user_black);
  o.Set("user_cblack", WrapArray(env, t.user_cblack, 4));
  o.Set("user_sat", t.user_sat);
  o.Set("med_passes", t.med_passes);
  o.Set("auto_bright_thr", convertFloat(t.auto_bright_thr));
  o.Set("adjust_maximum_thr", convertFloat(t.adjust_maximum_thr));
  o.Set("no_auto_bright", t.no_auto_bright);
  o.Set("use_fuji_rotate", t.use_fuji_rotate);
  o.Set("green_matching", t.green_matching);
  o.Set("dcb_iterations", t.dcb_iterations);
  o.Set("dcb_enhance_fl", t.dcb_enhance_fl);
  o.Set("fbdd_noiserd", t.fbdd_noiserd);
  o.Set("exp_correc", t.exp_correc);
  o.Set("exp_shift", convertFloat(t.exp_shift));
  o.Set("exp_preser", convertFloat(t.exp_preser));
  o.Set("use_rawspeed", t.use_rawspeed);
  o.Set("use_dngsdk", t.use_dngsdk);
  o.Set("no_auto_scale", t.no_auto_scale);
  o.Set("no_interpolation", t.no_interpolation);
  o.Set("raw_processing_options", t.raw_processing_options);
  o.Set("sony_arw2_posterization_thr", t.sony_arw2_posterization_thr);
  o.Set("coolscan_nef_gamma", convertFloat(t.coolscan_nef_gamma));
  o.Set("p4shot_order", t.p4shot_order);

  return o;
}

Napi::Object WrapInternalOutputParams(Napi::Env* env, libraw_internal_output_params_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("mix_green", t.mix_green);
  o.Set("raw_color", t.raw_color);
  o.Set("zero_is_bad", t.zero_is_bad);
  o.Set("shrink", t.shrink);
  o.Set("fuji_width", t.fuji_width);

  return o;
}

Napi::Object WrapP1Color(Napi::Env* env, libraw_P1_color_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("romm_cam", MapFloatArrayToDouble(env, t.romm_cam, 9));

  return o;
}

Napi::Object WrapDngLevels(Napi::Env* env, libraw_dng_levels_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("parsedfields", t.parsedfields);
  o.Set("dng_cblack", WrapArray(env, t.dng_cblack, LIBRAW_CBLACK_SIZE));
  o.Set("dng_black", t.dng_black);
  o.Set("dng_fcblack", WrapArray(env, t.dng_fcblack, LIBRAW_CBLACK_SIZE));
  o.Set("dng_fblack", t.dng_fblack);
  o.Set("dng_whitelevel", WrapArray(env, t.dng_whitelevel, 4));
  o.Set("default_crop", WrapArray(env, t.default_crop, 4));
  o.Set("preview_colorspace", t.preview_colorspace);
  o.Set("analogbalance", MapFloatArrayToDouble(env, t.analogbalance, 4));
  o.Set("asshotneutral", WrapArray(env, t.asshotneutral, 4));
  o.Set("baseline_exposure", convertFloat(t.baseline_exposure));
  o.Set("LinearResponseLimit", t.LinearResponseLimit);

  return o;
}

Napi::Object WrapDngColor(Napi::Env* env, libraw_dng_color_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("parsedfields", t.parsedfields);
  o.Set("illuminant", t.illuminant);
  Napi::Array calibration = Napi::Array::New(*env, 4);
  for (int i = 0; i < 4; i++)
  {
    calibration[i] = MapFloatArrayToDouble(env, t.calibration[i], 4);
  }
  o.Set("calibration", calibration);
  Napi::Array colormatrix = Napi::Array::New(*env, 4);
  for (int i = 0; i < 4; i++)
  {
    colormatrix[i] = MapFloatArrayToDouble(env, t.colormatrix[i], 3);
  }
  o.Set("colormatrix", colormatrix);
  Napi::Array forwardmatrix = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    forwardmatrix[i] = MapFloatArrayToDouble(env, t.forwardmatrix[i], 4);
  }
  o.Set("forwardmatrix", forwardmatrix);

  return o;
}

Napi::Object WrapPh1(Napi::Env* env, ph1_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("format", t.format);
  o.Set("key_off", t.key_off);
  o.Set("tag_21a", t.tag_21a);
  o.Set("t_black", t.t_black);
  o.Set("split_col", t.split_col);
  o.Set("black_col", t.black_col);
  o.Set("split_row", t.split_row);
  o.Set("black_row", t.black_row);
  o.Set("tag_210", convertFloat(t.tag_210));

  return o;
}

Napi::Object WrapColordata(Napi::Env* env, libraw_colordata_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("curve", WrapArray(env, t.curve, 0x10000));
  o.Set("cblack", WrapArray(env, t.cblack, LIBRAW_CBLACK_SIZE));
  o.Set("black", t.black);
  o.Set("data_maximum", t.data_maximum);
  o.Set("maximum", t.maximum);
  o.Set("linear_max", WrapArray(env, t.linear_max, 4));
  o.Set("fmaximum", convertFloat(t.fmaximum));
  o.Set("fnorm", convertFloat(t.fnorm));
  Napi::Array white = Napi::Array::New(*env, 8);
  for (int i = 0; i < 8; i++)
  {
    white[i] = WrapArray(env, t.white[i], 8);
  }
  o.Set("white", white);
  o.Set("cam_mul", MapFloatArrayToDouble(env, t.cam_mul, 4));
  o.Set("pre_mul", MapFloatArrayToDouble(env, t.pre_mul, 4));
  Napi::Array cmatrix = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    cmatrix[i] = MapFloatArrayToDouble(env, t.cmatrix[i], 4);
  }
  o.Set("cmatrix", cmatrix);
  Napi::Array ccm = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    ccm[i] = MapFloatArrayToDouble(env, t.ccm[i], 4);
  }
  o.Set("ccm", ccm);
  Napi::Array rgb_cam = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    rgb_cam[i] = MapFloatArrayToDouble(env, t.rgb_cam[i], 4);
  }
  o.Set("rgb_cam", rgb_cam);
  Napi::Array cam_xyz = Napi::Array::New(*env, 4);
  for (int i = 0; i < 4; i++)
  {
    cam_xyz[i] = MapFloatArrayToDouble(env, t.cam_xyz[i], 3);
  }
  o.Set("cam_xyz", cam_xyz);
  o.Set("phase_one_data", WrapPh1(env, t.phase_one_data));
  o.Set("flash_used", convertFloat(t.flash_used));
  o.Set("canon_ev", convertFloat(t.canon_ev));
  o.Set("model2", t.model2);
  o.Set("UniqueCameraModel", t.UniqueCameraModel);
  o.Set("LocalizedCameraModel", t.LocalizedCameraModel);
  o.Set("ImageUniqueID", t.ImageUniqueID);
  o.Set("RawDataUniqueID", t.RawDataUniqueID);
  o.Set("OriginalRawFileName", t.OriginalRawFileName);
  if (t.profile_length)
  {
    o.Set("profile", Napi::Buffer<char>::New(*env, (char *)t.profile, (std::size_t)t.profile_length));
  }
  o.Set("profile_length", t.profile_length);
  o.Set("black_stat", WrapArray(env, t.black_stat, 8));
  Napi::Array dng_color = Napi::Array::New(*env, 2);
  for (int i = 0; i < 2; i++)
  {
    dng_color[i] = WrapDngColor(env, t.dng_color[i]);
  }
  o.Set("dng_color", dng_color);
  o.Set("dng_levels", WrapDngLevels(env, t.dng_levels));
  Napi::Array WB_Coeffs = Napi::Array::New(*env, 256);
  for (int i = 0; i < 256; i++)
  {
    WB_Coeffs[i] = WrapArray(env, t.WB_Coeffs[i], 4);
  }
  o.Set("WB_Coeffs", WB_Coeffs);
  Napi::Array WBCT_Coeffs = Napi::Array::New(*env, 64);
  for (int i = 0; i < 64; i++)
  {
    WBCT_Coeffs[i] = MapFloatArrayToDouble(env, t.WBCT_Coeffs[i], 5);
  }
  o.Set("WBCT_Coeffs", WBCT_Coeffs);
  o.Set("as_shot_wb_applied", t.as_shot_wb_applied);
  Napi::Array P1_color = Napi::Array::New(*env, 2);
  for (int i = 0; i < 2; i++)
  {
    P1_color[i] = WrapP1Color(env, t.P1_color[i]);
  }
  o.Set("P1_color", P1_color);
  o.Set("raw_bps", t.raw_bps);
  o.Set("ExifColorSpace", t.ExifColorSpace);

  return o;
}

Napi::Object WrapGpsInfo(Napi::Env* env, libraw_gps_info_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("latitude", MapFloatArrayToDouble(env, t.latitude, 3));
  o.Set("longitude", MapFloatArrayToDouble(env, t.longitude, 3));
  o.Set("gpstimestamp", MapFloatArrayToDouble(env, t.gpstimestamp, 3));
  o.Set("altitude", convertFloat(t.altitude));
  o.Set("altref", t.altref);
  o.Set("latref", t.latref);
  o.Set("longref", t.longref);
  o.Set("gpsstatus", t.gpsstatus);
  o.Set("gpsparsed", t.gpsparsed);

  return o;
}

Napi::Object WrapImgother(Napi::Env* env, libraw_imgother_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("iso_speed", convertFloat(t.iso_speed));
  o.Set("shutter",  convertFloat(t.shutter));
  o.Set("aperture", convertFloat(t.aperture));
  o.Set("focal_len", convertFloat(t.focal_len));
  o.Set("timestamp", t.timestamp);
  o.Set("shot_order", t.shot_order);
  o.Set("gpsdata", WrapArray(env, t.gpsdata, 32));
  o.Set("parsed_gps", WrapGpsInfo(env, t.parsed_gps));
  o.Set("desc", t.desc);
  o.Set("artist", t.artist);
  o.Set("analogbalance", MapFloatArrayToDouble(env, t.analogbalance, 4));

  return o;
}

Napi::Value WrapThumbnail(Napi::Env* env, libraw_thumbnail_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("twidth", t.twidth);
  o.Set("theight", t.theight);
  o.Set("tlength", t.tlength);
  o.Set("tcolors", t.tcolors);

  return o;
}

Napi::Value WrapRawData(Napi::Env* env, libraw_rawdata_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("iparams", Wrapidata(env, t.iparams));
  o.Set("sizes", WrapImageSizes(env, t.sizes));
  o.Set("ioparams", WrapInternalOutputParams(env, t.ioparams));
  o.Set("color", WrapColordata(env, t.color));

  return o;
}

Napi::Value WrapLibRawData(Napi::Env* env, libraw_data_t* data)
{
  Napi::Object wrapper = Napi::Object::New(*env);

  wrapper.Set("sizes", WrapImageSizes(env, data->sizes));
  wrapper.Set("idata", Wrapidata(env, data->idata));
  wrapper.Set("lens", WrapLensInfo(env, data->lens));
  wrapper.Set("makernotes", WrapMakernotes(env, data->makernotes));
  wrapper.Set("shootinginfo", WrapShootinginfo(env, data->shootinginfo));
  wrapper.Set("params", WrapOutputParams(env, data->params));
  wrapper.Set("progress_flags", data->progress_flags);
  wrapper.Set("process_warnings", data->process_warnings);
  wrapper.Set("color", WrapColordata(env, data->color));
  wrapper.Set("other", WrapImgother(env, data->other));
  wrapper.Set("thumbnail", WrapThumbnail(env, data->thumbnail));
  wrapper.Set("rawdata", WrapRawData(env, data->rawdata));

  return wrapper;
}
