class OptionsMenuXbox extends UIScriptedMenu
{
	int m_selected_tab = 0;
	Widget m_focused_option;
	
	private ref ContextMenu m_context_menu;
	
	const int TAB_GAME = 0;
	const int TAB_SOUND = 1;
	//const int TAB_VIDEO = 2;
	//const int TAB_CONTROLS = 3;
	const int TABS_COUNT = 2;
	
	const int WIDGET_USER_ID_OFFSET = 10000;
	const int WIDGET_PROFILE_USER_ID_OFFSET = 20000; //associated widget are indexed same as EDayZProfilesOptions enum
	
	const int PANEL_WIDGET = 200;
	const int SCROLLBAR_BACKDROP_WIDGET = 201;
	const int TOP_OFFSET_WIDGET = 202;
	const int BOTTOM_OFFSET_WIDGET = 203;
	const int SCROLLBAR_SLIDER = 204;
	//shared between "focus" effect panel widget (1st) and panel widget, containing all buttons/content (2nd). Both are inserted into container widget. 
	const int PANEL_FOCUS_WIDGET = 205;

	int 	m_current_quality_value; //used to set preset buttons (hack-ish stuff)
	
	float 	m_original_bottom_offset_y;
	float 	m_scrollbar_value;
	//float	m_FOV_original_value;
	
	Widget 	m_current_scrolling_panel;
	Widget 	m_current_scrolling_backdrop_image;
	Widget 	m_current_top_offset;
	Widget 	m_current_bottom_offset;
	Widget 	m_current_scroll_slider;
	
	ref TIntArray m_action_indices;
	ref TIntArray m_keys;
	
	bool	m_system_cm;
	bool	m_global_cm;
	bool	m_direct_cm;
	bool	m_vehicle_cm;
	bool	m_radio_cm;
	
	CheckBoxWidget	m_system_cm_check;
	CheckBoxWidget	m_global_cm_check;
	CheckBoxWidget	m_direct_cm_check;
	CheckBoxWidget	m_vehicle_cm_check;
	CheckBoxWidget	m_radio_cm_check;
	CheckBoxWidget	m_ShowQuicBarCheck;
	
	Widget 			m_tab_panels[TABS_COUNT];
	Widget 			m_tabs[TABS_COUNT];
	ImageWidget 	m_tab_images[TABS_COUNT];
	
	ref array<ButtonWidget> m_array_buttons;
	ref array<float> m_original_widgets_y;
	
	SwitchOptionsAccess m_default_video_access;
	
	ButtonWidget m_invert_mouse_off_button;
	ButtonWidget m_invert_mouse_on_button;
	
	//Dropdown Menus
	CheckBoxWidget m_device_custom_scheme;
	
	SliderWidget m_current_scrollbar;
	
	OptionsAccessUI m_invert_mouse_option;
	
	//OptionsAccessUI m_FOV_option;
	
	ref TOptionsAccessUIArray m_optionsUI;
	ref TProfileOptionsUIArray m_profile_optionsUI;
	
	ref IngameHud	m_hud;	
	
	
	void OptionsMenuXbox()
	{
		m_action_indices = new TIntArray;
		m_keys = new TIntArray;
		m_optionsUI = new TOptionsAccessUIArray;
		m_profile_optionsUI = new TProfileOptionsUIArray;
	}
	
