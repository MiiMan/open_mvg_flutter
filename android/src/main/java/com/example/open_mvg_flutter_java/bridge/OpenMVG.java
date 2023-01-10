package com.example.open_mvg_flutter_java.bridge;

public class OpenMVG {

    static public native String stringFromJNI();

    static public native String imageList(String sImageDir, String sfileDatabase, String sOutputDir, String jfocal_pixels);

    static public native String computeFeatures(String jsSfM_Data_Filename, String jsOutDir, String jbUpRight, String jsImage_Describer_Method, String jsFeaturePreset);

    static public native String computeMatches(String jsSfM_Data_Filename, String jsMatchesDirectory, String jsGeometricModel, String jfDistRatio, String jsNearestMatchingMethod);

    static public native String incrementalReconstruct(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutDir, String jsIntrinsic_refinement_options, String jsSfMInitializer_method, String jcameraModel);

    static public native String computeDataColor(String jsSfM_Data_Filename_In, String jsOutputPLY_Out);

    static public native String refinePoses(String jsSfM_Data_Filename, String jsMatchesDir, String jsOutFile, String juseBundleAdjustment, String jbDirect_triangulation, String jsMatchFile);

    static public native String generatePairs(String input_file, String output_file);

    static public native String geometricFilter(String input_file, String output_file, String matches);
}
