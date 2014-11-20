/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
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
 $Log: meshio.cpp,v $
*****************************************************************************/
#include <Qt>

#include "io_vp.h"

#include "export_vp.h"
#include "import_vp.h"

#include <QMessageBox>
#include <QFileDialog>

#include "../../meshlab/mainwindow.h"
#include "../decorate_labels/decorate_labels.h"

using namespace vcg;

bool parseVP(const std::string &filename, CMeshO &m);

std::map<int,QString> vcg::tri::io::ExporterVP<CMeshO>::g_Bits;
std::map<int,QString> vcg::tri::io::ImporterVP<CMeshO>::g_Bits;

//void vpIOPlugin::initPreOpenParameter(const QString &format, const QString &/*fileName*/, RichParameterSet & parlst)
//{
//	if(format.toUpper() == tr("ASC"))
//	{
//			parlst.addParam(new RichInt("rowToSkip",0,"Header Row to be skipped","The number of lines that must be skipped at the beginning of the file."));
//			parlst.addParam(new RichBool("triangulate", true, "Grid triangulation", "if true it assumes that the points are arranged in a complete xy grid and it tries to perform a naive height field triangulation of the input data.  Lenght of the lines is detected automatically by searching x jumps. If the input point cloud data is not arranged as a xy regular height field, no triangles are created."));
//	}
//}

vpIOPlugin::vpIOPlugin()
{
	m_Landmarks<<"SS"<<"CS"<<"LLL"<<"RLL"<<"LAF"<<"LAR"<<"RAF"<<"RAR"<<"CB"<<"BH"<<"LA"<<"RA"<<"LAT"<<"LAB"<<"RAT"<<"RAB";
}

bool vpIOPlugin::open(const QString &formatName, const QString &fileName, MeshModel &m, int& mask, const RichParameterSet &parlst, CallBackPos *cb, QWidget *parent)
{
	bool result=0;
	if(formatName.toUpper() == tr("VP"))
		{

			connect(this, SIGNAL(GetDecorationByName(QString)),(MainWindow*)parent,SLOT(GetDecorationByName(QString)) );
			DecorateLabelsPlugin* dec=(DecorateLabelsPlugin*)(GetDecorationByName("Show Vertex Labels"));
			connect(this, SIGNAL(getValue(QString)),dec,SLOT(getValue(QString)) );
			for(QList<QString>::iterator it=m_Landmarks.begin();it!=m_Landmarks.end();++it)
			{
				int tmpValue=0;
				if((tmpValue=getValue((*it)))!=0)
					vcg::tri::io::ImporterVP<CMeshO>::g_Bits[tmpValue]=(*it);
				else
					QMessageBox::warning(parent, tr("Saving Error"), "Mismatched Landmark data!");
				//vcg::tri::io::ExporterVP<CMeshO>::g_Bits
			}


			m.Enable(mask);	
			result= tri::io::ImporterVP<CMeshO>::Open(m.cm, qPrintable(fileName),cb);
			if (result != 0) // all the importers return 0 on success
			{
				errorMessage = QString("Failed to open:")+fileName;
				return false;
			}
			
		return true;
		}
	return result;
}

bool vpIOPlugin::save(const QString &formatName, const QString &fileName, MeshModel &m, const int mask, const RichParameterSet &, vcg::CallBackPos *cb, QWidget *parent)
{
	QString errorMsgFormat = "Error encountered while exportering file %1:\n%2";
	connect(this, SIGNAL(GetDecorationByName(QString)),(MainWindow*)parent,SLOT(GetDecorationByName(QString)) );
	DecorateLabelsPlugin* dec=(DecorateLabelsPlugin*)(GetDecorationByName("Show Vertex Labels"));
	connect(this, SIGNAL(getValue(QString)),dec,SLOT(getValue(QString)) );
	for(QList<QString>::iterator it=m_Landmarks.begin();it!=m_Landmarks.end();++it)
	{
		int tmpValue=0;
		if((tmpValue=getValue((*it)))!=0)
			vcg::tri::io::ExporterVP<CMeshO>::g_Bits[tmpValue]=(*it);
		else
			QMessageBox::warning(parent, tr("Saving Error"), "Mismatched Landmark data!");
		//vcg::tri::io::ExporterVP<CMeshO>::g_Bits
	}
	int result = vcg::tri::io::ExporterVP<CMeshO>::Save(m.cm,qPrintable(fileName),mask);
	if(result!=0)
	{
        QMessageBox::warning(parent, tr("Saving Error"), errorMsgFormat.arg(qPrintable(fileName), vcg::tri::io::ExporterVP<CMeshO>::ErrorMsg(result)));
		return false;
	}
	return true;
}

/*
	returns the list of the file's type which can be imported
*/
QList<MeshIOInterface::Format> vpIOPlugin::importFormats() const
{
	QList<Format> formatList;
	formatList 
	  << Format("VP (Polhemus vertices)", tr("VP"));
	return formatList;
}

/*
	returns the list of the file's type which can be exported
*/
QList<MeshIOInterface::Format> vpIOPlugin::exportFormats() const
{
	QList<Format> formatList;
	formatList 
	  << Format("VP (Polhemus vertices)", tr("VP"));
	return formatList;
}

