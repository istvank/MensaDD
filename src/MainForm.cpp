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

#include "MainForm.h"

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::App;
using Osp::Base::Collection::HashMap;
using Osp::Base::Collection::ArrayList;
using Osp::Base::Collection::IEnumerator;

MainForm::MainForm(void) :
__pContextMenuDay(null),
__pOptionMenu(null),
__pListFood(null),
__pAnimation(null),
__pPopupAbout(null),
__pButtonAboutClose(null)
{
}

MainForm::~MainForm(void)
{
}

result
MainForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	__animationFrameList.RemoveAll(true);

	delete __pContextMenuDay;
	delete __pOptionMenu;
	delete __pMenuParser;
	delete __pMainItemFormat;
	delete __pSubItemFormat;
	if (__pPopupAbout) {
		delete __pPopupAbout;
	}

	return r;
}

bool
MainForm::Initialize()
{
	// Construct an XML form
	Construct(L"IDF_MAINFORM");

	__pMenuParser = new RSSFeedParser();

	return true;
}

void
MainForm::CreateOptionMenu(void) {
	String captionAbout;
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	pAppResource->GetString("IDS_ABOUT", captionAbout);

	__pOptionMenu = new OptionMenu();
	__pOptionMenu->Construct();
	__pOptionMenu->AddItem(captionAbout, ID_OPTIONMENU_ABOUT);
	__pOptionMenu->AddActionEventListener(*this);

	SetOptionkeyActionId(ID_OPTIONMENU);
	AddOptionkeyActionListener(*this);
}

void
MainForm::CreateContextMenuDay(void) {
	String captionToday;
	String captionTomorrow;
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	pAppResource->GetString("IDS_TODAY", captionToday);
	pAppResource->GetString("IDS_TOMORROW", captionTomorrow);

	__pContextMenuDay = new ContextMenu();
	__pContextMenuDay->Construct(Point(405, 105), CONTEXT_MENU_STYLE_LIST);
	__pContextMenuDay->AddItem(captionToday, ID_CONTEXT_TODAY);
	__pContextMenuDay->AddItem(captionTomorrow, ID_CONTEXT_TOMORROW);
	__pContextMenuDay->AddActionEventListener(*this);
}

void
MainForm::CreateLoadingAnimation() {
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();

	Bitmap *pBitmap1 = pAppResource->GetBitmapN("/progressingblue/progressing00.png");
	Bitmap *pBitmap2 = pAppResource->GetBitmapN("/progressingblue/progressing01.png");
	Bitmap *pBitmap3 = pAppResource->GetBitmapN("/progressingblue/progressing02.png");
	Bitmap *pBitmap4 = pAppResource->GetBitmapN("/progressingblue/progressing03.png");
	Bitmap *pBitmap5 = pAppResource->GetBitmapN("/progressingblue/progressing04.png");
	Bitmap *pBitmap6 = pAppResource->GetBitmapN("/progressingblue/progressing05.png");
	Bitmap *pBitmap7 = pAppResource->GetBitmapN("/progressingblue/progressing06.png");
	Bitmap *pBitmap8 = pAppResource->GetBitmapN("/progressingblue/progressing07.png");

	// Create AnimationFrames
	long duration = 500 / 8;
	AnimationFrame *pAniFrame1 = new AnimationFrame(*pBitmap1, duration);
	AnimationFrame *pAniFrame2 = new AnimationFrame(*pBitmap2, duration);
	AnimationFrame *pAniFrame3 = new AnimationFrame(*pBitmap3, duration);
	AnimationFrame *pAniFrame4 = new AnimationFrame(*pBitmap4, duration);
	AnimationFrame *pAniFrame5 = new AnimationFrame(*pBitmap5, duration);
	AnimationFrame *pAniFrame6 = new AnimationFrame(*pBitmap6, duration);
	AnimationFrame *pAniFrame7 = new AnimationFrame(*pBitmap7, duration);
	AnimationFrame *pAniFrame8 = new AnimationFrame(*pBitmap8, duration);

	__animationFrameList.Construct();
	__animationFrameList.Add(*pAniFrame1);
	__animationFrameList.Add(*pAniFrame2);
	__animationFrameList.Add(*pAniFrame3);
	__animationFrameList.Add(*pAniFrame4);
	__animationFrameList.Add(*pAniFrame5);
	__animationFrameList.Add(*pAniFrame6);
	__animationFrameList.Add(*pAniFrame7);
	__animationFrameList.Add(*pAniFrame8);

	__pAnimation = new Animation();
	__pAnimation->Construct(Rectangle(212,372,56,56), __animationFrameList);
	__pAnimation->SetRepeatCount(10000);
	//__pAnimation->AddAnimationEventListener(*this);
	__pAnimation->SetShowState(false);
	AddControl(*__pAnimation);
}

