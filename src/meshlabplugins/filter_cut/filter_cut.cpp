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

#include "filter_cut.h"
#include <QtScript>

#include <vcg/space/index/grid_static_ptr.h>
#include <vcg/complex/algorithms/closest.h>

using namespace vcg;

class GetClosestFace
{

typedef GridStaticPtr<CMeshO::FaceType, CMeshO::ScalarType > MetroMeshGrid;
typedef tri::FaceTmark<CMeshO> MarkerFace;

public:
	
	GetClosestFace(){}
	
	void init(CMeshO *_m)
	{
		m=_m;
		if(m) 
		{
			unifGrid.Set(m->face.begin(),m->face.end());
			markerFunctor.SetMesh(m);
			
			dist_upper_bound = m->bbox.Diag()/10.0f;
		}
	}
	
	CMeshO *m;
	
	MetroMeshGrid unifGrid;
	
	MarkerFace markerFunctor;
	
	float dist_upper_bound;
	
	CMeshO::FaceType * getFace(vcg::Point3f &p) 
	{
		assert(m);
		// the results
		vcg::Point3f closestPt;
		float dist = dist_upper_bound;
		const CMeshO::CoordType &startPt = p;

		// compute distance between startPt and the mesh S2
		CMeshO::FaceType   *nearestF=0;
		vcg::face::PointDistanceBaseFunctor<CMeshO::ScalarType> PDistFunct;
		dist=dist_upper_bound;
		
		nearestF =  unifGrid.GetClosest(PDistFunct,markerFunctor,startPt,dist_upper_bound,dist,closestPt);
		
		if(dist == dist_upper_bound) qDebug() << "Dist is = upper bound";
		
		return nearestF;
	}
};

// Constructor usually performs only two simple tasks of filling the two lists 
//  - typeList: with all the possible id of the filtering actions
//  - actionList with the corresponding actions. If you want to add icons to your filtering actions you can do here by construction the QActions accordingly

MeshCuttingPlugin::MeshCuttingPlugin() 
{ 
	typeList << FP_CUT_ALONG_BORDER<<FP_ADD_BORDER<<FP_SELECT_ALONG_POINTS<<FP_SELECT_SHELL;
  
  foreach(FilterIDType tt , types())
	  actionList << new QAction(filterName(tt), this);
}

// ST() must return the very short string describing each filtering action 
// (this string is used also to define the menu entry)
QString MeshCuttingPlugin::filterName(FilterIDType filterId) const
{
  switch(filterId) {
		case FP_CUT_ALONG_BORDER :  return QString("Cut mesh along border"); 
		case FP_ADD_BORDER :  return QString("Add border points"); 
		case FP_SELECT_ALONG_POINTS :  return QString("Select closest faces"); 
		case FP_SELECT_SHELL: return QString("Select one contiguaous shell");
		default : assert(0); 
	}
  return QString();
}

// Info() must return the longer string describing each filtering action 
// (this string is used in the About plugin dialog)
 QString MeshCuttingPlugin::filterInfo(FilterIDType filterId) const
{
  switch(filterId) {
		case FP_CUT_ALONG_BORDER :  return QString("Slice mesh along another meshes border."); 
		case FP_ADD_BORDER :  return QString("Add another meshes border."); 
		case FP_SELECT_ALONG_POINTS :  return QString("Select faces closest to another meshes verteces."); 
		case FP_SELECT_SHELL :  return QString("Select a random 'shell' of congious faces."); 
		default : assert(0); 
	}
	return QString("Unknown Filter");
}

// The FilterClass describes in which generic class of filters it fits. 
// This choice affect the submenu in which each filter will be placed 
// More than a single class can be choosen.
MeshCuttingPlugin::FilterClass MeshCuttingPlugin::getClass(QAction *a)
{
  switch(ID(a))
	{
		case FP_CUT_ALONG_BORDER :  return MeshFilterInterface::MeshCreation; 
		case FP_ADD_BORDER :  return MeshFilterInterface::PointSet; 
		case FP_SELECT_ALONG_POINTS :  return MeshFilterInterface::Selection; 
		case FP_SELECT_SHELL :  return MeshFilterInterface::Selection; 
		default : assert(0); 
	}
	return MeshFilterInterface::Generic;
}

