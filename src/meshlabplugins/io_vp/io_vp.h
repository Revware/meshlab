/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History

 $Log: meshio.h,v $
 
 *****************************************************************************/
#pragma once

#include <common/interfaces.h>

class vpIOPlugin : public QObject, public MeshIOInterface
{
  Q_OBJECT
	MESHLAB_PLUGIN_IID_EXPORTER(MESH_IO_INTERFACE_IID)
  Q_INTERFACES(MeshIOInterface)

public:
	vpIOPlugin();
	QList<Format> importFormats() const;
	QList<Format> exportFormats() const;
	virtual void GetExportMaskCapability(QString &format, int &capability, int &defaultBits) const;
	//virtual void initPreOpenParameter(const QString &/*format*/, const QString &/*fileName*/, RichParameterSet & /*par*/);

	bool open(const QString &formatName, const QString &fileName, MeshModel &m, int& mask, const RichParameterSet &, vcg::CallBackPos *cb=0, QWidget *parent=0);
	bool save(const QString &formatName, const QString &fileName, MeshModel &m, const int mask, const RichParameterSet &, vcg::CallBackPos *cb, QWidget *parent);
private:
	QList<QString>	m_Landmarks;//<<"SS"<<"R"<<"CS"<<"LLL"<<"RLL"<<"AP"<<"LAF"<<"LAR"<<"RAF"<<"RAR"<<"LLI"<<"RLI";//traditionally 1-15 on SS and R
signals:
	int getValue(QString);
	MeshDecorateInterface* GetDecorationByName(QString name);//"Show Vertex Labels"
};
