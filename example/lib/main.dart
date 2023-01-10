import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:io';

import 'package:flutter/services.dart';
import 'package:open_mvg_flutter_java/open_mvg_flutter_java.dart';
import 'package:path_provider/path_provider.dart';
import 'package:permission_handler/permission_handler.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String message = 'Unknown';
  final openMvgFlutterJavaPlugin = OpenMvgFlutterJava();

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    final directory = await getApplicationDocumentsDirectory();

    await Directory(directory.path.toString() + '/photogrammetry/images/').create();

    String imagesPath = directory.path.toString() + '/photogrammetry/images/';
    String matchesPath = directory.path.toString() + '/photogrammetry/matches/';
    String databasePath = directory.path.toString() + '/photogrammetry/sensor_width_camera_database.txt';

    message = await openMvgFlutterJavaPlugin.imageList(imagesPath, databasePath, matchesPath, (1920*1.2).toString()) ?? 'Error';
    setState(() {
      message = "Part 1: " + message;
    });

    String matchesFile = matchesPath + '/sfm_data.json';

    File(matchesPath + '/matches.f.txt').writeAsString('');
    message = await openMvgFlutterJavaPlugin.computeFeatures(matchesFile, matchesPath, 'false', 'SIFT', 'NORMAL') ?? 'Error';

    setState(() {
      message = "Part 2: " + message;
    });

    message = await openMvgFlutterJavaPlugin.computeMatches(matchesFile, matchesPath, 'e', '0.8f', 'AUTO') ?? 'Error';

    setState(() {
      message = "Part 3 (computeMatches): " + message;
    });

    message = await openMvgFlutterJavaPlugin.incrementalReconstruct(matchesFile, matchesPath, matchesPath, 'ADJUST_ALL', 'STELLAR', 'PINHOLE_CAMERA_RADIAL3') ?? 'Error';

    setState(() {
      message = "Part 4: (incrementalReconstruct): " + message;
    });


  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('$message\n'),
        ),
      ),
    );
  }
}
