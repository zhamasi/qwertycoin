// Copyright (c) 2012-2016, The CryptoNote developers, The Bytecoin developers, The Qwertycoin developers
// Copyright (c) 2017-2018, Karbo developers
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "LoggerMessage.h"

namespace Logging {

LoggerMessage::LoggerMessage(ILogger& logger, const std::string& category, Level level, const std::string& color)
	: std::ostream(this)
	, std::streambuf()
	, m_logger(logger)
	, m_sCategory(category)
	, m_nLogLevel(level)
	, m_sMessage(color)
	, m_tmTimeStamp(boost::posix_time::microsec_clock::local_time())
	, m_bGotText(false)
{}

LoggerMessage::LoggerMessage(LoggerMessage&& other)
	: std::ostream(std::move(other))
	, std::streambuf(std::move(other))
	, m_logger(other.m_logger)
	, m_sCategory(other.m_sCategory)
	, m_nLogLevel(other.m_nLogLevel)
	, m_sMessage(other.m_sMessage)
	, m_tmTimeStamp(boost::posix_time::microsec_clock::local_time())
	, m_bGotText(false)
{
	std::ostream::rdbuf(this);
}

LoggerMessage::~LoggerMessage()
{
	if (m_bGotText)
		(*this) << std::endl;
}

int LoggerMessage::sync()
{
	m_logger(m_sCategory, m_nLogLevel, m_tmTimeStamp, m_sMessage);
	m_bGotText = false;
	m_sMessage = Logging::DEFAULT;
	return 0;
}

std::streamsize LoggerMessage::xsputn(const char* s, std::streamsize n)
{
	m_bGotText = true;
	m_sMessage.append(s, n);
	return n;
}

int LoggerMessage::overflow(int c)
{
	m_bGotText = true;
	m_sMessage += static_cast<char>(c);
	return 0;
}

} //Logging
