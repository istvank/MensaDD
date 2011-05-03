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

#include "MensaDD.h"
#include "MainForm.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;

MensaDD::MensaDD()
{
}

MensaDD::~MensaDD()
{
}

Application*
MensaDD::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new MensaDD();
}

bool
MensaDD::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize UI resources and application specific data.
	// The application's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the application will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// Create a form
	MainForm *pMainForm = new MainForm();
	pMainForm->Initialize();

	// Add the form to the frame
	Frame *pFrame = GetAppFrame()->GetFrame();
	pFrame->AddControl(*pMainForm);

	// Set the current form
	pFrame->SetCurrentForm(*pMainForm);

	// Draw and Show the form
	pMainForm->Draw();
	pMainForm->Show();

	return true;
}

bool
MensaDD::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this application for termination.
	// The application's permanent data and context can be saved via appRegistry.
	return true;
}

void
MensaDD::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
}

void
MensaDD::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void
MensaDD::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void
MensaDD::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void
MensaDD::OnScreenOn (void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void
MensaDD::OnScreenOff (void)
{
	// TODO:
	//  Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}
