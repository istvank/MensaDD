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

#ifndef _MENSADD_H_
#define _MENSADD_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>

/**
 * [MensaDD] application must inherit from Application class
 * which provides basic features necessary to define an application.
 */
class MensaDD :
	public Osp::App::Application,
	public Osp::System::IScreenEventListener
{
public:

	/**
	 * [MensaDD] application must have a factory method that creates an instance of itself.
	 */
	static Osp::App::Application* CreateInstance(void);


public:
	MensaDD();
	~MensaDD();


public:


	// Called when the application is initializing.
	bool OnAppInitializing(Osp::App::AppRegistry& appRegistry);

	// Called when the application is terminating.
	bool OnAppTerminating(Osp::App::AppRegistry& appRegistry, bool forcedTermination = false);


	// Called when the application's frame moves to the top of the screen.
	void OnForeground(void);


	// Called when this application's frame is moved from top of the screen to the background.
	void OnBackground(void);

	// Called when the system memory is not sufficient to run the application any further.
	void OnLowMemory(void);

	// Called when the battery level changes.
	void OnBatteryLevelChanged(Osp::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	void OnScreenOn (void);

	// Called when the screen turns off.
	void OnScreenOff (void);
};

#endif
