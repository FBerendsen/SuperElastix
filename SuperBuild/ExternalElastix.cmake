#=========================================================================
#
#  Copyright Leiden University Medical Center, Erasmus University Medical 
#  Center and contributors
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0.txt
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#=========================================================================

set( proj Elastix )

set( ELASTIX_GIT_REPOSITORY http://github.com/kaspermarstal/elastix )
set( ELASTIX_GIT_TAG fa451dd33ac72dbded40dbf408db2c4e958469ac )

ExternalProject_Add( ${proj} 
  GIT_REPOSITORY ${ELASTIX_GIT_REPOSITORY}
  GIT_TAG ${ELASTIX_GIT_TAG}
  UPDATE_COMMAND ""
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
  --no-warn-unused-cli
  -DELASTIX_BUILD_TESTING:BOOL=OFF
  -DELASTIX_BUILD_EXECUTABLE:BOOL=OFF
  -DELASTIX_BUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DITK_DIR:PATH=${ITK_DIR}
  # explicitly set the types for 3d
  # -DELASTIX_IMAGE_3D_PIXELTYPES:STRING={"float;double"}
  # Elastix components configuration
  -DUSE_AdaptiveStochasticGradientDescent:BOOL=ON                                           
  -DUSE_AdvancedAffineTransformElastix:BOOL=ON
  -DUSE_AdvancedBSplineTransform:BOOL=ON                                           
  -DUSE_AdvancedKappaStatisticMetric:BOOL=ON                                           
  -DUSE_AdvancedMattesMutualInformationMetric:BOOL=ON                                           
  -DUSE_AdvancedMeanSquaresMetric:BOOL=ON                                           
  -DUSE_AdvancedNormalizedCorrelationMetric:BOOL=ON                                           
  -DUSE_AffineDTITransformElastix:BOOL=ON                                           
  -DUSE_BSplineInterpolator:BOOL=ON                                           
  -DUSE_BSplineInterpolatorFloat:BOOL=ON                                           
  -DUSE_BSplineResampleInterpolator:BOOL=ON                                           
  -DUSE_BSplineResampleInterpolatorFloat:BOOL=ON                                           
  -DUSE_BSplineStackTransform:BOOL=ON                                           
  -DUSE_BSplineTransformWithDiffusion:BOOL=ON                                           
  -DUSE_CMAEvolutionStrategy:BOOL=ON                                           
  -DUSE_CUDAResampler:BOOL=OFF                                          
  -DUSE_ConjugateGradient:BOOL=ON                                           
  -DUSE_ConjugateGradientFRPR:BOOL=ON                                           
  -DUSE_CorrespondingPointsEuclideanDistanceMetric:BOOL=ON
  -DUSE_DeformationFieldTransform:BOOL=ON                                           
  -DUSE_DisplacementMagnitudePenalty:BOOL=ON                                           
  -DUSE_DistancePreservingRigidityPenalty:BOOL=ON                                           
  -DUSE_EulerTransformElastix:BOOL=ON                                           
  -DUSE_FiniteDifferenceGradientDescent:BOOL=ON                                           
  -DUSE_FixedGenericPyramid:BOOL=ON                                           
  -DUSE_FixedRecursivePyramid:BOOL=ON                                           
  -DUSE_FixedShrinkingPyramid:BOOL=ON                                           
  -DUSE_FixedSmoothingPyramid:BOOL=ON                                           
  -DUSE_FullSampler:BOOL=ON                                           
  -DUSE_FullSearch:BOOL=ON                                           
  -DUSE_GradientDifferenceMetric:BOOL=ON                                           
  -DUSE_GridSampler:BOOL=ON                                           
  -DUSE_KNNGraphAlphaMutualInformationMetric:BOOL=OFF                                         
  -DUSE_LinearInterpolator:BOOL=ON                                           
  -DUSE_LinearResampleInterpolator:BOOL=ON                                           
  -DUSE_MissingStructurePenalty:BOOL=ON
  -DUSE_MovingRecursivePyramid:BOOL=ON                                           
  -DUSE_MovingShrinkingPyramid:BOOL=ON                                           
  -DUSE_MovingSmoothingPyramid:BOOL=ON                                           
  -DUSE_MultiBSplineTransformWithNormal:BOOL=ON                                           
  -DUSE_MultiInputRandomCoordinateSampler:BOOL=ON                                           
  -DUSE_MultiMetricMultiResolutionRegistration:BOOL=ON                                           
  -DUSE_MultiResolutionRegistration:BOOL=ON                                           
  -DUSE_MultiResolutionRegistrationWithFeatures:BOOL=ON                                           
  -DUSE_MutualInformationHistogramMetric:BOOL=ON                                           
  -DUSE_MyStandardResampler:BOOL=ON                                           
  -DUSE_NearestNeighborInterpolator:BOOL=ON                                           
  -DUSE_NearestNeighborResampleInterpolator:BOOL=ON                                           
  -DUSE_NormalizedGradientCorrelationMetric:BOOL=ON                                           
  -DUSE_NormalizedMutualInformationMetric:BOOL=ON                                           
  -DUSE_PatternIntensityMetric:BOOL=ON                                           
  -DUSE_PolydataDummyPenalty:BOOL=ON                                           
  -DUSE_Powell:BOOL=ON
  -DUSE_QuasiNewtonLBFGS:BOOL=ON                                           
  -DUSE_RSGDEachParameterApart:BOOL=ON                                           
  -DUSE_RandomCoordinateSampler:BOOL=ON                                           
  -DUSE_RandomSampler:BOOL=ON                                           
  -DUSE_RandomSamplerSparseMask:BOOL=ON                                           
  -DUSE_RayCastInterpolator:BOOL=ON                                           
  -DUSE_RayCastResampleInterpolator:BOOL=ON                                           
  -DUSE_ReducedDimensionBSplineInterpolator:BOOL=ON                                           
  -DUSE_ReducedDimensionBSplineResampleInterpolator:BOOL=ON                                           
  -DUSE_RegularStepGradientDescent:BOOL=ON                                           
  -DUSE_SimilarityTransformElastix:BOOL=ON                                           
  -DUSE_Simplex:BOOL=ON                                           
  -DUSE_SimultaneousPerturbation:BOOL=ON                                           
  -DUSE_SplineKernelTransform:BOOL=ON                                           
  -DUSE_StandardGradientDescent:BOOL=ON                                           
  -DUSE_StatisticalShapePenalty:BOOL=ON                                           
  -DUSE_TransformBendingEnergyPenanalty:BOOL=ON
  -DUSE_TransformRigidityPenalty:BOOL=ON                                           
  -DUSE_TranslationTransformElastix:BOOL=ON                                           
  -DUSE_VarianceOverLastDimensionMetric:BOOL=ON                                           
  -DUSE_ViolaWellsMutualInformationMetric:BOOL=ON                                           
  -DUSE_WeightedCombinationTransformElastix:BOOL=ON
  DEPENDS ITK
)

ExternalProject_Get_Property( Elastix binary_dir )
set( ELASTIX_USE_FILE "${binary_dir}/src/UseElastix.cmake" )

list( APPEND SUPERELASTIX_DEPENDENCIES ${proj} )