	void ~OptionsMenuXbox()
	{
		PPEffects.SetBlurMenu(0);
		m_Options.Revert();
		GetGame().EndOptionsVideo();
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/xbox/day_z_options_xbox.layout");
		
		m_context_menu = new ContextMenu;
		m_context_menu.Init(layoutRoot);
		m_context_menu.ShowBackdrop(true);
		
		PPEffects.SetBlurMenu(0.6); //0.5
		
		GetGame().BeginOptionsVideo();	
		
		m_tabs[TAB_GAME] = layoutRoot.FindAnyWidget("GamePanelWidget");
		m_tabs[TAB_SOUND] = layoutRoot.FindAnyWidget("SoundPanelWidget");
		//m_tabs[TAB_VIDEO] = layoutRoot.FindAnyWidget("VideoPanelWidget");
		//m_tabs[TAB_CONTROLS] = layoutRoot.FindAnyWidget("ControlsPanelWidget");
		
		//m_keyboard_tab = layoutRoot.FindWidget("KeyboardBindingPanelWidget");
		
		m_tab_panels[TAB_GAME] = layoutRoot.FindAnyWidget("GameTabPanelWidget");
		m_tab_panels[TAB_SOUND] = layoutRoot.FindAnyWidget("SoundTabPanelWidget");
		//m_tab_panels[TAB_VIDEO] = layoutRoot.FindAnyWidget("VideoTabPanelWidget");
		//m_tab_panels[TAB_CONTROLS] = layoutRoot.FindAnyWidget("ControlsTabPanelWidget");
		
		m_tab_images[TAB_GAME] = ImageWidget.Cast( layoutRoot.FindAnyWidget("GameButtonBackdropImageWidget") );
		m_tab_images[TAB_SOUND] = ImageWidget.Cast( layoutRoot.FindAnyWidget("SoundButtonBackdropImageWidget") );
		//m_tab_images[TAB_VIDEO] = ImageWidget.Cast( layoutRoot.FindAnyWidget("VideoButtonBackdropImageWidget") );
		//m_tab_images[TAB_CONTROLS] = ImageWidget.Cast( layoutRoot.FindAnyWidget("ControlsButtonBackdropImageWidget") );
		
		m_system_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("SystemCMCheckBox") );
		m_global_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("GlobalCMCheckBox") );
		m_direct_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("DirectCMCheckBox") );
		m_vehicle_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("VehicleCMCheckBox") );
		m_radio_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("RadioCMCheckBox") );
		m_ShowQuicBarCheck = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("ShowQuickBar") );
		
		int i = 0;
		int c = g_Game.GetInput().GetActionGroupsCount();
		string name;
		
		m_original_widgets_y = new array<float>;
		m_array_buttons = new array<ButtonWidget>;
		
		SelectTab(m_selected_tab);
		
		InitOptions();	
		//UpdateChatMessages();
		
		RegisterProfileOptionWidgets();
		//InitProfileSettings();
		
		return layoutRoot;
	}
	
	void InitOptions()
	{
		int i;
		int c;
		string ResolutionText;
		string text;
		OptionsAccessUI optionUI;
		OptionsAccess optionAccess;
		ListOptionsAccess loa;
		SwitchOptionsAccess soa;
		
		// cleanup first
		m_optionsUI.Clear();
		
		m_Options = new GameOptions;
		
		c = m_Options.GetOptionsCount();
		for (i = 0; i < c; i++)
		{
			optionUI = NULL;
			optionAccess = m_Options.GetOption(i);
			
			//TODO perform only once on init, if possible, save into some array for reference / revert values (?)
			switch (optionAccess.GetControlType())
			{
			case OA_CT_NUMERIC:
				optionUI = new SliderOptionsAccessUI;
			break;
			case OA_CT_SWITCH:
				optionUI = new ButtonOptionsAccessUI;
			break;
			case OA_CT_LIST:
				optionUI = new ListOptionsAccessUI; //ComboBoxOptionsAccessUI
			break;
			}
			
			switch (optionAccess.GetAccessType())
			{
				/*
			case AT_OPTIONS_FIELD_OF_VIEW:
				m_FOV_option = optionUI;
				NumericOptionsAccess noa = NumericOptionsAccess.Cast(optionAccess);
				m_FOV_original_value = noa.ReadValue();
			break;
			*/
			case AT_CONFIG_YREVERSED:
				m_invert_mouse_option = optionUI;
			break;
			case AT_OPTIONS_VIDEO_DEFAULT:
				m_default_video_access = SwitchOptionsAccess.Cast( optionAccess );
			break;
			}
			
			if (optionUI)
			{
				if (optionUI.Type().ToString() != "ListOptionsAccessUI")
				{
					optionUI.Attach(optionAccess, layoutRoot.FindAnyWidgetById(WIDGET_USER_ID_OFFSET + optionAccess.GetAccessType()));
					m_optionsUI.Insert(optionUI);
				}
				else
				{
					ListOptionsAccessUI.Cast(optionUI).Attach(optionAccess, GetArrayOfWidgetsById(WIDGET_USER_ID_OFFSET + optionAccess.GetAccessType()));
					m_optionsUI.Insert(optionUI);
				}
			}
		}
	}
	
	array<Widget> GetArrayOfWidgetsById(int id) //Gets all button widgets, or one non-button widget into an array, whichever fits
	{
		array<Widget> arrayOfWidgets = new array<Widget>;
		Widget newWidget = layoutRoot.FindAnyWidgetById( id );
		
		if ( newWidget )
		{
			while ( newWidget )
			{
				if ( newWidget.GetTypeName() == "ButtonWidget" )
				{
					arrayOfWidgets.Insert(newWidget);
				}
				newWidget = newWidget.GetSibling();
			}
			if ( arrayOfWidgets.Count() == 0 )
			{
				arrayOfWidgets.Insert(layoutRoot.FindAnyWidgetById( id )); //insert single non-button widget (SliderWidget, for example)
			}
		}
		return arrayOfWidgets;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		int i;
		int selected;
		string text;
		OptionsAccessUI item;
		SwitchOptionsAccess soa;
				
		for (i = 0; i < TABS_COUNT; i++)
		{
			if (w == m_tab_panels[i])
			{
				SelectTab(i);
				return true;
			}
		}
				
		return false;
	}
	
	bool ShowDropdownMenu(Widget w, ref array<string> items, string FnName)
	{
		float x_out;
		float y_out;
		
		if (items.Count() > 0)
		{
			m_context_menu.Clear();
			for (int i = 0; i < items.Count(); i++)
			{
				TContextMenuItem ContextMenuItems = new TContextMenuItem(0,"");
				ContextMenuItems.param1 = i;
				ContextMenuItems.param2 = items.Get(i);
				m_context_menu.Add(items.Get(i),this,FnName,ContextMenuItems);
			}
			
			w.GetScreenPos(x_out,y_out);
			m_context_menu.Show(x_out,y_out);
			
			return true;
		}
		return false;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (m_current_scrollbar.IsVisible())
		{
			ScrollWidgetFn(wheel);
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return FocusPanel(true,w);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return FocusPanel(false,w);
	}
		
	//Temporary solution to controller controls
	override void Update( float timeslice )
	{
		/*
		//BUTTON A
		if( GetGame().GetInput().GetActionDown( UAUISelect, false ) )
		{
			Print("Button A");
		}
		*/
		/*
		//BUTTON B
		if( GetGame().GetInput().GetActionDown( UAUIBack, false ) )
		{
			Print("Button B");
		}
		*/
		
		//BUTTON X - APPLY SETTINGS
		if( GetGame().GetInput().GetActionDown( UAUIFastEquipOrSplit, false ) )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call( this.ApplyFn );
		}

		//BUTTON Y - RESET TO DEFAULTS
		/*
		if( GetGame().GetInput().GetActionDown( UAUIRadialMenuPick, false ) )
		{
			switch( m_selected_tab )
			{
				case TAB_GAME: break;
				case TAB_SOUND: 
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call( this.DefaultAudioSettings );
					break;
				case TAB_VIDEO: 
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call( this.DefaultVideoSettings );
					break;
				case TAB_CONTROLS: break;
			}
			
			//GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call( this.DefaultSettings );
		}
		*/
		
		//LEFT BUMPER - TAB LEFT
		if( GetGame().GetInput().GetActionDown( UAUITabLeft, false ) )
		{
			SelectPreviousTab();
		}
		
		//RIGHT BUMPER - TAB RIGHT
		if( GetGame().GetInput().GetActionDown( UAUITabRight, false ) )
		{
			SelectNextTab();
		}
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		//FOCUS
		if ( w != m_focused_option )
		{
			Widget option_background;
			
			//reset previous option visuals
			if ( m_focused_option )
			{
				 option_background = m_focused_option.GetParent().FindAnyWidget( "OptionBackground" );
				if ( option_background ) 
				{
					option_background.Show( false );
				}
			}
			
			//set new option visuals
			Widget w_parent = w.GetParent();
			if ( w_parent && w_parent.GetUserID() == 205 )
			{
				option_background = w_parent.FindAnyWidget( "OptionBackground" );
				if ( option_background ) 
				{
					option_background.Show( true );
				}
				
				//set new focused widget
				m_focused_option = w;
			}
		}
		
		return true;	
	}
	//
	
	
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (w == m_system_cm_check)
		{
			m_system_cm = m_system_cm_check.IsChecked();
		}
		else if (w == m_global_cm_check)
		{
			m_global_cm  = m_global_cm_check.IsChecked();
		}
		else if (w == m_direct_cm_check)
		{
			m_direct_cm = m_direct_cm_check.IsChecked();
		}
		else if (w == m_vehicle_cm_check)
		{
			m_vehicle_cm = m_vehicle_cm_check.IsChecked();
		}
		else if (w == m_radio_cm_check)
		{
			m_radio_cm = m_radio_cm_check.IsChecked();
		}
		else if (w == m_current_scrollbar)
		{
			m_scrollbar_value = m_current_scrollbar.GetCurrent();
			m_scrollbar_value = m_scrollbar_value / m_current_scrollbar.GetMax();
			//m_scrollbar_value = m_new_scrollbar_value;
			ScrollWidgetFn(0); //0 value makes slider widget move widget Y positions
		}
		return false;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		int selected;
		
		return false;
	}
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.ApplyFn);
		
		if (result == DBB_YES)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.RestartFn);
			return true;
		}
		return false;
	}
	
	override void OnShow()
	{
		super.OnShow();
	}
	
	bool FocusPanel(bool show, Widget w)
	{
		Widget wid;
		Widget parent;
		if ( w.GetUserID() == PANEL_FOCUS_WIDGET )
		{
			wid = w;
		}
		if ( GetParentByID(w,PANEL_FOCUS_WIDGET) )
		{
			wid = GetParentByID(w,PANEL_FOCUS_WIDGET);
		}
		if ( wid )
		{
			parent = wid.GetParent();
			if ( parent )
			{
				GetChildByID(parent, PANEL_FOCUS_WIDGET).Show(show);
				return true;
			}
		}
		return false;
	}
	
	void SelectPreviousTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab - 1) % TABS_COUNT;
		if (currTab < 0)
		{
			currTab = TABS_COUNT - 1;
		}
		
		while(currTab != m_selected_tab)
		{
			if (m_tab_panels[currTab] != NULL)
			{
				SelectTab(currTab);
				return;
			}
			
			currTab = (currTab - 1) % TABS_COUNT;
			if (currTab < 0)
			{
				currTab = TABS_COUNT - 1;
			}
		}
	}
	
	void SelectNextTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab + 1) % TABS_COUNT;
		
		while(currTab != m_selected_tab)
		{
			if (m_tab_panels[currTab] != NULL)
			{
				SelectTab(currTab);	
				return;		
			}
			
			currTab = (currTab + 1) % TABS_COUNT;
		}
	}
	
	void SelectTab(int tab_id)
	{
		int i = 0;
		for (i = 0; i < TABS_COUNT; i++)
		{
			Widget tab = m_tabs[i];
			if (tab != NULL)
				tab.Show(i == tab_id);
			if (i == tab_id) //determines which tab is open
			{
				InitWidgetPosY(tab);
			}
		}
		
		for (i = 0; i < TABS_COUNT; i++)
		{
			ImageWidget backdrop = m_tab_images[i];
			if ( backdrop )
			{
				float alpha;
				if( i != tab_id )
					alpha = 0.4;
				else
					alpha = 0.9;
				backdrop.SetAlpha( alpha );
			}
		}
		
		m_selected_tab = tab_id;
	}
			
	void ScrollWidgetFn(int wheel)
	{
		int i = 0;
		float pos_x;
		float pos_y;
		float bottom_x;
		float bottom_y;
		float top_x;
		float top_y;
		float scroll_speed = wheel/20;
		//float slider_min;
		float slider_max = m_current_scrollbar.GetMax();
		float adjusted_value;
		float scrolling_limit;
		
		Widget Child = Widget.Cast(m_current_scrolling_panel.GetChildren());
		
		m_current_bottom_offset.GetPos(bottom_x,bottom_y);
		m_current_top_offset.GetPos(top_x,top_y);
		scrolling_limit = -top_y + bottom_y - 1;
		
		if ((bottom_y < (1 + scroll_speed) && wheel < 0) || (top_y >= (0 - scroll_speed) && wheel > 0))
		{
			return;
		}
		
		if (wheel != 0)
		{
			m_scrollbar_value = m_current_scrollbar.GetCurrent() / slider_max;
			adjusted_value = (m_scrollbar_value * scrolling_limit + scroll_speed) / scrolling_limit * slider_max;//m_scrollbar_value/(scrolling_limit + 1)
			m_current_scrollbar.SetCurrent(adjusted_value);
			m_scrollbar_value = adjusted_value;
		}
		else //scrolls by scrollbar dragging
		{
			//slider_max = m_video_scrollbar.GetMax(); //100
			adjusted_value = m_scrollbar_value * scrolling_limit - scrolling_limit;
		}
		
		while (Child)
		{
			if (wheel != 0)
			{
				Child.GetPos(pos_x,pos_y);
				Child.SetPos(pos_x, Math.Clamp(pos_y + scroll_speed, (-scrolling_limit - 1),(scrolling_limit + 1)) ,true);
			}
			else if (adjusted_value != 0) //scrolls by scrollbar dragging
			{
				Child.GetPos(pos_x,pos_y);
				Child.SetPos(pos_x,m_original_widgets_y.Get(i) + adjusted_value,true);
				i++;
			}
			Child = Child.GetSibling();
		}
		if (m_current_scroll_slider.IsVisible())
		{
			float junk;
			float y_clamp;
			m_current_scroll_slider.GetSize(junk, y_clamp);
			float positionY = m_current_scrollbar.GetCurrent() / m_current_scrollbar.GetMax();
			positionY = Math.Clamp((1 - positionY),0,(1-y_clamp));
			m_current_scroll_slider.SetPos(0,positionY,true);
		}
	}
	
	void UpdateChatMessages()
	{
		m_system_cm = GetProfileValueBool(SYSTEM_CHAT_MSG, true);
		m_global_cm = GetProfileValueBool(GLOBAL_CHAT_MSG, true);
		m_direct_cm = GetProfileValueBool(DIRECT_CHAT_MSG, true);
		m_vehicle_cm = GetProfileValueBool(VEHICLE_CHAT_MSG, true);
		m_radio_cm = GetProfileValueBool(RADIO_CHAT_MSG, true);	
	}
	
	//!Secondary init method for non-C++ based options (currently unused)
	void InitProfileSettings()
	{
	}
	
	//!Registers and inits non-C++ based options
	void RegisterProfileOptionWidgets()
	{
		int max = EDayZProfilesOptions.MAX;
		string str;
		ref array<Widget> buttonsArray;
		ProfileOptionsUI profileOptionUI;
		for (int i = 0; i < max; i++)
		{
			profileOptionUI = new ProfileOptionsUI;
			str = g_Game.GetProfileOptionMap().Get(i).param1;
			buttonsArray = new array<Widget>;
			
			switch (str)
			{
				case SHOW_CROSSHAIR:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.CROSSHAIR);
				break;
				case SHOW_HUD:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.HUD);
					/*buttonsArray.Insert(m_HudOffButton);
					buttonsArray.Insert(m_HudOnButton);*/
				break;
				case SHOW_QUICKBAR:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.QUICKBAR);
					/*buttonsArray.Insert(m_QuickbarOffButton);
					buttonsArray.Insert(m_QuickbarOnButton);*/
				break;
				case SYSTEM_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.SERVER_MESSAGES);
				break;
				case DIRECT_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.USERS_CHAT);
				break;
				case RADIO_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.RADIO_CHAT);
				break;
			}
			
			if (buttonsArray.Count() > 0)
			{
				profileOptionUI.Attach(i, buttonsArray);
				m_profile_optionsUI.Insert(profileOptionUI);
			}
		}
		//Print(str);
	}
	
	//!Saves non-C++ based options to profile file
	void SaveProfileOptions()
	{
		bool state;
		ButtonWidget offButton;
		array<Widget> wArray = new array<Widget>;
		for (int i = 0; i < m_profile_optionsUI.Count(); i++)
		{
			wArray.Clear();
			wArray = m_profile_optionsUI.Get(i).GetWidgetArray();
			if ( wArray.Count() > 0 )
			{
				offButton = ButtonWidget.Cast( wArray.Get(0) ); //OFF button
				state = offButton.GetState();
				g_Game.SetProfileOption(i,!state);
			}
		}
	}
		
	ref GameOptions m_Options;
	
	void ApplyFn()	
	{
		string value;
		GetGame().GetProfileString("usenewui", value);
		
		SaveProfileOptions();
		
		GetGame().SaveProfile();
		m_Options.Apply();
		//
		m_hud = IngameHud.Cast( GetGame().GetMission().GetHud() );
		if (m_hud)
		{
			m_hud.Init(m_hud.GetHudPanelWidget());
		}
		//
		Close();
	}
	
	void RestartFn()
	{
		g_Game.RequestRestart(IDC_MAIN_QUIT);
	}
	
	void DefaultVideoSettings()
	{
		if (m_default_video_access)
		{
			m_default_video_access.Switch();
			InitOptions();
		}
	}
	
	//TODO add functionality; set some reasonable default values either here (via NumericOptionsAccess) or in code
	void DefaultAudioSettings()
	{
		InitOptions();
	}
	
	void DefaultSettings()
	{
		InitOptions();
	}
	
	void SetMultipleWidgetFlags(OptionsAccessUI oaUI, int flag, bool setflags)
	{
		array<Widget> temparray = new array<Widget>;
		temparray = oaUI.GetWidgetArray();
		if (!temparray) 	return;
		
		int count = temparray.Count();
		if (setflags)
		{
			for (int i = 0; i < count; i++)
			{
				temparray.Get(i).SetFlags(flag);
			}
		}
		else
		{
			for (i = 0; i < count; i++)
			{
				temparray.Get(i).ClearFlags(flag);
			}
		}
	}
	
	void ListListOptionAccess(OptionsAccess optionAccess, out array<string> output)
	{
		if (optionAccess.GetControlType() == OA_CT_LIST )
		{
			string str;
			ListOptionsAccess loa = ListOptionsAccess.Cast(optionAccess);
			output.Clear();
			for (int i = 0; i < loa.GetItemsCount(); i++)
			{
				loa.GetItemText(i,str);
				output.Insert(str);
			}
		}
	}
	
	void InitWidgetPosY(Widget w) //CAUTION, used to initialize all panels and scrolling stuff...I hope...
	{
		bool scrollbar = false;
		int i = 0;
		float pos_x;
		float pos_y;
		float pos_y_2;
		array <float> widgets_zero_positions = new array <float>;
		Widget Child;
		
		m_current_scrolling_panel = NULL;
		m_current_top_offset = NULL;
		m_current_bottom_offset = NULL;
		m_current_scrollbar = NULL;
		m_current_scrolling_backdrop_image = NULL;
		
		m_original_widgets_y.Clear();
		
		Child = Widget.Cast(w.GetChildren());
		while (Child)
		{
			if (Child.GetUserID() == PANEL_WIDGET && !m_current_scrolling_panel)
			{
				m_current_scrolling_panel = Child;
				Child = m_current_scrolling_panel.GetChildren();
				while (Child) //widgets to be scrollable
				{
					if (Child.GetUserID() == TOP_OFFSET_WIDGET)
					{
						m_current_top_offset = Child;
					}
					if (Child.GetUserID() == BOTTOM_OFFSET_WIDGET)
					{
						m_current_bottom_offset = Child;
					}
					Child.GetPos(pos_x,pos_y);
					m_original_widgets_y.Insert(pos_y);
					Child = Child.GetSibling();
				}
				Child = Widget.Cast(w.GetChildren());
			}
			if (Child.GetUserID() == SCROLLBAR_BACKDROP_WIDGET)
			{
				m_current_scrolling_backdrop_image = Child;
				m_current_scrollbar = SliderWidget.Cast(Child.GetChildren());
				m_current_scroll_slider = GetChildByID(m_current_scrollbar, SCROLLBAR_SLIDER);
			}
			Child = Child.GetSibling();
		}
		//adjust widget location, hide/show scrollbar if needed
		m_current_top_offset.GetPos(pos_x, pos_y);
		m_current_bottom_offset.GetPos(pos_x, pos_y_2);
		
		m_current_scrolling_backdrop_image.Show((pos_y - pos_y_2) < -1);
		
		if (m_current_scrolling_backdrop_image.IsVisible() && pos_y != 0) //sets widgets to original positions, if needed
		{
			Child = m_current_scrolling_panel.GetChildren();
			while (Child)
			{
				Child.SetPos(pos_x,m_original_widgets_y.Get(i) - pos_y,true);
				Child.GetPos(pos_x,pos_y_2);
				widgets_zero_positions.Insert(pos_y_2);
				Child = Child.GetSibling();
				i++;
			}
			m_original_widgets_y.Clear();
			m_original_widgets_y.Copy(widgets_zero_positions);
		}
		m_scrollbar_value = 1;
	}
	
	Widget GetParentByID(Widget w, int id)
	{
		Widget parent;
		parent = w.GetParent();
		while (parent && parent.GetUserID() != id)
		{
			parent = parent.GetParent();
		}
		if (parent && parent.GetUserID() == id)
		{
			return parent;
		}
		else
		{
			return NULL;
		}
	}
	
	Widget GetChildByID(Widget w, int id)
	{
		Widget other;
		Widget child = w.GetChildren();
		while (child && child.GetUserID() != id)
		{
			if (child.GetChildren() == NULL)
			{
				child = child.GetSibling();
			}
			else
			{
				other = GetChildByID(child,id);
				if (other.GetUserID() == id)
				{
					return other;
				}
			}
		}
		if (child && child.GetUserID() == id)
		{
			return child;
		}
		else
		{
			return NULL;
		}
	}
}
