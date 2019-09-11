#pragma once
#include <QtGlobal>

enum class PacketType : qint16
{
	Invalid = -1,
	Presentation,
	Login,

	ProcessList,
	Shell,

	Camera,
	CameraConfig,
	CameraError,

	Desktop,
	DesktopConfig,

	Chat,
	Connection,


	MinPacketType = Presentation,
	MaxPacketType = Connection,
};