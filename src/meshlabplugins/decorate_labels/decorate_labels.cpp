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

#include <math.h>
#include <limits>
#include <stdlib.h>
#include "decorate_labels.h"
#include <wrap/gl/addons.h>
#include <wrap/qt/gl_label.h>

#include <meshlab/glarea.h>
#include <common/pluginmanager.h>


using namespace vcg;

QString DecorateLabelsPlugin::decorationInfo(FilterIDType id) const
{
  switch(id)
    {
    //case DP_SHOW_CUBEMAPPED_ENV :      return tr("Draws a customizable cube mapped background that is sync with trackball rotation");
		case DP_SHOW_LABELS:      return tr("Draws a label on verteces based on flag values");
		case DP_SHOW_BORDER :      return tr("Paints Border verteces green");
    }
    assert(0);
    return QString();
}
QString DecorateLabelsPlugin::decorationName(FilterIDType id) const
{
    switch(id)
    {
    //case DP_SHOW_CUBEMAPPED_ENV :      return tr("Cube mapped background");
		case DP_SHOW_LABELS :      return tr("Show Vertex Labels");
		case DP_SHOW_BORDER :      return tr("Paint Border");
    }
    assert(0);
    return QString();
}

void DecorateLabelsPlugin::initGlobalParameterSet(QAction *action, RichParameterSet &parset)
{
  switch(ID(action)){
  //case DP_SHOW_CUBEMAPPED_ENV :
  //if(!parset.hasParameter(CubeMapPathParam()))
  //    {
  //  QString cubemapDirPath = PluginManager::getBaseDirPath() + QString("/textures/cubemaps/uffizi.jpg");
  //  //parset.addParam(new RichString(CubeMapPathParam(), cubemapDirPath,"",""));
  //}
  //break;
  case DP_SHOW_LABELS :
      //parset.addParam(new RichFloat(BoxRatioParam(),2.0,"Box Ratio","The size of the grid around the object w.r.t. the bbox of the object"));
      //parset.addParam(new RichFloat(GridMajorParam(),10,"Major Spacing",""));
      //parset.addParam(new RichFloat(GridMinorParam(),1,"Minor Spacing",""));
      //parset.addParam(new RichBool(GridBackParam(),true,"Front grid culling",""));
      //parset.addParam(new RichBool(ShowShadowParam(),false,"Show silhouette",""));
      //parset.addParam(new RichColor(GridColorBackParam(),QColor(Color4b::Gray),"Back Grid Color",""));
      //parset.addParam(new RichColor(GridColorFrontParam(),QColor(Color4b::Gray),"Front grid Color",""));

    break;
  case DP_SHOW_BORDER:break;
  }
}

bool DecorateLabelsPlugin::startDecorate( QAction * action, MeshDocument &/*m*/, RichParameterSet * parset, GLArea * gla)
{
  switch(ID(action)){
    //case DP_SHOW_CUBEMAPPED_ENV :
    //if(parset->findParameter(CubeMapPathParam())== NULL) qDebug("CubeMapPath was not setted!!!");
    //  cubemapFileName = parset->getString(CubeMapPathParam());
    //break;
  case DP_SHOW_LABELS:

  break;
  case DP_SHOW_BORDER:

  break;
  }
    return true;
}