// This function define the needed parameters for each filter. Return true if the filter has some parameters
// it is called every time, so you can set the default value of parameters according to the mesh
// For each parameter you need to define, 
// - the name of the parameter, 
// - the string shown in the dialog 
// - the default value
// - a possibly long string describing the meaning of that parameter (shown as a popup help in the dialog)
void MeshCuttingPlugin::initParameterSet(QAction *action,MeshModel &m, RichParameterSet & parlst) 
{
	 switch(ID(action))	 {
		case FP_CUT_ALONG_BORDER :  		  
			parlst.addParam(new RichFloat("Distance",
												0,
												"Create points along the border at 'distace' intervals. \n\n",
												"0 will use only existing vertces."));
										break;		
		case FP_ADD_BORDER : 
 		  parlst.addParam(new RichBool ("Projected",
											true,
											"Project the points on to closest face",
											"Toggle the use of projection.\n\n"
											"If disabled the actual points will be used."));
		  parlst.addParam(new RichFloat("Distance",
												0,
												"Create points along the border at 'distace' intervals. \n\n",
												"0 will use only existing vertces."));
										break;
		case FP_SELECT_ALONG_POINTS :  
 		  parlst.addParam(new RichBool ("Border",
											true,
											"Use only the border of mesh",
											"Toggle the use of only the points marked as 'Border'.\n\n"
											"If disabled the all points will be used."));
 		  //parlst.addParam(new RichBool ("Adjacent",
					//						true,
					//						"Adjacent faces",
					//						"Toggle the selection of face adjacent to the .\n\n"
					//						"If disabled only the actual face will be selected."));
 		  parlst.addParam(new RichInt ("Depth",
											1,
											"Depth of Adjacent faces",
											"The selection of adjacent faces.\n\n"
											"0=Only projected face; 1=faces adjacent. 2+=faces adjacent to adjacent faces"));

		  parlst.addParam(new RichFloat("Distance",
												0,
												"Minimum Distance between point on the border. \n\n",
												"0 will use only existing vertces."));
											break;
		case FP_SELECT_SHELL: break;
		default : assert(0); 
	}
}

int MeshCuttingPlugin::SelectAdjacentFaces(CMeshO::FaceType* face,int depth=0)
{
	int faces=0;
	for(int i=0;i<3;++i)
	{
		face::Pos<CMeshO::FaceType> p(face,i);
		if(!p.FFlip()->IsS())
		{
			p.FFlip()->SetS();
			faces++;
			if(depth)
				faces+=SelectAdjacentFaces(p.FFlip(),depth-1);
		}
	}
	return faces;
}

