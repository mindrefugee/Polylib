/*
 * Polylib - Polygon Management Library
 *
 * Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
 * All rights reserved.
 *
 * Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

#ifndef polylib_trimesh_h
#define polylib_trimesh_h

#include <vector>
#include <map>
#include <algorithm>
#include "file_io/TriMeshIO.h"
#include "polygons/VertKDT.h"

#define M_MAX_ELEMENTS 15	/// VTreeのノードが持つ最大要素数

namespace PolylibNS {

template <typename T> class Polygons;
template <typename T> class VTree;
template <typename T> class PrivateTriangle;
template <typename T> class VertexList;
template <typename T> class VertKDT;


  ////////////////////////////////////////////////////////////////////////////
  ///
  /// クラス:TriMesh
  /// 三角形ポリゴン集合を管理するクラス（KD木用に特化したクラス)。
  ///
  ////////////////////////////////////////////////////////////////////////////
  template <typename T>
  class TriMesh : public Polygons<T> {
  public:
    ///
    /// コンストラクタ。
    ///
    TriMesh();

    ///
    /// コンストラクタ。
    ///
    /// @param[in] tolerance 頂点同一性チェックの基準値
    TriMesh(T tolerance);

    ///
    /// デストラクタ。
    ///
    ~TriMesh();

    ///
    /// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
    /// れる三角形ポリゴンリストを設定する。
    /// 三角形ポリゴン用のメモリ領域は、Polylib内で新たに確保される。
    ///
    ///  @param[in] trias 設定する三角形ポリゴンリスト。
    ///
    void init(
	      const std::vector<PrivateTriangle<T>*>	*trias
	      );


    ///
    /// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
    /// れる三角形ポリゴンリストを設定する。
    /// 三角形ポリゴン用のメモリ領域は、TriMesh 内で新たに確保する。
    ///
    ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
    ///  @param[in] idlist 三角形のid。
    ///  @param[in] n_start_tri vertlistの頂点開始位置
    ///  @param[in] n_start_id idlistのid開始位置
    ///  @param[in] n_tri 加える三角形の数

  void init(const T* vertlist,
	    const int* idlist,
	    const int n_start_tri,
	    const int n_start_id,
	    const unsigned int n_tri);



    ///
    /// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
    ///
    /// @param[in] trias	設定する三角形ポリゴンリスト。
    /// @attention m_idが重複するインスタンスは追加されない。
    /// @attention KD木の再構築は行わない。
    ///
    void add(
	     const std::vector<PrivateTriangle<T>*>  *trias
	     );


    ///
    /// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
    ///
    ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
    ///  @param[in] idlist 三角形のid。
    ///  @param[in] n_start_tri vertlistの頂点開始位置
    ///  @param[in] n_start_id idlistのid開始位置
    ///  @param[in] n_tri 加える三角形の数

  void add(const T* vertlist,
	    const int* idlist,
	    const int n_start_tri,
	    const int n_start_id,
	    const unsigned int n_tri);

    ///
    /// ファイルからデータの初期化。
    ///
    ///  @param[in] fmap	ファイル名、ファイルフォーマット。
    ///  @return PLSTAT_OK=成功/false=失敗
    ///
    POLYLIB_STAT import(
			const std::map<std::string, std::string> fmap,
			T scale = 1.0
			);

    ///
    /// Polygonsクラスに含まれる全ポリゴン情報からKD木を作成する。
    ///
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///
    POLYLIB_STAT build();

    ///
    /// TriMeshクラスが管理している三角形ポリゴン数を返す。
    ///
    int triangles_num();

    ///
    /// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
    ///
    ///  @param[in] bbox	検索範囲を示す矩形領域。
    ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
    ///						false:1頂点でも検索領域に含まれるものを抽出。
    ///  @return	抽出したポリゴンリストのポインタ。
    ///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
    ///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
    ///  @attention	オーバーロードメソッドあり。
    ///
    const std::vector<PrivateTriangle<T>*>* search(
						   BBox<T>	*bbox, 
						   bool	every
						   ) const;



    ///
    /// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
    ///
    ///  @param[in]		bbox		検索範囲を示す矩形領域
    ///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
    ///								false:1頂点でも検索領域に含まれるものを抽出。
    ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
    ///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
    ///  @attention	オーバーロードメソッドあり。
    ///
    POLYLIB_STAT search(
			BBox<T>							*bbox,
			bool							every,
			std::vector<PrivateTriangle<T>*>	*tri_list
			) const;

    ///
    /// 線形探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
    ///
    ///  @param[in] q_bbox	検索範囲を示す矩形領域。
    ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
    ///						false:1頂点でも検索領域に含まれるものを抽出。
    ///  @return	抽出したポリゴンリストのポインタ。
    ///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
    ///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
    ///
    const std::vector<PrivateTriangle<T>*>* linear_search(
							  BBox<T>	*q_bbox, 
							  bool	every
							  ) const;

    ///
    /// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
    ///
    ///  @param[in]		q_bbox		検索範囲を示す矩形領域。
    ///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
    ///								false:1頂点でも検索領域に含まれるものを抽出。
    ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
    ///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
    ///  @attention	オーバーロードメソッドあり。
    ///
    POLYLIB_STAT linear_search(
			       BBox<T>							*q_bbox, 
			       bool							every,
			       std::vector<PrivateTriangle<T>*>	*tri_list
			       ) const;

    ///
    /// KD木探索により、指定位置に最も近いポリゴンを検索する。
    ///
    ///  @param[in]     pos     指定位置
    ///  @return 検索されたポリゴン
    ///
    const PrivateTriangle<T>* search_nearest(
					     const Vec3<T>&    pos
					     ) const;

    ///
    /// 配下の全ポリゴンのm_exid値を指定値にする。
    ///
    /// @param[in]	id	指定値
    ///
    POLYLIB_STAT set_all_exid(
			      const int	id
			      ) const;

    
    
    //=======================================================================
    // Setter/Getter
    //=======================================================================
    ///
    /// TriMeshクラスが管理しているBoundingBoxを返す。
    ///
    BBox<T> get_bbox() const {
      return m_bbox;
    }

    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VertKDT<T> *get_vertkdt() const {
      return m_vertKDT;
    }

    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VTree<T> *get_vtree() const {
      return m_vtree;
    }

    virtual void print_memory_size() const {
      unsigned int memsize_vtree=m_vtree->memory_size();
      unsigned int memsize_vkdt=m_vertKDT->memory_size();
      unsigned int memsize_pt_list=
	(sizeof(PrivateTriangle<T>)+sizeof(PrivateTriangle<T>*))
	*(this->m_tri_list->size());
      unsigned int memsize_vt_list=(sizeof(Vertex<T>)+sizeof(Vertex<T>*))
	*(this->m_vertex_list->size());
      unsigned int memsize_all=
	memsize_vt_list+memsize_pt_list+memsize_vkdt+memsize_vtree;
      PL_DBGOSH<< "TriMesh<T>::"<<__func__<<std::endl;
      PL_DBGOSH<< "size of VertexList<T>      "<< memsize_vt_list<<std::endl;
      PL_DBGOSH<< "size of VertKDT<T>         "<<memsize_vkdt<<std::endl;
      PL_DBGOSH<< "size of PrivateTriangle<T> "<<memsize_pt_list<<std::endl;
      PL_DBGOSH<< "size of VTree<T>           "<<memsize_vtree<<std::endl;
      PL_DBGOSH<< "---------------------------------------------"<<std::endl;
      PL_DBGOSH<< "total size                 "<<memsize_all<<std::endl;
    }

  private:
    ///
    /// 三角形ポリゴンリストの初期化。
    ///
    void init_tri_list();

    ///
    /// 頂点リストの初期化。
    ///
    void init_vertex_list();

    ///
    /// 頂点リストのいれかえ
    ///
    void replace_vertex_list(VertexList<T> *vlist);


    /// 
    /// 重複頂点の削除
    ///
    void vtx_compaction();

    //=======================================================================
    // クラス変数
    //=======================================================================
    /// 全三角形ポリゴンを外包するBoundingBox。
    BBox<T>	m_bbox;

    /// KD木クラス。
    VTree<T>	*m_vtree;

    /// KD木クラス。
    VertKDT<T>	*m_vertKDT;

    /// MAX要素数。
    int		m_max_elements;
    /// 2 点の同一性チェックのtolerance
    T m_tolerance ;
  };

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::TriMesh()
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	this->m_vertex_list = NULL;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = 1.0e-10; // tempolary
}
// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::TriMesh(T tolerance)
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertex_list = NULL;
	m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = tolerance;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::~TriMesh()
{
	delete m_vtree;
	if (this->m_tri_list != NULL) {
	  typename std::vector<PrivateTriangle<T>*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			delete *itr;
		}
		this->m_tri_list->clear();
	}

	delete this->m_tri_list;

	// if (this->m_vtx_list != NULL) {
	//   typename std::vector<Vertex<T>*>::iterator itr;
	// 	for (itr = this->m_vtx_list->begin(); itr != this->m_vtx_list->end(); itr++) {
	// 		delete *itr;
	// 	}
	// 	this->m_vtx_list->clear();
	// }

	// delete this->m_vtx_list;

	if (this->m_vertex_list != NULL) {
	  delete this->m_vertex_list;
	}

}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
  void TriMesh<T>::init(const T* vertlist,
			const int* idlist,
			const int n_start_tri,
			const int n_start_id,
			const unsigned int n_tri)
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " in."<<std::endl;
#endif


  init_tri_list();
  init_vertex_list();
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of init tri_list and vert_list"<<std::endl;
  PL_DBGOSH <<"test "<<idlist[n_start_id]<<" "<< idlist[n_start_id+1]<<std::endl;
#endif

  for(int i=0;i<n_tri;++i) {
    int id=n_start_tri+i*9;
    Vertex<T>* vtx_tri[3];
    for(int j=0;j<3;++j){
      Vertex<T>* v=new Vertex<T>(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);

      // vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
      //if(vtx_tri[j]!=v) delete v;
      //はじめに、重複を許して作成する。
      this->m_vertex_list->vtx_add_nocheck(v);
      vtx_tri[j]=v;
    }
    //int id2=n_start_id+i;
    //PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);
    PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[n_start_id]);
    this->m_tri_list->push_back(tri);
  }

  //最後に。頂点重複をチェックして、ポリゴン情報内のポインタも付け替える。
  //  POLYLIB_STAT status = TriMeshIO::vtx_compaction<T>(this->m_vertex_list,
  //this->m_tri_list);

  //  POLYLIB_STAT status = vtx_compaction();

  vtx_compaction();

#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of vtx_comaction"<<std::endl;
#endif

 }

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init(const std::vector<PrivateTriangle<T>*>* trias)
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ <<std::endl;
#endif
  init_tri_list();
#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ << " init_tri_list end"<<std::endl;
#endif

  VertKDT<T>* new_vertKDT=new VertKDT<T>(this->m_max_elements);
  VertexList<T>* new_vertex_list= new VertexList<T>(new_vertKDT,this->m_tolerance);
  //  std::map<Vertex<T>*,int> vmap;

  typename std::vector<PrivateTriangle<T>*>::const_iterator itr;

  //first correct vertices
  for (itr = trias->begin(); itr != trias->end(); itr++) {
    try{
      Vertex<T>** tmpvert=(*itr)->get_vertex();
      if(tmpvert==0) throw "tmpvert";
      //      PL_DBGOSH<< "TriMesh<T>::" << __func__ << " get_vertex "<< tmpvert <<std::endl;
      Vertex<T>* tmpvert_in[3];

      // make deep copy!
      Vertex<T>* tmp = new Vertex<T>(*(tmpvert[0]));
      //      PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 0 "<< tmp <<std::endl;
      if(tmp==0) throw "tmp";
      new_vertex_list->vtx_add_nocheck(tmp);
      tmpvert_in[0]=tmp;
      Vertex<T>* tmp1 = new Vertex<T>(*(tmpvert[1]));
      //PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 1 "<< tmp1 <<std::endl;
      if(tmp1==0) throw "tmp1";
      new_vertex_list->vtx_add_nocheck(tmp1);
      tmpvert_in[1]=tmp1;
      Vertex<T>* tmp2 = new Vertex<T>(*(tmpvert[2]));
      //PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 2 "<< tmp2 <<std::endl;
      if(tmp2==0) throw "tmp2";
      new_vertex_list->vtx_add_nocheck(tmp2);
      tmpvert_in[2]=tmp2;

      // new_vertex_list->vtx_add_nocheck(tmpvert[0]);
      // new_vertex_list->vtx_add_nocheck(tmpvert[1]);
      // new_vertex_list->vtx_add_nocheck(tmpvert[2]);
      /* vmap[tmpvert[0]]=1; */
      /* vmap[tmpvert[1]]=1; */
      /* vmap[tmpvert[2]]=1; */
      //    PrivateTriangle<T>* tri=new PrivateTriangle<T>( (*itr)->get_vertex(),
      PrivateTriangle<T>* tri=new PrivateTriangle<T>( tmpvert_in,
						      (*itr)->get_normal(), 
						      (*itr)->get_area(),
						      (*itr)->get_id()
						      );
      this->m_tri_list->push_back(tri);
    }
    catch (char* str) {
      std::cerr << "TriMesh::"<<__func__<<" error in "<<str<<std::endl;
    }
  }

