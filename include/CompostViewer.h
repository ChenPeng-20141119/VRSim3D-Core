//////////////////////////////////////////////////////////////////////////
///�ļ���������ͼ
///�����ߣ���־��
///����ʱ�䣺2012��12��12��
///��������������osg::CompositeViewer ʵ�ֶ���ͼ�����������������뵥��ͼosgViewer::Viewerֻ��ѡ��һʹ��
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
	//�л�ȫ���ͷ���ģʽ
	void setSceneFullOrPartition(unsigned int num);
	//���˫������л�ȫ���ͷ���ģʽ
	void setClicksetSceneFullOrPartition(bool isClick);

private:
	//���������Ļ���
	osg::GraphicsContext* setGraphicsContext();
	//
	void partition(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene,Style style = ST_AUTOMATIC);
	void partition(osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	//��ֱ�ֲ�
	void setVertical(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene);
	//ˮƽ�ֲ�
	void setHorizontal(int viewerNum,osg::GraphicsContext* gc,osg::Node* scene);
	//����������ԣ�viewPort�ȣ�
	void setCameraPPT(int numview,int allview,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	void setCameraPPT(int numview,osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style = ST_AUTOMATIC);
	//���Զ���������
	void setViewPos(osg::GraphicsContext* gc,const int& numview,const int& allview,
					   float& x, float& y, float& width, float& hight);
	//ȫ����ʾview
	void setFullScene(osgViewer::View* view);
	//����view״̬
	void setViewsState(osgViewer::View* view);
	//���浱ǰ���view�����崰��λ�ô�С��ϵ
	void saveNowViewPos(osgViewer::View* view);
	osg::ref_ptr<osgViewer::CompositeViewer> m_compositeViewer;
	//��¼���ǰview��ViewerPort��Сλ��
	std::vector<float> m_viewPort;
	//ͼ��������
	osg::GraphicsContext* m_gc;
	bool m_isFullScene;
	//֮ǰ������ȫ����ָ��view
	unsigned int m_num;
};
#endif