bool MeshCuttingPlugin::applyFilter(QAction *filter, MeshDocument &md, RichParameterSet & par, vcg::CallBackPos *cb)
{
	CMeshO &m = md.mm()->cm;
	CMeshO &m2=(*md.meshList.begin())->cm;

	GetClosestFace getClosestFace;

	switch(ID(filter))
	{
		case FP_SELECT_SHELL:
			{
	md.mm()->updateDataMask(MeshModel::MM_FACEMARK);
	md.mm()->updateDataMask(MeshModel::MM_FACEFACETOPO);//FFAdjacencyEnabled

	for(CMeshO::FaceIterator fi=m.face.begin();fi!=m.face.end();++fi)
		if(!(*fi).IsD())
		{
			(*fi).SetS();
			break;
		}
	tri::UpdateSelection<CMeshO>::FaceConnectedFF(m);

	vcg::Point3f vect(0,0,0);
	vcg::Point3f vect2(0,0,0);

	//count the faces
	int faces1=0;
	int faces2=0;
	for(CMeshO::FaceIterator fi=m.face.begin();fi!=m.face.end();++fi)
		if(!(*fi).IsD())
		{
			//assert((*fi).HasNormal());
			if((*fi).IsS())
				//vect+=(*fi).cN();
				faces1++;
			else
				//vect2+=(*fi).cN();
				faces2++;

		}
	if(faces2<faces1)
	{
		tri::UpdateSelection<CMeshO>::FaceInvert(m);
	}

	//Log("Sum of selected Normals %f %f %f",vect.X(),vect.Y(),vect.Z());
	//Log("Sum of unselected Normals %f %f %f",vect2.X(),vect2.Y(),vect2.Z());

	break;
			}
		case FP_SELECT_ALONG_POINTS:
			{
	int faces=0;

	//need to update the mask
	md.mm()->updateDataMask(MeshModel::MM_FACEMARK);
	md.mm()->updateDataMask(MeshModel::MM_FACEFACETOPO);//FFAdjacencyEnabled
	float dist=0;
	if((dist=par.getFloat("Distance"))>0)
	{
		if(m2.edge.size()==0)
		{
			Log("Failed! The border contains no edges.");
			return false;
		}
		for(int j=0;j<m2.edge.size();++j)
		{
			cb(100*j/m2.edge.size(), "Searching edges...");
			m2.edge[j].V(0)->IsB();
			if(!par.getBool("Border")||m2.edge[j].V(0)->IsB()||m2.edge[j].V(1)->IsB())
			{
				getClosestFace.init(&m);

				//make a point list?
				double dist1=0, dist2=vcg::Distance(m2.edge[j].V(0)->P(),m2.edge[j].V(1)->P());
				std::vector<vcg::Point3f> _point_list;
				_point_list.push_back(m2.edge[j].V(0)->P());
				vcg::Point3f vect=m2.edge[j].V(1)->P()-m2.edge[j].V(0)->P();
				vect.Normalize();
				vect*=dist;
				while(dist2>dist1)
				{
					vcg::Point3f newpoint=(*_point_list.rbegin())+vect;
					_point_list.push_back(newpoint);
					dist1=vcg::Distance(m2.edge[j].V(0)->P(),newpoint);
				}
				_point_list.push_back(m2.edge[j].V(1)->P());

				for(int i=0;i<_point_list.size();++i)
				{
					CMeshO::FaceType* face=getClosestFace.getFace(_point_list[i]);
					face->SetS();
					int depth=par.getInt("Depth");
					//if(par.getBool("Adjacent"))
					if(depth)
					{
						faces+=SelectAdjacentFaces(face,depth-1);
					}
					faces++;
				}
			}
		}
	}else{
		for(int j=0;j<m2.vert.size();++j)
		{
			// Typical usage of the callback for showing a nice progress bar in the bottom. 
			// First parameter is a 0..100 number indicating percentage of completion, the second is an info string.		
			cb(100*j/m2.vert.size(), "Searching points...");
			if(!par.getBool("Border")||m2.vert[j].IsB())
			{
				getClosestFace.init(&m);
				CMeshO::FaceType* face=getClosestFace.getFace(m2.vert[j].P());
				face->SetS();
				int depth=par.getInt("Depth");
				//if(par.getBool("Adjacent"))
				if(depth)
				{
					faces+=SelectAdjacentFaces(face,depth-1);
				}
				faces++;
			}
		}
	}
	// Log function dump textual info in the lower part of the MeshLab screen. 
	Log("Successfully selected %i faces",faces);
	break;
			}
		case FP_ADD_BORDER:
			{
	float dist=0;
	if((dist=par.getFloat("Distance"))>0)
	{
		for(int j=0;j<m2.edge.size();++j)
		{
			cb(100*j/m2.edge.size(), "Searching edges...");
			m2.edge[j].V(0)->IsB();
			if(/*!par.getBool("Border")||*/m2.edge[j].V(0)->IsB()||m2.edge[j].V(1)->IsB())
			{
				//make a point list?
				double dist1=0, dist2=vcg::Distance(m2.edge[j].V(0)->P(),m2.edge[j].V(1)->P());
				std::vector<vcg::Point3f> _point_list;
				_point_list.push_back(m2.edge[j].V(0)->P());
				vcg::Point3f vect=m2.edge[j].V(1)->P()-m2.edge[j].V(0)->P();
				vect.Normalize();
				vect*=dist;
				while(dist2>dist1)
				{
					vcg::Point3f newpoint=(*_point_list.rbegin())+vect;
					_point_list.push_back(newpoint);
					dist1=vcg::Distance(m2.edge[j].V(0)->P(),newpoint);
				}
				_point_list.push_back(m2.edge[j].V(1)->P());
				
				//int start=m.vn;
				//tri::Allocator<CMeshO>::AddVertices(m,_point_list.size());

				for(int i=0;i<_point_list.size();++i)
				{
					if(par.getBool("Projected"))
					{
						//GetClosestFace getClosestFace;
						getClosestFace.init(&m);
						CMeshO::FaceType* face=getClosestFace.getFace(_point_list[i]);
						face->SetS();

						vcg::Point3f newpoint2;
						CMeshO::ScalarType dist;
						TrianglePointDistance<CMeshO::ScalarType>(vcg::Triangle3<CMeshO::ScalarType>(face->V(0)->P(),face->V(1)->P(),face->V(2)->P()),_point_list[i],dist,newpoint2);
						if(newpoint2==_point_list[i])
							continue;
						tri::Allocator<CMeshO>::AddVertex(m,newpoint2);
					}else{
						tri::Allocator<CMeshO>::AddVertex(m,_point_list[i]);
					}
				}

				Log("Added %i points",_point_list.size());
			}
		}
	}else{
		//std::vector<vcg::Point3f> _point_list;
		int points=0;
		for(int j=0;j<m2.vert.size();++j)
		{
			cb(100*j/m2.vert.size(), "Searching points...");
			if(/*!par.getBool("Border")||*/m2.vert[j].IsB())
			{
				//_point_list.push_back(m2.vert[j].P());
				if(par.getBool("Projected"))
				{
					//GetClosestFace getClosestFace;
					getClosestFace.init(&m);
					CMeshO::FaceType* face=getClosestFace.getFace(m2.vert[j].P());
					face->SetS();

					vcg::Point3f newpoint2;
					CMeshO::ScalarType dist;
					TrianglePointDistance<CMeshO::ScalarType>(vcg::Triangle3<CMeshO::ScalarType>(face->V(0)->P(),face->V(1)->P(),face->V(2)->P()),m2.vert[j].P(),dist,newpoint2);
					if(newpoint2==m2.vert[j].P())
						continue;
					tri::Allocator<CMeshO>::AddVertex(m,newpoint2);
				}else{
					tri::Allocator<CMeshO>::AddVertex(m,m2.vert[j].P());
				}
				points++;
			}
		}
		//int start=m.vn;
		//tri::Allocator<CMeshO>::AddVertices(m,_point_list.size());

		//for(int i=0;i<_point_list.size();++i)
		//{
		//	m.vert[i+start].P()=_point_list[i];
		//}

		Log("Added %i points",points);
	}
	break;
			}
		case FP_CUT_ALONG_BORDER:
			{
	int faces=0;

	//need to update the mask
	md.mm()->updateDataMask(MeshModel::MM_FACEMARK);
	md.mm()->updateDataMask(MeshModel::MM_FACEFACETOPO);//FFAdjacencyEnabled

#if 1
		for(int j=0;j<m2.edge.size();++j)
		{
			cb(100*j/m2.edge.size(), "Searching edges...");
			//m2.edge[j].V(0)->IsB();
			if(m2.edge[j].V(0)->IsB()||m2.edge[j].V(1)->IsB())
			{
				BreakFaceOnEdge(m,m2.edge[j].V(0)->P(),m2.edge[j].V(1)->P());
			}
		}
#else
	float dist=0;
	if((dist=par.getFloat("Distance"))>0)
	{
		for(int j=0;j<m2.edge.size();++j)
		{
			cb(100*j/m2.edge.size(), "Searching edges...");
			//m2.edge[j].V(0)->IsB();
			if(m2.edge[j].V(0)->IsB()||m2.edge[j].V(1)->IsB())
			{
				getClosestFace.init(&m);

				//make a point list?
				double dist1=0, dist2=vcg::Distance(m2.edge[j].V(0)->P(),m2.edge[j].V(1)->P());
				std::vector<vcg::Point3f> _point_list;
				_point_list.push_back(m2.edge[j].V(0)->P());
				vcg::Point3f vect=m2.edge[j].V(1)->P()-m2.edge[j].V(0)->P();
				vect.Normalize();
				vect*=dist;
				while(dist2>dist1)
				{
					vcg::Point3f newpoint=(*_point_list.rbegin())+vect;
					_point_list.push_back(newpoint);
					dist1=vcg::Distance(m2.edge[j].V(0)->P(),newpoint);
				}
				_point_list.push_back(m2.edge[j].V(1)->P());

				for(int i=0;i<_point_list.size();++i)
				{
					if(BreakFaceOnPoint(m,_point_list[i]))
						faces+=2;
				}
			}
		}
	}else{
		//std::vector<vcg::Point3f> _point_list;
		for(int j=0;j<m2.vert.size();++j)
		{
			// Typical usage of the callback for showing a nice progress bar in the bottom. 
			// First parameter is a 0..100 number indicating percentage of completion, the second is an info string.		
			cb(100*j/m2.vert.size(), "Searching points...");
			if(m2.vert[j].IsB())
			{
				if(BreakFaceOnPoint(m,m2.vert[j].P()))
					faces+=2;
			}
		}
	}
#endif
	// Log function dump textual info in the lower part of the MeshLab screen. 
	Log("Successfully added %i faces",faces);
	break;
			}
	}
  
	return true;
}