#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ << " create tempolary VertexList and tri_list"<<std::endl;
#endif


  /* init_vertex_list(); */
  /* m_vertKDT=new_vertKDT; */
  /* this->m_vertex_list=new_vertex_list; */
  replace_vertex_list(new_vertex_list);



  //  POLYLIB_STAT status = TriMeshIO::vtx_compaction<T>(new_vertex_list, this->m_tri_list);
  //  POLYLIB_STAT status = vtx_compaction();
  vtx_compaction();

}


// public /////////////////////////////////////////////////////////////////////
// std::sort用ファンクタ
template <typename T>
struct PrivTriaLess{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() < r->get_id();
	}
};
// std::equal用ファンクタ
template <typename T>
struct PrivTriaEqual{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() == r->get_id();
	}
};

 
// public /////////////////////////////////////////////////////////////////////
 template <typename T>
 void TriMesh<T>::add(const T* vertlist,
			const int* idlist,
			const int n_start_tri,
			const int n_start_id,
			const unsigned int n_tri)
   {


     for(int i=0;i<n_tri;++i) {
       int id=n_start_tri+i*9;
       Vertex<T>* vtx_tri[3];
       for(int j=0;j<3;++j){
	 Vertex<T>* v=new Vertex<T>(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);
	 vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
	 if(vtx_tri[j]!=v) delete v;
       }
       int id2=n_start_id+i;
       PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);
       this->m_tri_list->push_back(tri);
     }
     
     // 三角形リストをID順にソート
     std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess<T>() );

     // ID重複ぶんを削除
     this->m_tri_list->erase(
			     std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual<T>()),
			     this->m_tri_list->end());
   }


