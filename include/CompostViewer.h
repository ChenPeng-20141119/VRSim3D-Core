//////////////////////////////////////////////////////////////////////////
///文件名：多视图
///创建者：高志清
///创建时间：2012年12月12日
///内容描述：采用osg::CompositeViewer 实现多视图（分屏）管理，此类与单视图osgViewer::Viewer只能选其一使用
//////////////////////////////////////////////////////////////////////////
#ifndef COMPOSTVIEWER_H
#define COMPOSTVIEWER_H 1

#include <osg/GraphicsContext>
#include <osgViewer/CompositeViewer>

#include <osgGA/GUIEventAdapter>

class CompostViewer
{
	enum Style
	{
		ST_AUTOMATIC,
		ST_VERTICAL,
		ST_HORIZONTAL

	};
	//class SelectEnvent: public osgGA::GUIEventHandler
	//{
	//public:
	//	SelectEnvent(CompostViewer* compositeViewer)
	//	{
	//		m_compositeViewer = compositeViewer;	
	//	}
	//	//virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	//private:
	//	CompostViewer* m_compositeViewer;
	//};
public:
	CompostViewer(void);
	~CompostViewer(void);
	void setCompostViewers(int viewerNum,osg::Node* scene,Style style = ST_AUTOMATIC);
	void setCompostViewers(osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	osgViewer::CompositeViewer* getViewer(){return m_compositeViewer.get();}
	//切换全屏和分屏模式
	void setSceneFullOrPartition(unsigned int num);
	//添加双击点击切换全屏和分屏模式
	void setClicksetSceneFullOrPartition(bool isClick);

private:
	//设置上下文环境
	osg::GraphicsContext* setGraphicsContext();
	//
	void partition(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene,Style style = ST_AUTOMATIC);
	void partition(osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	//垂直分布
	void setVertical(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene);
	//水平分布
	void setHorizontal(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene);
	//设置相机属性（viewPort等）
	void setCameraPPT(int numview,int allview,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	void setCameraPPT(int numview,osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	//对自动布局设置
	void setViewPos(osg::GraphicsContext* gc,const int& numview,const int& allview,
					   float& x, float& y, float& width, float& hight);
	//全屏显示view
	void setFullScene(osgViewer::View* view);
	//设置view状态
	void setViewsState(osgViewer::View* view);
	//保存当前点击view与整体窗口位置大小关系
	void saveNowViewPos(osgViewer::View* view);
	osg::ref_ptr<osgViewer::CompositeViewer> m_compositeViewer;
	//记录点击前view的ViewerPort大小位置
	std::vector<float> m_viewPort;
	//图像上下文
	osg::GraphicsContext* m_gc;
	bool m_isFullScene;
	//之前操作过全屏或分割的view
	unsigned int m_num;
};
#endif
