// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "openMVG/geometry/frustum.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_filters_frustum.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/system/logger.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>
#include <string>

using namespace openMVG;
using namespace openMVG::sfm;

/// Export camera frustrums as a triangle PLY file
int main(int argc, char **argv)
{
  OPENMVG_LOG_INFO << "Export camera frustums";

  CmdLine cmd;

  std::string sSfM_Data_Filename;
  std::string sOutFile = "";
  double z_near = -1.;
  double z_far = -1.;

  cmd.add( make_option('i', sSfM_Data_Filename, "input_file") );
  cmd.add( make_option('o', sOutFile, "output_file") );
  cmd.add( make_option('n', z_near, "z_near") );
  cmd.add( make_option('f', z_far, "z_far") );
  cmd.add( make_switch('c', "colorize") );

  try {
    if (argc == 1) throw std::string("Invalid command line parameter.");
    cmd.process(argc, argv);
  } catch (const std::string& s) {
    std::cerr << "Usage: " << argv[0] << '\n'
    << "[-i|--input_file] path to a SfM_Data scene\n"
    << "[-o|--output_file] PLY file to store the camera frustums as triangle meshes.\n"
    << "[-n|--z_near] 'optional' distance of the near camera plane\n"
    << "[-f|--z_far] 'optional' distance of the far camera plane\n"
    << "[-c|--colorize] 'optional' colorize the camera frustums.\n"
    << std::endl;

    OPENMVG_LOG_ERROR << s;
    return EXIT_FAILURE;
  }

  // Load input SfM_Data scene
  SfM_Data sfm_data;
  if (!Load(sfm_data, sSfM_Data_Filename, ESfM_Data(VIEWS|INTRINSICS|EXTRINSICS))) {
    OPENMVG_LOG_ERROR << "The input SfM_Data file \""<< sSfM_Data_Filename << "\" cannot be read.";
    return EXIT_FAILURE;
  }

  // Assert that we can create the output directory/file
  if (!stlplus::folder_exists( stlplus::folder_part(sOutFile) ))
    if (!stlplus::folder_create( stlplus::folder_part(sOutFile) ))
      return EXIT_FAILURE;

  // Detect if colorization is requested
  const bool colorize = cmd.used('c');

  // If sfm_data have not structure, cameras are displayed as tiny normalized cones
  const Frustum_Filter frustum_filter(sfm_data, z_near, z_far);
  if (!sOutFile.empty())
  {
    if (frustum_filter.export_Ply(sOutFile, colorize))
      return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