// public /////////////////////////////////////////////////////////////////////
template <typename T>
void
TriMesh<T>::add(
	const std::vector<PrivateTriangle<T>*> *trias
)
{
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::add_triangles() in." << std::endl;
#endif
	unsigned int i;

	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
	}

	// ひとまず全部追加
	for( i=0; i<trias->size(); i++ ) {
		this->m_tri_list->push_back( new PrivateTriangle<T>( *(trias->at(i)) ) );
	}

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess<T>() );

	// ID重複ぶんを削除
	this->m_tri_list->erase(
	std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual<T>()),
		this->m_tri_list->end());
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::import(const std::map<std::string, std::string> fmap, T scale)
{

  //      PL_DBGOSH << __func__ << " scale "<< scale <<std::endl;

	init_tri_list();
	//PL_DBGOSH << __func__ << " scale 1 "<< scale <<std::endl;
	init_vertex_list();
	//PL_DBGOSH << __func__ << " scale 2 "<< scale <<std::endl;

	POLYLIB_STAT ret = TriMeshIO::load<T>(this->m_vertex_list,
					      this->m_tri_list, fmap, scale);
	if(ret!=PLSTAT_OK) return ret;

	vtx_compaction();		

	return ret;

	//return TriMeshIO::load<T>(this->m_tri_list, fmap, scale);
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::build()
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh::build() start" << std::endl;
#endif // DEBUG
  
  BBox<T> bbox;

  //	BBox<T> bbox=this->m_vertex_list->get_bbox();

	// now bbox is taken from VertKDT.


// #ifdef DEBUG
//   PL_DBGOSH << "TriMesh::build() test" << std::endl;
//   BBox<T> vtx_bbox=this->m_vertex_list->get_bbox();
//   PL_DBGOSH << "TriMesh::build() vertex_list bbox min"
// 	    << vtx_bbox.min<<"max"<< vtx_bbox.max <<std::endl;

//   PL_DBGOSH << "TriMesh::build() vertex_list 0 1 2 "
// 	    << this->m_vertex_list->ith(0)<<" "
// 	    << this->m_vertex_list->ith(1)<<" "
// 	    << this->m_vertex_list->ith(2)<<std::endl;
// #endif // DEBUG
  int counter=0;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;
	/// TriMeshクラスに含まれる全三角形ポリゴンを外包するBoundingBoxを計算
	bbox.init();
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
	  counter++;
#ifdef DEBUG
	  PL_DBGOSH << "TriMesh::build() polygon loop " << counter << std::endl;
#endif // DEBUG


	  //		const Vec3<T>* vtx_arr = (*itr)->get_vertex();

	  Vertex<T>** vtx_arr= (*itr)->get_vertex();
#ifdef DEBUG
  PL_DBGOSH << "TriMesh::build() polygon get_vertex "
	    << vtx_arr[0]<<" "
	    << vtx_arr[1]<<" "
	    << vtx_arr[2]<< std::endl;
#endif // DEBUG

  for (int i = 0; i < 3; i++) {
#ifdef DEBUG
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[0] <<std::endl;
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[1] <<std::endl;
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[2] <<std::endl;
#endif // DEBUG

	  //   if( ( (*(vtx_arr[i]))[0] > 1000) || ( (*(vtx_arr[i]))[1] > 1000) ||( (*(vtx_arr[i]))[2] > 1000) ){
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[0] <<std::endl;
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[1] <<std::endl;
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[2] <<std::endl;
	  //     }


	  bbox.add(  (Vec3<T>) (*(vtx_arr[i])) );
	  }

	}

	m_bbox = bbox;
