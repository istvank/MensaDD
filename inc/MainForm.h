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

#ifndef _MAINFORM_H_
#define _MAINFORM_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>
#include <FApp.h>
#include "RSSFeedParser.h"

enum Day {
	Today,
	Tomorrow
};

class MainForm :
	public Osp::Ui::Controls::Form,
	public Osp::Ui::IActionEventListener,
	public IFeedParserListener
{

// Construction
public:
	MainForm(void);
	virtual ~MainForm(void);
	bool Initialize(void);

	void CreateOptionMenu(void);
	void CreateContextMenuDay(void);
	void CreateLoadingAnimation(void);

	void StartLoadingAnimation(void);
	void StopLoadingAnimation(void);

	void ShowAboutPopup(void);
	void HideAboutPopup(void);

	result RetrieveMenu(Day day);

private:
    Osp::Ui::Controls::CustomListItemFormat *__pMainItemFormat;
    Osp::Ui::Controls::CustomListItemFormat *__pSubItemFormat;

// Implementation
protected:
	// Controls
	Osp::Ui::Controls::Button *__pButtonDay;
	Osp::Ui::Controls::ContextMenu *__pContextMenuDay;
	Osp::Ui::Controls::OptionMenu *__pOptionMenu;
	Osp::Ui::Controls::ExpandableList *__pListFood;
	Osp::Ui::Controls::Animation* __pAnimation;
	Osp::Base::Collection::ArrayList __animationFrameList;
	Osp::Ui::Controls::Popup *__pPopupAbout;
	Osp::Ui::Controls::Button *__pButtonAboutClose;
	// Actions
	static const int ID_SOFTKEY_0 = 100;
	static const int ID_SOFTKEY_1 = 101;
	static const int ID_OPTIONMENU = 102;
	static const int ID_OPTIONMENU_ABOUT = 103;
	static const int ID_BUTTON_DAY = 200;
	static const int ID_CONTEXT_TODAY = 201;
	static const int ID_CONTEXT_TOMORROW = 202;
	static const int ID_BUTTON_ABOUTCLOSE = 203;
	static const int ITEM_MAIN_ID = 301;
	static const int ITEM_SUB_ID = 302;
	static const int TEXT_ID = 303;
	static const int PRICESTUDENT_ID = 304;
	static const int PRICESTAFF_ID = 305;
	static const int REMARK_ID = 306;

	// members
	RSSFeedParser *__pMenuParser;
	Day currentDay;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IFeedParserListener
	virtual void OnParsingCompleted(Osp::Base::Collection::ArrayList &feedData);
	virtual void OnParsingFailure(Osp::Base::String &error);

};

#endif	//_MAINFORM_H_
