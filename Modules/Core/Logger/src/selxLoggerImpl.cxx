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

#ifndef LoggerImpl_cxx
#define LoggerImpl_cxx

#define GLOG(severity, msg) LOG(severity) << msg; ;

#include "selxLoggerImpl.h"
#include "glog/log_severity.h"

namespace selx
{

Logger::LoggerImpl
::LoggerImpl()
{
  google::InitGoogleLogging( "SuperElastix" );
}

Logger::LoggerImpl
::~LoggerImpl()
{
}

void
Logger::LoggerImpl
::Log( SeverityType const severity, MessageType const message )
{
  FLAGS_logtostderr = 1;
  switch( severity ) {
    case INFO:
      GLOG( INFO, message );
      google::FlushLogFiles(INFO);
      break;
    case WARNING:
      GLOG( WARNING, message );
      google::FlushLogFiles(WARNING);
      break;
    case ERROR:
      GLOG( ERROR, message);
      google::FlushLogFiles(ERROR);
      break;
    case FATAL:
      GLOG( FATAL, message );
      break;
    default:
      GLOG( INFO, message ) ;
      google::FlushLogFiles(WARNING);
      break;
  }


}

void
Logger::LoggerImpl
::Log( SeverityType const severity, Logger::ChannelType const channel, MessageType const message )
{
  std::string const channelMessage = "<" + channel + "> " + message;
  this->Log( severity, channelMessage );
}

}

#endif // LoggerImpl_cxx
