#include <napi.h>
#include "libraw/libraw.h"
#include "wraptypes.h"

Napi::Object Wrapidata(Napi::Env* env, libraw_iparams_t iparams)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("guard", iparams.guard);
  o.Set("make", iparams.make);
  o.Set("model", iparams.model);
  o.Set("software", iparams.software);
  o.Set("dng_version", iparams.dng_version);
  o.Set("is_foveon", iparams.is_foveon);
  o.Set("colors", iparams.colors);
  o.Set("filters", iparams.filters);
  o.Set("cdesc", iparams.cdesc);
  o.Set("xmplen", iparams.xmplen);

  return o;
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


  return o;
}

Napi::Object WrapDngLens(Napi::Env* env, libraw_dnglens_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("MinFocal", t.MinFocal);
  o.Set("MaxFocal", t.MaxFocal);
  o.Set("MaxAp4MinFocal", t.MaxAp4MinFocal);
  o.Set("MaxAp4MaxFocal", t.MaxAp4MaxFocal);

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
  o.Set("MinFocal", t.MinFocal);
  o.Set("MaxFocal", t.MaxFocal);
  o.Set("MaxAp4MinFocal", t.MaxAp4MinFocal);
  o.Set("MaxAp4MaxFocal", t.MaxAp4MaxFocal);
  o.Set("MinAp4MinFocal", t.MinAp4MinFocal);
  o.Set("MinAp4MaxFocal", t.MinAp4MaxFocal);
  o.Set("MaxAp", t.MaxAp);
  o.Set("MinAp", t.MinAp);
  o.Set("CurFocal", t.CurFocal);
  o.Set("CurAp", t.CurAp);
  o.Set("MaxAp4CurFocal", t.MaxAp4CurFocal);
  o.Set("MinAp4CurFocal", t.MinAp4CurFocal);
  o.Set("MinFocusDistance", t.MinFocusDistance);
  o.Set("FocusRangeIndex", t.FocusRangeIndex);
  o.Set("LensFStops", t.LensFStops);
  o.Set("TeleconverterID", t.TeleconverterID);
  o.Set("Teleconverter", t.Teleconverter);
  o.Set("AdapterID", t.AdapterID);
  o.Set("Adapter", t.Adapter);
  o.Set("AttachmentID", t.AttachmentID);
  o.Set("Attachment", t.Attachment);
  // o.Set("FocalUnits", t.FocalUnits);
  o.Set("FocalLengthIn35mmFormat", t.FocalLengthIn35mmFormat);

  return o;
}

Napi::Object WrapLensInfo(Napi::Env* env, libraw_lensinfo_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("MinFocal", t.MinFocal);
  o.Set("MaxFocal", t.MaxFocal);
  o.Set("MaxAp4MinFocal", t.MaxAp4MinFocal);
  o.Set("MaxAp4MaxFocal", t.MaxAp4MaxFocal);
  o.Set("EXIF_MaxAp", t.EXIF_MaxAp);
  o.Set("LensMake", t.LensMake);
  o.Set("Lens", t.Lens);
  o.Set("LensSerial", t.LensSerial);
  o.Set("InternalLensSerial", t.InternalLensSerial);
  o.Set("FocalLengthIn35mmFormat", t.FocalLengthIn35mmFormat);
  o.Set("dng", WrapDngLens(env, t.dng));
  o.Set("makernotes", WrapMakernotesLens(env, t.makernotes));

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
  o.Set("FlashSetting", t.FlashSetting);
  o.Set("FlashType", t.FlashType);
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
  // o.Set("ExposureProgram", t.ExposureProgram);
  o.Set("nMEshots", t.nMEshots);
  o.Set("MEgainOn", t.MEgainOn);
  o.Set("ME_WB", WrapArray(env, t.ME_WB, 4));
  o.Set("AFFineTune", t.AFFineTune);
  o.Set("AFFineTuneIndex", t.AFFineTuneIndex);
  o.Set("AFFineTuneAdj", t.AFFineTuneAdj);
  // these fields are defined in LibRaw's types but compiler can't seem to reference them.
  // o.Set("LensDataVersion", t.LensDataVersion);
  // o.Set("FlashInfoVersion", t.FlashInfoVersion);
  // o.Set("ColorBalanceVersion", t.ColorBalanceVersion);
  // o.Set("key", t.key);
  // o.Set("NEFBitDepth", WrapArray(env, t.NEFBitDepth, 4));
  // o.Set("HighSpeedCropFormat", t.HighSpeecCropFormat);
  // o.Set("ttttt", t.SensorHighSpeedCrop);
  // o.Set("SensorWidth", t.SensorWidth);
  // o.Set("SensorHeight", t.SensorHeight);

  return o;
}

Napi::Object WrapMakernotes(Napi::Env* env, libraw_makernotes_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  // o.Set("canon", WrapCanonMakernotes(env, t.canon));
  // o.Set("nikon", WrapNikonMakernotes(env, t.nikon));
  return o;
}

