
import 'open_mvg_flutter_java_platform_interface.dart';

class OpenMvgFlutterJava {
  Future<String?> getPlatformVersion() {
    return OpenMvgFlutterJavaPlatform.instance.getPlatformVersion();
  }

  @override
  Future<String?> stringFromJNI() {
    return OpenMvgFlutterJavaPlatform.instance.stringFromJNI();
  }

  @override
  Future<String?> imageList(String sImageDir, String sfileDatabase, String sOutputDir, String jfocal_pixels) {
    return OpenMvgFlutterJavaPlatform.instance.imageList(sImageDir, sfileDatabase, sOutputDir, jfocal_pixels);
  }

  @override
  Future<String?> computeFeatures(String jsSfM_Data_Filename, String jsOutDir, String jbUpRight, String jsImage_Describer_Method, String jsFeaturePreset) async {
    return OpenMvgFlutterJavaPlatform.instance.computeFeatures(jsSfM_Data_Filename, jsOutDir, jbUpRight, jsImage_Describer_Method, jsFeaturePreset);
  }

  @override
  Future<String?> computeMatches(String jsSfM_Data_Filename, String jsMatchesDirectory, String jsGeometricModel, String jfDistRatio, String jsNearestMatchingMethod) async {
    return OpenMvgFlutterJavaPlatform.instance.computeMatches(jsSfM_Data_Filename, jsMatchesDirectory, jsGeometricModel, jfDistRatio, jsNearestMatchingMethod);
  }

  @override
  Future<String?> incrementalReconstruct(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutDir, String jsIntrinsic_refinement_options, String jsSfMInitializer_method, String jcameraModel) async {
    return OpenMvgFlutterJavaPlatform.instance.incrementalReconstruct(jsSfM_Data_Filename, jsMatchesDir, jsOutDir, jsIntrinsic_refinement_options, jsSfMInitializer_method, jcameraModel);
  }

  @override
  Future<String?> computeDataColor(String inData, String outData) {
    return OpenMvgFlutterJavaPlatform.instance.computeDataColor(inData, outData);
  }

  @override
  Future<String?> refinePoses(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutFile, String juseBundleAdjustment, String jbDirect_triangulation, String jsMatchFile) async {
    return OpenMvgFlutterJavaPlatform.instance.refinePoses(jsSfM_Data_Filename, jsMatchesDir, jsOutFile, juseBundleAdjustment, jbDirect_triangulation, jsMatchFile);
  }
}
