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

#include "RSSFeedParser.h"

using namespace Osp::Base;
using namespace Osp::Net::Http;
using namespace Osp::Xml;
using Osp::Base::Utility::StringUtil;
using Osp::Base::Utility::Uri;
using Osp::Base::Collection::ArrayList;
using Osp::Base::Collection::HashMap;

RSSFeedParser::RSSFeedParser(void) :
__pBuffer(null),
__bodyLen(-1),
__pSession(null)
{
}

RSSFeedParser::~RSSFeedParser(void) {
}

result
RSSFeedParser::RetrieveFeed(const Osp::Base::String &feedURL, const IFeedParserListener& listener) {
	result r = E_SUCCESS;

	__pListener = const_cast<IFeedParserListener *>(&listener);

	Uri *feedUri = new Uri();
	feedUri->SetUri(feedURL);

	// start request
	HttpGet(feedUri->GetHost(), feedURL);

	return r;
}

result
RSSFeedParser::HttpGet(const String &hostAddr, const String &requestAddr) {
	result r = E_SUCCESS;
	HttpTransaction *pTransaction = null;
	HttpRequest *pRequest = null;

	if (__pBuffer) {
		delete __pBuffer;
		__pBuffer = null;
	}
	if (__bodyLen > -1) {
		__bodyLen = -1;
	}
	if (__pSession) {
		delete __pSession;
		__pSession = null;
	}
	if (__pSession == null) {
		__pSession = new HttpSession();
		r = __pSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, hostAddr, null);
		if (IsFailed(r)) {
			goto CATCH;
		}
	}

	pTransaction = __pSession->OpenTransactionN();
	if (null == pTransaction) {
		goto CATCH;
	}

	r = pTransaction->AddHttpTransactionListener(*this);
	if (IsFailed(r)) {
		goto CATCH;
	}

	pRequest = const_cast<HttpRequest*>(pTransaction->GetRequest());
	if (pRequest == null) {
		goto CATCH;
	}

	r = pRequest->SetUri(requestAddr);
	if (IsFailed(r)) {
		goto CATCH;
	}

	r = pRequest->SetMethod(NET_HTTP_METHOD_GET);
	if (IsFailed(r)) {
		goto CATCH;
	}

	r = pTransaction->Submit();
	if (IsFailed(r)) {
		goto CATCH;
	}

	return r;

CATCH:
	__pListener->OnParsingFailure(*(new String(GetErrorMessage(r))));
	return r;
}

result
RSSFeedParser::ParseFeed() {
	result r = E_SUCCESS;

	char *strBody = new char[__bodyLen + 1];

	int index;
	for (index = 0; index < __bodyLen; ++index) {
		byte b;

		if (__pBuffer->GetByte(b) == E_SUCCESS) {
			strBody[index] = (char)b;
		}
	}
	strBody[index] = '\0';

	xmlDoc *doc = NULL;
	xmlXPathContextPtr xpathCtx;
	xmlXPathObjectPtr xpathObj;

	ArrayList *pItems;

	int len = strlen(strBody);

	doc = xmlReadMemory(strBody, len, "xml", NULL, 0);
	if (doc == NULL) {
		__pListener->OnParsingFailure(*(new String(L"Error loading XML.")));
		return E_FAILURE;
	}


	// create XPath context
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		xmlFreeDoc(doc);
		goto CATCH;
	}

	// evaluate XPath expression
	xpathObj = xmlXPathEvalExpression((xmlChar*)"//item", xpathCtx);
	if (xpathObj == NULL) {
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		goto CATCH;
	}

	pItems = new ArrayList();
	pItems->Construct();

	r = GetXPathItems(xpathObj->nodesetval, *pItems);

	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	if (IsFailed(r)) {
		goto CATCH;
	}

	__pListener->OnParsingCompleted(*pItems);

	return r;

CATCH:
	AppLog("ParseFeed() failed. (%s)", GetErrorMessage(r));
	__pListener->OnParsingFailure(*(new String(L"Parsing Failed.")));
	return r;
}

result
RSSFeedParser::GetXPathItems(Osp::Xml::xmlNodeSetPtr nodes, Osp::Base::Collection::ArrayList &items) {
	result r = E_SUCCESS;

	int size;
	int i;
	HashMap *pItem = null;
	String nodeName;
	String nodeText;
	size = (nodes) ? nodes->nodeNr : 0;

	// iterate over items
	for (i = 0; i < size; i++) {
		// author is the last child of item
		xmlNodePtr itemChildNode;

		pItem = new HashMap();
		pItem->Construct();

		// iterate over elements of item
		for (itemChildNode = nodes->nodeTab[i]->children; itemChildNode != NULL; itemChildNode = itemChildNode->next) {
			if (itemChildNode->type == XML_ELEMENT_NODE) {

				StringUtil::Utf8ToString((char*)itemChildNode->name, nodeName);
				StringUtil::Utf8ToString((char*)itemChildNode->children->content, nodeText);

				// add text content to hashmap with element name as key.
				pItem->Add(*(new String(nodeName)), *(new String(nodeText)));
			}
		}

		// add item hashmap to items list
		items.Add(*pItem);
	}

	return r;
}

// IHttpTransactionEventListener
void
RSSFeedParser::OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen) {
	HttpResponse* pHttpResponse = httpTransaction.GetResponse();

	if (pHttpResponse->GetStatusCode() == NET_HTTP_STATUS_OK) {
		// read content of RSS file
		__pBuffer = pHttpResponse->ReadBodyN();
		__bodyLen = availableBodyLen;
	}

	// now parse
	ParseFeed();
}

void
RSSFeedParser::OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r) {
	delete &httpTransaction;
	delete __pSession;
	__pSession = null;

	__pListener->OnParsingFailure(*(new String(L"Download Failed.")));
}

void
RSSFeedParser::OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize) {
}

void
RSSFeedParser::OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs) {
}

void
RSSFeedParser::OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction) {
	delete &httpTransaction;
	delete __pSession;
	__pSession = null;
}

void
RSSFeedParser::OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert) {
	// we do not want to support https so we do not resume
	//httpTransaction.Resume();
}