bool MeshCuttingPlugin::BreakFaceOnEdge(CMeshO& m,Point3f p0,Point3f p1)
{
	GetClosestFace getClosestFace;
	getClosestFace.init(&m);
	CMeshO::FaceType* face0=getClosestFace.getFace(p0);
	CMeshO::FaceType* face1=getClosestFace.getFace(p1);

	if(face0==face1)
	{
		vcg::Point3f newpoint0,newpoint1;
		CMeshO::ScalarType dist=0;
		bool para=false;
		int points=0;
		CMeshO::VertexType* npp[2];
		for(int i=0;i<3;++i)
		{
			SegmentSegmentDistance<CMeshO::ScalarType>(vcg::Segment3f(p0,p1),vcg::Segment3f(face0->V(i)->P(),face0->V((i+1)%3)->P()),dist,para,newpoint0,newpoint1);
			if(!para)
			{
				if(newpoint1==face0->V(i)->P()||newpoint1==face0->V((i+1)%3)->P())
					continue;
				CMeshO::VertexType* np=&(*tri::Allocator<CMeshO>::AddVertex(m,newpoint1));
				np->SetS();
				npp[points]=np;
				points++;
			}
		}
		if(points==2)
		{
		}else{
			//if only one we can split on opposite vertex?
			face0->SetS();
		}
	}else{
		//split?
		double full_dist=vcg::Distance(p0,p1);
		if(full_dist<=0.1)
			return false;
		vcg::Point3f vect=p1-p0;
		vect.Normalize();
		vect*=(full_dist/2);
		vcg::Point3f newpoint=p0+vect;
		BreakFaceOnEdge(m,p0,newpoint);
		BreakFaceOnEdge(m,newpoint,p1);
	}
	return true;
}

