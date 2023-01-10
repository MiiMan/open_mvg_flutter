import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'open_mvg_flutter_java_platform_interface.dart';

/// An implementation of [OpenMvgFlutterJavaPlatform] that uses method channels.
class MethodChannelOpenMvgFlutterJava extends OpenMvgFlutterJavaPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('open_mvg_flutter_java');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('stringFromJNI');
    return version;
  }

  @override
  Future<String?> stringFromJNI() async {
    final str = await methodChannel.invokeMethod<String>('stringFromJNI');
    return str;
  }

  @override
  Future<String?> imageList(String sImageDir, String sfileDatabase, String sOutputDir, String jfocal_pixels) async {

    final str = await methodChannel.invokeMethod<String>('imageList', <String, dynamic>{
      'sImageDir': sImageDir,
      'sfileDatabase': sfileDatabase,
      'sOutputDir': sOutputDir,
      'jfocal_pixels': jfocal_pixels
    });
    return str;
  }

  @override
  Future<String?> computeFeatures(String jsSfM_Data_Filename, String jsOutDir, String jbUpRight, String jsImage_Describer_Method, String jsFeaturePreset) async {

    final str = await methodChannel.invokeMethod<String>('computeFeatures', <String, dynamic>{
      'jsSfM_Data_Filename': jsSfM_Data_Filename,
      'jsOutDir': jsOutDir,
      'jbUpRight': jbUpRight,
      'jsImage_Describer_Method': jsImage_Describer_Method,
      'jsFeaturePreset': jsFeaturePreset
    });

    return str;
  }

  @override
  Future<String?> computeMatches(String jsSfM_Data_Filename, String jsMatchesDirectory, String jsGeometricModel, String jfDistRatio, String jsNearestMatchingMethod) async {

    final str = await methodChannel.invokeMethod<String>('computeMatches', <String, dynamic>{
      'jsSfM_Data_Filename': jsSfM_Data_Filename,
      'jsMatchesDirectory': jsMatchesDirectory,
      'jsGeometricModel': jsGeometricModel,
      'jfDistRatio': jfDistRatio,
      'jsNearestMatchingMethod': jsNearestMatchingMethod
    });

    return str;
  }

  @override
  Future<String?> incrementalReconstruct(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutDir, String jsIntrinsic_refinement_options, String jsSfMInitializer_method, String jcameraModel) async {

    final str = await methodChannel.invokeMethod<String>('incrementalReconstruct', <String, dynamic>{
      'jsSfM_Data_Filename': jsSfM_Data_Filename,
      'jsMatchesDir': jsMatchesDir,
      'jsOutDir': jsOutDir,
      'jsIntrinsic_refinement_options': jsIntrinsic_refinement_options,
      'jsSfMInitializer_method': jsSfMInitializer_method,
      'jcameraModel': jcameraModel
    });

    return str;
  }

  @override
  Future<String?> computeDataColor(String inData, String outData) async {

    final str = await methodChannel.invokeMethod<String>('computeDataColor', <String, dynamic>{
      'in': inData,
      'out': outData,
    });
    return str;
  }

  @override
  Future<String?> refinePoses(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutFile, String juseBundleAdjustment, String jbDirect_triangulation, String jsMatchFile) async {

    final str = await methodChannel.invokeMethod<String>('refinePoses', <String, dynamic>{
      'jsSfM_Data_Filename': jsSfM_Data_Filename,
      'jsMatchesDir': jsMatchesDir,
      'jsOutFile': jsOutFile,
      'juseBundleAdjustment': juseBundleAdjustment,
      'jbDirect_triangulation': jbDirect_triangulation,
      'jsMatchFile': jsMatchFile
    });

    return str;
  }
}