bool DecorateLabelsPlugin::startDecorate( QAction * action, MeshModel &m, RichParameterSet * parset, GLArea * gla)
{
  switch(ID(action)){
    //case DP_SHOW_CUBEMAPPED_ENV :
    //if(parset->findParameter(CubeMapPathParam())== NULL) qDebug("CubeMapPath was not setted!!!");
    //  cubemapFileName = parset->getString(CubeMapPathParam());
    //break;
  case DP_SHOW_LABELS:

  break;
  case DP_SHOW_BORDER:

  break;
  }
    return true;
}
//void DecorateLabelsPlugin::decorateDoc(QAction *a, MeshDocument &m, RichParameterSet * parset,GLArea *gla, QPainter *, GLLogStream &)
//{
//  static QString lastname("unitialized");
//    switch(ID(a))
//    {
//    //  case DP_SHOW_CUBEMAPPED_ENV :
//    //    {
//    //  if(!cm.IsValid() || (lastname != cubemapFileName ) )
//    //  {
//    //    qDebug( "Current CubeMapPath Dir: %s ",qPrintable(cubemapFileName));
//    //    glewInit();
//    //    bool ret = cm.Load(qPrintable(cubemapFileName));
//    //    lastname=cubemapFileName;
//    //    if(! ret ) return;
//    //    //QMessageBox::warning(gla,"Cubemapped background decoration","Warning unable to load cube map images: " + cubemapFileName );
//    //    cm.radius=10;
//    //  }
//    //        if(!cm.IsValid()) return;
//
//    //  Matrix44f tr;
//    //        glGetv(GL_MODELVIEW_MATRIX,tr);
//    //        // Remove the translation from the current matrix by simply padding the last column of the matrix
//    //  tr.SetColumn(3,Point4f(0,0,0,1.0));
//    //        //Remove the scaling from the the current matrix by adding an inverse scaling matrix
//    //        float scale = 1.0/pow(tr.Determinant(),1.0f/3.0f);
//    //        Matrix44f Scale;
//    //        Scale.SetDiagonal(scale);
//    //        tr=tr*Scale;
//
//    //        glMatrixMode(GL_PROJECTION);
//    //        glPushMatrix();
//    //        glMatrixMode(GL_MODELVIEW);
//    //        glPushMatrix();
//    //            cm.DrawEnvCube(tr);
//    //        glPopMatrix();
//    //        glMatrixMode(GL_PROJECTION);
//    //        glPopMatrix();
//    //        glMatrixMode(GL_MODELVIEW);
//    //} break;
//  case DP_SHOW_LABELS :
//    {
//		m.meshList
//    } break;
//  }
//}

void DecorateLabelsPlugin::decorateMesh(QAction *a, MeshModel &mm, RichParameterSet *parset, GLArea *gla, QPainter *p, GLLogStream &)
{
    switch(ID(a))
    {
		case DP_SHOW_LABELS :
			{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_LIGHTING);
	//glColor3f(.4f,.0f,.0f);
	//glColor(Color4b::Red);

				for(CMeshO::VertexIterator vi=mm.cm.vert.begin();vi!=mm.cm.vert.end();++vi)
				{
					for(std::map<int,QString>::iterator flag_it=m_Labels.begin();flag_it!=m_Labels.end();++flag_it)
					{
						if((*vi).IsUserBit((*flag_it).first))
						{
							//draw the label
							glLabel::Mode m;
							m.color=Color4b(Color4b::Red);;
							glLabel::render(p, (*vi).P(),(*flag_it).second,m);
							//glLabel::render(p, (*vi).P(),(*flag_it).second);
						}
					}
				}
	glPopAttrib();
				break;
			}
		case DP_SHOW_BORDER:
			{
    glPushAttrib(GL_ENABLE_BIT|GL_VIEWPORT_BIT|	  GL_CURRENT_BIT |  GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor(Color4b::Black);
    glDepthRange (0.0, 0.9999);
    glDepthFunc(GL_LEQUAL);
	glBegin(GL_POINTS);
	glColor(vcg::Color4b::Green/*mm.cm.C()*/);

				for(CMeshO::VertexIterator vi=mm.cm.vert.begin();vi!=mm.cm.vert.end();++vi)
					if(!(*vi).IsD()&&(*vi).IsB())
					{
						glVertex((*vi).P());
					}
    glEnd();
	glBegin(GL_LINES);
	//glColor(vcg::Color4b::Green/*mm.cm.C()*/);
				for(CMeshO::EdgeIterator ei=mm.cm.edge.begin();ei!=mm.cm.edge.end();++ei)
					if(!(*ei).IsD()&&((*ei).IsB()||(*ei).V(0)->IsB()))
					{
						glVertex((*ei).V(0)->P());
						glVertex((*ei).V(1)->P());
					}
    glEnd();
    glPopAttrib();
	break;
			}
	}
}

void  DecorateLabelsPlugin::setValue(int flag,QString labeltext)
{
	m_Labels[flag]=labeltext;
}

bool  DecorateLabelsPlugin::hasValue(QString labeltext)
{
	for(std::map<int,QString>::iterator flag_it=m_Labels.begin();flag_it!=m_Labels.end();++flag_it)
	{
		if(labeltext.compare((*flag_it).second)==0)
		{
			return true;
		}
	}
	return false;
}

 int  DecorateLabelsPlugin::getValue(QString labeltext)
 {
	for(std::map<int,QString>::iterator flag_it=m_Labels.begin();flag_it!=m_Labels.end();++flag_it)
	{
		if(labeltext.compare((*flag_it).second)==0)
		{
			return (*flag_it).first;
		}
	}
	return 0;
 }


MESHLAB_PLUGIN_NAME_EXPORTER(DecorateLabelsPlugin)