void
MainForm::StartLoadingAnimation() {
	__pAnimation->SetShowState(true);
	__pAnimation->Play();
}

void
MainForm::StopLoadingAnimation() {
	__pAnimation->SetShowState(false);
	__pAnimation->Stop();
}

void
MainForm::ShowAboutPopup() {
	if (__pPopupAbout == null) {
		// lazy loading (not on form initialization)
		__pPopupAbout = new Popup();
		__pPopupAbout->Construct("IDP_ABOUT");
		__pButtonAboutClose = static_cast<Button *>(__pPopupAbout->GetControl("IDC_ABOUT_BUTTON"));
		__pButtonAboutClose->SetActionId(ID_BUTTON_ABOUTCLOSE);
		__pButtonAboutClose->AddActionEventListener(*this);
		EditArea *pEditAreaAbout = static_cast<EditArea *>(__pPopupAbout->GetControl("IDC_ABOUT_EDITAREA"));
		pEditAreaAbout->SetKeypadEnabled(false);
		pEditAreaAbout->SetCursorPosition(0);
	}
	__pPopupAbout->SetShowState(true);
	__pPopupAbout->Show();
}

void
MainForm::HideAboutPopup() {
	__pPopupAbout->SetShowState(false);
	Draw();
	Show();
}

result
MainForm::RetrieveMenu(Day day) {
	result r = E_SUCCESS;

	String emptyText;
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	pAppResource->GetString("IDS_LOADING", emptyText);
	__pListFood->SetTextOfEmptyList(emptyText);
	__pListFood->RequestRedraw();

	currentDay = day;

	String strReq(L"http://www.studentenwerk-dresden.de/feeds/speiseplan.rss");

	switch (day) {
	case Tomorrow:
		strReq.Append("?tag=morgen");
		break;
	default:
		break;
	}

	__pMenuParser->RetrieveFeed(strReq, *this);

	return r;
}

result
MainForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Get the button via resource ID
	__pButtonDay = static_cast<Button *>(GetControl(L"IDC_BUTTON_DAY"));
	if (__pButtonDay != null) {
		__pButtonDay->SetActionId(ID_BUTTON_DAY);
		__pButtonDay->AddActionEventListener(*this);
	}

	// set list reference
	__pListFood = static_cast<ExpandableList *>(GetControl(L"IDC_LIST_FOOD"));

	// set soft keys
	SetSoftkeyActionId(SOFTKEY_0, ID_SOFTKEY_0);
	AddSoftkeyActionListener(SOFTKEY_0, *this);
	SetSoftkeyActionId(SOFTKEY_1, ID_SOFTKEY_1);
	AddSoftkeyActionListener(SOFTKEY_1, *this);

	CreateOptionMenu();
	CreateContextMenuDay();

	// create ExpandableList main item format
	__pMainItemFormat = new CustomListItemFormat();
	__pMainItemFormat->Construct();
	__pMainItemFormat->AddElement(TEXT_ID, Rectangle(10, 25, 450, 80));
	//__pMainItemFormat->SetElementEventEnabled(TEXT_ID, false);

	// create ExpandableList sub item format
	__pSubItemFormat = new CustomListItemFormat();
	__pSubItemFormat->Construct();
	__pSubItemFormat->AddElement(TEXT_ID, Rectangle(10, 10, 350, 90));
	__pSubItemFormat->AddElement(PRICESTUDENT_ID, Rectangle(380, 10, 90, 35));
	Color grey = Color(8750729, false);
	__pSubItemFormat->AddElement(PRICESTAFF_ID, Rectangle(380, 55, 90, 35), 38, grey, grey);
	__pSubItemFormat->AddElement(REMARK_ID, Rectangle(390, 10, 90, 80), 38, Color::COLOR_RED, Color::COLOR_RED);

	// create loading animation
	CreateLoadingAnimation();
	// start animation
	StartLoadingAnimation();

	// load today's menu
	currentDay = Today;
	RetrieveMenu(currentDay);

	return r;
}

