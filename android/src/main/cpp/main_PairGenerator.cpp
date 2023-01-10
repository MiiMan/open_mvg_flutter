// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2019 Romuald PERROT

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <iostream>
#include <jni.h>

/**
 * @brief Current list of available pair mode
 *
 */
enum EPairMode
{
  PAIR_EXHAUSTIVE = 0, // Build every combination of image pairs
  PAIR_CONTIGUOUS = 1  // Only consecutive image pairs (useful for video mode)
};

using namespace openMVG;
using namespace openMVG::sfm;

void usage( const char* argv0 )
{
  std::cerr << "Usage: " << argv0 << '\n'
            << "[-i|--input_file]         A SfM_Data file\n"
            << "[-o|--output_file]        Output file where pairs are stored\n"
            << "\n[Optional]\n"
            << "[-m|--pair_mode] mode     Pair generation mode\n"
            << "       EXHAUSTIVE:        Build all possible pairs. [default]\n"
            << "       CONTIGUOUS:        Build pairs for contiguous images (use it with --contiguous_count parameter)\n"
            << "[-c|--contiguous_count] X Number of contiguous links\n"
            << "       X: will match 0 with (1->X), ...]\n"
            << "       2: will match 0 with (1,2), 1 with (2,3), ...\n"
            << "       3: will match 0 with (1,2,3), 1 with (2,3,4), ...\n"
            << std::endl;
}

// This executable computes pairs of images to be matched
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_open_1mvg_1flutter_1java_bridge_OpenMVG_generatePairs(
        JNIEnv *env,
        jobject /* this */,
        jstring input_file, jstring output_file)
{
  jboolean copy = true;

  std::string sSfMDataFilename = env->GetStringUTFChars(input_file, &copy);
  std::string sOutputPairsFilename = env->GetStringUTFChars(output_file, &copy);
  std::string sPairMode        = "EXHAUSTIVE";
  int         iContiguousCount = -1;



  // 0. Parse parameters
  std::cout << " You called:\n"
            << "--input_file       : " << sSfMDataFilename << "\n"
            << "--output_file      : " << sOutputPairsFilename << "\n"
            << "Optional parameters\n"
            << "--pair_mode        : " << sPairMode << "\n"
            << "--contiguous_count : " << iContiguousCount << "\n"
            << std::endl;

  if ( sSfMDataFilename.empty() )
  {
    std::cerr << "[Error] Input file not set." << std::endl;
    return env->NewStringUTF("FAILED");
  }
  if ( sOutputPairsFilename.empty() )
  {
    std::cerr << "[Error] Output file not set." << std::endl;
    return env->NewStringUTF("FAILED");
  }

  EPairMode pairMode;
  if ( sPairMode == "EXHAUSTIVE" )
  {
    pairMode = PAIR_EXHAUSTIVE;
  }
  else if ( sPairMode == "CONTIGUOUS" )
  {
    if ( iContiguousCount == -1 )
    {
      std::cerr << "[Error] Contiguous pair mode selected but contiguous_count not set." << std::endl;
      return env->NewStringUTF("FAILED");
    }

    pairMode = PAIR_CONTIGUOUS;
  }

  // 1. Load SfM data scene
  std::cout << "Loading scene.";
  SfM_Data sfm_data;
  if ( !Load( sfm_data, sSfMDataFilename, ESfM_Data( VIEWS | INTRINSICS ) ) )
  {
    std::cerr << std::endl
              << "The input SfM_Data file \"" << sSfMDataFilename << "\" cannot be read." << std::endl;
    return env->NewStringUTF("FAILED");
  }
  const size_t NImage = sfm_data.GetViews().size();

  // 2. Compute pairs
  std::cout << "Computing pairs." << std::endl;
  Pair_Set pairs;
  switch ( pairMode )
  {
    case PAIR_EXHAUSTIVE:
    {
      pairs = exhaustivePairs( NImage );
      break;
    }
    case PAIR_CONTIGUOUS:
    {
      pairs = contiguousWithOverlap( NImage, iContiguousCount );
      break;
    }
    default:
    {
      std::cerr << "Unknown pair mode" << std::endl;
      return env->NewStringUTF("FAILED");
    }
  }

  // 3. Save pairs
  std::cout << "Saving pairs." << std::endl;
  if ( !savePairs( sOutputPairsFilename, pairs ) )
  {
    std::cerr << "Failed to save pairs to file: \"" << sOutputPairsFilename << "\"" << std::endl;
    return env->NewStringUTF("FAILED");
  }

  return env->NewStringUTF("SUCCESS");
}