Napi::Object WrapShootinginfo(Napi::Env* env, libraw_shootinginfo_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("DriverMode", t.DriveMode);
  o.Set("FocusMode", t.FocusMode);
  o.Set("MeteringMode", t.MeteringMode);
  o.Set("AFPoint", t.AFPoint);
  o.Set("ExposureMode", t.ExposureMode);
  // o.Set("ExposureProgram", t.ExposureProgram);
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
  // o.Set("user_mul", WrapArray(env, t.usr_mul, 4));
  o.Set("shot_select", t.shot_select);
  o.Set("bright", t.bright);
  o.Set("threshold", t.threshold);
  o.Set("half_size", t.half_size);
  o.Set("four_color_rgb", t.four_color_rgb);
  o.Set("highlight", t.highlight);
  o.Set("use_auto_wb", t.use_auto_wb);
  o.Set("use_camera_wb", t.use_camera_wb);
  o.Set("use_camera_matrix", t.use_camera_matrix);
  o.Set("output_color", t.output_color);

  // o.Set("output_profile", t.output_profile);

  o.Set("output_bps", t.output_bps);
  o.Set("output_tiff", t.output_tiff);
  o.Set("user_flip", t.user_flip);
  o.Set("user_qual", t.user_qual);
  o.Set("user_black", t.user_black);
  o.Set("user_cblack", WrapArray(env, t.user_cblack, 4));
  o.Set("user_sat", t.user_sat);
  o.Set("med_passes", t.med_passes);
  o.Set("auto_bright_thr", t.auto_bright_thr);
  o.Set("adjust_maximum_thr", t.adjust_maximum_thr);
  o.Set("no_auto_bright", t.no_auto_bright);
  o.Set("use_fuji_rotate", t.use_fuji_rotate);
  o.Set("green_matching", t.green_matching);
  o.Set("dcb_iterations", t.dcb_iterations);
  o.Set("dcb_enhance_fl", t.dcb_enhance_fl);
  o.Set("fbdd_noiserd", t.fbdd_noiserd);
  o.Set("exp_correc", t.exp_correc);
  o.Set("exp_shift", t.exp_shift);
  o.Set("exp_preser", t.exp_preser);
  o.Set("use_rawspeed", t.use_rawspeed);
  o.Set("use_dngsdk", t.use_dngsdk);
  o.Set("no_auto_scale", t.no_auto_scale);
  o.Set('no_interpolation', t.no_interpolation);
  o.Set('raw_processing_options', t.raw_processing_options);
  // o.Set('max_raw_memory_mb', t.max_raw_memory_mb);
  o.Set('sony_arw2_posterization_thr', t.sony_arw2_posterization_thr);
  o.Set('coolscan_nef_gamma', t.coolscan_nef_gamma);
  // TODO incompleete

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

  o.Set("romm_cam", WrapArray(env, t.romm_cam, 9));

  return o;
}

Napi::Object WrapDngLevels(Napi::Env* env, libraw_dng_levels_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("parsedfields", t.parsedfields);
  // o.Set("dng_cblack", t.dng_cblack);
  o.Set("dng_black", t.dng_black);
  // o.Set("dng_fblack", t.dng_fblack);
  o.Set("dng_whitelevel", WrapArray(env, t.dng_whitelevel, 4));
  o.Set("default_crop", WrapArray(env, t.default_crop, 4));
  o.Set("preview_colorspace", t.preview_colorspace);
  o.Set("analogbalance", WrapArray(env, t.analogbalance, 4));
  // o.Set("asshotneutral", WrapArray(env, t.asshotneutral, 4));
  // o.Set("baseline_exposure", t.baseline_exposure);
  // o.Set("LinearResponseLimit", t.LinearResponseLimit);

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
    calibration[i] = WrapArray(env, t.calibration[i], 4);
  }
  o.Set("calibration", calibration);
  Napi::Array colormatrix = Napi::Array::New(*env, 4);
  for (int i = 0; i < 4; i++)
  {
    colormatrix[i] = WrapArray(env, t.colormatrix[i], 3);
  }
  o.Set("colormatrix", colormatrix);
  Napi::Array forwardmatrix = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    forwardmatrix[i] = WrapArray(env, t.forwardmatrix[i], 4);
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
  o.Set("tag_210", t.tag_210);

  return o;
}

