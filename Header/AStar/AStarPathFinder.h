#pragma once

#include <vector>
#include <set>
#include <map>
using namespace std;

struct PathPoint 
{
	PathPoint() : x(0), z(0)
	{

	}

	bool operator == (const PathPoint& PathPoint) const
	{
		return ((x == PathPoint.x) && (z == PathPoint.z));
	}
	PathPoint & operator = ( const PathPoint & t )
	{
		x = t.x;
		z = t.z;
		return ( *this ) ;
	}
	int x, z;
};
typedef vector<PathPoint> PATHVEC;
typedef std::vector< std::vector<int> > VecMapData;

struct PathNode
{
	PathNode() : mParent(NULL)
	{

	}

	PathPoint	mPathPoint;		// �ڵ�������ƽ���
	float		mF;				// �ڵ����ֵ, f = g + h
	float		mG;				// gֵ, ��TILESIZE���
	float		mH;				// hֵ��Ϊ��ǰ�㵽Ŀ���ľ���
	PathNode*	mParent;		// �ýڵ�ĸ��ڵ�
};


struct NodeSort
{
	bool operator() (const PathNode* node1, const PathNode* node2) const
	{
		return node1->mF < node2->mF;
	}
};

class AStarPathFinder 
{
public:
	typedef multiset<PathNode*, NodeSort> NODESET;
	typedef NODESET::iterator NODESETITER;
	typedef map<DWORD, PathNode*> NODEFINDMAP;
	typedef NODEFINDMAP::iterator NODEMAPITER;

	AStarPathFinder();
	~AStarPathFinder();

	// ����
	void clear();
	// ��Ѱ·��
	// @sx, sy ��ʼ�����꣬ @dx�� dyĿ�������
	bool findPath(int sx, int sz, int dx, int dz, VecMapData& mapData);
	// �õ�·������
	PATHVEC& getPath();

private:
	// �������ŵ���ӽڵ�
	// @bestNode ���ŵ�
	void generateNodes(PathNode* bestNode, int dx, int dz, VecMapData& mapData );
	bool CanMove( VecMapData& mapData, int x, int z );
	// �ж����ŵ㣬ͬʱ����OPEN��CLOSED��
	void updateNode(PathNode* bestNode, int sx, int sz, int dx, int dz, float tileSize);
	// �Ƿ���Ŀ���
	bool isDestNode(PathNode* node);
	// �õ����ŵ㣬ͨ�������������map�ṹ�ĵ�һ���ڵ���
	PathNode* getBestNode();
	// ����·����ͨ�����Ŀ�����������ڵ�ֱ����ʼ�㣬����������·��
	void setPath(PathNode* node, int sx, int sz);

	void insertNodeInOpen(PathNode* node);
	void deleteNodeFromeOpen(int sx, int sz);
	void insertNodeInClosed(PathNode* node);
	void deleteNodeFromeClosed(int sx, int sz);
	PathNode* findOpenNode(PathNode* node);
	PathNode* findClosedNode(PathNode* node);

private:
	// OPEN�� �洢δ�������Ľڵ�, ��һλ�ǽڵ�����
	// ��������
	NODESET	mOpen;
	// CLOSED���洢�������Ľڵ㣬��һλ�ǽڵ�����
	// ��������
	NODESET	mClosed;
	NODEFINDMAP mFindOpen;
	NODEFINDMAP mFindClosed;

	// ��������·��
	PATHVEC	mPath;
	// չ���ڵ�ʱ�Ĳο������������ķ�1����λչ���ڵ�
	float TILESIZE;
	float TILESIZE2;
};
