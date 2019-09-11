#pragma once
#include <QtGlobal>

enum class ShellAction : qint8
{
	Start,
	Push,
	DownloadExec,
	MoreData = Push,
	Stop
};