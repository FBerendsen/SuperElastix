/*=========================================================================
 *
 *  Copyright Leiden University Medical Center, Erasmus University Medical
 *  Center and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "gtest/gtest.h"
#include "selxLogger.h"

using namespace selx;

Logger* logger = new Logger();

TEST( LoggerTest, INFO )
{ 
  logger->Log( INFO, "Message at severity INFO" );
}

TEST( LoggerTest, WARNING )
{
  logger->Log( WARNING, "Message at severity WARNING " );
}

TEST( LoggerTest, ERROR )
{
  logger->Log( ERROR, "Message at severity ERROR" );
}


TEST( LoggerTest, Channel )
{
  logger->Log( INFO, "ITERATION", "Message to channel ITERATION at severity INFO" );
}

TEST( LoggerTest, FATAL )
{
  // This terminates the program
  // logger->Log( FATAL, "Message at severity FATAL" );
}


