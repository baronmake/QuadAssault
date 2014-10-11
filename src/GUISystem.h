#ifndef GUISystem_h__
#define GUISystem_h__

#include "TUICore.h"
#include "TUICommon.h"

#include "Singleton.h"

class Texture;
class IText;

enum 
{
	UI_ANY = -1 ,
	UI_STAGE_ID  = 100  ,
	UI_EDIT_ID   = 1000 ,
	UI_WIDGET_ID = 2000,
};

enum
{
	EVT_BUTTON_CLICK    ,
	EVT_TEXTCTRL_CHANGE   ,
	EVT_TEXTCTRL_ENTER  ,
	EVT_CHOICE_SELECT   ,
	EVT_LISTCTRL_DCLICK ,
	EVT_LISTCTRL_SELECT ,
	EVT_SLIDER_CHANGE ,

	EVT_ENTER_UI ,
	EVT_EXIT_UI  ,

	EVT_EXTERN_EVENT_ID ,
};


class  GWidget : public TUICore< GWidget >
{
public:
	GWidget( Vec2i const& pos , Vec2i const& size , GWidget* parent );
	~GWidget();

	int  getID(){ return mId; }

	virtual bool onChildEvent( int event , int id , GWidget* ui ){  return true; }


	void* getUserData(){ return mUserData; }
	void  setUserData( void* data ) { mUserData = data; }


	virtual void onMouse( bool beIn ){  /*sendEvent( ( beIn ) ? EVT_ENTER_UI : EVT_EXIT_UI );*/  }
	virtual void onRender(){}
	virtual void onUpdateUI(){}
	virtual bool onKeyMsg( unsigned key , bool beDown ){ return true; }
	virtual void onHotkey( unsigned key ){}
	virtual void onFocus( bool beF ){}

	virtual void  updateFrame( int frame ){}

	//Use for Prop Edit
	virtual void  inputData(){}
	virtual void  outputData(){}
	

	void onPrevRender(){}
	void onPostRenderChildren();
	void onPostRender(){}

	virtual void  onRenderSiblingsEnd(){}
	//bool doClipTest();

	GWidget*  findChild( int id , GWidget* start = NULL );

	void doModal(){  getManager()->startModal( this );  }

protected:

	void sendEvent( int eventID );

	void*  mUserData;
	int    mId;
};

typedef UIPackage< GWidget > GUI;
typedef GUI::Manager         WidgetManager;

class GUISystem : public SingletonT< GUISystem >
{
public:

	void     render();
	void     sendMessage( int event , int id , GWidget* sender );
	GWidget* findTopWidget( int id , GWidget* start = NULL );
	void     addWidget( GWidget* widget );
	void     cleanupWidget();


	WidgetManager mManager;
};


class  GPanel : public GUI::Panel< GPanel >
{
	typedef GUI::Panel< GPanel > BaseClass;
public:
	GPanel( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
protected:
	void onRender();
};

class  GButtonBase : public GUI::Button< GButtonBase >
{
	typedef GUI::Button< GButtonBase > BaseClass;
public:
	GButtonBase( int id , Vec2i const& pos , Vec2i const& size  , GWidget* parent )
		:BaseClass( pos , size , parent )
	{
		mId = id;
	}
	virtual void onClick(){  sendEvent( EVT_BUTTON_CLICK );  }
};

class HelpTextPanel : public GPanel
{




};

class GTextButton : public GButtonBase
{
	typedef GButtonBase BaseClass;
public:
	GTextButton( int id , Vec2i const& pos , Vec2i const& size  , GWidget* parent );
	~GTextButton();
	void   onRender();
	IText* text;
};

class GImageButton : public GButtonBase
{
	typedef GButtonBase BaseClass;
public:
	GImageButton( int id , Vec2i const& pos , Vec2i const& size  , GWidget* parent );
	~GImageButton();

	void onRender();
	void setHelpText( char const* str );
	void onRenderSiblingsEnd();
	Texture* texImag;
	IText*   mHelpText;
};

class  GFrame : public GUI::Panel< GFrame >
{
	typedef GUI::Panel< GFrame > BaseClass;
public:
	GFrame( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
	static int const TopSideHeight = 20;
protected:
	bool onMouseMsg( MouseMsg const& msg );
	void onRender();
};


class GTextCtrl : public GUI::TextCtrl< GTextCtrl >
{
	typedef GUI::TextCtrl< GTextCtrl > BaseClass;
public:
	GTextCtrl( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent );
	~GTextCtrl();

	void setFontSize( unsigned size );

	void onEditText();
	void onModifyValue();
	void onPressEnter(){ sendEvent( EVT_TEXTCTRL_ENTER ); }

	void onRender();

	IText* text;
};

class GChoice : public GUI::Choice< GChoice >
{
	typedef GUI::Choice< GChoice > BaseClass;
public:

	GChoice( int id , Vec2i const& pos , Vec2i const& size , GWidget* parent )
		:BaseClass(  pos , size , parent )
	{
		mId = id;
	}

	struct MyData 
	{
		IText* text;
	};

	typedef MyData ExtraData;

	void onRender();

	void onItemSelect( unsigned select ){  sendEvent( EVT_CHOICE_SELECT );  }
	int  getMenuItemHeight(){ return 20; }
	void onAddItem( Item& item );
	void onRemoveItem( Item& item );
	void doRenderItem( Vec2i const& pos , Item& item , bool beLight );
	void doRenderMenuBG( Menu* menu );
};


#endif // GUISystem_h__