/*
	returns the mask on the basis of the file's type. 
	otherwise it returns 0 if the file format is unknown
*/
void vpIOPlugin::GetExportMaskCapability(QString &format, int &capability, int &defaultBits) const
{
	capability=tri::io::ExporterVP<CMeshO>::GetExportMaskCapability();
	defaultBits=capability;
	return;
}
// 
//bool parseVP(const std::string &filename, CMeshO &m) {
//  FILE *fp = fopen(filename.c_str(), "rb");
//  if (!fp) {
//		return false;
//  }
//
//  int err = 0;
//
//  // Read header info
//  int numPoints, numFaces;
//  err |= readHeader(fp, numPoints, numFaces);
//
//  if (err) {
//   // Error::setError("Error parsing .tri\n");
//    fclose(fp);
//    return 1;
//  }
//	qDebug("Reading a mesh of %i vert and %i faces",numPoints,numFaces);
//	
//	tri::Allocator<CMeshO>::AddVertices(m,numPoints);
//	tri::Allocator<CMeshO>::AddFaces(m,numFaces);
//
//  // Read points
//  float x, y, z;
//  for (int i = 0; i < numPoints; ++i) {
//    err = readPoint(fp, x, y, z);
//		m.vert[i].P()=Point3f(x, y, z);
//  }
//
//  if (err) {
//    //Error::setError("Error parsing .tri\n");
//    fclose(fp);		
//		return false;
//  }
//
//  // Read faces
//  int p1, p2, p3;
//  for (int i = 0; i < numFaces; ++i) {
//    err |= readFace(fp, p1, p2, p3);
//		assert(p1>=0 && p1<numPoints);
//		assert(p2>=0 && p2<numPoints);
//		assert(p3>=0 && p3<numPoints);
//		
//    m.face[i].V(0)= &m.vert[p1];
//    m.face[i].V(1)= &m.vert[p2];
//    m.face[i].V(2)= &m.vert[p3];
//  }
// 	
//	QString texPNG(filename.c_str());
//	texPNG = texPNG.left(texPNG.length()-3)+"PNG";
//	QImage texturePNG;
//	texturePNG.load(texPNG);
//	if(!texturePNG.isNull())
//	{
//		qDebug("Image Loaded %s has %i keys",qPrintable(texPNG),texturePNG.textKeys().size());
//		QString infoPNG=texturePNG.text("uv");
//		if(!infoPNG.isNull())
//		{
//			m.textures.push_back(qPrintable(texPNG));
//			qDebug("Loading texture %s",qPrintable(texPNG));
//			QStringList numList = infoPNG.split(" ", QString::SkipEmptyParts);
//			qDebug("Found %i numbers for %i faces",numList.size(),numFaces);
//			for (int i = 0; i < numFaces ; ++i) 
//						{
//							for(int j=0;j<3;++j)
//									{
//										float s=(numList.at(i*6+j*2+0).toInt())/float(texturePNG.width());
//										float t=(numList.at(i*6+j*2+1).toInt())/float(texturePNG.height());
//										m.face[i].WT(j).U()=s;				
//										m.face[i].WT(j).V()=1.0-t;				
//										m.face[i].WT(j).N()=0;
//									}
//						}
//			return true;
//		}
//	}
//
//	QString texJPG(filename.c_str());
//	texJPG = texJPG.left(texJPG.length()-3)+"JPG";
//	QImage textureJPG;
//	textureJPG.load(texJPG);
//	if(textureJPG.isNull())
//				return false;
//
//  if (!feof(fp) ) {
//			char texCode[4];
//			bool floatFlag=false;
//			
//			int ret=fread(texCode,sizeof(char),4,fp);
//			qDebug("TexString code is '%s' (int:%d) (float:%f)\n", texCode, *(int*)(&texCode[0]),  *(float*)(&texCode[0]));
//
//			if(feof(fp)){
//					qDebug("Premature end of file");
//					return false;
//				}
//
//			if(texCode==QString("TC00")) floatFlag=false;
//			
//			m.textures.push_back(qPrintable(texJPG));
//			qDebug("Loading texture %s",qPrintable(texJPG));
//			
//			for (int i = 0; i < numFaces ; ++i) 
//			{
//				if(feof(fp)){
//					qDebug("Premature end of file after reading %i tex faces",i);
//					return false;
//				}
//				float s,t;
//				short ss,ts;
//				for(int j=0;j<3;++j)
//					{
//						if(floatFlag){
//							fread(&s,sizeof(float),1,fp);
//							fread(&t,sizeof(float),1,fp);
//						} else {
//							fread(&ss,sizeof(short),1,fp);
//							fread(&ts,sizeof(short),1,fp);
//							s=ss/float(textureJPG.width());
//							t=ts/float(textureJPG.height());
//						}
//						
//						m.face[i].WT(j).U()=s;				
//						m.face[i].WT(j).V()=1.0-t;				
//						m.face[i].WT(j).N()=0;
//					}
//			} 
//  } // if ! eof
//  fclose(fp);
//
//  if (err) return false;
//  
//
//  return true;
//}


 
 
MESHLAB_PLUGIN_NAME_EXPORTER(vpIOPlugin)
