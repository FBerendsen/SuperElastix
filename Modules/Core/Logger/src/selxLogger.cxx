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

#ifndef Logger_cxx
#define Logger_cxx

#include "selxLogger.h"
#include "selxLoggerImpl.h"

namespace selx {

Logger
::Logger( void ) : m_Pimple( new Logger::LoggerImpl ) {};

Logger
::~Logger( void ) = default;
               
void
Logger
::Log( SeverityType severity, MessageType message )
{
  this->m_Pimple->Log( severity, message );
}

void
Logger
::Log( SeverityType severity, ChannelType channel, MessageType message )
{
  this->m_Pimple->Log( severity, channel, message );
}

} // namespace selx

#endif // Logger.cxx