Napi::Object WrapColordata(Napi::Env* env, libraw_colordata_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  // o.Set("curve", WrapArray(env, t.curve, 0x10000));
  o.Set("black", t.black);
  o.Set("data_maximum", t.data_maximum);
  o.Set("maximum", t.maximum);
  o.Set("linear_max", WrapArray(env, t.linear_max, 4));
  o.Set("fmaximum", t.fmaximum);
  o.Set("fnorm", t.fnorm);
  Napi::Array white = Napi::Array::New(*env, 8);
  for (int i = 0; i < 8; i++)
  {
    white[i] = WrapArray(env, t.white[i], 8);
  }
  o.Set("white", white);
  o.Set("cam_mul", WrapArray(env, t.cam_mul, 4));
  o.Set("pre_mul", WrapArray(env, t.pre_mul, 4));
  Napi::Array cmatrix = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    cmatrix[i] = WrapArray(env, t.cmatrix[i], 4);
  }
  o.Set("cmatrix", cmatrix);
  Napi::Array ccm = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    ccm[i] = WrapArray(env, t.ccm[i], 4);
  }
  o.Set("ccm", ccm);
  Napi::Array rgb_cam = Napi::Array::New(*env, 3);
  for (int i = 0; i < 3; i++)
  {
    rgb_cam[i] = WrapArray(env, t.rgb_cam[i], 4);
  }
  o.Set("rgb_cam", rgb_cam);
  Napi::Array cam_xyz = Napi::Array::New(*env, 4);
  for (int i = 0; i < 4; i++)
  {
    cam_xyz[i] = WrapArray(env, t.cam_xyz[i], 3);
  }
  o.Set("cam_xyz", cam_xyz);
  o.Set("phase_one_data", WrapPh1(env, t.phase_one_data));
  o.Set("flash_used", t.flash_used);
  o.Set("canon_ev", t.canon_ev);
  o.Set("model2", t.model2);
  o.Set("UniqueCameraModel", t.UniqueCameraModel);
  o.Set("LocalizedCameraModel", t.LocalizedCameraModel);
  // TODO profile
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
    WBCT_Coeffs[i] = WrapArray(env, t.WBCT_Coeffs[i], 5);
  }
  o.Set("WBCT_Coeffs", WBCT_Coeffs);
  Napi::Array P1_color = Napi::Array::New(*env, 2);
  for (int i = 0; i < 2; i++)
  {
    P1_color[i] = WrapP1Color(env, t.P1_color[i]);
  }
  o.Set("P1_color", P1_color);
  // not found on current version type
  // o.Set("as_shot_wb_applied", t.as_shot_wb_applied);
  // o.Set("raw_bps", t.raw_bps);
  // o.Set("ExifColorSpace", t.ExifColorSpace);

  return o;
}

Napi::Object WrapGpsInfo(Napi::Env* env, libraw_gps_info_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  o.Set("latitude", WrapArray(env, t.latitude, 3));
  o.Set("longtitude", WrapArray(env, t.longtitude, 3));
  o.Set("gpstimestamp", WrapArray(env, t.gpstimestamp, 3));
  o.Set("altitude", t.altitude);
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

  o.Set("iso_speed", t.iso_speed);
  o.Set("shutter", t.shutter);
  o.Set("aperture", t.aperture);
  o.Set("focal_len", t.focal_len);
  o.Set("timestamp", t.timestamp);
  o.Set("shot_order", t.shot_order);
  o.Set("gpsdata", WrapArray(env, t.gpsdata, 32));
  o.Set("parsed_gps", WrapGpsInfo(env, t.parsed_gps));
  o.Set("desc", t.desc);
  o.Set("artist", t.artist);
  // o.Set("analogbalance", WrapArray(env, t.analogbalance, 4));

  return o;
}

Napi::Object WrapRawdata(Napi::Env* env, libraw_rawdata_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  // TODO: support raw_image and other missing fields

  o.Set("iparams", Wrapidata(env, t.iparams));
  o.Set("sizes", WrapImageSizes(env, t.sizes));
  o.Set("ioparams", WrapInternalOutputParams(env, t.ioparams));
  o.Set("color", WrapColordata(env, t.color));

  return o;
}

Napi::Value WrapThumbnail(Napi::Env* env, libraw_thumbnail_t t)
{
  Napi::Object o = Napi::Object::New(*env);

  // o.Set("tformat", t.tformat);
  o.Set("twidth", t.twidth);
  o.Set("theight", t.theight);
  o.Set("tlength", t.tlength);
  o.Set("tcolors", t.tcolors);

  return o;
}

Napi::Value WrapLibRawData(Napi::Env* env, libraw_data_t* data)
{
  Napi::Object wrapper = Napi::Object::New(*env);
  wrapper.Set("color", WrapColordata(env, data->color));
  wrapper.Set("sizes", WrapImageSizes(env, data->sizes));
  wrapper.Set("idata", Wrapidata(env, data->idata));
  wrapper.Set("lens", WrapLensInfo(env, data->lens));
  wrapper.Set("makernotes", WrapMakernotes(env, data->makernotes));
  wrapper.Set("shootinginfo", WrapShootinginfo(env, data->shootinginfo));
  wrapper.Set("params", WrapOutputParams(env, data->params));
  wrapper.Set("progress_flags", data->progress_flags);
  wrapper.Set("process_warnings", data->process_warnings);
  wrapper.Set("other", WrapImgother(env, data->other));
  wrapper.Set("thumbnail", WrapThumbnail(env, data->thumbnail));

  return wrapper;
}
