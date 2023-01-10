package com.example.open_mvg_flutter_java;

import android.os.Environment;

import androidx.annotation.NonNull;

import com.example.open_mvg_flutter_java.bridge.OpenMVG;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/** OpenMvgFlutterJavaPlugin */
public class OpenMvgFlutterJavaPlugin implements FlutterPlugin, MethodCallHandler {
  /// The MethodChannel that will the communication between Flutter and native Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine and unregister it
  /// when the Flutter Engine is detached from the Activity
  private MethodChannel channel;

  static {
    System.loadLibrary("openMVG");
  }

  @Override
  public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
    channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "open_mvg_flutter_java");
    channel.setMethodCallHandler(this);
  }

  @Override
  public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
    if (call.method.equals("getPlatformVersion")) {
      result.success("Android " + android.os.Build.VERSION.RELEASE);
    } else if (call.method.equals("stringFromJNI")){
      result.success(OpenMVG.stringFromJNI());
    } else if (call.method.equals("imageList")){

      final String sImageDir = call.argument("sImageDir");
      final String sfileDatabase = call.argument("sfileDatabase");
      final String sOutputDir = call.argument("sOutputDir");
      final String jfocal_pixels = call.argument("jfocal_pixels");

      result.success(OpenMVG.imageList(sImageDir, sfileDatabase, sOutputDir, jfocal_pixels));

    } else if (call.method.equals("computeFeatures")){

      final String jsSfM_Data_Filename = call.argument("jsSfM_Data_Filename");
      final String jsOutDir = call.argument("jsOutDir");
      final String jbUpRight = call.argument("jbUpRight");
      final String jsImage_Describer_Method = call.argument("jsImage_Describer_Method");
      final String jsFeaturePreset = call.argument("jsFeaturePreset");

      result.success(OpenMVG.computeFeatures(jsSfM_Data_Filename, jsOutDir, jbUpRight, jsImage_Describer_Method, jsFeaturePreset));

    } else if (call.method.equals("computeMatches")){

      final String jsSfM_Data_Filename = call.argument("jsSfM_Data_Filename");
      final String jsMatchesDirectory = call.argument("jsMatchesDirectory");
      final String jsGeometricModel = call.argument("jsGeometricModel");
      final String jfDistRatio = call.argument("jfDistRatio");
      final String jsNearestMatchingMethod = call.argument("jsNearestMatchingMethod");

      result.success(OpenMVG.computeMatches(jsSfM_Data_Filename, jsMatchesDirectory, jsGeometricModel, jfDistRatio, jsNearestMatchingMethod));

    }  else if (call.method.equals("incrementalReconstruct")){

      final String jsSfM_Data_Filename = call.argument("jsSfM_Data_Filename");
      final String jsMatchesDir = call.argument("jsMatchesDir");
      final String jsOutDir = call.argument("jsOutDir");
      final String jsIntrinsic_refinement_options = call.argument("jsIntrinsic_refinement_options");
      final String jsSfMInitializer_method = call.argument("jsSfMInitializer_method");
      final String jcameraModel = call.argument("jcameraModel");

      result.success(OpenMVG.incrementalReconstruct(jsSfM_Data_Filename, jsMatchesDir, jsOutDir, jsIntrinsic_refinement_options, jsSfMInitializer_method, jcameraModel));

    } else if (call.method.equals("computeDataColor")){
      final String in = call.argument("in");
      final String out = call.argument("out");

      result.success(OpenMVG.computeDataColor(in, out));

    } else if (call.method.equals("refinePoses")){

      final String jsSfM_Data_Filename = call.argument("jsSfM_Data_Filename");
      final String jsMatchesDir = call.argument("jsMatchesDir");
      final String jsOutDir = call.argument("jsOutDir");
      final String juseBundleAdjustment = call.argument("juseBundleAdjustment");
      final String jbDirect_triangulation = call.argument("jbDirect_triangulation");
      final String jsMatchFile = call.argument("jsMatchFile");

      result.success(OpenMVG.refinePoses(jsSfM_Data_Filename, jsMatchesDir, jsOutDir, juseBundleAdjustment, jbDirect_triangulation, jsMatchFile));

    } else if (call.method.equals("stringFromJNI")){
      result.success(OpenMVG.stringFromJNI());
    } else {
      result.notImplemented();
    }
  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    channel.setMethodCallHandler(null);
  }
}
