import 'package:flutter_test/flutter_test.dart';
import 'package:open_mvg_flutter_java/open_mvg_flutter_java.dart';
import 'package:open_mvg_flutter_java/open_mvg_flutter_java_platform_interface.dart';
import 'package:open_mvg_flutter_java/open_mvg_flutter_java_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockOpenMvgFlutterJavaPlatform
    with MockPlatformInterfaceMixin
    implements OpenMvgFlutterJavaPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final OpenMvgFlutterJavaPlatform initialPlatform = OpenMvgFlutterJavaPlatform.instance;

  test('$MethodChannelOpenMvgFlutterJava is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelOpenMvgFlutterJava>());
  });

  test('getPlatformVersion', () async {
    OpenMvgFlutterJava openMvgFlutterJavaPlugin = OpenMvgFlutterJava();
    MockOpenMvgFlutterJavaPlatform fakePlatform = MockOpenMvgFlutterJavaPlatform();
    OpenMvgFlutterJavaPlatform.instance = fakePlatform;

    expect(await openMvgFlutterJavaPlugin.getPlatformVersion(), '42');
  });
}
