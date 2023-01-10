import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'open_mvg_flutter_java_method_channel.dart';

abstract class OpenMvgFlutterJavaPlatform extends PlatformInterface {
  /// Constructs a OpenMvgFlutterJavaPlatform.
  OpenMvgFlutterJavaPlatform() : super(token: _token);

  static final Object _token = Object();

  static OpenMvgFlutterJavaPlatform _instance = MethodChannelOpenMvgFlutterJava();

  /// The default instance of [OpenMvgFlutterJavaPlatform] to use.
  ///
  /// Defaults to [MethodChannelOpenMvgFlutterJava].
  static OpenMvgFlutterJavaPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [OpenMvgFlutterJavaPlatform] when
  /// they register themselves.
  static set instance(OpenMvgFlutterJavaPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  @override
  Future<String?> stringFromJNI() async {
    throw UnimplementedError('stringFromJNI() has not been implemented.');
  }

  @override
  Future<String?> imageList(String sImageDir, String sfileDatabase, String sOutputDir, String jfocal_pixels) async {
    throw UnimplementedError('imageList() has not been implemented.');
  }

  @override
  Future<String?> computeFeatures(String jsSfM_Data_Filename, String jsOutDir, String jbUpRight, String jsImage_Describer_Method, String jsFeaturePreset) async {
    throw UnimplementedError('computeFeatures() has not been implemented.');
  }

  @override
  Future<String?> computeMatches(String jsSfM_Data_Filename, String jsMatchesDirectory, String jsGeometricModel, String jfDistRatio, String jsNearestMatchingMethod) async {
    throw UnimplementedError('computeMatches() has not been implemented.');
  }

  @override
  Future<String?> incrementalReconstruct(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutDir, String jsIntrinsic_refinement_options, String jsSfMInitializer_method, String jcameraModel) async {
    throw UnimplementedError('incrementalReconstruct() has not been implemented.');
  }

  @override
  Future<String?> computeDataColor(String inData, String outData) async {
    throw UnimplementedError('computeDataColor() has not been implemented.');
  }

  @override
  Future<String?> refinePoses(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutFile, String juseBundleAdjustment, String jbDirect_triangulation, String jsMatchFile) async {
    throw UnimplementedError('refinePoses() has not been implemented.');
  }
}