bool MeshCuttingPlugin::BreakFaceOnPoint(CMeshO& m,Point3f p)
{
	GetClosestFace getClosestFace;
	getClosestFace.init(&m);
	CMeshO::FaceType* face=getClosestFace.getFace(p/*m2.vert[j].P()*/);
	face->SetS();

	vcg::Point3f newpoint2;
	CMeshO::ScalarType dist;
	TrianglePointDistance<CMeshO::ScalarType>(vcg::Triangle3<CMeshO::ScalarType>(face->V(0)->P(),face->V(1)->P(),face->V(2)->P()),p/*m2.vert[j].P()*/,dist,newpoint2);
	if(newpoint2==/*m2.vert[j].P()*/p)
		return false;

	int index=m.vn;
	CMeshO::VertexType* np=&(*tri::Allocator<CMeshO>::AddVertex(m,newpoint2));


	CMeshO::VertexType* v0=face->V(0);
	CMeshO::VertexType* v1=face->V(1);
	CMeshO::VertexType* v2=face->V(2);
	tri::Allocator<CMeshO>::DeleteFace(m,*face);
	tri::Allocator<CMeshO>::AddFace(m, v0, v1, np);
	tri::Allocator<CMeshO>::AddFace(m, v1, v2, np);
	tri::Allocator<CMeshO>::AddFace(m, v2, v0, np);
	return true;
}

MESHLAB_PLUGIN_NAME_EXPORTER(MeshCuttingPlugin)
