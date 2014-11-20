namespace vcg {
    namespace tri {
        namespace io {
            template <class SaveMeshType>
            class ExporterVP
            {

            public:
				typedef typename SaveMeshType::VertexIterator VertexIterator;
                typedef typename SaveMeshType::VertexPointer VertexPointer;
                typedef typename SaveMeshType::FaceIterator FaceIterator;
                typedef typename SaveMeshType::FacePointer FacePointer;

				static std::map<int,QString> g_Bits;

                static int Save(SaveMeshType &m, const char * filename, int mask=0 )
                {

					std::map<int,int> m_Marks;//index of verts flagged as landmarks

                    FILE *fp= fopen(filename,"w");
                    if(!fp) return -1;
                    FaceIterator fi;
                    fprintf(fp,
"-----------------------------\n\
        Object: 0\n\
        ObjRev: 3\n\
       ObjName: Mesh_1\n\
      Moveable: 1\n\
 MarksMoveable: 0\n\
PointsMoveable: 0\n\
          OPal: 0\n\
          IPal: 1\n\
       RefType: 1\n\
        RefOff: 1\n\
     ChangeInc: 0.050\n\
       TopoInc: 0.082\n\
       ObjRect: 0\n\
 VerticeOffset: 0\n\
 VerticeFormat: 0\n\
   VerticeMask: 1\n\
        Ref_p0: -1\n\
        Ref_p1: -1\n\
        Ref_p2: -1\n\n");


					fprintf(fp,"     Vertices: %d\n",m.vn);
										int j;
					std::vector<int> FlagV; 
					VertexPointer  vp;
					VertexIterator vi;
					for(j=0,vi=m.vert.begin();vi!=m.vert.end();++vi)
					{
						vp=&(*vi);
						if(vp->IsD())
							continue;
						int tmpFlag=vp->Flags();
						fprintf(fp,"% 8.4f % 8.4f % 8.4f\n" ,vp->P()[0],vp->P()[1],vp->P()[2]);
						//add user flags for marks??
						for(std::map<int,QString>::iterator it=g_Bits.begin();it!=g_Bits.end();++it)
						{
							if(vp->IsUserBit((*it).first))
								m_Marks[(*it).first]=j;
						}
						vp->Flags()=j++;//index
					}

					int SS=0;
					fprintf(fp,"\n     Marks: %d\n",m_Marks.size());
					for(std::map<int,int>::iterator mi=m_Marks.begin();mi!=m_Marks.end();++mi)
					{
						int t=(*mi).first;
						QString s=g_Bits[t];

						char blah[256];
						for(int i=0;i<s.length();++i)
							blah[i]=s[i].toAscii();
						blah[s.length()]=0;
						if(s.compare("SS")==0)
						{
							SS++;
							fprintf(fp,"%d %s%d\n",(*mi).second,blah,SS);
						}else{
							fprintf(fp,"%d %s\n",(*mi).second,blah);
						}
					}


					fprintf(fp,"\n     QPolygons: %d\n",m.fn);
					for(fi=m.face.begin();fi!=m.face.end();++fi)
					{
						FacePointer f=&(*fi);
						if( ! f->IsD() )
						{ 
							fprintf(fp,"%d %d %d\n",f->cV(0)->Flags(),	f->cV(1)->Flags(), f->cV(2)->Flags() );
						}
					}

					//duplicate for other type
					fprintf(fp,"\n     Polygons: %d\n",m.fn);
					for(fi=m.face.begin();fi!=m.face.end();++fi)
					{
						FacePointer f=&(*fi);
						if( ! f->IsD() )
						{ 
							fprintf(fp,"%d %d %d\n",f->cV(0)->Flags(),	f->cV(1)->Flags(), f->cV(2)->Flags() );
						}
					}

					fprintf(fp,"\n     ObjectEnd:\n");

                    fclose(fp);

					//clear vert flags
					//for(j=0,vi=m.vert.begin();vi!=m.vert.end();++vi)
					//{
					//	vp=&(*vi);
					//	if(vp->IsD())
					//		continue;
					//	vp->Flags()=0;

					//	//reenter marks
					//	for(std::map<int,int>::iterator mi=m_Marks.begin();mi!=m_Marks.end();++mi)
					//	{
					//		if(j==(*mi).second)
					//		{
					//			vp->Flags()=((*mi).first);
					//			break;
					//		}
					//	}
					//}

                    return 0;
                }
                /*
            returns mask of capability one define with what are the saveable information of the format.
        */
                static int GetExportMaskCapability()
                {
                    int capability = 0;

                    //vert
                    capability |= vcg::tri::io::Mask::IOM_VERTCOORD;
					capability |= vcg::tri::io::Mask::IOM_VERTFLAGS;
                    capability |= vcg::tri::io::Mask::IOM_BITPOLYGONAL;

                    return capability;
                }

                static const char *ErrorMsg(int error)
                {
                    if(error==-1) return "unable to open file";
                    return "Ok, no errors";
                }
            };
        } // end namespace io
    } // end namespace tri
} // end namespace vcg