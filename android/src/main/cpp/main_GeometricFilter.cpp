// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2012, 2019 Pierre MOULON, Romuald PERROT

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "openMVG/features/akaze/image_describer_akaze.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/features/feature.hpp"
#include "openMVG/graph/graph.hpp"
#include "openMVG/graph/graph_stats.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"
#include "openMVG/matching/pairwiseAdjacencyDisplay.hpp"
#include "openMVG/matching_image_collection/Cascade_Hashing_Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/E_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust_Angular.hpp"
#include "openMVG/matching_image_collection/Eo_Robust.hpp"
#include "openMVG/matching_image_collection/F_ACRobust.hpp"
#include "openMVG/matching_image_collection/GeometricFilter.hpp"
#include "openMVG/matching_image_collection/H_ACRobust.hpp"
#include "openMVG/matching_image_collection/Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider_cache.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/stl/stl.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <jni.h>

using namespace openMVG;
using namespace openMVG::matching;
using namespace openMVG::robust;
using namespace openMVG::sfm;
using namespace openMVG::matching_image_collection;

enum EGeometricModel
{
  FUNDAMENTAL_MATRIX       = 0,
  ESSENTIAL_MATRIX         = 1,
  HOMOGRAPHY_MATRIX        = 2,
  ESSENTIAL_MATRIX_ANGULAR = 3,
  ESSENTIAL_MATRIX_ORTHO   = 4,
  ESSENTIAL_MATRIX_UPRIGHT = 5
};