void
MainForm::OnActionPerformed(const Osp::Ui::Control& source, int actionId) {
	String caption;
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();

	switch(actionId) {
		case ID_BUTTON_DAY:
			AppLog("Day Button is clicked! \n");
			// show Context Menu
			__pContextMenuDay->SetShowState(true);
			__pContextMenuDay->Show();
			break;
		case ID_CONTEXT_TODAY:
			AppLog("Today chosen.");
			__pListFood->RemoveAllItems();
			__pListFood->RequestRedraw();
			StartLoadingAnimation();
			pAppResource->GetString("IDS_TODAY", caption);
			__pButtonDay->SetText(caption);
			__pButtonDay->RequestRedraw();
			// request menu
			RetrieveMenu(Today);
			break;
		case ID_CONTEXT_TOMORROW:
			AppLog("Tomorrow chosen.");
			__pListFood->RemoveAllItems();
			__pListFood->RequestRedraw();
			StartLoadingAnimation();
			pAppResource->GetString("IDS_TOMORROW", caption);
			__pButtonDay->SetText(caption);
			__pButtonDay->RequestRedraw();
			// request menu
			RetrieveMenu(Tomorrow);
			break;
		case ID_OPTIONMENU:
			__pOptionMenu->SetShowState(true);
			__pOptionMenu->Show();
			break;
		case ID_OPTIONMENU_ABOUT:
			AppLog("Showing about.");
			ShowAboutPopup();
			break;
		case ID_SOFTKEY_0:
			AppLog("Refreshing.");
			__pListFood->RemoveAllItems();
			__pListFood->RequestRedraw();
			StartLoadingAnimation();
			RetrieveMenu(currentDay);
			break;
		case ID_SOFTKEY_1:
			AppLog("Closing app.");
			Application::GetInstance()->Terminate();
			break;
		case ID_BUTTON_ABOUTCLOSE:
			AppLog("Closing popup.");
			HideAboutPopup();
			break;
		default:
			break;
	}
}

void
MainForm::OnParsingCompleted(ArrayList &feedData) {
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	if (feedData.GetCount() == 0) {
		// no service this day
		String emptyText;
		pAppResource->GetString("IDS_NOSERVICE", emptyText);
		__pListFood->SetTextOfEmptyList(emptyText);
	} else {
		IEnumerator *pEnum = feedData.GetEnumeratorN();
		HashMap *pItem = null;
		int mensaIndex = -1;
		String *currentMensa = null;
		String currencySymbol;
		pAppResource->GetString("IDS_CURRENCY", currencySymbol);
		String soldOut;
		pAppResource->GetString("IDS_SOLDOUT", soldOut);

		// iterate over items (meals) in feed
		while (pEnum->MoveNext() == E_SUCCESS) {
			pItem = (HashMap *)pEnum->GetCurrent();

			// check if mensa is already in the list (we assume the feed to be ordered)
			if (!currentMensa || !(static_cast<String *>(pItem->GetValue(*(new String("author")))))->Equals(*currentMensa, false)) {
				currentMensa = static_cast<String *>(pItem->GetValue(*(new String("author"))));
				mensaIndex++;
				// Create a main item of the ExpandableList
				CustomListItem* pMainItem = new CustomListItem();
				pMainItem->Construct(100);
				pMainItem->SetItemFormat(*__pMainItemFormat);
				pMainItem->SetElement(TEXT_ID, *(new String(*currentMensa)));
				// Add the item to the ExpandableList
				__pListFood->AddItem(*pMainItem, mensaIndex);
			}

			String *title = static_cast<String *>(pItem->GetValue(*(new String("title"))));
			title->Trim();
			String priceStudents;
			String priceStaff;
			int stringLength = title->GetLength();

			// Create a sub item of the ExpandableList
			CustomListItem *pSubItem = new CustomListItem();
			pSubItem->Construct(100);
			pSubItem->SetItemFormat(*__pSubItemFormat);

			// get prices
			if (title->EndsWith(L" EUR)")) {
				// parse price
				title->SubString(stringLength - 20, 5, priceStudents);
				priceStudents.Append(currencySymbol);
				title->SubString(stringLength - 9, 5, priceStaff);
				priceStaff.Append(currencySymbol);
				title->Remove(stringLength - 22, 22);

				pSubItem->SetElement(PRICESTUDENT_ID, priceStudents);
				pSubItem->SetElement(PRICESTAFF_ID, priceStaff);
			} else if (title->EndsWith(L"(ausverkauft)")) {
				// sold out
				title->Remove(stringLength - 14, 14);
				pSubItem->SetElement(REMARK_ID, soldOut);
			}

			pSubItem->SetElement(TEXT_ID, *title);

			// Add sub item to the ExpandableList
			__pListFood->AddSubItem(mensaIndex, *pSubItem);
		}

		// clean up
		delete pEnum;
	}

	__pListFood->RequestRedraw();
	__pListFood->ScrollToTop();

	StopLoadingAnimation();
}

void
MainForm::OnParsingFailure(String &error) {
	String title;
	String text;
	String emptyText;
	AppResource *pAppResource = Application::GetInstance()->GetAppResource();
	pAppResource->GetString("IDS_APPNAME", title);
	pAppResource->GetString("IDS_FAILURE", text);
	pAppResource->GetString("IDS_ERROR", emptyText);

	__pListFood->SetTextOfEmptyList(emptyText);
	__pListFood->RequestRedraw();

	MessageBox *pMessageBox = new MessageBox();
	pMessageBox->Construct(title, text, MSGBOX_STYLE_OK );
	pMessageBox->Show();
	delete pMessageBox;

	StopLoadingAnimation();
}
