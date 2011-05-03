/**********************************************************************
 * (C) Copyright 2011, Istvan Koren
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the LICENSE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **********************************************************************/

#ifndef RSSFEEDPARSER_H_
#define RSSFEEDPARSER_H_

#include <FBase.h>
#include <FNet.h>
#include <FXml.h>

class IFeedParserListener :
	public Osp::Base::Runtime::IEventListener,
	public Osp::Base::Object
{
public:
	virtual void OnParsingCompleted(Osp::Base::Collection::ArrayList &feedData)=0;
	virtual void OnParsingFailure(Osp::Base::String &error)=0;
};

class RSSFeedParser :
	public Osp::Net::Http::IHttpTransactionEventListener
{
public:
	RSSFeedParser(void);
	virtual ~RSSFeedParser(void);

	result RetrieveFeed(const Osp::Base::String &feedURL, const IFeedParserListener &listener);

// Implementation
protected:
	Osp::Base::ByteBuffer *__pBuffer;
	int __bodyLen;
	Osp::Net::Http::HttpSession *__pSession;

public:
	result HttpGet(const Osp::Base::String &hostAddr, const Osp::Base::String &requestAddr);
	result ParseFeed(void);
	// feed specific
	result GetXPathItems(Osp::Xml::xmlNodeSetPtr nodes, Osp::Base::Collection::ArrayList &items);

private:
	IFeedParserListener* __pListener;

public:
	// IHttpTransactionEventListener
	virtual void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs);
	virtual void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert);

};

#endif /* RSSFEEDPARSER_H_ */