/// Compute corresponding features between a series of views:
/// - Load view images description (regions: features & descriptors)
/// - Compute putative local feature matches (descriptors matching)
/// - Compute geometric coherent feature matches (robust model estimation from putative matches)
/// - Export computed data
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_open_1mvg_1flutter_1java_bridge_OpenMVG_geometricFilter(
                JNIEnv *env,
                jobject /* this */,
                jstring input_file, jstring output_file, jstring matches
                )
{

  jboolean copy = true;

  // The scene
  std::string sSfM_Data_Filename = env->GetStringUTFChars(input_file, &copy);
  // The input matches
  std::string sPutativeMatchesFilename = env->GetStringUTFChars(matches, &copy);
  // The output matches
  std::string sFilteredMatchesFilename = env->GetStringUTFChars(output_file, &copy);

  std::string  sGeometricModel   = "f";
  bool         bForce            = false;
  bool         bGuided_matching  = false;
  int          imax_iteration    = 2048;
  unsigned int ui_max_cache_size = 0;

  if ( sFilteredMatchesFilename.empty() )
  {
    std::cerr << "It is an invalid output file" << std::endl;
    return env->NewStringUTF("FAILED");
  }
  if ( sSfM_Data_Filename.empty() )
  {
    OPENMVG_LOG_ERROR << "It is an invalid SfM file";
    return env->NewStringUTF("FAILED");
  }
  if ( sPutativeMatchesFilename.empty() )
  {
    OPENMVG_LOG_ERROR << "It is an invalid putative matche file";
    return env->NewStringUTF("FAILED");
  }

  const std::string sMatchesDirectory = stlplus::folder_part( sPutativeMatchesFilename );

  EGeometricModel eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
  switch ( std::tolower(sGeometricModel[ 0 ], std::locale()) )
  {
    case 'f':
      eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
      break;
    case 'e':
      eGeometricModelToCompute = ESSENTIAL_MATRIX;
      break;
    case 'h':
      eGeometricModelToCompute = HOMOGRAPHY_MATRIX;
      break;
    case 'a':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ANGULAR;
      break;
    case 'u':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_UPRIGHT;
      break;
    case 'o':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ORTHO;
      break;
    default:
      return env->NewStringUTF("FAILED");
  }

  // -----------------------------
  // - Load SfM_Data Views & intrinsics data
  // a. Load putative descriptor matches
  // [a.1] Filter matches with input pairs
  // b. Geometric filtering of putative matches
  // + Export some statistics
  // -----------------------------

  //---------------------------------------
  // Read SfM Scene (image view & intrinsics data)
  //---------------------------------------
  SfM_Data sfm_data;
  if ( !Load( sfm_data, sSfM_Data_Filename, ESfM_Data( VIEWS | INTRINSICS ) ) )
  {
    return env->NewStringUTF("FAILED");
  }

  //---------------------------------------
  // Load SfM Scene regions
  //---------------------------------------
  // Init the regions_type from the image describer file (used for image regions extraction)
  using namespace openMVG::features;
  // Consider that the image_describer.json is inside the matches directory (which is bellow the sfm_data.bin)
  const std::string        sImage_describer = stlplus::create_filespec( sMatchesDirectory, "image_describer.json" );
  std::unique_ptr<Regions> regions_type     = Init_region_type_from_file( sImage_describer );
  if ( !regions_type )
  {
    return env->NewStringUTF("FAILED");
  }

  //---------------------------------------
  // a. Compute putative descriptor matches
  //    - Descriptor matching (according user method choice)
  //    - Keep correspondences only if NearestNeighbor ratio is ok
  //---------------------------------------

  // Load the corresponding view regions
  std::shared_ptr<Regions_Provider> regions_provider;
  if ( ui_max_cache_size == 0 )
  {
    // Default regions provider (load & store all regions in memory)
    regions_provider = std::make_shared<Regions_Provider>();
  }
  else
  {
    // Cached regions provider (load & store regions on demand)
    regions_provider = std::make_shared<Regions_Provider_Cache>( ui_max_cache_size );
  }

  // Show the progress on the command line:
  system::LoggerProgress progress;

  if ( !regions_provider->load( sfm_data, sMatchesDirectory, regions_type, &progress ) )
  {
    return env->NewStringUTF("FAILED");
  }

  PairWiseMatches map_PutativeMatches;
  //---------------------------------------
  // A. Load initial matches
  //---------------------------------------
  if ( !Load( map_PutativeMatches, sPutativeMatchesFilename ) )
  {
    return env->NewStringUTF("FAILED");
  }

  //---------------------------------------
  // b. Geometric filtering of putative matches
  //    - AContrario Estimation of the desired geometric model
  //    - Use an upper bound for the a contrario estimated threshold
  //---------------------------------------

  std::unique_ptr<ImageCollectionGeometricFilter> filter_ptr(
      new ImageCollectionGeometricFilter( &sfm_data, regions_provider ) );

  if ( filter_ptr )
  {
    system::Timer timer;
    const double  d_distance_ratio = 0.6;

    PairWiseMatches map_GeometricMatches;
    switch ( eGeometricModelToCompute )
    {
      case HOMOGRAPHY_MATRIX:
      {
        const bool bGeometric_only_guided_matching = true;
        filter_ptr->Robust_model_estimation(
            GeometricFilter_HMatrix_AC( 4.0, imax_iteration ),
            map_PutativeMatches,
            bGuided_matching,
            bGeometric_only_guided_matching ? -1.0 : d_distance_ratio,
            &progress );
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case FUNDAMENTAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
            GeometricFilter_FMatrix_AC( 4.0, imax_iteration ),
            map_PutativeMatches,
            bGuided_matching,
            d_distance_ratio,
            &progress );
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
            GeometricFilter_EMatrix_AC( 4.0, imax_iteration ),
            map_PutativeMatches,
            bGuided_matching,
            d_distance_ratio,
            &progress );
        map_GeometricMatches = filter_ptr->Get_geometric_matches();

        //-- Perform an additional check to remove pairs with poor overlap
        std::vector<PairWiseMatches::key_type> vec_toRemove;
        for ( const auto& pairwisematches_it : map_GeometricMatches )
        {
          const size_t putativePhotometricCount = map_PutativeMatches.find( pairwisematches_it.first )->second.size();
          const size_t putativeGeometricCount   = pairwisematches_it.second.size();
          const float  ratio                    = putativeGeometricCount / static_cast<float>( putativePhotometricCount );
          if ( putativeGeometricCount < 50 || ratio < .3f )
          {
            // the pair will be removed
            vec_toRemove.push_back( pairwisematches_it.first );
          }
        }
        //-- remove discarded pairs
        for ( const auto& pair_to_remove_it : vec_toRemove )
        {
          map_GeometricMatches.erase( pair_to_remove_it );
        }
      }
      break;
      case ESSENTIAL_MATRIX_ANGULAR:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_ESphericalMatrix_AC_Angular<false>(4.0, imax_iteration),
          map_PutativeMatches, bGuided_matching, d_distance_ratio, &progress);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX_UPRIGHT:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_ESphericalMatrix_AC_Angular<true>(4.0, imax_iteration),
          map_PutativeMatches, bGuided_matching, d_distance_ratio, &progress);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX_ORTHO:
      {
        filter_ptr->Robust_model_estimation(
            GeometricFilter_EOMatrix_RA( 2.0, imax_iteration ),
            map_PutativeMatches,
            bGuided_matching,
            d_distance_ratio,
            &progress );
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
    }

    //---------------------------------------
    //-- Export geometric filtered matches
    //---------------------------------------
    if ( !Save( map_GeometricMatches, sFilteredMatchesFilename ) )
    {
      return env->NewStringUTF("FAILED");;
    }

    // -- export Geometric View Graph statistics
    graph::getGraphStatistics(sfm_data.GetViews().size(), getPairs(map_GeometricMatches));

    //-- export Adjacency matrix

    PairWiseMatchingToAdjacencyMatrixSVG( sfm_data.GetViews().size(),
                                          map_GeometricMatches,
                                          stlplus::create_filespec( sMatchesDirectory, "GeometricAdjacencyMatrix", "svg" ) );

    const Pair_Set outputPairs = getPairs( map_GeometricMatches );

    //-- export view pair graph once geometric filter have been done
    {
      std::set<IndexT> set_ViewIds;
      std::transform( sfm_data.GetViews().begin(), sfm_data.GetViews().end(), std::inserter( set_ViewIds, set_ViewIds.begin() ), stl::RetrieveKey() );
      graph::indexedGraph putativeGraph( set_ViewIds, outputPairs );
      graph::exportToGraphvizData(
          stlplus::create_filespec( sMatchesDirectory, "geometric_matches" ),
          putativeGraph );
    }

  }
  return env->NewStringUTF("SUCCESS");;
}
