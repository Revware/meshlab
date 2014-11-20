#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/clean.h>

namespace vcg {
namespace tri {
namespace io {

/** 
This class encapsulate a filter for importing raw format pointcloud.
there exists many raw formats. each one with a particular sintax even if they only contains 

*/
template <class MESH_TYPE>
class ImporterVP
{
public:

typedef typename MESH_TYPE::VertexPointer VertexPointer;
typedef typename MESH_TYPE::ScalarType ScalarType;
typedef typename MESH_TYPE::VertexType VertexType;
typedef typename MESH_TYPE::FaceType FaceType;
typedef typename MESH_TYPE::VertexIterator VertexIterator;
typedef typename MESH_TYPE::FaceIterator FaceIterator;
static std::map<int,QString> g_Bits;


enum RAWError {
	E_NOERROR,				// 0
		// Error open
	E_CANTOPEN,				// 1
	E_UNESPECTEDEOF,        // 2
	E_NO_POINTS,				//3
};

static const char *ErrorMsg(int error)
{
  static const char * raw_error_msg[] =
  {
	"No errors",
	"Can't open file",
	"Premature End of file",
	"Failed to import any point. Use simple ascii files with just x y z coords."
	};

  if(error>3 || error<0) return "Unknown error";
  else return raw_error_msg[error];
};

/*!
*	Standard call for reading a mesh
*	\param m			the destination mesh
*	\param filename		the name of the file to read from
*	\param triangulate	if true, the mesh will be triangulated, otherwise only points will be stored
*	\param lineskip  	number of lines to be skipped at the begin of the file
*	\return				the operation result
*/
static int Open( MESH_TYPE &m, const char * filename, CallBackPos *cb=0, bool triangulate=false, int lineskip=0)
{
	m.Clear();

	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL)
	{
		qDebug("Failed opening of %s",filename);
		return E_CANTOPEN;
		//return false;
	}

	int state=0;//begining
	char buf[1024];
	Point3f pp;
	int num=0;
	int ret=-1;
	int a,b,c;
	std::string str,type,mark;
	while(!feof(fp))
	{
		bool fgetOut=fgets(buf,1024,fp);
		if( fgetOut == 0 ) continue;
		switch(state)
		{
		case 0://read until find a state
			str=buf;
			if(str.find(':')!=-1)//switch back to sscanf??  sscanf(buf,"%s: %i",type,&num);
			{
				type=str.substr(str.find_first_not_of(" \t"),str.find(':')-str.find_first_not_of(" \t"));
				if(strcmp("Vertices",type.c_str())==0)
				{
					state=1;
				}else if(strcmp("Polygons",type.c_str())==0){
					state=2;
				}else if(strcmp("Marks",type.c_str())==0){
					state=3;
				}
			}
			break;
		case 1://read vertex
			if((ret=sscanf(buf,"%f %f %f",&pp.X(), &pp.Y(), &pp.Z()))==3)
			{
				VertexIterator vi=tri::Allocator<MESH_TYPE>::AddVertices(m,1);
				(*vi).P().Import(pp);
			}else{
				state=0;
			}
			break;
		case 2://read polygon
			if((ret=sscanf(buf,"%i %i %i",&a,&b,&c))==3)
			{
				tri::Allocator<MESH_TYPE>::AddFace(m,&m.vert[a],&m.vert[b],&m.vert[c]);
			}else{
				state=0;
			}
			break;
		case 3://read marks
			char tmp[256];
			if((ret=sscanf(buf,"%i %s",&a,tmp))==2)
			{
				mark=tmp;
				for(std::map<int,QString>::iterator it=g_Bits.begin();it!=g_Bits.end();++it)
				{
					if((*it).second.compare(mark.c_str())==0)
						m.vert[a].Flags()=(*it).first;
				}
			}else{
				state=0;
			}
			break;
		}
	}
	fclose(fp);
    return E_NOERROR;
  }

}; // end class
} // end Namespace tri
} // end Namespace io
} // end Namespace vcg