//#define DEBUG
#ifdef DEBUG
	Vec3<T> min = m_bbox.getPoint(0);
	Vec3<T> max = m_bbox.getPoint(7);
	PL_DBGOSH << "TriMesh::build:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif
	//PL_DBGOSH << "TriMesh::build:min=(" <<m_bbox.min<< "),max=(" <<m_bbox.max<< ")" << std::endl;

	//#undef DEBUG
	// 木構造作成
	if (m_vtree != NULL) delete m_vtree;
	m_vtree = new VTree<T>(m_max_elements, m_bbox, this->m_tri_list);
	// if (m_vertKDT!=NULL) delete m_vertKDT;
	// m_vertKDT = new VertKDT<T>(m_max_elements, m_bbox, this->m_vertex_list);
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::build: end of vtree " << std::endl;
#endif

	return PLSTAT_OK;
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
int TriMesh<T>::triangles_num() {
	if (this->m_tri_list == NULL)		return 0;
	else						return this->m_tri_list->size();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*> *TriMesh<T>::search(
	BBox<T>	*bbox, 
	bool	every
) const {
  //#define DEBUG
#ifdef DEBUG
	Vec3<T> min = bbox->getPoint(0);
	Vec3<T> max = bbox->getPoint(7);
	PL_DBGOSH << "TriMesh::search:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif

	return m_vtree->search(bbox, every);
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	return m_vtree->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>* TriMesh<T>::linear_search(
	BBox<T>	*q_bbox, 
	bool	every
) const {
	std::vector<PrivateTriangle<T>*>		   *tri_list = new std::vector<PrivateTriangle<T>*>;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		//const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		Vertex<T>** vtx_arr = (*itr)->get_vertex();
		for (int i = 0; i < 3; i++) {
		  bbox.add( (Vec3<T>) *vtx_arr[i]);
		}
		if (every == true) {
		  if (q_bbox->contain( (Vec3<T>) *vtx_arr[0]) == true && 
		      q_bbox->contain( (Vec3<T>) *vtx_arr[1]) == true &&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[2]) == true)
			{
				tri_list->push_back(*itr);
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->crossed(bbox) == true				||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
			}
		}
	}
	return tri_list;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::linear_search(
	BBox<T>						*q_bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	if (tri_list == NULL) return PLSTAT_ARGUMENT_NULL;

	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		//const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		Vertex<T>** vtx_arr = (*itr)->get_vertex();

		for (int i = 0; i < 3; i++) {
		  bbox.add( (Vec3<T>) *vtx_arr[i]);
		}
		if (every == true) {
		  if (q_bbox->contain( (Vec3<T>) *vtx_arr[0]) == true	&&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[1]) == true	&&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[2]) == true) {
		    tri_list->push_back(*itr);
#ifdef DEBUG
			PL_DBGOSH << "TriMesh<T>::linear_search:IN TRUE" << std::endl;
			PL_DBGOSH << "     vertex 0:" << (Vec3<T>) *vtx_arr[0] << std::endl;
			PL_DBGOSH << "     vertex 1:" << (Vec3<T>) *vtx_arr[1] << std::endl;
			PL_DBGOSH << "     vertex 2:" << (Vec3<T>) *vtx_arr[2] << std::endl;
#endif
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				q_bbox->crossed(bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
#ifdef DEBUG
				PL_DBGOSH << "TriMesh::linear_search:IN FALSE" << std::endl;
#endif
			}
		}
#ifdef DEBUG
		for (int i=0; i<8; i++) {
			PL_DBGOSH << "TriMesh::linear_search:q_box[" << i << "]:" 
					  << q_bbox->getPoint(i) << std::endl;
		}
	    PL_DBGOSH << "TriMesh::linear_searc:" << " id:" << (*itr)->get_id()
				  << ",v(" << vtx_arr << ")" << std::endl;
#endif
	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* TriMesh<T>::search_nearest(
	const Vec3<T>&    pos
) const {
	return m_vtree->search_nearest(pos);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::set_all_exid(
	const int    id
) const {
	// 全ポリゴンのm_exidをidで上書き
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		(*itr)->set_exid( id );
	}
	return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init_tri_list()
{
  if (this->m_tri_list == NULL) {
    this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
  }
  else {
    typename std::vector<PrivateTriangle<T>*>::iterator itr;
    for (itr = this->m_tri_list->begin();
	 itr != this->m_tri_list->end();
	 itr++) {
      delete *itr;
    }
    
    this->m_tri_list->clear();
  }

}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init_vertex_list()
  {
    //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
    if (this->m_vertex_list == NULL) {
      //std::cout << __func__ << " VertexList is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertexList is deleting"<< std::endl;
      this->m_vertex_list->vtx_clear();
      delete this->m_vertex_list;

    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
    if (this->m_vertKDT == NULL) {
      //std::cout << __func__ << " VertKDT is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
      delete this->m_vertKDT;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

    this->m_vertKDT = new VertKDT<T>(m_max_elements);
    //std::cout << __func__ << " VertKDT end"<< std::endl;
    this->m_vertex_list = new VertexList<T>(this->m_vertKDT,this->m_tolerance);
    //std::cout << __func__ << " vertex end"<< std::endl;
    //this->m_vertex_list = new VertexList<T>;
#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
  }

// private ////////////////////////////////////////////////////////////////////
template <typename T>
  void TriMesh<T>::replace_vertex_list(VertexList<T>* vlist)
  {
    //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
    if (this->m_vertex_list == NULL) {
      //std::cout << __func__ << " VertexList is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertexList is deleting"<< std::endl;
      this->m_vertex_list->vtx_clear();
      delete this->m_vertex_list;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
    if (this->m_vertKDT == NULL) {
      //std::cout << __func__ << " VertKDT is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
      delete this->m_vertKDT;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

	//    m_vertKDT = new VertKDT<T>(m_max_elements);
    //std::cout << __func__ << " VertKDT end"<< std::endl;
    //    vlist->setKDT(m_vertKDT);
    this->m_vertex_list = vlist;
    m_vertKDT = vlist->getKDT();
    //    this->m_vertex_list = new VertexList<T>(this->m_vertKDT,this->m_tolerance);
    //std::cout << __func__ << " vertex end"<< std::endl;
    //this->m_vertex_list = new VertexList<T>;


#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
  }



// private ////////////////////////////////////////////////////////////////////
 template <typename T>
   void TriMesh<T>::vtx_compaction()
 {

   std::map<Vertex<T>*,Vertex<T>*>* vtx_map=new  std::map<Vertex<T>*,Vertex<T>*>;
;
   this->m_vertex_list->vertex_compaction(vtx_map);
   typename std::vector<PrivateTriangle<T>*>::iterator itr;
   
#ifdef DEBUG
    int counter =0;
#endif
   for(itr=this->m_tri_list->begin();
       itr!=this->m_tri_list->end();
       itr++){

     Vertex<T>**  tmp_list=  (*itr)->get_vertex();
     bool vertex_replace=false;
     for(int i=0;i<3;i++){
#ifdef DEBUG
       if(counter<10)
	 /PL_DBGOSH << "pointer map old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<std::endl;
#endif
       if((*vtx_map)[tmp_list[i]]==tmp_list[i]){
	 // do nothing
       } else { // substitute 
#ifdef DEBUG
	 if(counter<10)
	   PL_DBGOSH << "pointer swaped old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<std::endl;
#endif
	 tmp_list[i]=(*vtx_map)[tmp_list[i]];
	 vertex_replace=true;
       }
       
     }
     if(vertex_replace){ //vertex replaced
       (*itr)->set_vertexes(tmp_list,true,true);
       
       if((*itr)->get_area()==0){ //zero area check
	 PL_DBGOSH << __func__ 
		   << " Warning : polygon contains a triangle that its area is zero." << std::endl;
	 PL_DBGOSH <<  "vertex0 ("<< *(tmp_list[0]) <<")"<<std::endl;
	 PL_DBGOSH <<  "vertex1 ("<< *(tmp_list[1]) <<")"<<std::endl;
	 PL_DBGOSH <<  "vertex2 ("<< *(tmp_list[2]) <<")"<<std::endl;

       }
     } else {
       // vertex not replaced
       (*itr)->set_vertexes(tmp_list,false,false);
     }
     
#ifdef DEBUG
           counter++;
#endif
   }

}


} //namespace PolylibNS

#endif  // polylib_trimesh_h
